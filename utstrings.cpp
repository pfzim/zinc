//#include "stdafx.h"									//dEmon (c) 2002
#include "utstrings.h"
#include "zdbg.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snprintf.h"
#include <windows.h>

//char nullstring[] = "\0(null)";
char *nullstring = "\0(null)";

// work like trim() without reallocate buffer
char *strippath(char *path, unsigned long flags)
{
	char *ec;
	char *sc;
	char *ext;
	char *dir;

	if(!path)
	{
		return nullstring;
	}

    /*
	// method 1 - work - checked!
	if(!*path)
	{
		return path;
	}

	ec = path;
	sc = ec;
	dir = path;
	ext = NULL;

	while(*ec)
	{
		if((*ec == '\\') || (*ec == '/'))
		{
			ext = NULL;
			if(~flags & FPI_DIR)
			{
				sc = path;
				ec++;
				continue;
			}
			dir = sc + 1;
		}
		else if(*ec == '.')
		{
			ext = sc;
		}
		*sc = *ec;
		ec++;
		sc++;
	}

	*sc = '\0'; // here error if path = nullstring

	if(ext && (~flags & FPI_FEXT))
	{
		*ext = '\0';
		ext = NULL;
	}

	if(~flags & FPI_FNAME)
	{
		if(ext)
		{
			strdel(dir, ext - dir);
		}
		else
		{
			*dir = '\0';
		}
	}
	//*/

	// method 2 - work - checked!
	ec = path;
	ext = NULL;
	dir = NULL;

	while(*ec)
	{
		if((*ec == '\\') || (*ec == '/'))
		{
			dir = ec;
			ext = NULL;
		}
		else if(*ec == '.')
		{
			ext = ec;
		}
		ec++;
	}

	//unsigned long len;
	//len = ec - path + 1;

	// backward method work too
	//dir = path + strlen(path);
	//while((dir != path) && (*dir != '\\') && (*dir != '/'))
	//{
	//	if((*dir == '.') && !ext)
	//	{
	//		ext = dir;
	//	}
	//	dir--;
	//}

	if(ext)
	{
		if(~flags & FPI_FEXT)
		{
			*ext = '\0';
		}
		ec = ext;
	}

	sc = path;

	if(dir && (flags & FPI_DIR))
	{
		sc = dir + 1;
	}

	if(flags & FPI_FNAME)
	{
		if(dir)
		{
			ec = dir + 1;
		}
		else
		{
			ec = path;
		}
	}

	if(sc != ec)
	{
		//memdel(path, len, sc, ec - sc);
		strdel(sc, ec - sc);
	}
	//*/

	return path;
}

char *alloc_filepath(const char *lpszFullPath, unsigned long dwOption)
{
	// вижу ошибку: не извлечет расширение или просто имя файла из "filename.ext"
	// alloc_filepath("filename.ext", FPI_FEXT) вернет nullstring, что не правильно
	// вместо этой функции ипользовать strippath

	//dwOption can be
	//FPI_DIR			0x01
	//FPI_FNAME			0x02
	//FPI_FEXT			0x04
	char *lpstr;
	char *lpdir = (char *)lpszFullPath;
	char *lpfile = strrchr(lpdir, '\\');	// filename
	char *lpext;							// extention
	char *lpst = NULL;						// start
	char *lpen = NULL;						// end
	if(lpfile)
	{
		lpfile++;						// file name start here
		lpext = strrchr(lpfile, '.');	// extention start here

		if(dwOption & FPI_DIR)
		{
			lpst = lpdir;
			lpen = lpfile;
		}
		if(lpext)
		{
			if(dwOption & FPI_FNAME)
			{
				if(!lpst) lpst = lpfile;
				lpen = lpext;
			}
			if(dwOption & FPI_FEXT)
			{
				if(!lpst) lpst = lpext;
				lpen = lpext + strlen(lpext);
			}
		}
		if(lpen-lpst >0)
		{
			if(lpstr = (char *) zalloc(lpen-lpst+1))
			{
				strncpy_tiny(lpstr, lpst, lpen-lpst);
				//*(lpstr+(int)(lpen-lpst)) = '\0';
				//MessageBox(NULL, lpstr, lpszFullPath, MB_OK);
				return lpstr;
			}
		}
	}
	return nullstring;
}

unsigned long _maxprm(const char *lpszCommand, const char *lpszSeparators)
{
	// _maxprm("unsigned long dwOptions=0x00000000", " =") == 2
	// _maxprm("lpszString,(DWORD)lpSize,MAKEWORD(800,600)", ",") == 2
	//расчитано на то что строку привели в прядок cut()
	//удалили лишние пробелы табы и т.п.
	unsigned long dwCurPos = 0, dwParamCount = 0;
	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			//AND (lpszCommand[dwCurPos+1] != '\0')
			//забей, может быто параметр пустой "param1,,param3"
			dwParamCount++; // !!!!
		} else
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else
		if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}
	
	//исправлено: возвращает количество параметров разделёных
	//сепараторами (включая пустые параметры и первый (единственный)
	//ничем не разделёный)
	if(dwCurPos)
	{
		dwParamCount++;
	}

	return dwParamCount;
}

/* оказывается есть strchr()
extern inline char *__instrstr(const char *lpszChars, char chChar)
{
	char szChar[] = {chChar, '\0'};
	return strstr(lpszChars, szChar);
}
*/

unsigned long _cprs(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators)
{
	// возвращает позицию где нашёл сепаратор номер начиная с нуля
	// переделал!!! теперь учитывает скобки и сторки!
	// думаю что нужно ещё не много переделать! убрать использование dwParamNum
	unsigned long dwCurPos = 0;
	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			dwParamNum--;
		}
		if(dwParamNum == 0) //именно здесь так должно быть!
		{
			break;
		}
		// далее не включил в else if потому что если сепаратор из {[("'
		// и более чем первый встретившийся то нужно пропустить как блок
		// возможно же что будут вложеные а их считать не надо!
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}
	if(lpszCommand[dwCurPos] != '\0')
	{
		dwParamNum--; // ???
	}
	return dwCurPos; //-1
}

// сделать аналогичную функцию pai, которая будет возвращать числовой параметр через strtoul()
char *_pa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators)
{
	static char szString[_MAX_PATH];
	unsigned long dwCurPos = 0, dwInPos = 0;
	//, dwParamNum = 0;
	/* зачем???
	if(!lpszCommand)
	{
		szString[0] = '\0';
		return szString;
	}
	*/
	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			if(dwParamNum == 0) break;
			dwParamNum--;
			if(dwParamNum == 0) dwInPos = dwCurPos+1;
		} else
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else
		if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}

	if(dwParamNum == 0)
	{
		//strncpy(szString, &lpszCommand[dwInPos], min(dwCurPos-dwInPos, MAX_PATH));
		//szString[min(dwCurPos-dwInPos, MAX_PATH)] = '\0';
		strncpy_tiny(szString, &lpszCommand[dwInPos], __min(dwCurPos-dwInPos, _MAX_PATH));
	}
	else
	{
		szString[0] = '\0';
	}
	return szString;
}

unsigned long _ulpa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators)
{
	char szString[_MAX_PATH];
	unsigned long dwCurPos = 0, dwInPos = 0;

	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			if(dwParamNum == 0) break;
			dwParamNum--;
			if(dwParamNum == 0) dwInPos = dwCurPos+1;
		} else
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else
		if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}

	if(dwParamNum == 0)
	{
		strncpy_tiny(szString, &lpszCommand[dwInPos], __min(dwCurPos-dwInPos, _MAX_PATH));
	}
	else
	{
		szString[0] = '\0';
	}
	
	return strtoul(szString, NULL, 10);
}

long _ipa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators)
{
	char szString[_MAX_PATH];
	unsigned long dwCurPos = 0, dwInPos = 0;

	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			if(dwParamNum == 0) break;
			dwParamNum--;
			if(dwParamNum == 0) dwInPos = dwCurPos+1;
		} else
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else
		if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}

	if(dwParamNum == 0)
	{
		strncpy_tiny(szString, &lpszCommand[dwInPos], __min(dwCurPos-dwInPos, _MAX_PATH));
	}
	else
	{
		szString[0] = '\0';
	}
	
	return strtol(szString, NULL, 10);
}

char *alloc_getpa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators)
{
	//делать free() после использования!!!
	char *lpstr = nullstring;
	unsigned long dwCurPos = 0, dwInPos = 0;
	//, dwParamNum = 0;
	/*
	if(!lpszCommand)
	{
		szString[0] = '\0';
		return szString;
	}
	*/
	while(lpszCommand[dwCurPos] != '\0')
	{
		if(__instrstr(lpszSeparators, lpszCommand[dwCurPos]))
		{
			if(dwParamNum == 0) break;
			dwParamNum--;
			if(dwParamNum == 0) dwInPos = dwCurPos+1;
		} else
		if(__instrstr("\"\'", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszCommand[dwCurPos]);
		} else
		if(__instrstr("([{", lpszCommand[dwCurPos]))
		{
			dwCurPos += _skipblock(&lpszCommand[dwCurPos]);
		}
		dwCurPos++;
	}

	if(dwParamNum == 0)
	{
		/*
		lpstr = (char *) zalloc(dwCurPos-dwInPos+1);
		if(lpstr)
		{
			//strncpy(lpstr, &lpszCommand[dwInPos], dwCurPos-dwInPos);
			//lpstr[dwCurPos-dwInPos] = '\0';
			strncpy_tiny(lpstr, &lpszCommand[dwInPos], dwCurPos-dwInPos);
		}
		*/
		lpstr = alloc_strncpy(&lpszCommand[dwInPos], dwCurPos-dwInPos);
	}

	return lpstr?lpstr:nullstring;
}

unsigned long _skipstring(const char *lpszString)
{
	// lpszString должна начинаться с \" or \' из этого определяем
	// каким символом должна закрыться строка
	// возвращает позицию закрывающейся кавычки от начала строки начиная с нуля
	unsigned long dwCurPos = 1;
	//if(__instrstr("\"\'", lpszString[dwCurPos]))
	//{
		//char szEndChar = lpszString[dwCurPos++];
		while(lpszString[dwCurPos] != '\0')
		{
			if(lpszString[dwCurPos] == '\\')
			{
				if(lpszString[++dwCurPos] == '\0')
				{
					break;
				}
			} else
			if(lpszString[dwCurPos] == lpszString[0])
			{
				return dwCurPos;
			}
			dwCurPos++;
		}
	//}
	return 0L;
}

unsigned long _skipspaces(const char *lpszString)
{
	//пропускает пробелы и табы
	unsigned long dwCurPos = 0;
	while((lpszString[dwCurPos] != '\0') && ((lpszString[dwCurPos] == 0x20) || (lpszString[dwCurPos] == 0x09)))
	{
		dwCurPos++;
	}
	return dwCurPos;
}

unsigned long _skipblock(const char *lpszString)
{
	// пропускает блок заключённый в скобки
	// строка должна начинаться с открывающейся скобки
	// возвращает позицию закрывающейся скобки от начала строки начиная с нуля

	/*
	if(!__instrstr("({[", lpszString[0]))
	{
		// дополнительная проверка, можно убрать!
		return 0L;
	}
	/**/
	unsigned long dwOpenedBlocks = 1;
	unsigned long dwCurPos = 1;
	char chClose = charInvert(lpszString[0]);
	while(lpszString[dwCurPos] != '\0') // && (dwOpenedBlocks > 0)
	{
		if(lpszString[dwCurPos] == chClose)
		{
			if(--dwOpenedBlocks <= 0)
			{
				return dwCurPos;
			}
		} else
		if(lpszString[dwCurPos] == lpszString[0])
		{
			dwOpenedBlocks++;
		} else
		if(__instrstr("\"\'", lpszString[dwCurPos]))
		{
			dwCurPos += _skipstring(&lpszString[dwCurPos]);
		}
		dwCurPos++;
	}
	return 0L;
}

char *alloc_getblock(const char *lpszString, unsigned long *lpdwPos)
{
	//после пользования сделать free()
	// lpdwPos -позиция закрывающейся скобки от начала строки начиная с нуля
	char *lpstr = NULL;
	unsigned long pos;
	pos = _skipblock(lpszString);
	if(lpdwPos)
	{
		*lpdwPos = pos;
	}
	if(pos)
	{
		//lpstr = (char *) zalloc(pos);
		lpstr = alloc_strncpy(lpszString+1, pos-1);
		//lpstr[pos-1] = '\0';
	}

	return lpstr?lpstr:nullstring;
}

/*
char *alloc_cut(char *lpszString)
{

}
//*/

char *alloc_killspaces(char *lpszString)
{
	//использовать free()
	char *lpstr = NULL;
	unsigned long cpos = 0, spos = 0, epos = 0;
	bool StartPositionSet = false;
	while(lpszString[cpos] != '\0')
	{
		if((lpszString[cpos] != 0x20) && (lpszString[cpos] != 0x09))
		{
			if(!StartPositionSet)
			{
				spos = cpos;
				epos = cpos;
				StartPositionSet = true;
			}
			else
			{
				epos = cpos;
			}
		}
		cpos++;
	}

	if(StartPositionSet)
	{
		lpstr = alloc_strncpy(&lpszString[spos], epos - spos +1);
		/*lpstr = (char *) zalloc(epos - spos +2);
		if(lpstr)
		{
			lpstr = alloc_strncpy_tiny(&lpszString[spos], epos - spos +1);
			//lpstr[epos - spos +1] = '\0';
		}*/
	}

	return lpstr?lpstr:nullstring;
}

char *ltrim(char *in)
{
	char *ch;
	char *ch2;

	if(!isempty(in))
	{
		ch = in;

		while(*ch && (*ch == ' ' || *ch == '\t' || *ch == '\r' || *ch == '\n'))
		{
			ch++;
		}

		ch2 = in;
		while(*ch)
		{
			*ch2 = *ch;
			ch++;
			ch2++;
		}

		*ch2 = '\0';

		/*
		while(*ch)
		{
			strcpy(in, ch);
			return in;
		}
		*/

		//return nullstring;
		// нельзя возвращать nullstring по той причине,
		// что переменная in могла быть выделена (alloc)
		//*in = '\0';
	}

	return in;
}

char *rtrim(char *in)
{
	char *ch;
	char *end;

	if(!isempty(in))
	{
		end = NULL;
		ch = in;

		while(*ch)
		{
			if(*ch != ' ' && *ch != '\t' && *ch != '\r' && *ch != '\n')
			{
				end = ch;
			}

			ch++;
		}

		if(end)
		{
			*(end+1) = '\0';
			return in;
		}

		//return nullstring;
		// нельзя возвращать nullstring по той причине,
		// что переменная in могла быть выделена (alloc)
		*in = '\0';
	}

	return in;
}

char *trim(char *in)
{
	//return ltrim(rtrim(in));

	char *ch;
	char *ch2;
	char *end;

	if(!isempty(in))
	{
		ch = in;

		while(*ch && (*ch == ' ' || *ch == '\t' || *ch == '\r' || *ch == '\n'))
		{
			ch++;
		}

		end = NULL;
		ch2 = in;

		while(*ch)
		{
			if(*ch != ' ' && *ch != '\t' && *ch != '\r' && *ch != '\n')
			{
				end = ch2;
			}

			*ch2 = *ch;
			ch++;
			ch2++;
		}

		if(end)
		{
			*(end+1) = '\0';
		}
		else
		{
			*in = '\0';
		}
	}

	return in;
}

char *ltrim_any(char *in, char *any)
{
	char *ch;
	char *ch2;
	char *tr;

	if(!isempty(in))
	{
		ch = in;
		tr = "\0";

		while(*ch && (*ch == ' ' || *ch == '\t' || *ch == '\r' || *ch == '\n'))
		{
			tr = any;
			while(*tr && *ch != *tr)
			{
				tr++;
			}

			ch++;
		}

		ch2 = in;
		while(*ch)
		{
			*ch2 = *ch;
			ch++;
			ch2++;
		}

		*ch2 = '\0';
	}

	return in;
}

char *rtrim_any(char *in, char *any)
{
	char *ch;
	char *end;
	char *tr;

	if(!isempty(in))
	{
		end = NULL;
		ch = in;

		while(*ch)
		{
			tr = any;
			while(*tr && *ch != *tr)
			{
				tr++;
			}

			if(!*tr)
			{
				end = ch;
			}

			ch++;
		}

		if(end)
		{
			*(end+1) = '\0';
			return in;
		}

		*in = '\0';
	}

	return in;
}

char *trim_any(char *in, char *any)
{
	char *ch;
	char *ch2;
	char *end;
	char *tr;

	if(!isempty(in))
	{
		ch = in;
		tr = "\0";

		while(*ch && !*tr)
		{
			tr = any;
			while(*tr && *ch != *tr)
			{
				tr++;
			}

			ch++;
		}

		end = NULL;
		ch2 = in;

		while(*ch)
		{
			tr = any;
			while(*tr && *ch != *tr)
			{
				tr++;
			}

			if(!*tr)
			{
				end = ch;
			}

			*ch2 = *ch;
			ch++;
			ch2++;
		}

		if(end)
		{
			*(end+1) = '\0';
		}
		else
		{
			*in = '\0';
		}
	}

	return in;
}

char *killspaces(char *in)
{
	char *ch1;
	char *ch2;

	if(!isempty(in))
	{
		ch1 = in;
		ch2 = in;

		while(*ch2)
		{
			if(*ch2 != ' ' && *ch2 != '\t' && *ch2 != '\r' && *ch2 != '\n')
			{
				*ch1 = *ch2;
				ch1++;
			}

			ch2++;
		}

		*ch1 = *ch2;
	}
	return in;
}

// убивает первый слеш в "\\" и "\/"
char *killdoubleslashes(char *in)
{
	char *ch1;
	char *ch2;

	if(!isempty(in))
	{
		ch1 = in;
		ch2 = in;

		while(*ch2)
		{
			//if(((*ch2 != '\\') && (*ch2 != '/')) || ((*(ch2+1) != '\\') && (*(ch2+1) != '/'))) было так - мне кажется ошибка
			if((*ch2 != '\\') || ((*(ch2+1) != '\\') && (*(ch2+1) != '/')))
			{
				*ch1 = *ch2;
				ch1++;
			}
			ch2++;
		}

		*ch1 = *ch2;
	}
	return in;
}

char *strncpy_tiny(char *out, const char *in, size_t len)
{
	// len -- exclude null-termanator
	//
	// out -- size = len+1!!!

	if(isempty(in))
	{
		*out = '\0';
	}
	else
	{
		strncpy(out, in, len);
		*(out+len) = '\0';
	}
	return out;
}

char *alloc_string(const char *in)
{
	if(!isempty(in))
	{
		register char *lpstr = (char *)zalloc(strlen(in)+1);
		ZASSERT(lpstr);
		if(lpstr)
		{
			strcpy(lpstr, in);
			return lpstr;
		}
	}

	return nullstring;
}

char *alloc_strcat(char **out, const char *in)
{
	register unsigned long len = 0;
	if(!out) return nullstring;
	if(*out) len = strlen(*out);
	register char *lpstr = (char *)zalloc(len+strlen(in)+1);
	ZASSERT(lpstr);
	if(lpstr)
	{
		if(isalloc(*out))
		{
			strcpy(lpstr, *out);
			zfree(*out);
		}
		else
		{
			*lpstr = '\0';
		}
		strcat(lpstr, in);
		*out = lpstr;
	}

	return *out;
}

char *alloc_strncpy(const char *lpin, size_t len)
{
	//char *lpstr = (char *)zalloc(len+1);
	register char *lpstr;
	_asm
	{
		mov		edi, lpin
		or		ecx, 0FFFFFFFFh
		xor		eax, eax
		repne scasb
		not		ecx
		dec		ecx
		cmp		ecx, len
		jng		alloc
		mov		ecx, len
	alloc:
		mov		len, ecx		// temporary fix
		add		ecx, 1
		push	ecx
		call	zalloc
		mov		lpstr, eax
		add		esp, 04h
	}
	ZASSERT(lpstr);
	if(lpstr)
	{
		strncpy_tiny(lpstr, lpin, len);
		return lpstr;
	}

	return nullstring;
}

char *alloc_strncat(char **lpout, const char *lpin, size_t len)
{
	register size_t inlen = strlen(lpin);
	if(inlen < len) len = inlen;
	len += strlen(*lpout);
	register char *lpstr = (char *) zalloc(len+1);
	ZASSERT(lpstr);
	if(lpstr)
	{
		if(isalloc(*lpout))
		{
			strcpy(lpstr, *lpout);
			zfree(*lpout);
		}
		else
		{
			*lpstr = '\0';
		}
		strncat(lpstr, lpin, len);
		*lpout = lpstr;
	}

	return *lpout;
	/*
	char *lpstr;
	_asm
	{
		mov		ebx, len
		add		ebx, 1
		mov		edi, lpin
		or		ecx, 0FFFFFFFFh
		xor		eax, eax
		repne scasb
		not		ecx
		cmp		ecx, ebx
		jge		outlen				;jmp if ecx >= ebx
		mov		ebx, ecx
	outlen:
		mov		edi, lpout
		test	edi, edi
		jz		alloc
		or		ecx, 0FFFFFFFFh
		repne scasb
		not		ecx
		dec		ecx
		add		ebx, ecx
	alloc:
		push	ebx
		call	zalloc
		add		esp, 04h
		test	eax, eax
		jz		exitf
		mov		edi, eax
		mov		ecx, ebx
		xor		eax, eax
		mov		esi, lpout
		test	esi, esi
		jz		catstr
		repne	movsb
	catstr:
		mov		esi, lpin
		repne	movsb
		push	lpout
		call	free_str
		add		esp, 04h
		mov		eax, lpstr
		ret
	exitf:
		mov		eax, lpout
		test	eax, eax
		jnz		exitr
		mov		eax, NULL
	exitr:
		ret
	}
	/*
	if(lpstr)
	{
		strcpy(lpstr, lpout);
		strncat(lpstr, lpin, len);
		free_str(lpout);
		lpout = lpstr;
	}

	return lpout;
	*/
}

int isfloat(char *in, int fix)
{
	int point;
	point = 0;

	if(*in && (*in == '-'))
	{
		in++;
	}

	while(*in)
	{
		if(point)
		{
			point++;
		}

		if(!point && (*in == '.' || *in == ','))
		{
			// меняем запятую на точку!
			if(*in == ',' && fix)
			{
				*in = '.';
			}

			in++;
			if(isdigit(*in))
			{
				point = 1;
			}
			else
			{
				continue;
			}
		}
		else if(!isdigit(*in))
		{
			return -1;
		}

		in++;
	}

	return point;
}

int isinteger(char *in)
{
	if(*in && (*in == '-'))
	{
		in++;
	}

	while(*in)
	{
		if(!isdigit(*in))
		{
			return -1;
		}

		in++;
	}

	return 0;
}

// возвращает строку до \n (нужно доработать: если перед \n идёт \r - его надо обрезать)
char *alloc_getstring(const char *buffer, unsigned long len, unsigned long *pos)
{
	register char *lnend;
	register unsigned long lnsize;

	if(*pos >= len) return nullstring;

	lnend = (char *) memchr(buffer+*pos, '\n', len-*pos);
	lnsize = (lnend?lnend-buffer:len)-*pos+1;

	char *lpstr = (char *) zalloc(lnsize+1);
	ZASSERT(lpstr);
	if(lpstr)
	{
		memcpy(lpstr, buffer+*pos, lnsize);
		*(lpstr+lnsize) = '\0';
		*pos += lnsize;
		return lpstr;
	}
	return nullstring;
}

// not tested!!!
// не проверял работоспособность данной функции
char *alloc_getstring_rtrim(char *buffer, unsigned long len, unsigned long *pos)
{
	register char *in;
	register char prev;
	register char *out;
	unsigned long line_size;

	if(*pos >= len) return nullstring;

	prev = 0;
	in = buffer + *pos;
	out = buffer + len;
	while((in < out) && (*in != '\n'))
	{
		prev = *in;
		in++;
	}

	line_size = in - buffer - *pos;

	if(prev == '\r')
	{
		line_size--;
	}

	out = (char *) zalloc(line_size+1);
	ZASSERT(out);
	if(out)
	{
		memcpy(out, buffer + *pos, line_size);
		*(out + line_size) = '\0';
		*pos = in - buffer;
		return out;
	}

	return nullstring;
}

char *alloc_str_delete(char **lpstr, unsigned long pos, unsigned long len)
{
	return strdel((*lpstr) + pos, len);
	/*
	unsigned long size = strlen(*lpstr);
	char *lptemp = (char *) zalloc(size-len+1);
	ZASSERT(lptemp);
	if(lptemp)
	{
		if(pos > 0) strncpy_tiny(lptemp, *lpstr, pos);
		if((size-pos-len) > 0)
		{
			strncpy_tiny(lptemp+pos, *lpstr+pos+len, size-pos-len);
		}
		else
		{
			*(lptemp+size-len) = '\0';
		}

		free_str(*lpstr);
		*lpstr = lptemp;
		return lptemp;
	}
	return nullstring;
	*/
}

char *alloc_str_replace(char **lpstr, unsigned long pos, unsigned long len, char *value)
{
	unsigned long size = strlen(*lpstr);
	unsigned long value_size = strlen(value);
	char *lptemp = (char *) zalloc(size-len+value_size+1);
	ZASSERT(lptemp);
	if(lptemp)
	{
		if(pos > 0) strncpy_tiny(lptemp, *lpstr, pos);
		strcpy(lptemp+pos, value);
		if((size-pos-len) > 0) strncpy_tiny(lptemp+pos+value_size, *lpstr+pos+len, size-pos-len);

		free_str(*lpstr);
		*lpstr = lptemp;
		return lptemp;
	}
	return nullstring;
}

char *alloc_str_replace(char **str, char *key, char *val)
{
	char *in;
	char *in2;
	char *out;
	char *out2;
	unsigned long str_len;
	unsigned long val_len;
	unsigned long count;
	unsigned long j;

	if(isempty(*str) || isempty(key))
	{
		return *str;
	}

	if(!val)
	{
		val = nullstring;
		val_len = 0;
	}
	else
	{
		val_len = strlen(val);
	}

	str_len = 0;
	in = *str;
	j = 0;
	count = 0;
	while(*in)
	{
		if(*in == *key)
		{
			j = 1;
			in2 = in+1;
			while(key[j] && (*in2 == key[j]))
			{
				j++;
				in2++;
			}

			if(!key[j])
			{
				in += j;
				str_len += val_len;
				count++;
				continue;
			}
		}

		str_len++;
		in++;
	}

	if(count)
	{
#ifdef _DEBUG
		ZTRACE("str_len = %d -> %d\n", strlen(*str), str_len);
		//str_len += (val_len*count - key_len*count);
		//ZTRACE(" %d\n", str_len);
#endif

		out2 = (char *) zalloc(str_len + 1);
		if(out2)
		{
			out = out2;
			in = *str;
			while(*in)
			{
				if(*in == *key)
				{
					j = 1;
					in2 = in+1;
					while(key[j] && (*in2 == key[j]))
					{
						j++;
						in2++;
					}

					if(!key[j])
					{
						in2 = val;
						while(*in2)
						{
							*out = *in2;
							out++;
							in2++;
						}
						in += j;
						continue;
					}
				}

				*out = *in;
				in++;
				out++;
			}

			*out = 0;

			zfree(*str);
			*str = out2;
		}
	}

	return *str;
}

char *replace_variable(char **lpstr, LPKEYWORD_NODE lpkey)
{
	unsigned long i, pos;

	pos = 0;
lb_continue:
	while(*(*lpstr+pos))
	{
		if(*(*lpstr+pos) == RV_KEYSYM)
		{
			if(*(*lpstr+pos+1) == RV_KEYSYM)
			{
				//alloc_str_delete(lpstr, pos, 1);
				strdel((*lpstr) + pos, 1);
			}
			else
			{
				for(i = 0; lpkey[i].szKey; i++)
				{
					if(strncmp(*lpstr+pos+1, lpkey[i].szKey, strlen(lpkey[i].szKey)) == 0)
					{
						alloc_str_replace(lpstr, pos, strlen(lpkey[i].szKey)+1, lpkey[i].szValue);
						pos += strlen(lpkey[i].szValue);
						goto lb_continue;
					}
				}

				return nullstring; //incorrect key
			}
		}

		pos++;
	}

	return *lpstr;
}

int isunreserved(char c)
{
      return isalnum(c) || (c == '-') || (c == '_') || (c == '.') || (c == '!') || (c == '~') || (c == '*') || (c == '\'') || (c == '(') || (c == ')');
}

unsigned long url_string_parse(LP_QUERY_STRING_NODE *main_node, char *text)
{
	// [scheme ':' ['//' userinfo '@' host ':' port] '/'] path ['?' query '#' fragment]
	// scheme ':' authority '/' path '?' query '#' fragment
	//
	// перепроверить утверждение: адрес www.example.com/path/to/file будет разобран
	// как %path% (и это правильное поведение для URI переданных через параметр HTTP заголовка
	// Location: и не совсем правильно для URI введенных пользователем в строке адреса)

	char *str = text;
	char *authority;
	char *temp = text;
	char *temp2;

    if(!isalpha(*str))
	{
		goto lb_path;
	}
	
	str++;
	
	while(*str && (isalnum(*str) || (*str == '+') || (*str == '-') || (*str == '.'))) { str++; }

	if(*str != ':')
	{
		goto lb_path;
	}

	//scheme = str - temp;

	if((str - temp) > 0)
	{
		query_string_put(main_node, RV_KEYSTR "scheme" RV_KEYSTR, temp, str - temp);
	}
	
	temp = str+1;

//lb_authority:

	str++;
    if((*str != '/') && (*(str+1) != '/'))
	{
		goto lb_path;
	}

	str += 2;
	temp += 2;
	
	while(*str && (isunreserved(*str) || (*str == '%') || (*str == '$') || (*str == ',') || (*str == ';') || (*str == ':') || (*str == '@') || (*str == '&') || (*str == '=') || (*str == '+'))) { str++; }

	//authority = str - temp;

	if((str - temp) > 0)
	{
		authority = query_string_put(main_node, RV_KEYSTR "authority" RV_KEYSTR, temp, str - temp)->value;
		temp = strchr(authority, '@');
		if(temp)
		{
			//userinfo = temp - authority;
			query_string_put(main_node, RV_KEYSTR "userinfo" RV_KEYSTR, authority, temp - authority);
			temp++;
		}
		else
		{
			temp = authority;
		}

		temp2 = strchr(temp, ':');
		if(temp2)
		{
			//host = temp2 - temp;
			query_string_put(main_node, RV_KEYSTR "host" RV_KEYSTR, temp, temp2 - temp);
			//port = temp2
			query_string_put(main_node, RV_KEYSTR "port" RV_KEYSTR, temp2+1, 0);
		}
		else
		{
			//host = temp
			query_string_put(main_node, RV_KEYSTR "host" RV_KEYSTR, temp, 0);
		}
	}

	temp = str;

	if(*str != '/')
	{
		goto lb_query;
	}
	
lb_path:

	while(*str && (isunreserved(*str) || (*str == '/') || (*str == '%') || (*str == '$') || (*str == ',') || (*str == ';') || (*str == ':') || (*str == '@') || (*str == '&') || (*str == '=') || (*str == '+'))) { str++; }

	//path = str - temp;
	query_string_put(main_node, RV_KEYSTR "path" RV_KEYSTR, temp, str - temp);
	temp = str;

lb_query:

	if(*str != '?')
	{
		goto lb_fragment;
	}

	str++;

	while(*str && (*str != '#')) { str++;}

	temp++;
	//query = str - temp;
	query_string_put(main_node, RV_KEYSTR "query" RV_KEYSTR, temp, str - temp);
	temp = str;

lb_fragment:

	if(*str != '#')
	{
		goto lb_end;
	}

	str++;
	while(*str) { str++;}

	temp++;
	//fragment = str - temp
	query_string_put(main_node, RV_KEYSTR "fragment" RV_KEYSTR, temp, str - temp);

lb_end:
	return 0L;
}

unsigned long query_string_parse(LP_QUERY_STRING_NODE *main_node, char *text)
{
	#define PQS_NULL		0x00000000
	#define PQS_KEY			0x00000001

	unsigned long len;
	unsigned long options;
	unsigned long dwReturn = 0x00000000;
	char *str_cur;
	char *str_last;
	char *str_temp;
	LP_QUERY_STRING_NODE temp_node;
	LP_QUERY_STRING_NODE find_node;

	*main_node = NULL;

	if(!main_node || !text) return 0L;

	temp_node = (LP_QUERY_STRING_NODE) zalloc(sizeof(QUERY_STRING_NODE));
	
	if(!temp_node) return 0L;

	memset(temp_node, 0, sizeof(QUERY_STRING_NODE));

	options = PQS_KEY;
	*main_node = temp_node;
	str_last = text;
	str_cur = text;

	while(*str_cur)
	{
		if((*str_cur == '=') || (*str_cur == '&') || !*(str_cur+1))
		{
			if(!(options & PQS_KEY) && (*str_cur == '=') && *(str_cur+1))
			{
				goto lb_next;
			}

			len = str_cur - str_last;
			
			//if(((*str_cur != '=') && (*str_cur != '&') && (*str_cur)) || !*(str_cur+1))
			if(!*(str_cur+1))
			{
				len++;
			}

			if(
				(len > 0)
				&& ((options & PQS_KEY) || (temp_node->key))
				&& !temp_node->value
				)
			{
				str_temp = (char *) zalloc(len+1);
				if(!str_temp)
				{
					dwReturn = 0x00000001;
					goto lb_end;
				}

				strncpy_tiny(str_temp, str_last, len);

				if(options & PQS_KEY)
				{
					temp_node->key = str_temp;
				}
				else
				{
					temp_node->value = str_temp;
				}
			}

			str_last = str_cur+1;

			if(*str_cur == '&')
			{
				options = PQS_KEY;
				if(temp_node->key || temp_node->value)
				{
					temp_node->next_node = (LP_QUERY_STRING_NODE) zalloc(sizeof(QUERY_STRING_NODE));
					
					if(!temp_node->next_node) return 0L;

					temp_node = temp_node->next_node;
					memset(temp_node, 0, sizeof(QUERY_STRING_NODE));
				}
			}
			else
			{
				options = PQS_NULL;
			}
		}

lb_next:
		str_cur++;
	}

lb_end:

	if(!temp_node->key && !temp_node->value)
	{
		if(temp_node == *main_node)
		{
			*main_node = NULL;
		}
		else
		{
			find_node = *main_node;
			while(find_node->next_node != temp_node)
			{
				find_node = find_node->next_node;
			}

			find_node->next_node = NULL;
		}

		zfree(temp_node);
	}
	
	return dwReturn;
}

void query_string_free(LP_QUERY_STRING_NODE *main_node)
{
	LP_QUERY_STRING_NODE temp_node;
	while(*main_node)
	{
		temp_node = *main_node;
		*main_node = (*main_node)->next_node;

		free_str(temp_node->key);
		free_str(temp_node->value);
				
		zfree(temp_node);
	}
	main_node = NULL;
}

LP_QUERY_STRING_NODE query_string_put(LP_QUERY_STRING_NODE *main_node, const char *key, char *value, unsigned long size)
{
	LP_QUERY_STRING_NODE temp_node;

	temp_node = (LP_QUERY_STRING_NODE) zalloc(sizeof(QUERY_STRING_NODE));
			
	if(!temp_node) return 0L;
	memset(temp_node, 0, sizeof(QUERY_STRING_NODE));

	if(size == 0)
	{
		size = strlen(value);
	}
	
	temp_node->key = (char *) zalloc(strlen(key)+1);
	ZASSERT(temp_node->key);
	temp_node->value = (char *) zalloc(size+1);
	ZASSERT(temp_node->value);

	strcpy(temp_node->key, key);
	strncpy_tiny(temp_node->value, value, size);

	temp_node->next_node = *main_node;
	*main_node = temp_node;

	return temp_node;
}

char *query_string_get(LP_QUERY_STRING_NODE main_node, const char *key)
{
	while(main_node)
	{
		if(main_node->key && (stricmp(key, main_node->key) == 0))
		{
			return main_node->value;
		}

		main_node = main_node->next_node;
	}

	return nullstring;
}

// Escape Sequences *************************************************
//  \a Bell (alert)
//  \b Backspace
//  \f Formfeed
//  \n New line
//  \r Carriage return
//  \t Horizontal tab
//  \v Vertical tab
//  \' Single quotation mark
//  \" Double quotation mark
//  \\ Backslash
//  \? Literal question mark
//  \ooo ASCII character in octal notation
//  \xhhh ASCII character in hexadecimal notation

char *alloc_unescapestring(const char *lpszInput, unsigned long *lpdwLen)
{
	//lpdwLen [out] - длина строки не включая '\0'
	//сделать чтобы возвращал ещё сколько символов
	//было записано в буфер, а то strlen() выдаст не
	//правильное значение если в строке был '\0'
	// input string ["test\r\nmessage on 2 lines"]
	// out string   [testADmessage on 2 lines]
	//
	// добавить поддержку строк с одинарной кавычкой
	//
	
	unsigned long dwCurPos = 0, dwOutPos = 0;

	//if(lpszInput[0] != '\"')
	//{
	//	return nullstring;
	//}
	if(lpszInput[0] == '\"')
	{
		dwCurPos++;
	}

	while((lpszInput[dwCurPos] != '\"') && (lpszInput[dwCurPos] != '\0'))
	{
		if((lpszInput[dwCurPos] == '\\') && (lpszInput[dwCurPos+1] != '\0'))
		{
			dwCurPos++;
			if(	(lpszInput[dwCurPos] == '\\') ||
				(lpszInput[dwCurPos] == '\"') ||
				(lpszInput[dwCurPos] == '\'') ||
				(lpszInput[dwCurPos] == 'n') ||
				(lpszInput[dwCurPos] == 'r') ||
				(lpszInput[dwCurPos] == 't') ||
				(lpszInput[dwCurPos] == 'v') ||
				(lpszInput[dwCurPos] == 'a') ||
				(lpszInput[dwCurPos] == 'b') ||
				(lpszInput[dwCurPos] == 'f') ||
				(lpszInput[dwCurPos] == '?') ||
				(lpszInput[dwCurPos] == '0'))
			{
			}
			else if(lpszInput[dwCurPos] == 'x')
			{
				register int i = 1;
				while(	
						//(lpszInput[dwCurPos+i] != '\"') &&
						(lpszInput[dwCurPos+i] != '\0') &&
						(i <= 3) &&
						strchr("0123456789ABCDEFabcdef", lpszInput[dwCurPos+i])
						)
				{
					i++;
				}
				dwCurPos += i-1; //(min=0; max=3)
				//dwOutPos += i>0?1:0;
			}
			else //octal
			{
				register int i = 0;
				while(	//(lpszInput[dwCurPos+i] != '\"') &&
						(lpszInput[dwCurPos+i] != '\0') &&
						(i <= 2) &&
						strchr("01234567", lpszInput[dwCurPos+i])
						)
				{
					i++;
				}
				dwCurPos += i-1; //(min=-1; max=2)
			}
		}

		dwOutPos++;
		dwCurPos++;
	}

	//if(lpszInput[dwCurPos] != '\"')
	//{
	//	return nullstring;
	//}

	char *lpstr = (char *) zalloc(dwOutPos+1);

	if(lpstr)
	{
		dwCurPos = 0;
		if(lpszInput[0] == '\"')
		{
			dwCurPos++;
		}
		dwOutPos = 0;
		while((lpszInput[dwCurPos] != '\"') && (lpszInput[dwCurPos] != '\0'))
		{
			if((lpszInput[dwCurPos] == '\\') && (lpszInput[dwCurPos+1] != '\0'))
			{
				dwCurPos++;
				if(lpszInput[dwCurPos] == '\\')
				{
					lpstr[dwOutPos++] = '\\';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == '\"')
				{
					lpstr[dwOutPos++] = '\"';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == '\'')
				{
					lpstr[dwOutPos++] = '\'';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'n')
				{
					lpstr[dwOutPos++] = '\n';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'r')
				{
					lpstr[dwOutPos++] = '\r';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 't')
				{
					lpstr[dwOutPos++] = '\t';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'v')
				{
					lpstr[dwOutPos++] = '\v';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'a')
				{
					lpstr[dwOutPos++] = '\a';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'b')
				{
					lpstr[dwOutPos++] = '\b';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'f')
				{
					lpstr[dwOutPos++] = '\f';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == '?')
				{
					lpstr[dwOutPos++] = '\?';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == '0')
				{
					lpstr[dwOutPos++] = '\0';
					dwCurPos++;
				}
				else if(lpszInput[dwCurPos] == 'x')
				{
					register int i = 0;
					while(	
							//(lpszInput[dwCurPos+i+1] != '\"') &&
							(lpszInput[dwCurPos+i+1] != '\0') &&
							(i <= 2) &&
							isxdigit(lpszInput[dwCurPos+i+1])
							)
					{
						i++;
					}
					if(i>0)
					{
						char temp[4];
						strncpy(temp, &lpszInput[dwCurPos+1], i);
						temp[i] = '\0';
						lpstr[dwOutPos++] = (char) strtoul(temp, NULL, 16);
						dwCurPos += i+1;
					}
				}
				else //octal
				{
					register int i = 0;
					while(	//(lpszInput[dwCurPos+i] != '\"') &&
							(lpszInput[dwCurPos+i] != '\0') &&
							(i <= 2) &&
							strchr("01234567", lpszInput[dwCurPos+i])
							)
					{
						i++;
					}
					if(i>0)
					{
						char temp[4];
						strncpy(temp, &lpszInput[dwCurPos], i);
						temp[i] = '\0';
						lpstr[dwOutPos++] = (char) strtoul(temp, NULL, 8);
						dwCurPos += i;
					}
				}
			}
			else
			{
				lpstr[dwOutPos++] = lpszInput[dwCurPos++];
			}

			//dwCurPos++;
		}

		lpstr[dwOutPos] = '\0';
		if(lpdwLen) *lpdwLen = dwOutPos;
	}

	return lpstr?lpstr:nullstring;
}

//***
char *alloc_escapestring(const char *lpszInput, unsigned long dwLen)
{
	// lpszInput = [text"text"text]
	// lpszOutput = ["text\"text\"text"]
	char hex_table[] = "0123456789ABCDEF";
	unsigned long dwCurPos = 0, dwOutPos = 0;
	while(dwCurPos < dwLen)
	{
		if(	(lpszInput[dwCurPos] == '\\') ||
			(lpszInput[dwCurPos] == '\"') ||
			(lpszInput[dwCurPos] == '\'') ||
			(lpszInput[dwCurPos] == '\n') ||
			(lpszInput[dwCurPos] == '\r') ||
			(lpszInput[dwCurPos] == '\t') ||
			(lpszInput[dwCurPos] == '\v') ||
			(lpszInput[dwCurPos] == '\a') ||
			(lpszInput[dwCurPos] == '\b') ||
			(lpszInput[dwCurPos] == '\f') ||
			(lpszInput[dwCurPos] == '\?') ||
			(lpszInput[dwCurPos] == '\0'))
		{
			dwOutPos++;
		}
		else if(((unsigned char)lpszInput[dwCurPos] < 0x20) ||
				((unsigned char)lpszInput[dwCurPos] > 0xFF))
		{
			dwOutPos += 3;
		}

		dwOutPos++;
		dwCurPos++;
	}

	char *lpstr = (char *) zalloc(dwOutPos+2+1);
	if(lpstr)
	{
		dwOutPos = 1;
		dwCurPos = 0;
		lpstr[0] = '\"';
		while(dwCurPos < dwLen)
		{
			if((lpszInput[dwCurPos] == '\\'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = '\\';
			}
			else if((lpszInput[dwCurPos] == '\"'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = '\"';
			}
			else if((lpszInput[dwCurPos] == '\''))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = '\'';
			}
			else if((lpszInput[dwCurPos] == '\n'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'n';
			}
			else if((lpszInput[dwCurPos] == '\r'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'r';
			}
			else if((lpszInput[dwCurPos] == '\t'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 't';
			}
			else if((lpszInput[dwCurPos] == '\v'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'v';
			}
			else if((lpszInput[dwCurPos] == '\a'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'a';
			}
			else if((lpszInput[dwCurPos] == '\b'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'b';
			}
			else if((lpszInput[dwCurPos] == '\f'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = 'f';
			}
			else if((lpszInput[dwCurPos] == '\?'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = '?';
			}
			else if((lpszInput[dwCurPos] == '\0'))
			{
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos] = '0';
			}
			else if(((unsigned char)lpszInput[dwCurPos] < 0x20) ||
					((unsigned char)lpszInput[dwCurPos] > 0xFF))
			{
				//интервал символов которые нужно
				//конвертировать в формат '\x01'..'\xFFF'
				lpstr[dwOutPos++] = '\\';
				lpstr[dwOutPos++] = 'x';
				lpstr[dwOutPos++] = hex_table[(unsigned char)lpszInput[dwCurPos]>>4];
				lpstr[dwOutPos] = hex_table[(unsigned char)lpszInput[dwCurPos]&0x0F];
			}
			else
			{
				lpstr[dwOutPos] = lpszInput[dwCurPos];
			}

			dwOutPos++;
			dwCurPos++;
		}
		lpstr[dwOutPos++] = '\"';
		lpstr[dwOutPos] = '\0';
	}

	return lpstr?lpstr:nullstring;
}

char *alloc_urlencode(char *text)
{
	char hex_table[] = "0123456789ABCDEF";
	if(isempty(text))
	{
		return nullstring;
	}

	unsigned long i;
	char *out;

	out = (char *) zalloc(strlen(text)*3+1);

	if(!out)
	{
		return nullstring;
	}

	i = 0;
	while(*text)
	{
		out[i++] = '%';
		out[i++] = hex_table[*text >> 4 & 0x0F];
		out[i++] = hex_table[*text & 0x0F];

		text++;
	}

	out[i] = '\0';

	return out;
}

//int isxdigit(char ch)
//{
//	return	isdigit(ch)
//			|| (ch == 'a') || (ch == 'b') || (ch == 'c') || (ch == 'd') || (ch == 'e') || (ch == 'f')
//			|| (ch == 'A') || (ch == 'B') || (ch == 'C') || (ch == 'D') || (ch == 'E') || (ch == 'F');
//}

char hex2char(char *hex)
{
	char val;

	if((*hex >= '0') && (*hex <= '9'))
	{
		val = (*hex - '0') << 4;
	}
	else if((*hex >= 'A') && (*hex <= 'F'))
	{
		val = (*hex - 'A' + 10) << 4;
	}
	else if((*hex >= 'a') && (*hex <= 'f'))
	{
		val = (*hex - 'a' + 10) << 4;
	}
	else
	{
		val = 0;
	}

	hex++;

	if((*hex >= '0') && (*hex <= '9'))
	{
		val |= *hex - '0';
	}
	else if((*hex >= 'A') && (*hex <= 'F'))
	{
		val |= *hex - 'A' + 10;
	}
	else if((*hex >= 'a') && (*hex <= 'f'))
	{
		val |= *hex - 'a' + 10;
	}

	return val;
}

char *urldecode(char *url)
{
	char *ch1;
	char *ch2;

	ch1 = url;
	ch2 = url;

	while(*ch2)
	{
		if(*ch2 == '%')
		{
			ch2++;
			if(isxdigit(*ch2) && isxdigit(*(ch2+1)))
			{
				*ch1 = hex2char(ch2);
				ch1++;
				ch2 += 2;
				continue;
			}
			else if(!*ch2)
			{
				break;
			}
		}
		else if(*ch2 == '+')
		{
			*ch1 = ' ';
			ch1++;
			ch2++;
			continue;
		}
		*ch1 = *ch2;
		ch1++;
		ch2++;
	}
	*ch1 = *ch2;

	return url;
}

char *strdel_word(char *in, char *word)
{
	char *ch1;
	char *ch2;
	char *find;

	if(!isempty(in))
	{
		ch1 = in;
		ch2 = in;
		find = word;

		while(*ch2)
		{
			if(*ch2 == *find)
			{
				find++;
				if(!*find)
				{
					ch1 -= find - word;
					ch1++;
					ch2++;
					find = word;
					continue;
				}
			}
			else
			{
				find = word;
			}

			*ch1 = *ch2;
			ch1++;
			ch2++;
		}

		*ch1 = *ch2;
	}

	return in;
}

char *strdel(char *in, unsigned long count)
{
	char *ch1;
	char *ch2;

	if(!isempty(in))
	{
		ch1 = in;
		ch2 = in;

		while(*ch2 && count)
		{
			ch2++;
			count--;
		}

		while(*ch2)
		{
			*ch1 = *ch2;
			ch1++;
			ch2++;
		}

		*ch1 = *ch2;
	}

	return in;
}

char *strmov_bytes(char *in, unsigned long count)
{
	char *ch1;
	char *ch2;
	unsigned long size;

	if(in)
	{
		size = strlen(in);
		ch2 = in+size;
		ch1 = ch2+count;

		while(size)
		{
			*ch1 = *ch2;
			ch1--;
			ch2--;
			size--;
		}

		*ch1 = *ch2;
	}

	return in;
}

void *memdel(void *in, unsigned long size, unsigned long offset, unsigned long count)
{
	char *ch1;
	char *ch2;

	if(offset+count < size)
	{
		ch1 = (char *) in+offset;
		offset += count;
		ch2 = (char *) in+offset;

		while(offset < size)
		{
			*ch1 = *ch2;
			ch1++;
			ch2++;
			offset++;
		}
	}

	return in;
}

// функция смещает блок данных начиная с offset и заканчивая size на count байтов
// на выходе требутся буфер размером не менее size + count?
void *memmov(void *in, unsigned long size, unsigned long offset, unsigned long count)
{
	char *ch1;
	char *ch2;

	ch2 = (char *) in+size-1;
	ch1 = ch2+count;

	while(size > offset)
	{
		*ch1 = *ch2;
		ch1--;
		ch2--;
		size--;
	}

	return in;
}

char *swapchar(char *in, char och, char nch)
{
	char *ch;

	ch = in;
	while(*ch)
	{
		if(*ch == och)
		{
			*ch = nch;
		}

		ch++;
	}

	return in;
}

// compare file extension "filename.zip", ".zip"
int extcmp(char *str, char *ext)
{
	unsigned long j;
	unsigned long i;

	i = strlen(str);
	j = strlen(ext);

	if(i < j)
	{
		return 0;
	}

	return (stricmp(str + i - j, ext) == 0);
}

// move to utstrings: code checked - ok (remove 1024 limit - ok)
LIST_VARIABLE_NODE **variables_put_string(LIST_VARIABLE_NODE **lplist, char *name, const char *strings, ...)
{
	LIST_VARIABLE_NODE *lpnode;
	//LIST_VARIABLE_NODE *lptemp;
	LIST_VARIABLE_NODE **bind_point;
	char *out;

	if(!lplist)
	{
		return NULL;
	}

	lpnode = (LIST_VARIABLE_NODE *) zalloc(sizeof(LIST_VARIABLE_NODE));

	if(!lpnode)
	{
		return NULL;
	}

	memset(lpnode, 0, sizeof(LIST_VARIABLE_NODE));

	va_list list;
	va_start(list, strings);
 	vasprintf(&out, strings, list);
	va_end(list);

	lpnode->flags = LVF_STRING | LVF_ALLOC;
	lpnode->key = alloc_string(name);
	lpnode->value.str = out;

	/*
	if(*lplist)
	{
		lptemp = *lplist;
		while(lptemp->next_node)
		{
			lptemp = lptemp->next_node;
		}
		lptemp->next_node = lpnode;
	}
	else
	{
		*lplist = lpnode;
	}

	return lpnode;
	*/

	bind_point = lplist;

	while(*bind_point)
	{
		bind_point = &(*bind_point)->next_node;
	}

	*bind_point = lpnode;

	return bind_point;
}

// move to utstrings: code checked - ok
LIST_VARIABLE_NODE *variables_get_by_name(LIST_VARIABLE_NODE *lplist, char *name)
{
	while(lplist)
	{
		if(stricmp(name, lplist->key) == 0)
		{
			return lplist;
		}
		lplist = lplist->next_node;
	}

	return NULL;
}

LIST_VARIABLE_NODE **variables_node_to_bindpoint(LIST_VARIABLE_NODE **list, LIST_VARIABLE_NODE *node)
{
	LIST_VARIABLE_NODE **bind_point;

	bind_point = list;
	while(*bind_point != node)
	{
		bind_point = &(*bind_point)->next_node;
	}

	return bind_point;
}

// move to utstrings: code checked - ok
void variables_free(LIST_VARIABLE_NODE **list)
{
	LIST_VARIABLE_NODE *node;

	while(*list)
	{
		node = *list;
		*list = (*list)->next_node;

		free_str(node->key);
		if((node->flags & LVF_ALLOC) && (node->flags & LVF_STRING))
		{
			free_str(node->value.str);
		}

		zfree(node);
	}

	*list = NULL;
}

// неправильная функция!!! берёт текст до какого-то слова
// get string before word (not delimeters)
// move to utstrings: code checked - skipped
char *alloc_getstringbefore(const char *buffer, unsigned long len, unsigned long *pos, const char *word)
{
	register unsigned long lnend;
	register unsigned long lnsize;

	if(*pos >= len) return nullstring;

	lnend = *pos;
	//while((lnend < len) && !strchr(delimeters, *(buffer+lnend)))
	while((lnend < len) && (strncmp(word, buffer+lnend, strlen(word)) != 0))
	{
		lnend++;
	}
	lnsize = lnend-*pos;

	char *lpstr = NULL;
	if(lnsize > 0)
	{
		lpstr = (char *) zalloc(lnsize+1);
		if(lpstr)
		{
			memcpy(lpstr, buffer+*pos, lnsize);
			*(lpstr+lnsize) = '\0';
			*pos += lnsize;
		}
	}
	//while((*pos < len) && strchr(delimeters, *(buffer+*pos)))
	//{
	//	(*pos)++;
	//}
	(*pos) += strlen(word);
	return lpstr?lpstr:nullstring;
}

// не проверял работоспособность данной функции
// можно немного переделать и с качестве делиметра так же использовать '\0'
char alloc_getstringbefore(char **out, const char *buffer, unsigned long len, unsigned long *pos, char *delimeters/*, unsigned long delimeters_count*/)
{
	register char *end;
	register char *ch;
	register char *dl;
	register unsigned long line_size;

	if(*pos >= len)
	{
		*out = nullstring;
		return 0;
	}

	ch = (char *) buffer + *pos;
	end = (char *) buffer + len;
	dl = delimeters;

	while(*ch != *dl)
	{
		dl++;
		if(!*dl)
		{
			ch++;
			dl = delimeters;
			if(ch > end)
			{
				break;
			}
		}
	}

	line_size = ch - buffer - *pos;

	*out = (char *) zalloc(line_size+1);
	ZASSERT(*out);
	if(*out)
	{
		memcpy(*out, buffer+*pos, line_size);
		*((*out)+line_size) = '\0';
		*pos += line_size;
	}
	else
	{
		*out = nullstring;
	}

	if(ch <= end)
	{
		return *ch;
	}

	return 0;
}

// move to utstrings: code checked - require remake (remove 1024 limit - ok)
char *alloc_string_ex(char *strings, ...)
{
	char *out;
	va_list list;
	va_start(list, strings);
 	vasprintf(&out, strings, list);
	va_end(list);

	return out;
}

char *alloc_text2binary(char *data, unsigned long data_size, char zero, char one)
{
	char *out;
	char *result;
	unsigned long bit;

	//_asm int 3;
	result = (char *) zalloc((data_size * 8) + 1);

	if(result)
	{
		out = result;
		while(data_size)
		{
			for(bit = 0x00000080; bit; bit >>= 0x01)
			{
				*out = (*data & bit)?one:zero;
				out++;
			}

			data++;
			data_size--;
		}

		*out = '\0';
	}

	return result;
}

char *alloc_binary2text(char *data, unsigned long *data_size, char zero)
{
	char *out;
	char *result;
	unsigned long bit;
	unsigned long out_size;

	//_asm int 3;
	out_size = 0;
	result = (char *) zalloc((strlen(data)/8) + 2);

	if(result)
	{
		out = result;
		bit = 0x80;
		*out = 0;
		while(*data)
		{
			if(!bit)
			{
				bit = 0x80;
				out++;
				*out = 0;
				out_size++;
			}

			if(*data != zero)
			{
				*out |= bit;
			}

			bit >>= 0x01;

			data++;
		}

		out++;
		*out = '\0';
	}

	if(data_size)
	{
		*data_size = out_size;
	}

	return result;
}

// move to utstrings: code checked - ok (may be remove isempty())
LP_QUERY_STRING_NODE query_node_get(LP_QUERY_STRING_NODE main_node, const char *key)
{
	while(main_node)
	{
		if(!isempty(main_node->key) && (stricmp(key, main_node->key) == 0))
		{
			return main_node;
		}

		main_node = main_node->next_node;
	}

	return NULL;
}

// move to utstrings: code checked - require correct
char *query_build(LP_QUERY_STRING_NODE main_node)
{
	char *lpstr = nullstring;
	if(main_node)
	{
		//lpstr = alloc_string_ex("%s=%s", !isempty(main_node->key)?main_node->key:nullstring, !isempty(main_node->value)?main_node->value:nullstring);
		while(main_node)
		{
			if(!isempty(lpstr))
			{
				alloc_strcat(&lpstr, "&");
			}
			
			if(!isempty(main_node->key))
			{
				alloc_strcat(&lpstr, main_node->key);
				alloc_strcat(&lpstr, "=");
				if(!isempty(main_node->value))
				{
					alloc_strcat(&lpstr, main_node->value);
				}
			}

			main_node = main_node->next_node;
		}
	}

	return lpstr;
}

char *sql_query_build(unsigned long qtype, db_list_node *db_list, char *table_name, char *additional, char *condition, ...)
{
	// INSERT INTO table_name (db_list->keys) VALUES (db_list->values)
	// UPDATE table_name SET db_list->keys = db_list->values [WHERE condition] [additional]
	// SELECT (condition | *) FROM table_name [WHERE db_list->keys] [additional]
	// DELETE FROM table_name [WHERE db_list->keys] [additional]

	char *query;
	char *temp_str1;
	char *temp_str2;
	register int flag_set;
	va_list list;

	query = nullstring;
	temp_str1 = nullstring;
	temp_str2= nullstring;
	flag_set = 0;

	if(qtype & SG_INSERT)
	{
		while(db_list)
		{
			if(flag_set)
			{
				alloc_strcat(&temp_str1, ", ");
				alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->key);
				alloc_strcat(&temp_str2, ", ");
				alloc_strcat(&temp_str2, ((DB_SQL_QUERY_NODE *) db_list->data)->value);
			}
			else
			{
				temp_str1 = alloc_string(((DB_SQL_QUERY_NODE *) db_list->data)->key);
				temp_str2 = alloc_string(((DB_SQL_QUERY_NODE *) db_list->data)->value);
				flag_set = 1;
			}

			db_list = db_list->next_node;
		}

		if(flag_set)
		{
			query = alloc_string("INSERT INTO ");
			alloc_strcat(&query, table_name);
			alloc_strcat(&query, " (");
			alloc_strcat(&query, temp_str1);
			alloc_strcat(&query, ") VALUES (");
			alloc_strcat(&query, temp_str2);
			alloc_strcat(&query, ")");

			free_str(temp_str1);
			free_str(temp_str2);
		}
	}
	else if(qtype & SG_UPDATE)
	{
		while(db_list)
		{
			if(flag_set)
			{
				alloc_strcat(&temp_str1, ", ");
				alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->key);
			}
			else
			{
				temp_str1 = alloc_string(((DB_SQL_QUERY_NODE *) db_list->data)->key);
				flag_set = 1;
			}

			alloc_strcat(&temp_str1, "=");
			alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->value);

			db_list = db_list->next_node;
		}

		if(flag_set)
		{
			query = alloc_string("UPDATE ");
			alloc_strcat(&query, table_name);
			alloc_strcat(&query, " SET ");
			alloc_strcat(&query, temp_str1);
			if(!isempty(condition))
			{
				alloc_strcat(&query, " WHERE ");

				va_start(list, condition);
				vasprintf(&temp_str2, condition, list);
				va_end(list);

				alloc_strcat(&query, temp_str2);
			}

			free_str(temp_str1);
			free_str(temp_str2);

			if(!isempty(additional))
			{
				alloc_strcat(&query, " ");
				alloc_strcat(&query, additional);
			}
		}
	}
	else if(qtype & (SG_SELECT | SG_DELETE))
	{
		while(db_list)
		{
			/*
			if(flag_set)
			{
				alloc_strcat(&temp_str1, " AND ");
				alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->key);
			}
			else
			{
				temp_str1 = alloc_string(((DB_SQL_QUERY_NODE *) db_list->data)->key);
				flag_set = 1;
			}

			alloc_strcat(&temp_str1, "=");
			alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->value);
			*/

			if(flag_set)
			{
				if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_AND)
				{
					alloc_strcat(&temp_str1, " AND ");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_OR)
				{
					alloc_strcat(&temp_str1, " OR ");
				}
			}

			if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_GROUP_OPEN)
			{
				alloc_strcat(&temp_str1, " (");
			}

			if(!isempty(((DB_SQL_QUERY_NODE *) db_list->data)->key))
			{
				alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->key);

				if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & (SG_LIKE | SG_BEGINS | SG_CONTAINE | SG_ENDS))
				{
					alloc_strcat(&temp_str1, " LIKE ");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_LSS)
				{
					alloc_strcat(&temp_str1, "<");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_GTR)
				{
					alloc_strcat(&temp_str1, ">");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_LEQ)
				{
					alloc_strcat(&temp_str1, "<=");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_GEQ)
				{
					alloc_strcat(&temp_str1, ">=");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_NEQ)
				{
					alloc_strcat(&temp_str1, "<>");
				}
				else if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_IS)
				{
					alloc_strcat(&temp_str1, " IS ");
				}
				else
				{
					alloc_strcat(&temp_str1, "=");
				}

				alloc_strcat(&temp_str1, ((DB_SQL_QUERY_NODE *) db_list->data)->value);
				flag_set = 1;
			}

			if(((DB_SQL_QUERY_NODE *) db_list->data)->flags & SG_GROUP_CLOSE)
			{
				alloc_strcat(&temp_str1, ") ");
			}

			db_list = db_list->next_node;
		}

		if(qtype & SG_DELETE)
		{
			query = alloc_string("DELETE FROM ");
		}
		else
		{
			query = alloc_string("SELECT ");
			if(isempty(condition))
			{
				alloc_strcat(&query, "*");
			}
			else
			{
				va_start(list, condition);
				vasprintf(&temp_str2, condition, list);
				va_end(list);

				alloc_strcat(&query, temp_str2);
			}
			alloc_strcat(&query, " FROM ");
		}
		alloc_strcat(&query, table_name);

		if(flag_set)
		{
			alloc_strcat(&query, " WHERE ");
			alloc_strcat(&query, temp_str1);

			free_str(temp_str1);
			free_str(temp_str2);
		}

		if(!isempty(additional))
		{
			alloc_strcat(&query, " ");
			alloc_strcat(&query, additional);
		}
	}

	return query;
}

// переделать: кодирование производить в  sql_query_build(), а здесь просто добавление
// require optimize!
db_list_node *sql_query_add(db_list_node **db_list, char *key, char *value, unsigned long vtype)
{
	DB_SQL_QUERY_NODE *sql_node;
	char *temp_str;

	sql_node = (DB_SQL_QUERY_NODE *) zalloc(sizeof(DB_SQL_QUERY_NODE));
	if(sql_node)
	{
		sql_node->flags = vtype;
		if(!isempty(key))
		{
			sql_node->key = alloc_string("`");
			alloc_strcat(&sql_node->key, key); // add_slashes here?
			alloc_strcat(&sql_node->key, "`");

			switch(vtype & SG_TYPE_MASK)
			{
				case SG_STRING:
					temp_str = alloc_string(value);
					//alloc_addslashes(&temp_str);
					alloc_dbescape(&temp_str);
					if(vtype & SG_PASSWORD)
					{
						sql_node->value = alloc_string("PASSWORD('");
					}
					else
					{
						sql_node->value = alloc_string("'");
					}
					if(vtype & SG_ENDS)
					{
						alloc_strcat(&sql_node->value, "%");
					}
					alloc_strcat(&sql_node->value, temp_str);
					if(vtype & SG_BEGINS)
					{
						alloc_strcat(&sql_node->value, "%");
					}
					if(vtype & SG_PASSWORD)
					{
						alloc_strcat(&sql_node->value, "')");
					}
					else
					{
						alloc_strcat(&sql_node->value, "'");
					}
					free_str(temp_str);
					break;
				case SG_RAWVAL:
					sql_node->value = alloc_string(value);
					break;
				case SG_NUMBER:
					sql_node->value = alloc_string_ex("%d", value);
					break;
				case SG_DEFAULT:
					sql_node->value = alloc_string("DEFAULT");
					break;
				default:
					sql_node->value = alloc_string("NULL");
			}
		}
		else
		{
			sql_node->key = nullstring;
			sql_node->value = nullstring;
		}

		list_add(db_list, sql_node);
	}

	return *db_list;
}

// check and move to utstrings: unchecked!!!
char *alloc_dbescape(char **string)
{
	char *ch;
	char *out;
	char *out_ch;
	unsigned long len, count;

	len = 0;
	count = 0;
	ch = *string;

	while(*ch)
	{
		if((*ch == '\'') || (*ch == '\\'))
		{
			count++;
		}
		len++;
		ch++;
	}

	if(!count)
	{
		return *string;
	}

	out = (char *) zalloc(len+count+1);
	if(out)
	{
		ch = *string;
		out_ch = out;
		while(*ch)
		{
			if((*ch == '\'') || (*ch == '\\'))
			{
				*out_ch = *ch;
				out_ch++;
			}
			*out_ch = *ch;
			out_ch++;
			ch++;
		}

		*out_ch = '\0';

		free_str(*string);

		*string = out;

		return out;
	}

	return nullstring;
}

// check and move to utstrings: unchecked!!!
char *alloc_addslashes(char **string)
{
	char *ch;
	char *out;
	char *out_ch;
	unsigned long len, count;

	len = 0;
	count = 0;
	ch = *string;

	while(*ch)
	{
		if((*ch == '\'') || (*ch == '\"') || (*ch == '\\'))
		{
			count++;
		}
		len++;
		ch++;
	}

	if(!count)
	{
		return *string;
	}

	out = (char *) zalloc(len+count+1);
	if(out)
	{
		ch = *string;
		out_ch = out;
		while(*ch)
		{
			if((*ch == '\'') || (*ch == '\"') || (*ch == '\\'))
			{
				*out_ch = '\\';
				out_ch++;
			}
			*out_ch = *ch;
			out_ch++;
			ch++;
		}

		*out_ch = '\0';

		free_str(*string);

		*string = out;

		return out;
	}

	return nullstring;
}

void cb_sql_free(void *data)
{
	free_str(((DB_SQL_QUERY_NODE *) data)->key);
	free_str(((DB_SQL_QUERY_NODE *) data)->value);
	zfree(data);
}


// кривая реализация, возможно есть ошибка в подсчете длины нового буфера
char *format_number(char *str)
{
	char *out;
	char *ch;
	unsigned long point;

	if(!*str)
	{
		return nullstring;
	}

	ch = str;
	while(*ch && (*ch != '.') && (*ch != ','))
	{
		ch++;
	}

	point = (ch - str) % 3;

	out = (char *) zalloc(((ch - str) / 3) + strlen(str) + 1);
	if(!out)
	{
		return nullstring;
	}

	if(!point)
	{
		point = 3;
	}

	ch = out;
	while(*str && (*str != '.') && (*str != ','))
	{
		if(!point)
		{
			*ch = ' ';
			ch++;
			point = 3;
		}
		point--;
		*ch = *str;
		ch++;
		str++;
	}

	while(*str)
	{
		*ch = *str;
		ch++;
		str++;
	}

	*ch = 0;

	return out;
}

char *crypt_xor(char *buf, unsigned long len, char *hash, unsigned long hash_len)
{
	unsigned long pos;
	unsigned long hpos;

	hpos = 0;
	for(pos = 0; pos < len; pos++)
	{
		buf[pos] = buf[pos] ^ hash[hpos++];
		if(hpos > hash_len)
		{
			hpos = 0;
		}
	}

	return buf;
}

// input format: yyyy-mm-dd
unsigned long strtodate(char *date_str)
{
	/*
	unsigned long ret_code;

	ret_code = _ulpa(date_str, 0, "-./ ") << 9;
	ret_code |= (_ulpa(date_str, 1, "-./ ") & 0x0F) << 5;
	ret_code |= _ulpa(date_str, 2, "-./ ") & 0x1F;

	//ZTRACE("%s: 0x%.8X\n", date_str, ret_code);

	return ret_code;
	*/
	return zdate(_ulpa(date_str, 2, "-./ "), _ulpa(date_str, 1, "-./ "), _ulpa(date_str, 0, "-./ "));
}

// input format: dd.mm.yyyy
unsigned long strtodate2(char *date_str)
{
	/*
	unsigned long ret_code;

	ret_code = _ulpa(date_str, 0, "-./ ") & 0x1F;
	ret_code |= (_ulpa(date_str, 1, "-./ ") & 0x0F) << 5;
	ret_code |= _ulpa(date_str, 2, "-./ ") << 9;

	//ZTRACE("%s: 0x%.8X\n", date_str, ret_code);

	return ret_code;
	*/
	return zdate(_ulpa(date_str, 0, "-./ "), _ulpa(date_str, 1, "-./ "), _ulpa(date_str, 2, "-./ "));
}

// input format: hh:mm:ss.mss
unsigned long strtotime(char *date_str)
{
	return ztime(_ulpa(date_str, 0, "-./: "), _ulpa(date_str, 1, "-./: "), _ulpa(date_str, 2, "-./: "), _ulpa(date_str, 3, "-./: "));
}

// input format: xx.xx.xx hh:mm:ss.mss
unsigned long strtotime2(char *date_str)
{
	return ztime(_ulpa(date_str, 3, "-./: "), _ulpa(date_str, 4, "-./: "), _ulpa(date_str, 5, "-./: "), _ulpa(date_str, 6, "-./: "));
}

// окруление денежной единицы в большую сторону до целых копеек 1234.5624 -> 1234.5700
__int64 roundcurrency(__int64 num)
{
	register int i;

	i = (int) num % 100;

	if(i > 0)
	{
		num += 100 - i;
	}

	return num;
}

// преобразует денежную единицу из строки (char *) "1 234.5678" -> (int64) 12345678
__int64 strtocurrency(char *str)
{
	register __int64 n = 0;
	register int neg = 0;
	register int j = 0;

	while(*str && ((*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n')))
	{
		str++;
	}

	if(*str == '-')
	{
		neg = 1;
		str++;
	}
	else if(*str == '+')
	{
		str++;
	}

	while(((*str >= '0') && (*str <= '9')) || (*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n'))
	{
		if(((*str >= '0') && (*str <= '9')))
		{
			n = (n * 10) + ((*str) - '0');
		}
		str++;
	}

	j = 4;
	if((*str == '.') || (*str == ','))
	{
		str++;
		while((j > 0) && (((*str >= '0') && (*str <= '9')) || (*str == ' ') || (*str == '\t') || (*str == '\r') || (*str == '\n')))
		{
			if(((*str >= '0') && (*str <= '9')))
			{
				j--;
				n = (n * 10) + ((*str) - '0');
			}
			str++;
		}

	}

	while(j > 0)
	{
		n = n * 10;
		j--;
	}

	return (neg ? -n : n);
}

// преобразует денежную единицу в строку (int64) 12345678 -> (char *) "1 234.5678"
char *currencytostr(__int64 num, char *str)
{
	register __int64 i;
	register char *p = str;
	register char *q = str;
	register int j, k, neg;

	if(num == 0)
	{
		*p++ = '0';
		*p = 0;

		return str;
	}

	if(num < 0)
	{
		neg = 1;
		num = -num;
	}
	else
	{
		neg = 0;
	}

	k = 0;
	for(j = 4; j > 0; j--)
	{
		i = num % 10;

		if(i | k)
		{
			*p++ = "0123456789ABCDEF"[i];
			k = 1;
		}

		num /= 10;
	}

	if(k)
	{
		*p++ = '.';
	}

	if(!num)
	{
		*p++ = '0';
	}
	else
	{
		j = 0;
		while(num > 0)
		{
			i = num % 10;

			if(j == 3)
			{
				j = 0;
				*p++ = ' ';
			}

			*p++ = "0123456789ABCDEF"[i]; // '0' + i;
			j++;
			num /= 10;
		}
	}

	if(neg)
		*p++ = '-';

	*p-- = 0;
	q = str;

	while(p > q)
	{
		j = *q;
		*q++ = *p;
		*p-- = j;
	}

	return str;
}

char *currencytostr2(__int64 num, char *str)
{
	register __int64 i;
	register char *p = str;
	register char *q = str;
	register int j, k, neg;

	if(num == 0)
	{
		*p++ = '0';
		*p = 0;

		return str;
	}

	if(num < 0)
	{
		neg = 1;
		num = -num;
	}
	else
	{
		neg = 0;
	}

	k = 0;
	for(j = 4; j > 0; j--)
	{
		i = num % 10;

		if(i | k)
		{
			*p++ = "0123456789ABCDEF"[i];
			k = 1;
		}

		num /= 10;
	}

	if(k)
	{
		*p++ = '.';
	}

	if(!num)
	{
		*p++ = '0';
	}
	else
	{
		while(num > 0)
		{
			i = num % 10;

			*p++ = "0123456789ABCDEF"[i]; // '0' + i;
			num /= 10;
		}
	}

	if(neg)
		*p++ = '-';

	*p-- = 0;
	q = str;

	while(p > q)
	{
		j = *q;
		*q++ = *p;
		*p-- = j;
	}

	return str;
}

/*
int itoa (int num, char *str, int radix)
{
  register int i, neg = 0;
  register char *p = str;
  register char *q = str;

  if (radix == 0)
    radix = 10;
   else
     if (radix < 2 || radix > RADIX_MAX)
       return (radix);

  if (num == 0)
    {
      *p++ = '0';
      *p = 0;

      return (0);
    }

  if (num < 0)
    {
      neg = 1;
      num = -num;
    }

  while (num > 0)
   {
     i = num % radix;

     if (i > 9)
       i += 7;

     *p++ = '0' + i;
     num /= radix;
   }

  if (neg)
    *p++ = '-';

  *p-- = 0;
  q = str;

  while (p > q)
   {
     i = *q;
     *q++ = *p;
     *p-- = i;
   }

  return (0);
}
*/

#define DG_POWER 6              // Энто допустимая степень числа 1000 для __int64:
                                // При необходимости его легко увеличить,
								// дополнив массив 'a_power' и заменив
								// тип __int64 на более серьезный

typedef struct _s_POWER
{
	int sex;
	char *one;
	char *four;
	char *many;
} s_POWER;

typedef struct _s_UNIT
{
	char *one[2];
	char *two;
	char *dec;
	char *hun;
} s_UNIT;

s_POWER a_power[] = {
	{0, NULL          , NULL           , NULL            },  // 1
	{1, "тысяча "     , "тысячи "      , "тысяч "        },  // 2
	{0, "миллион "    , "миллиона "    , "миллионов "    },  // 3
	{0, "миллиард "   , "миллиарда "   , "миллиардов "   },  // 4
	{0, "триллион "   , "триллиона "   , "триллионов "   },  // 5
	{0, "квадриллион ", "квадриллиона ", "квадриллионов "},  // 6
	{0, "квинтиллион ", "квинтиллиона ", "квинтиллионов "}   // 7
};

s_UNIT digit[10] = {
	{{""       , ""       }, "десять "      , ""            , ""          },
	{{"один "  , "одна "  }, "одиннадцать " , "десять "     , "сто "      },
	{{"два "   , "две "   }, "двенадцать "  , "двадцать "   , "двести "   },
	{{"три "   , "три "   }, "тринадцать "  , "тридцать "   , "триста "   },
	{{"четыре ", "четыре "}, "четырнадцать ", "сорок "      , "четыреста "},
	{{"пять "  , "пять "  }, "пятнадцать "  , "пятьдесят "  , "пятьсот "  },
	{{"шесть " , "шесть " }, "шестнадцать " , "шестьдесят " , "шестьсот " },
	{{"семь "  , "семь "  }, "семнадцать "  , "семьдесят "  , "семьсот "  },
	{{"восемь ", "восемь "}, "восемнадцать ", "восемьдесят ", "восемьсот "},
	{{"девять ", "девять "}, "девятнадцать ", "девяносто "  , "девятьсот "}
};

char *alloc_dig2str(__int64 p_summa, int p_sex, char *p_one, char *p_four, char *p_many)
{
	int i;
	int mny;
	char *str;
	char *result = nullstring;
	__int64 divisor; //делитель

	a_power[0].sex  = p_sex;
	a_power[0].one  = p_one;
	a_power[0].four = p_four;
	a_power[0].many = p_many;

	if(p_summa == (__int64) 0)
	{
		result = alloc_string("ноль ");
		alloc_strcat(&result, p_many);
		return result;
	}

	if(p_summa < (__int64) 0)
	{
		result = alloc_string("минус ");
		p_summa = -p_summa;
	}

	for(i = 0, divisor = (__int64) 1; i < DG_POWER; i++)
	{
		divisor *= (__int64) 1000;
	}

	for(i = DG_POWER-1; i >= 0; i--)
	{
		divisor /= 1000;
		mny = (int) (p_summa / divisor);
		p_summa %= divisor;
		str = nullstring;

		if(mny == 0)
		{
			if(i > 0) continue;
			alloc_strcat(&str, a_power[i].many);
		}
		else
		{
			if(mny >= 100)
			{
				alloc_strcat(&str, digit[mny/100].hun);
				mny %= 100;
			}

			if(mny >= 20)
			{
				alloc_strcat(&str, digit[mny/10].dec);
				mny %= 10;
			}

			if(mny >= 10)
			{
				alloc_strcat(&str, digit[mny-10].two);
			}
			else if(mny >= 1)
			{
				alloc_strcat(&str, digit[mny].one[a_power[i].sex]);
			}
			
			switch(mny)
			{
				case 1:
					alloc_strcat(&str, a_power[i].one);
					break;
				case 2: case 3:	case 4:
					alloc_strcat(&str, a_power[i].four);
					break;
				default:
					alloc_strcat(&str, a_power[i].many);
					break;
			};
		}

		alloc_strcat(&result, str);
		free_str(str);
	}

	return result;
}

char *alloc_dig2str(double p_summa0, int p_sex, char *p_one, char *p_four, char *p_many, char *k_one, char *k_four, char *k_many)
{
	int i;
	int mny;
	char *str;
	char *result = nullstring;
	__int64 divisor; //делитель
	int kop;
	char tmp_buf[33];
	__int64 p_summa;
	//double yy;
	//double zz;

	a_power[0].sex  = p_sex;
	a_power[0].one  = p_one;
	a_power[0].four = p_four;
	a_power[0].many = p_many;

	/*
	zz = modf(p_summa0, &yy) * 100;
	p_summa = (__int64) yy;

	//modf(zz, &yy);
	yy = floor(zz);
	kop = (int) yy;
	*/

	p_summa = (__int64) (p_summa0 + 0.00005); //DBL_EPSILON
	kop = (int) ((p_summa0 - p_summa + 0.00005) * 100);

	if(p_summa == (__int64) 0)
	{
		result = alloc_string("ноль ");
		alloc_strcat(&result, p_many);
	}
	else
	{
		if(p_summa < (__int64) 0)
		{
			result = alloc_string("минус ");
			p_summa = -p_summa;

			kop = -kop;
		}

		for(i = 0, divisor = (__int64) 1; i < DG_POWER; i++)
		{
			divisor *= (__int64) 1000;
		}

		for(i = DG_POWER-1; i >= 0; i--)
		{
			divisor /= 1000;
			mny = (int) (p_summa / divisor);
			p_summa %= divisor;
			str = nullstring;

			if(mny == 0)
			{
				if(i > 0) continue;
				alloc_strcat(&str, a_power[i].many);
			}
			else
			{
				if(mny >= 100)
				{
					alloc_strcat(&str, digit[mny/100].hun);
					mny %= 100;
				}

				if(mny >= 20)
				{
					alloc_strcat(&str, digit[mny/10].dec);
					mny %= 10;
				}

				if(mny >= 10)
				{
					alloc_strcat(&str, digit[mny-10].two);
				}
				else if(mny >= 1)
				{
					alloc_strcat(&str, digit[mny].one[a_power[i].sex]);
				}
				
				switch(mny)
				{
					case 1:
						alloc_strcat(&str, a_power[i].one);
						break;
					case 2: case 3:	case 4:
						alloc_strcat(&str, a_power[i].four);
						break;
					default:
						alloc_strcat(&str, a_power[i].many);
						break;
				};
			}

			alloc_strcat(&result, str);
			free_str(str);
		}
	}

	// копейки

	alloc_strcat(&result, " ");
	//itoa(kop, tmp_buf, 10);
	_snprintf(tmp_buf, 32, "%.2d", kop);
	alloc_strcat(&result, tmp_buf);
	alloc_strcat(&result, " ");

	if(kop >= 20)
	{
		kop %= 10;
	}

	switch(kop)
	{
		case 1:
			alloc_strcat(&result, k_one);
			break;
		case 2: case 3:	case 4:
			alloc_strcat(&result, k_four);
			break;
		default:
			alloc_strcat(&result, k_many);
			break;
	};

	return result;
}

char *alloc_currency2str(__int64 p_summa0, int p_sex, char *p_one, char *p_four, char *p_many, char *k_one, char *k_four, char *k_many)
{
	int i;
	int mny;
	char *str;
	char *result = nullstring;
	__int64 divisor; //делитель
	int kop;
	char tmp_buf[33];
	__int64 p_summa;
	//double yy;
	//double zz;

	a_power[0].sex  = p_sex;
	a_power[0].one  = p_one;
	a_power[0].four = p_four;
	a_power[0].many = p_many;

	/*
	zz = modf(p_summa0, &yy) * 100;
	p_summa = (__int64) yy;

	//modf(zz, &yy);
	yy = floor(zz);
	kop = (int) yy;
	*/

	p_summa0 = roundcurrency(p_summa0);
	p_summa = p_summa0 / 10000;
	kop = (int) (roundcurrency(p_summa0) % 10000) / 100;

	if(p_summa == (__int64) 0)
	{
		result = alloc_string("ноль ");
		alloc_strcat(&result, p_many);
	}
	else
	{
		if(p_summa < (__int64) 0)
		{
			result = alloc_string("минус ");
			p_summa = -p_summa;

			kop = -kop;
		}

		for(i = 0, divisor = (__int64) 1; i < DG_POWER; i++)
		{
			divisor *= (__int64) 1000;
		}

		for(i = DG_POWER-1; i >= 0; i--)
		{
			divisor /= 1000;
			mny = (int) (p_summa / divisor);
			p_summa %= divisor;
			str = nullstring;

			if(mny == 0)
			{
				if(i > 0) continue;
				alloc_strcat(&str, a_power[i].many);
			}
			else
			{
				if(mny >= 100)
				{
					alloc_strcat(&str, digit[mny/100].hun);
					mny %= 100;
				}

				if(mny >= 20)
				{
					alloc_strcat(&str, digit[mny/10].dec);
					mny %= 10;
				}

				if(mny >= 10)
				{
					alloc_strcat(&str, digit[mny-10].two);
				}
				else if(mny >= 1)
				{
					alloc_strcat(&str, digit[mny].one[a_power[i].sex]);
				}
				
				switch(mny)
				{
					case 1:
						alloc_strcat(&str, a_power[i].one);
						break;
					case 2: case 3:	case 4:
						alloc_strcat(&str, a_power[i].four);
						break;
					default:
						alloc_strcat(&str, a_power[i].many);
						break;
				};
			}

			alloc_strcat(&result, str);
			free_str(str);
		}
	}

	// копейки

	alloc_strcat(&result, " ");
	//itoa(kop, tmp_buf, 10);
	_snprintf(tmp_buf, 32, "%.2d", kop);
	alloc_strcat(&result, tmp_buf);
	alloc_strcat(&result, " ");

	if(kop >= 20)
	{
		kop %= 10;
	}

	switch(kop)
	{
		case 1:
			alloc_strcat(&result, k_one);
			break;
		case 2: case 3:	case 4:
			alloc_strcat(&result, k_four);
			break;
		default:
			alloc_strcat(&result, k_many);
			break;
	};

	return result;
}

/*
// cuted from bo2k
char *EscapeString(char *svStr)
{
	// Determine final size
	int nEscapes=0,i,count;
	count=lstrlen(svStr);
	for(i=0;i<count;i++) {
		if(svStr[i]=='\a') nEscapes++;
		else if(svStr[i]=='\b') nEscapes++;
		else if(svStr[i]=='\f') nEscapes++;
		else if(svStr[i]=='\n') nEscapes++;
		else if(svStr[i]=='\r') nEscapes++;
		else if(svStr[i]=='\t') nEscapes++;
		else if(svStr[i]=='\v') nEscapes++;
		else if(svStr[i]=='\\') nEscapes++;
		else if(svStr[i]<' ' || svStr[i]>'~') nEscapes+=3;
	}
	
	// Allocate output buffer
	char *svOutBuf=(char*)zalloc(lstrlen(svStr)+nEscapes+1);

	// Escape things
	int j=0;
	for(i=0;i<count;i++) {
		char c;
		c=svStr[i];
		if(c>=' ' && c<='~') { svOutBuf[j]=c; j++; }
		else if(c=='\a') { svOutBuf[j]='\\'; svOutBuf[j+1]='a'; j+=2; }
		else if(c=='\b') { svOutBuf[j]='\\'; svOutBuf[j+1]='b'; j+=2; }
		else if(c=='\f') { svOutBuf[j]='\\'; svOutBuf[j+1]='f'; j+=2; }
		else if(c=='\n') { svOutBuf[j]='\\'; svOutBuf[j+1]='n'; j+=2; }
		else if(c=='\r') { svOutBuf[j]='\\'; svOutBuf[j+1]='r'; j+=2; }
		else if(c=='\t') { svOutBuf[j]='\\'; svOutBuf[j+1]='t'; j+=2; }
		else if(c=='\v') { svOutBuf[j]='\\'; svOutBuf[j+1]='v'; j+=2; }
		else if(c=='\\') { svOutBuf[j]='\\'; svOutBuf[j+1]='\\'; j+=2; }
		else {
			wsprintf(svOutBuf+j,"\\x%1.1X%1.1X",(c>>4),c&15);
			j+=4;
		}
	}
	svOutBuf[j]='\0';

	return svOutBuf;
}

char *UnescapeString(char *svStr)
{
	int len=lstrlen(svStr);
    char *svTemp;

	// Count number of '%' characters
	int nCount;
	nCount=0;
	svTemp=svStr;
	while(*svTemp) {
		if(*svTemp=='%') nCount++;
		svTemp++;
	}

	// Allocate memory
	svTemp=(char *)zalloc(lstrlen(svStr)+1+nCount);
	if(svTemp==NULL) return NULL;
	memset(svTemp,0,lstrlen(svStr)+1+nCount);
	
	// Convert string to preserve '%' chars
	char *svCvtTo, *svCvtFrom;
	svCvtFrom=svStr;
	svCvtTo=svTemp;
	while(*svCvtFrom) {
		if(*svCvtFrom=='%') {
			*(svCvtTo++)='%';
		}
		*(svCvtTo++)=*(svCvtFrom++);
	}

	// Convert escape chars (funky kludge, eh?)
	wsprintf(svStr,svTemp);
	free_str(svTemp);
	
	return svStr;
}*/

