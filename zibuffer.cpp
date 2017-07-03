//#include "stdafx.h"								// dEmon/edge  [2002]
#include "zibuffer.h"
#include "utstrings.h"
#include <stdlib.h>

HANDLE __initbufferedread(const char *szFileName, EXTBUFFER *zBuffer, unsigned long dwBufferSize)
{
	if((zBuffer->lpBuffer = (char *)zalloc(dwBufferSize)) == NULL)
	{
		return 0L;
	}
	zBuffer->dwBufferSize = dwBufferSize;
	zBuffer->dwFilePos = 0;
	//zBuffer->dwOptions = dwOptions;
	if((zBuffer->hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
	{
		if((zBuffer->dwFileSize = GetFileSize(zBuffer->hFile, NULL)) <= 0)
		{
			__destructbufferedreadwrite(zBuffer);
			return 0L;
		}
//**/	zBuffer->dwBufferFill = zBuffer->dwFileSize < zBuffer->dwBufferSize ? zBuffer->dwFileSize : zBuffer->dwBufferSize;
//**/	if(ReadFile(zBuffer->hFile, zBuffer->lpBuffer, zBuffer->dwBufferFill, &zBuffer->dwBufferFill, NULL) == NULL)
		if(ReadFile(zBuffer->hFile, zBuffer->lpBuffer, zBuffer->dwBufferSize, &zBuffer->dwBufferFill, NULL) == NULL)
		{
			__destructbufferedreadwrite(zBuffer);
			return 0L;
		}
	} 
	else
	{
		zfree(zBuffer->lpBuffer);
		return 0L;
	}
	return zBuffer->hFile;
}

/*
HANDLE __duplicatebufferedreadwrite(EXTBUFFER *zBufferIn, EXTBUFFER *zBufferOut, unsigned long dwBufferSize)
{
	zBufferOut->lpBuffer = (char *)zalloc(dwBufferSize);
	zBufferOut->dwBufferSize = dwBufferSize;
	zBufferOut->dwFilePos = 0;
	zBufferOut->dwBufferFill = 0;
	zBufferOut->dwFileSize = zBufferIn->dwFileSize;
	return zBufferOut->hFile = zBufferIn->hFile;
}
*/

int __get(EXTBUFFER *zBuffer, unsigned long dwFilePos)
{
	// backward so bad! --> add
	// ***BRW_FORWARD|BRW_BACKWARD|BRW_CENTERED***
	if((dwFilePos >= zBuffer->dwFilePos) && (dwFilePos - zBuffer->dwFilePos < zBuffer->dwBufferFill))
	{
		return zBuffer->lpBuffer[dwFilePos - zBuffer->dwFilePos];
	}
	else if(dwFilePos < zBuffer->dwFileSize)
	{
		zBuffer->dwFilePos = SetFilePointer(zBuffer->hFile, dwFilePos, NULL, FILE_BEGIN);
		if(ReadFile(zBuffer->hFile, zBuffer->lpBuffer, zBuffer->dwBufferSize, &zBuffer->dwBufferFill, NULL) == NULL)
		{
			return -1;
		}
		return zBuffer->lpBuffer[0];
	}

	return -1; //EOF
}

HANDLE __initbufferedwrite(const char *szFileName, EXTBUFFER *zBuffer, unsigned long dwBufferSize)
{
	zBuffer->lpBuffer = (char *)zalloc(dwBufferSize);
	zBuffer->dwBufferSize = dwBufferSize;
	zBuffer->dwBufferFill = 0;
	zBuffer->dwFilePos = 0;
	zBuffer->dwFileSize = 0;
	if((zBuffer->hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		__destructbufferedreadwrite(zBuffer);
		return 0L;
	}
	return zBuffer->hFile;
}

void __add(EXTBUFFER *zBuffer, char szChar)
{
	if(zBuffer->dwBufferFill >= zBuffer->dwBufferSize)
	{
		__donebufferedwrite(zBuffer);
	}
	zBuffer->lpBuffer[zBuffer->dwBufferFill++] = szChar;
}

unsigned long __donebufferedwrite(EXTBUFFER *zBuffer)
{
	//flush buffer
	register unsigned long dwNumberOfBytesWritten;
	if(WriteFile(zBuffer->hFile, zBuffer->lpBuffer, zBuffer->dwBufferFill, &dwNumberOfBytesWritten, NULL) == NULL)
	{
		return zBuffer->dwBufferFill;
	}
	if(dwNumberOfBytesWritten != zBuffer->dwBufferFill)
	{
		register unsigned long dw = 0;
		while(dw+dwNumberOfBytesWritten <= zBuffer->dwBufferFill)
		{
			zBuffer->lpBuffer[dw] = zBuffer->lpBuffer[dw+dwNumberOfBytesWritten];
			dw++;
		}
		return zBuffer->dwBufferFill = dw;
	}
	return zBuffer->dwBufferFill = 0L;
}

void __destructbufferedreadwrite(EXTBUFFER *zBuffer)
{
	if(zBuffer->hFile != INVALID_HANDLE_VALUE) CloseHandle(zBuffer->hFile);
	zBuffer->dwBufferFill = 0;
	zBuffer->dwBufferSize = 0;
	zBuffer->dwFileSize = 0;
	zBuffer->dwFilePos = 0;
	if(zBuffer->lpBuffer) zfree(zBuffer->lpBuffer);
}

int __getblock(EXTBUFFER *zb, char *out, unsigned long st, unsigned long en)
{
	//buffer size must be (end - start + 2) именно два!!!
	if(st > en)
	{
		*out = '\0';
		return 1;
	}
	unsigned long dwBytesReaded=0;
	//unsigned long dwEndDelta=0;
	if(st < zb->dwFilePos)								//1
	{
		SetFilePointer(zb->hFile, st, NULL, FILE_BEGIN);
		dwBytesReaded = (en < zb->dwFilePos)?(en-st+1):(zb->dwFilePos-st);
		if(ReadFile(zb->hFile, out, dwBytesReaded, &dwBytesReaded, NULL) == NULL)
		{
			return 2;
		}

		if(en >= zb->dwFilePos)							//1.2
		{
			//dwBytesReaded = ((en>zb->dwFilePos+zb->dwBufferFill)?zb->dwBufferFill:en-zb->dwFilePos)+1;
			dwBytesReaded = (en >= zb->dwFilePos + zb->dwBufferFill)?zb->dwBufferFill:(en - zb->dwFilePos+1);
			memcpy(&out[zb->dwFilePos - st], zb->lpBuffer, dwBytesReaded);
		}
	}

	if(zb->dwFilePos + zb->dwBufferFill <= en)			//2
	{
		SetFilePointer(zb->hFile, (zb->dwFilePos+zb->dwBufferFill<st)?st:zb->dwFilePos+zb->dwBufferFill, NULL, FILE_BEGIN);
		dwBytesReaded = (zb->dwFilePos + zb->dwBufferFill < st)?(en - st + 1):(en - zb->dwFilePos - zb->dwBufferFill + 1);
		if(ReadFile(zb->hFile, &out[((zb->dwFilePos+zb->dwBufferFill<st)?st:zb->dwFilePos+zb->dwBufferFill)-st], dwBytesReaded, &dwBytesReaded, NULL) == NULL)
		{
			return 2;
		}
	}

	if(		(st >= zb->dwFilePos)						//3
		&&	(st < zb->dwFilePos + zb->dwBufferFill))
	{
		dwBytesReaded = (en >= zb->dwFilePos + zb->dwBufferFill)?(zb->dwFilePos + zb->dwBufferFill - st):(en - st + 1) /*+1*/;
		memcpy(out, &zb->lpBuffer[st - zb->dwFilePos], dwBytesReaded);
	}

	out[en-st+1] = '\0';
	return 0;
}

void __addblock(LPEXTBUFFER zBuffer, const char *szBuffer, unsigned long dwSize)
{
	unsigned long temp_pos = 0, tmp;
	do
	{
		if(zBuffer->dwBufferFill >= zBuffer->dwBufferSize)
		{
			__donebufferedwrite(zBuffer);
		}
		tmp = (zBuffer->dwBufferSize-zBuffer->dwBufferFill<dwSize-temp_pos)?zBuffer->dwBufferSize-zBuffer->dwBufferFill:dwSize-temp_pos;
		memcpy(&zBuffer->lpBuffer[zBuffer->dwBufferFill], &szBuffer[temp_pos], tmp);
		temp_pos += tmp;
		zBuffer->dwBufferFill += tmp;
	}
	while(temp_pos < dwSize);
}


unsigned long __seek(EXTBUFFER *zBuffer, unsigned long dwFromPos, const char *lpszChars, int fErrorOnEof)
{
	//seek any of chars in zBuffer from dwFromPos position
	//return position any of chars [from "0123..."]
	// example:
	//   in->zBuffer.sz[] = "param=10";
	//   _seek(zBuffer, 0, " =.,:");
	//   _seek() return 5 (?!?)
	while(!__iseof(zBuffer, dwFromPos)) 
	{
	    if(strchr(lpszChars, __get(zBuffer, dwFromPos)))
		{
			return dwFromPos;
		}
		dwFromPos++;
	}
	return (fErrorOnEof)?(BRW_INVALID_POINTER):(dwFromPos-1); //смотри __seekcsp();
}

unsigned long __seekcsp(EXTBUFFER *zBuffer, unsigned long dwFromPos, unsigned long *lpdwStartPos, unsigned long *lpdwEndPos, const char *lpszChars, int fErrorOnEof)
{
	// seek clear spaces (trim).
	// ищет один из символов в файле возвращая информацию о впереди
	// идущей "строке".
	//
	// в отличии от __seekcsps() НЕ пропускает "строки" и блоки из
	// скобок!
	//
	// [out] lpdwStartPos - возвращает позицию в файле первого не
	//                      пробела и не таба от начала поиска.
	// [out] lpdwEndPos   - возвращает позицию в файле последнего не
	//                      пробела и не таба перед найденым символом.
	// в случае если символ не был найден или до найденного символа
	// были обни пробелы или табы, то lpdwStartPos > lpdwEndPos.
	// return value: позиция искомого символа в файле, если же
	// символ не найден, то возвращает ~0L, если же fErrorOnEof=FALSE
	// позицию в файле равную file_size-1.

	register char chCurrent;
	register int StartPositionSet = FALSE;

	if(lpdwEndPos) *lpdwEndPos = 0;
	if(lpdwStartPos) *lpdwStartPos = 0;

	while(!__iseof(zBuffer, dwFromPos)) 
	{
		chCurrent = __get(zBuffer, dwFromPos);
	    if(strchr(lpszChars, chCurrent))
		{
			if(!StartPositionSet)
			{
				if(lpdwStartPos) *lpdwStartPos = 1;		//было dwFromPos;
				if(lpdwEndPos) *lpdwEndPos = 0;			//было dwFromPos;
			}

			return dwFromPos;
		}

		if((chCurrent != 0x20) && (chCurrent != 0x09))
		{
			if(!StartPositionSet)
			{
				if(lpdwStartPos) *lpdwStartPos = dwFromPos;
				if(lpdwEndPos) *lpdwEndPos = dwFromPos;
				StartPositionSet = TRUE;
			}
			else
			{
				if(lpdwEndPos) *lpdwEndPos = dwFromPos;
			}
		}
		dwFromPos++;
	}

	if(!StartPositionSet)
	{
		if(lpdwStartPos) *lpdwStartPos = 1;	//было dwFromPos - это ошибка?
		if(lpdwEndPos) *lpdwEndPos = 0;		//было dwFromPos - это ошибка?
	}

	return (fErrorOnEof)?(BRW_INVALID_POINTER):(dwFromPos-1); //изменил! добавил -1!
	//07.03.2004: вернул -1. т.о. по достижению конца файла если
	//            fErrorOnEof = TRUE функция фозвращает ~0L в другом
	//            случае возвращает конечную позицию в файле!
	//***
	//пока что убрал -1, теперь возвращает уже не существующую
	//позицию в файле! нужно для ini_load()
	//если нужен -1 то аргументируй это здесь!
	//***
	//вернул -1 и сделал так что если в промемежитке между dwFromPos
	//и сепаратором нету символов или одни пробелы (пустое пространство)
	//то возвращаем (enpos < stpos)
	//аргумент2:
	//в ini_load() требуется чтобы всегда возвращался символ на котором
	//остановились т.е. он должен быть в диапозоне lpszChars, а если
	//сделать -1 то получается что символ в него не входит!
	//из-за этого могут возникнуть ошибки! [бред]
}

unsigned long __seekcsps(EXTBUFFER *zBuffer, unsigned long dwFromPos, unsigned long *lpdwStartPos, unsigned long *lpdwEndPos, const char *lpszChars, int fErrorOnEof)
{
	// seek clear spaces (trim) exclude strings and blocks from search.
	//
	// ищет один из символов в файле пропуская "строки" и блоки из скобок
	// возвращая информацию о впереди идущей "строке".
	//
	// смотри __seekcsp();
	// return value: в дополнение к __seekcsp() возврещает значение
	// BRW_INVALID_POINTER про не возможности обхода блока из скобок
	// или сторок.

	register char chCurrent;
	register int StartPositionSet = FALSE;

	if(lpdwEndPos) *lpdwEndPos = 0;
	if(lpdwStartPos) *lpdwStartPos = 0;

	while(!__iseof(zBuffer, dwFromPos)) 
	{
		chCurrent = __get(zBuffer, dwFromPos);
	    if(strchr(lpszChars, chCurrent))
		{
			if(!StartPositionSet)
			{
				if(lpdwStartPos) *lpdwStartPos = 1;		//было dwFromPos;
				if(lpdwEndPos) *lpdwEndPos = 0;			//было dwFromPos;
			}

			return dwFromPos;
		}
		
		if((chCurrent != 0x20) && (chCurrent != 0x09))
		{
			if(!StartPositionSet)
			{
				if(lpdwStartPos) *lpdwStartPos = dwFromPos;
				StartPositionSet = TRUE;
			}
			
			// * skip ***
			if(strchr("\"\'", chCurrent))
			{
				dwFromPos = __skipstring(zBuffer, dwFromPos);
				if(dwFromPos == BRW_INVALID_POINTER) 
				{
					if(lpdwStartPos) *lpdwStartPos = 1;
					if(lpdwEndPos) *lpdwEndPos = 0;
					return BRW_INVALID_POINTER;
				}
			}
			else if(strchr("([{", chCurrent))
			{
				dwFromPos = __skipblock(zBuffer, dwFromPos);
				if(dwFromPos == BRW_INVALID_POINTER)
				{
					if(lpdwStartPos) *lpdwStartPos = 1;
					if(lpdwEndPos) *lpdwEndPos = 0;
					return BRW_INVALID_POINTER;
				}
			}
			// * end skip ***

			if(lpdwEndPos) *lpdwEndPos = dwFromPos;
		}
		dwFromPos++;
	}

	if(!StartPositionSet)
	{
		if(lpdwStartPos) *lpdwStartPos = 1;	//было dwFromPos - это ошибка?
		if(lpdwEndPos) *lpdwEndPos = 0;		//было dwFromPos - это ошибка?
	}

	return (fErrorOnEof)?(BRW_INVALID_POINTER):(dwFromPos-1); //изменил! добавил -1!
}

unsigned long __skipstring(EXTBUFFER *zBuffer, unsigned long dwFromPos)
{
	// dwFromPos должн начинаться с \" or \' из этого определяем
	// каким символом должна закрыться строка.
	// возвращает позицию закрывающейся кавычки в файле или
	// BRW_INVALID_POINTER если не найдена закрывающаяся кавычка.

	register char chEnd = __get(zBuffer, dwFromPos++);
	register char chCurrent;

	//if(__instrstr("\"\'", lpszString[dwCurPos]))
	//{
		//char szEndChar = lpszString[dwCurPos++];
		while(!__iseof(zBuffer, dwFromPos))
		{
			chCurrent = __get(zBuffer, dwFromPos);
			if(chCurrent == '\\')
			{
				if(__iseof(zBuffer, ++dwFromPos))
				{
					break;
				}
			} else
			if(chCurrent == chEnd)
			{
				return dwFromPos;
			}
			dwFromPos++;
		}
	//}
	return BRW_INVALID_POINTER;
}

unsigned long __skipblock(EXTBUFFER *zBuffer, unsigned long dwFromPos)
{
	// пропускает блок заключённый в скобки.
	// строка должна начинаться с открывающейся скобки.
	// возвращает позицию закрывающейся скобки или
	// BRW_INVALID_POINTER если не обнаружена и достигнут конец файла.

	register char chOpen = __get(zBuffer, dwFromPos++);
	register char chClose = charInvert(chOpen);
	register char chCurrent;
	register unsigned long dwOpenedBlocks = 1;

	/*
	// дополнительная проверка, можно убрать!
	if(!__instrstr("({[", lpszString[0]))
	{
		return 0L;
	}
	*/
	while(!__iseof(zBuffer, dwFromPos)) // && (dwOpenedBlocks > 0)
	{
		chCurrent = __get(zBuffer, dwFromPos);
		if(chCurrent == chClose)
		{
			if(--dwOpenedBlocks <= 0)
			{
				return dwFromPos;
			}
		} else
		if(chCurrent == chOpen)
		{
			dwOpenedBlocks++;
		} else
		if(__instrstr("\"\'", chCurrent))
		{
			dwFromPos = __skipstring(zBuffer, dwFromPos);
			if(dwFromPos == BRW_INVALID_POINTER) break; //error
		}
		dwFromPos++;
	}
	return BRW_INVALID_POINTER;
}

unsigned long __seekstring(EXTBUFFER *zBuffer, unsigned long from_pos, const char *seek_string)
{
	// что-то типа strstr только для bufferedread
	// return value: позиция первого символа в файле или
	// BRW_INVALID_POINTER если строка не найдена.

	// note:
	// уменьшать dwFromPos если пошли по ложному пути!!! [complite]

	register unsigned long seek_pos=0;

	while(!__iseof(zBuffer, from_pos))
	{
		if(__get(zBuffer, from_pos) == seek_string[seek_pos])
		{
			seek_pos++;
			if(seek_string[seek_pos] == '\0') return from_pos-(seek_pos -1);
		}
		else if(seek_pos > 0)
		{
			// сделано для того чтобы не возвращатся к предидущим
			// позициям в файле, а обойтись искомой строкой.
			int n = 1;
			while(seek_pos-n > 0)
			{
				if(strncmp(seek_string, seek_string+n, seek_pos-n) == 0)
				{
					break;
				}
				n++;
			}
			seek_pos -= n;
			continue;
			// old school
			//from_pos -= seek_pos;
			//seek_pos = 0;
		}
		from_pos++;
	}
	return BRW_INVALID_POINTER;
}

char *__alloc_getblock(EXTBUFFER *zBuffer, unsigned long dwStartPos, unsigned long dwEndPos)
{
	if(dwStartPos <= dwEndPos)
	{
		char *lpstr = (char *) zalloc(dwEndPos-dwStartPos+2);
		if(lpstr)
		{
			memset(lpstr, 32, dwEndPos-dwStartPos+1);
			lpstr[dwEndPos-dwStartPos+1] = '\0';
			__getblock(zBuffer, lpstr, dwStartPos, dwEndPos);
			return lpstr;
		}
	}

	return NULL;
}
