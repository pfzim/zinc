//#include "stdafx.h"								//pf_ziminski  [2002]
#include "zini.h"
#include "zibuffer.h"
#include "utstrings.h"
#include "crc32.h"
#include <stdlib.h>
#include "snprintf.h"

// notes:
// убрать лишнии проверки указателей (исключить двойную проверку)!
// не всё корректно работает без секций (не проверено)!
// удалять пробелы, табы или если есть коментарии то добавлять к ним!
//
// имеется уязвимость: возможно переполнение буфера в ini_flush()
//
// NEW_INI_STYLE - для того чтобы включить все параметры в кавычки
// CONVER_TO_NEW - определить один раз и запустить приложение
//                 для конвертации параметров в new style
// CAN_RETURN_DEFAULT - определить как 1 для того чтобы
//                      ini_get_string() по возможности возвращал
//                      адрес переданого ему значения default.
//
// 01.02.2003	переделал функции которые возвращают значения ключей
//				теперь если ключ или значение ключа не найдено то
//				функция возвращает параметер nullstring, а не NULL.
//				[думаю можно возвращать и параметр default если он]
//				[не NULL       [смотри CAN_RETURN_DEFAULT = 0x0001]
// 07.03.2004	предлагаю реализовать возможность прямого возвращения
//				функцией get_ini_string() значения из ini_node.
//              [давно уже реализовано]

int ini_load(LPINI_NODE *ini_node, const char *lpszFileName)
{
	// return value: при успешном завершеннии возвращает значение
	// INI_SUCCESS. если же во время разбора файла произошла ошибка,
	// разобранная информация в ini_node НЕ уничтожается!

	*ini_node = NULL;

	register unsigned long dwCurPos = 0;
	register unsigned long temp_pos;
	register unsigned long stpos;
	register unsigned long enpos;
	int result = INI_SUCCESS;
	EXTBUFFER zBuffer;
	LPINI_NODE temp_node = NULL;

	if(__initbufferedread(lpszFileName, &zBuffer, MAX_BUFFER_INI_KB*1024) == NULL)
	{
		return INI_OPEN_FAILURE;
	}

	if((*ini_node = (LPINI_NODE) zalloc(sizeof(INI_NODE))) == NULL)
	{
		result = INI_ALLOC_FAILURE;
		goto end_loop;
	}
	memset(*ini_node, 0, sizeof(INI_NODE));

	temp_node = *ini_node;

	while(!__iseof(&zBuffer, dwCurPos))
	{
		switch(__get(&zBuffer, dwCurPos))
		{
			case 0x20:
			case 0x09:
				//ignore this chars
				break;
			case 0x0A:
			case 0x0D:
				//allocate new node
				if(temp_node->node_id != 0)
				{
					if((temp_node->next_node = (LPINI_NODE)zalloc(sizeof(INI_NODE))) == NULL)
					{
						temp_node = NULL;
						result = INI_ALLOC_FAILURE;
						goto end_loop;
					}
					memset(temp_node->next_node, 0, sizeof(INI_NODE));

					temp_node->next_node->prev_node = temp_node;
					temp_node = temp_node->next_node;
				}
				break;
			case ';':
			case '#':
				//comment founded
				temp_pos = __seekcsp(&zBuffer, dwCurPos+1, &stpos, &enpos, "\r\n", TRUE);
				if(temp_pos == BRW_INVALID_POINTER)
				{
					result = INI_NO_END_LINE;
					goto end_loop;
				}
				if(enpos >= stpos) // is no empty comment?
				{
					if((temp_node->szComment = (char *)zalloc(enpos-stpos+2)) == NULL)
					{
						result = INI_ALLOC_FAILURE;
						goto end_loop;
					}
					temp_node->node_id |= INI_COMMENT;
					__getblock(&zBuffer, temp_node->szComment, stpos, enpos);
				}
				dwCurPos = temp_pos-1;
				break;
			case '['://section//:']'
				//section name founded
				temp_pos = __seekcsp(&zBuffer, dwCurPos+1, &stpos, &enpos, "]\r\n", TRUE);
				if((temp_pos == BRW_INVALID_POINTER) ||
					(enpos < stpos) ||					//пустое название
					__get(&zBuffer, temp_pos) != ']')	//нет закрывающейся скобки
				{
					result = INI_PARSE_ERROR;
					goto end_loop;
				}
				//можно добавить проверку на закрывающуюся скобку ']'
				if((temp_node->szSection = (char *)zalloc(enpos-stpos+2)) == NULL)
				{
					result = INI_ALLOC_FAILURE;
					goto end_loop;
				}
				temp_node->node_id |= INI_SECTION;
				__getblock(&zBuffer, temp_node->szSection, stpos, enpos);
				if(strchr("\r\n", __get(&zBuffer, dwCurPos)))
				{
					temp_pos = temp_pos -1;
				}
				dwCurPos = temp_pos;
				break;
			default:
				//key founded
				temp_pos = __seekcsp(&zBuffer, dwCurPos, &stpos, &enpos, "=;#\r\n", TRUE);
				if((temp_pos == BRW_INVALID_POINTER) || (enpos < stpos))
				{
					result = INI_PARSE_ERROR;
					goto end_loop;
				}
				if((temp_node->szKey = (char *)zalloc(enpos-stpos+2)) == NULL)
				{
					result = INI_ALLOC_FAILURE;
					goto end_loop;
				}
				temp_node->node_id |= INI_KEY;
				__getblock(&zBuffer, temp_node->szKey, stpos, enpos);
				if(__get(&zBuffer, temp_pos) == '=')
				{
					dwCurPos = temp_pos+1;
					temp_pos = __iniseekcsp(&zBuffer, dwCurPos, &stpos, &enpos, ";#\r\n", TRUE);
					if(temp_pos == BRW_INVALID_POINTER)
					{
						result = INI_PARSE_ERROR;
						goto end_loop;
					}
					if(enpos >= stpos)
					{
						if((temp_node->szValue = (char *)zalloc(enpos-stpos+2)) == NULL)
						{
							result = INI_ALLOC_FAILURE;
							goto end_loop;
						}
						temp_node->node_id |= INI_VALUE;
						__getblock(&zBuffer, temp_node->szValue, stpos, enpos);
#if defined(NEW_INI_STYLE) && (NEW_INI_STYLE == 1)
						char *tempvalue = alloc_unescapestring(temp_node->szValue, NULL);
						if(!tempvalue)
						{
							result = INI_ALLOC_FAILURE;
							goto end_loop;
						}
						free_str(temp_node->szValue);
						temp_node->szValue = tempvalue;
#endif
					}
				}
				dwCurPos = temp_pos-1;
		} //switch

		dwCurPos++;
	} //while

end_loop: // correct exit
	if(temp_node && temp_node->node_id == 0)
	{
		ini_free_node(ini_node, temp_node);
	}

	__destructbufferedreadwrite(&zBuffer);
	return result;
}

int ini_flush(LPINI_NODE ini_node, const char *lpszFileName, int fUseCRC)
{
	EXTBUFFER zBuffer;

	if(__initbufferedwrite(lpszFileName, &zBuffer, MAX_BUFFER_INI_KB) == NULL)
	{
		return INI_OPEN_FAILURE;
	}

	LPINI_NODE temp_node = ini_node;

	while(temp_node)
	{
		if(temp_node->node_id & INI_SECTION)
		{
			if(temp_node != ini_node)
			{
				__addblock(&zBuffer, "\r\n", 2);
			}
			__addblock(&zBuffer, "[", 1);
			__addblock(&zBuffer, temp_node->szSection, strlen(temp_node->szSection));
			__addblock(&zBuffer, "]", 1);
		}
		if(temp_node->node_id & INI_KEY)
		{
			__addblock(&zBuffer, temp_node->szKey, strlen(temp_node->szKey));
			__addblock(&zBuffer, "=", 1);
		}
		if(temp_node->node_id & INI_VALUE)
		{
#if (defined(NEW_INI_STYLE) && (NEW_INI_STYLE == 1)) || defined(CONVER_TO_NEW)
			char *tempvalue;
			tempvalue = alloc_escapestring(temp_node->szValue, strlen(temp_node->szValue));
			__addblock(&zBuffer, tempvalue, strlen(tempvalue));
			free_str(tempvalue);
#else
			__addblock(&zBuffer, temp_node->szValue, strlen(temp_node->szValue));
#endif
		}
		if(temp_node->node_id & INI_COMMENT)
		{
			//*
			if(!(temp_node->node_id & ~INI_COMMENT)) // only comment
			{
				__addblock(&zBuffer, ";", 1);
				__addblock(&zBuffer, temp_node->szComment, strlen(temp_node->szComment));
			}
			else if(!(temp_node->node_id & ~(INI_COMMENT|INI_CRC32)))
			{
				// special comment include crc32
				__addblock(&zBuffer, "\r\n;", 3);
				__addblock(&zBuffer, temp_node->szComment, strlen(temp_node->szComment));
			}
			else // comment after key or section
			{
				__addblock(&zBuffer, "\t;", 2);
				__addblock(&zBuffer, temp_node->szComment, strlen(temp_node->szComment));
			}
			//*/ // не забудь поменять и crc
		}
		__addblock(&zBuffer, "\r\n", 2);
		temp_node = temp_node->next_node;
	}

	__donebufferedwrite(&zBuffer);			// flush buffer
	__destructbufferedreadwrite(&zBuffer);	// close file

	return INI_SUCCESS;
}

int ini_free(LPINI_NODE *ini_node)
{
	LPINI_NODE temp_node;
	temp_node = *ini_node;
	*ini_node = NULL;
	while(temp_node)
	{
		if(temp_node->node_id & INI_SECTION)
		{
			free_str(temp_node->szSection);
		}
		if(temp_node->node_id & INI_COMMENT)
		{
			free_str(temp_node->szComment);
		}
		if(temp_node->node_id & INI_KEY)
		{
			free_str(temp_node->szKey);
		}
		if(temp_node->node_id & INI_VALUE)
		{
			free_str(temp_node->szValue);
		}
		if(temp_node->next_node)
		{
			temp_node = temp_node->next_node;
			zfree(temp_node->prev_node);
		}
		else
		{
			zfree(temp_node);
			temp_node = NULL;
		}
	}
	return INI_SUCCESS;
}

LPINI_NODE ini_get_node(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey)
{
	if(!ini_node)
	{
		return NULL;
	}

	LPINI_NODE temp_node = ini_node;
	if(!isempty(lpszSection))
	{
		while(temp_node)
		{
			if((temp_node->node_id & INI_SECTION)
			&&(strcmp(lpszSection, temp_node->szSection) == 0))
			{
				if(lpszKey)	break;
				return temp_node;
			}
			temp_node = temp_node->next_node;
		}
	}
	else if(temp_node->node_id & INI_SECTION)
	{
		return NULL;
	}

	if(temp_node && !isempty(lpszKey))
	{
		do
		{
			if((temp_node->node_id & INI_KEY)
			&&(strcmp(lpszKey, temp_node->szKey) == 0))
			{
				return temp_node;
			}
			temp_node = temp_node->next_node;
		}
		while(temp_node && !(temp_node->node_id & INI_SECTION));
	}
	/*else if(temp_node) // no section name (first)
	{
		return temp_node;
	}*/

	return NULL;
}

char *ini_alloc_get_string(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, const char *lpszDefault)
{
	char *lpstr = NULL, *lptemp;
	lptemp = (char *) ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(lptemp))
	{
		lpstr = (char *) zalloc(strlen(lptemp) + 1);
		if(lpstr)
		{
			strcpy(lpstr, lptemp);
		}
	}
	else if(lpszDefault)
	{
		lpstr = (char *) zalloc(strlen(lpszDefault) + 1);
		if(lpstr)
		{
			strcpy(lpstr, lpszDefault);
		}
	}

	return lpstr?lpstr:nullstring;
}

const char *ini_get_string(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, const char *lpszDefault, char *lpszValue, unsigned long *lpdwSize)
{
	// [out] lpszValue - адрес буфера под строку. может быть NULL.
	// [out] lpdwSize - объём памяти выделенный под lpszValue если
	//                  lpdwSize = 0 то записывает в него требуемый
	//                  объём под lpszValue. может быть NULL.
	//                  Этот параметр передаётся в strncpy_tiny,
	//                  поэтому он не включает себя нуль-терминатор.
	//
	// return value: адрес szValue из ini_node или в случае ошибки
	// пустая строка "" (emptystring). если же определёно
	// CAN_RETURN_DEFAULT как 0x0001 то вместо emptystring
	// возвращается адрес lpszDefault, если же lpszDefault == NULL
	// возвращвется emptystring;

	if(isempty(lpszKey))
	{
		if(lpszValue && lpdwSize && *lpdwSize)
		{
			if(lpszDefault)
			{
				strncpy_tiny(lpszValue, lpszDefault, *lpdwSize);
				//lpszValue[*lpdwSize-1] = '\0';
				//*lpdwSize = 0;
			}
			else
			{
				lpszValue[0] = '\0';
				//*lpdwSize = 0;
			}
		}
		else if(lpdwSize)
		{
			*lpdwSize = 0;
		}

#if defined(CAN_RETURN_DEFAULT) && (CAN_RETURN_DEFAULT == 1)
		return lpszDefault?lpszDefault:emptystring;
#else
		return emptystring;
#endif
	}

	LPINI_NODE temp_node = ini_get_node(ini_node, lpszSection, lpszKey);

	if(!temp_node || ((temp_node->node_id & INI_VALUE) != INI_VALUE))
	{
		if(lpszValue && lpdwSize && *lpdwSize)
		{
			if(lpszDefault)
			{
				strncpy_tiny(lpszValue, lpszDefault, *lpdwSize);
				//lpszValue[*lpdwSize-1] = '\0';
			}
			else
			{
				lpszValue[0] = '\0';
				//*lpdwSize = 0;
			}
		}
		else if(lpdwSize)
		{
			*lpdwSize = 0;
		}
#if defined(CAN_RETURN_DEFAULT) && (CAN_RETURN_DEFAULT == 1)
		return lpszDefault?lpszDefault:emptystring;
#else
		return emptystring;
#endif
	}

	if(lpszValue && lpdwSize && *lpdwSize)
	{
		strncpy_tiny(lpszValue, temp_node->szValue, *lpdwSize);
		//lpszValue[*lpdwSize-1] = '\0';
	}
	else if(lpdwSize)
	{
		*lpdwSize = strlen(temp_node->szValue)+1;
	}

#if defined(CAN_RETURN_DEFAULT) && (CAN_RETURN_DEFAULT == 1)
	return temp_node->szValue?temp_node->szValue:lpszDefault?lpszDefault:emptystring;
#else
	return temp_node->szValue?temp_node->szValue:emptystring;
#endif
}

LPINI_NODE ini_add_node(LPINI_NODE *ini_node)
{
	LPINI_NODE new_node, temp_node;
	if((new_node = (LPINI_NODE)zalloc(sizeof(INI_NODE))) == NULL)
	{
		return NULL;
	}
	memset(new_node, 0, sizeof(INI_NODE));

	if(temp_node = *ini_node)
	{
		while(temp_node->next_node)
		{
			temp_node = temp_node->next_node;
		}
		temp_node->next_node = new_node;
		new_node->prev_node = temp_node;
	}
	else
	{
		*ini_node = new_node;
	}

	return new_node;
}

void ini_free_node(LPINI_NODE *ini_node, LPINI_NODE del_node)
{
	if(*ini_node == del_node)
	{
		*ini_node = (*ini_node)->next_node;
	}
	if(del_node->prev_node)
	{
		del_node->prev_node->next_node = del_node->next_node;
	}
	if(del_node->next_node)
	{
		del_node->next_node->prev_node = del_node->prev_node;
	}

	if(del_node->node_id & INI_SECTION)
	{
		free_str(del_node->szSection);
	}
	if(del_node->node_id & INI_COMMENT)
	{
		free_str(del_node->szComment);
	}
	if(del_node->node_id & INI_KEY)
	{
		free_str(del_node->szKey);
	}
	if(del_node->node_id & INI_VALUE)
	{
		free_str(del_node->szValue);
	}

	zfree(del_node);
}

int ini_put_string(LPINI_NODE *ini_node, const char *lpszSection, const char *lpszKey, const char *lpszComment, const char *lpszValue, ...)
{
	// не использовать ключи без секций в файле с секциями!
	// код для этого не предусмотрен.

	if(isempty(lpszKey) || !ini_node)
	{
		return INI_INVALID_PARAMETER;
	}

	LPINI_NODE temp_node = NULL;
	LPINI_NODE sect_node = *ini_node;

	if(!isempty(lpszSection))
	{
		sect_node = ini_get_node(*ini_node, lpszSection, NULL);
	}

	if(sect_node)
	{
		temp_node = ini_get_node(sect_node, lpszSection, lpszKey);
	}

	if(temp_node)
	{
		// можно не затирать старый комментарий
		if((temp_node->node_id & INI_COMMENT) && (lpszComment))
		{
			temp_node->node_id ^= INI_COMMENT;
			free_str(temp_node->szComment);
			temp_node->szValue = 0;
		}
		if(temp_node->node_id & INI_VALUE)
		{
			temp_node->node_id ^= INI_VALUE;
			free_str(temp_node->szValue);
			temp_node->szValue = 0;
		}
	}
	else
	{
		if(sect_node)
		{
			//ищем конец секции (последний ключ секции)
			//чтобы добавить в конец!
			//можно немного видоизменить в начале, чтобы
			//повторно не перебирать ключи (если их много)!
			while((sect_node->next_node) &&
				!(sect_node->next_node->node_id & INI_SECTION))
			{
				sect_node = sect_node->next_node;
			}
		}
		else if(!isempty(lpszSection))
		{
			//создаём секцию если таковая не была найдена
			if((sect_node = ini_add_node(ini_node)) == NULL)
			{
				return INI_ALLOC_FAILURE;
			}

			sect_node->szSection = (char *)zalloc(strlen(lpszSection)+1);
			if(!sect_node->szSection)
			{
				return INI_ALLOC_FAILURE;
			}
			sect_node->node_id |= INI_SECTION;
			strcpy(sect_node->szSection, lpszSection);
		}

		if((temp_node = (LPINI_NODE)zalloc(sizeof(INI_NODE))) == NULL)
		{
			return INI_ALLOC_FAILURE;
		}
		memset(temp_node, 0, sizeof(INI_NODE));

		temp_node->prev_node = sect_node;
		if(sect_node)
		{
			temp_node->next_node = sect_node->next_node;
			sect_node->next_node = temp_node;
		}
		else
		{
			temp_node->next_node = *ini_node;
			*ini_node = temp_node;
		}

		if(temp_node->next_node)
		{
			temp_node->next_node->prev_node = temp_node;
		}

		if((temp_node->szKey = (char *)zalloc(strlen(lpszKey)+1)) == NULL)
		{
			return INI_ALLOC_FAILURE;
		}
		temp_node->node_id |= INI_KEY;
		strcpy(temp_node->szKey, lpszKey);
	}

	if(!isempty(lpszComment))
	{
		if((temp_node->szComment = (char *)zalloc(strlen(lpszComment)+1)) == NULL)
		{
			return INI_ALLOC_FAILURE;
		}
		temp_node->node_id |= INI_COMMENT;
		strcpy(temp_node->szComment, lpszComment);
	}

	if(!isempty(lpszValue))
	{
		va_list arguments;

		va_start(arguments, lpszValue);
		vasprintf(&temp_node->szValue, lpszValue, arguments);
		va_end(arguments);

		temp_node->node_id |= INI_VALUE;
	}

	return INI_SUCCESS;
}

long ini_get_long(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, long lpszDefault)
{
	const char *temp_lpstr = ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(temp_lpstr))
	{
		return atol(temp_lpstr);
	}

	return lpszDefault;
}

double ini_get_double(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, double lpszDefault)
{
	const char *temp_lpstr = ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(temp_lpstr))
	{
		return atof(temp_lpstr);
	}

	return lpszDefault;
}

int ini_get_int(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, int lpszDefault)
{
	const char *temp_lpstr = ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(temp_lpstr))
	{
		return atoi(temp_lpstr);
	}

	return lpszDefault;
}

__int64 ini_get_int64(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, __int64 lpszDefault)
{
	const char *temp_lpstr = ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(temp_lpstr))
	{
		return _atoi64(temp_lpstr);
	}

	return lpszDefault;
}

int ini_get_bool(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, int lpszDefault)
{
	const char *temp_lpstr = ini_get_string(ini_node, lpszSection, lpszKey, NULL, NULL, NULL);
	if(!isempty(temp_lpstr))
	{
		return	(stricmp(temp_lpstr, "yes") == 0) ||
				(stricmp(temp_lpstr, "on") == 0) ||
				(stricmp(temp_lpstr, "true") == 0) ||
				(stricmp(temp_lpstr, "enable") == 0) ||
				atoi(temp_lpstr);
	}

	return lpszDefault;
}

int ini_free_section(LPINI_NODE *ini_node, const char *lpszSection, int fFullClear)
{
	if(!ini_node || !*ini_node)
	{
		return INI_INVALID_PARAMETER;
	}

	LPINI_NODE temp_node;
	
	if(isempty(lpszSection))
	{
		temp_node = *ini_node;
	}
	else
	{
		temp_node = ini_get_node(*ini_node, lpszSection, NULL);
	}

	if(temp_node)
	{
		LPINI_NODE temp_next = temp_node->next_node;

		if(fFullClear) //clear with section name
		{
			ini_free_node(ini_node, temp_node);
		}

		while(temp_next && !(temp_next->node_id & INI_SECTION))
		{
			temp_node = temp_next->next_node;
			ini_free_node(ini_node, temp_next);
			temp_next = temp_node;
		}
	}

	return INI_SUCCESS;
}

int ini_crc_check_del(LPINI_NODE *ini_node)
{
	unsigned long orign_checksum = 0L;
	unsigned long checksum = ~0UL;

	LPINI_NODE temp_node;

	if(temp_node = *ini_node)
	{
		while(temp_node->next_node)
		{
			if(temp_node->node_id & INI_SECTION)
			{
				crc32(checksum, temp_node->szSection);
			}
			if(temp_node->node_id & INI_KEY)
			{
				crc32(checksum, temp_node->szKey);
			}
			if(temp_node->node_id & INI_VALUE)
			{
				crc32(checksum, temp_node->szValue);
			}

			temp_node = temp_node->next_node;
		}

		/*
		if(temp_node->node_id & INI_SECTION)
		{
			crc32(checksum, temp_node->szSection);
		}
		if(temp_node->node_id & INI_KEY)
		{
			crc32(checksum, temp_node->szKey);
		}
		if(temp_node->node_id & INI_VALUE)
		{
			crc32(checksum, temp_node->szValue);
		}
		//***/
		if((temp_node->node_id == INI_COMMENT)
		&& (strlen(temp_node->szComment) == 22)
		&& strstr(temp_node->szComment, "checksum is 0x"))
		{
			orign_checksum = strtoul(&temp_node->szComment[14], NULL, 16);
			//удаляем комментарий
			ini_free_node(ini_node, temp_node);

			return (~checksum == orign_checksum);
		}
	}

	return 0;
}

unsigned long ini_crc_append(LPINI_NODE *ini_node)
{
	//использовать только перед сохранением!!!
	unsigned long checksum = 0xFFFFFFFF;

	LPINI_NODE new_node, temp_node;
	if((new_node = (LPINI_NODE)zalloc(sizeof(INI_NODE))) == NULL)
	{
		return NULL;
	}
	memset(new_node, 0, sizeof(INI_NODE));

	if(temp_node = *ini_node)
	{
		while(temp_node->next_node)
		{
			if(temp_node->node_id & INI_SECTION)
			{
				crc32(checksum, temp_node->szSection);
			}
			if(temp_node->node_id & INI_KEY)
			{
				crc32(checksum, temp_node->szKey);
			}
			if(temp_node->node_id & INI_VALUE)
			{
				crc32(checksum, temp_node->szValue);
			}

			temp_node = temp_node->next_node;
		}

		if(temp_node->node_id & INI_SECTION)
		{
			crc32(checksum, temp_node->szSection);
		}
		if(temp_node->node_id & INI_KEY)
		{
			crc32(checksum, temp_node->szKey);
		}
		if(temp_node->node_id & INI_VALUE)
		{
			crc32(checksum, temp_node->szValue);
		}

		if((temp_node->node_id == INI_COMMENT)
		&& (strlen(temp_node->szComment) == 22)
		&& strstr(temp_node->szComment, "checksum is 0x"))
		{
			temp_node = temp_node->prev_node;
			ini_free_node(ini_node, temp_node->next_node);
		}

		temp_node->next_node = new_node;
		new_node->prev_node = temp_node;
	}
	else
	{
		*ini_node = new_node;
	}

	checksum = ~checksum;

	//"checksum is 0xFFFFFFFF"
	new_node->node_id = INI_COMMENT | INI_CRC32;
	asprintf(&new_node->szComment, "checksum is 0x%.8X", checksum); //"\r\n;checksum is 0x%.8X"

	return checksum;
}
