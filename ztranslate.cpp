// переделать таким образом, чтобы ненайденые значения
// автоматически добавлялись, а на выходе всё
// flush'илось в файл [сделано]

//#include "stdafx.h"								//pf_ziminski  [2009]
#include "ztranslate.h"
#include <string.h>
#include <windows.h>
#include "zini.h"
#include "utstrings.h"
#include "zdbg.h"

#ifdef _DEBUG
#include "snprintf.h"
#include "zibuffer.h"
#endif

//* class for auto create language pack and destroy on exit
class CLangPack
{
	protected:
		INI_NODE *handle;
		langpack_info info;

	public:
		CLangPack()
		{
			handle = NULL;
			LoadLanguagePack();
		}

		~CLangPack()
		{
			FreeLanguagePack();
		}

		INI_NODE *GetHandle() const
		{
			return handle;
		}

		void SetHandle(INI_NODE *ini_node)
		{
			handle = ini_node;
		}

		langpack_info *GetInfo()
		{
			return &info;
		}
};

CLangPack cLangPackProcess;

// from Miranda IM project
unsigned long NameHashFunction(const char *szStr)
{
#if defined _M_IX86 && !defined _NUMEGA_BC_FINALCHECK && !defined NOINLINEASM
	__asm {
		xor   edx,edx
		xor   eax,eax
		mov   esi,szStr
		mov   al,[esi]
		dec   esi
		xor   cl,cl
	lph_top:	 //only 4 of 9 instructions in here don't use AL, so optimal pipe use is impossible
		xor   edx,eax
		inc   esi
		and   cl,31
		movzx eax,byte ptr [esi]
		add   cl,5
		test  al,al
		rol   eax,cl		 //rol is u-pipe only, but pairable
		                 //rol doesn't touch z-flag
		jnz   lph_top  //5 clock tick loop. not bad.

		xor   eax,edx
	}
#else
	unsigned long hash=0;
	int i;
	int shift=0;
	for(i=0;szStr[i];i++) {
		hash^=szStr[i]<<shift;
		if (shift>24) hash^=(szStr[i]>>(32-shift))&0x7F;
		shift=(shift+5)&0x1F;
	}
	return hash;
#endif
}

unsigned long LangPackHash(const char *szStr)
{
#if defined _M_IX86 && !defined _NUMEGA_BC_FINALCHECK && !defined __GNUC__
	__asm {				//this is mediocrely optimised, but I'm sure it's good enough
		xor  edx,edx
		mov  esi,szStr
		xor  cl,cl
lph_top:
		xor  eax,eax
		and  cl,31
		mov  al,[esi]
		inc  esi
		test al,al
		jz   lph_end
		rol  eax,cl
		add  cl,5
		xor  edx,eax
		jmp  lph_top
lph_end:
		mov  eax,edx
	}
#else
	unsigned long hash=0;
	int i;
	int shift=0;
	for(i=0;szStr[i];i++) {
		hash^=szStr[i]<<shift;
		if(shift>24) hash^=(szStr[i]>>(32-shift))&0x7F;
		shift=(shift+5)&0x1F;
	}
	return hash;
#endif
}

int lng_convert(INI_NODE *ini_node)
{
	unsigned long hash;

	while(ini_node)
	{
		if((ini_node->node_id & INI_KEY) && ini_node->szKey)
		{
			//hash = NameHashFunction(ini_node->szKey);
			hash = strtoul(ini_node->szKey, NULL, 16);
			free_str(ini_node->szKey);
			ini_node->szKey = (char *) hash;
		}
		ini_node = ini_node->next_node;
	}

	return 0;
}

int lng_free(INI_NODE *ini_node)
{
	LPINI_NODE temp_node;
	temp_node = ini_node;
	//*ini_node = NULL;
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

	return 0;
}

#ifdef _DEBUG
int lng_flush(LPINI_NODE ini_node, const char *lpszFileName, int fUseCRC)
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
			char temp_str[32];
			ultoa((unsigned long) temp_node->szKey, temp_str, 16);
			__addblock(&zBuffer, temp_str, strlen(temp_str));
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
#endif

LPINI_NODE lng_get_node(LPINI_NODE ini_node, const char *lpszSection, unsigned long lpszKey)
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

	if(temp_node)
	{
		do
		{
			if((temp_node->node_id & INI_KEY)
			&& (lpszKey == (unsigned long) temp_node->szKey))
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

#ifdef _DEBUG
int lng_put_string(LPINI_NODE *ini_node, const char *lpszSection, unsigned long lpszKey, const char *lpszComment, const char *lpszValue, ...)
{
	// не использовать ключи без секций в файле с секциями!
	// код для этого не предусмотрен.

	if(!ini_node)
	{
		return INI_INVALID_PARAMETER;
	}

	LPINI_NODE temp_node = NULL;
	LPINI_NODE sect_node = *ini_node;

	if(!isempty(lpszSection))
	{
		sect_node = lng_get_node(*ini_node, lpszSection, NULL);
	}

	if(sect_node)
	{
		temp_node = lng_get_node(sect_node, lpszSection, lpszKey);
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

		temp_node->node_id |= INI_KEY;
		temp_node->szKey = (char *) lpszKey;
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

const char *lng_get_string(LPINI_NODE *ini_node, const char *lpszSection, unsigned long lpszKey, const char *lpszDefault)
#else
const char *lng_get_string(LPINI_NODE ini_node, const char *lpszSection, unsigned long lpszKey, const char *lpszDefault)
#endif
{
#ifdef _DEBUG
	LPINI_NODE temp_node = lng_get_node(*ini_node, lpszSection, lpszKey);
#else
	LPINI_NODE temp_node = lng_get_node(ini_node, lpszSection, lpszKey);
#endif
	if(!temp_node || ((temp_node->node_id & INI_VALUE) != INI_VALUE) || !temp_node->szValue)
	{
		if(lpszDefault)
		{
#ifdef _DEBUG
			lng_put_string(ini_node, lpszSection, lpszKey, NULL, "%s", lpszDefault);
#endif
			return lpszDefault;
		}
		else
		{
			return emptystring;
		}
	}

	return temp_node->szValue;
}

// загружаем файл с переводом
int LoadLanguagePack()
{
	INI_NODE *ini_node;
	char *exe_path;

	ini_node = NULL;
	exe_path = (char *) zalloc(MAX_PATH);
	if(exe_path)
	{
		GetModuleFileName(NULL, exe_path, MAX_PATH);
		strippath(exe_path, FPI_DIR);
	}
	else
	{
		exe_path = nullstring;
	}

	//GetUserDefaultUILanguage();
	alloc_strcat(&exe_path, "langpack.ini");
	ini_load(&ini_node, exe_path);
	lng_convert(ini_node);

	free_str(exe_path);

	langpack_info *info;

	info = cLangPackProcess.GetInfo();


#ifdef _DEBUG
	info->file_name = "langpack.ini";
	info->mod_name = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("mod_name"), "langpack");
	info->version = strtoul(lng_get_string(&ini_node, "Info", NameHashFunction("Version"), "1"), NULL, 10);
	info->flags = strtoul(lng_get_string(&ini_node, "Info", NameHashFunction("Flags"), "0"), NULL, 10);
	info->name = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("Name"), "LangPack name undefined");
	info->copyright = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("Copyright"), "Copyright undefined");
	info->description = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("Description"), "Description undefined");
	info->mail = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("E-Mail"), "none@none.none");
	info->url = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("URL"), "http://www.example.com/langpack/home/");
	info->update = (char *) lng_get_string(&ini_node, "Info", NameHashFunction("Update"), "http://www.example.com/langpack/update/script.php?app=%mod_name%&build=%build%");
#else
	info->file_name = "langpack.ini";
	info->mod_name = (char *) lng_get_string(ini_node, "Info", NameHashFunction("mod_name"), "langpack");
	info->version = strtoul(lng_get_string(ini_node, "Info", NameHashFunction("Version"), "1"), NULL, 10);
	info->flags = strtoul(lng_get_string(ini_node, "Info", NameHashFunction("Flags"), "0"), NULL, 10);
	info->name = (char *) lng_get_string(ini_node, "Info", NameHashFunction("Name"), "LangPack name undefined");
	info->copyright = (char *) lng_get_string(ini_node, "Info", NameHashFunction("Copyright"), "Copyright undefined");
	info->description = (char *) lng_get_string(ini_node, "Info", NameHashFunction("Description"), "Description undefined");
	info->mail = (char *) lng_get_string(ini_node, "Info", NameHashFunction("E-Mail"), "none@none.none");
	info->url = (char *) lng_get_string(ini_node, "Info", NameHashFunction("URL"), "http://www.example.com/langpack/home/");
	info->update = (char *) lng_get_string(ini_node, "Info", NameHashFunction("Update"), "http://www.example.com/langpack/update/script.php?app=%mod_name%&build=%build%");
#endif

	cLangPackProcess.SetHandle(ini_node);

	return 0;
}

int FreeLanguagePack()
{
	INI_NODE *ini_node;

	ini_node = cLangPackProcess.GetHandle();
	cLangPackProcess.SetHandle(NULL);

#ifdef _DEBUG
	char *exe_path;

	exe_path = (char *) zalloc(MAX_PATH);
	if(exe_path)
	{
		GetModuleFileName(NULL, exe_path, MAX_PATH);
		strippath(exe_path, FPI_DIR);
	}
	else
	{
		exe_path = nullstring;
	}

	//GetUserDefaultUILanguage();
	alloc_strcat(&exe_path, "langpack.ini");
	lng_flush(ini_node, exe_path, 0);
	free_str(exe_path);
#endif

	lng_free(ini_node);

	return 0;
}

char *Translate(char *text, unsigned long line)
{
	// если файл с переводом загружен, то берём хеш английской строки и по ней ищем перевод

#ifdef _DEBUG
	INI_NODE *ini_node;
	char *temp_str;
	unsigned long hash;

	hash = NameHashFunction(text) ^ line;
	ZTRACE("%.8X=\"%s\"\n", hash, text);

	ini_node = cLangPackProcess.GetHandle();
	temp_str = (char *) lng_get_string(&ini_node, "LanguagePack", hash, text);
	cLangPackProcess.SetHandle(ini_node);

	return temp_str;
#else
	return (char *) lng_get_string(cLangPackProcess.GetHandle(), "LanguagePack", NameHashFunction(text) ^ line, text);
#endif
}

/*
unsigned long LangPackVersion()
{
#ifdef _DEBUG
	INI_NODE *ini_node;
	char *temp_str;

	ini_node = cLangPackProcess.GetHandle();
	temp_str = (char *) lng_get_string(&ini_node, "LanguagePack", hash, text);
	cLangPackProcess.SetHandle(ini_node);

	return temp_str;
#else
	return strtoul(lng_get_string(&cLangPackProcess.GetHandle(), "Info", NameHashFunction("Version"), "1"), NULL, 10);
#endif
}

char *LangPackFile()
{
	return "langpack.ini";
}
*/

langpack_info *LangPackInfo()
{
	return cLangPackProcess.GetInfo();
}
