//#include "stdafx.h"								//pf_ziminski  [2003]
#include "zlog.h"
#include <stdarg.h>
//#include <malloc.h>
#include "zalloc.h"

HANDLE _openlog(LPCSTR lpszLogFileName, BOOL fUseLog)
{
	if(fUseLog)
	{
		HANDLE hFile;
		hFile = CreateFile(lpszLogFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			return hFile;
		}
	}
	return INVALID_HANDLE_VALUE;
}

int _log(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString)
{
	if(!fUseLog)
	{
		return 0;
	}

	unsigned long dwNumberOfBytesWritten, dwFS, dwMS;
	dwMS = lstrlen(lpszString);
	dwFS = GetFileSize(hFile, NULL)+dwMS;
	if(dwFS > MAX_LOG_SIZE_KB*1024)
	{
		_truncatetop(hFile, dwFS - (MAX_LOG_SIZE_KB*1024));
	}
	if(!WriteFile(hFile, lpszString, dwMS, &dwNumberOfBytesWritten, NULL) ||
	  (dwNumberOfBytesWritten < dwMS))
	{
		/* откат - бред!
		if(dwNumberOfBytesWritten > 0)
		{
			SetFilePointer(hFile, SetFilePointer(hFile, 0, NULL, FILE_CURRENT)-dwNumberOfBytesWritten, NULL, FILE_CURRENT);
			SetEndOfFile(hFile);
		}
		*/
		return 1;
	}
	else
	{
		if(fFlushBuffers)
		{
			FlushFileBuffers(hFile);
		}
	}
	return 0;
}

DWORD _logex(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...)
{
	if(fUseLog)
	{
		char realstring[1024];
		va_list list;
		va_start(list, lpszString);
		wvsprintf(realstring, lpszString, list);
		va_end(list);
		return _log(hFile, TRUE, fFlushBuffers, realstring);
	}
	return 1L;
}

DWORD _log_date(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...)
{
	if(fUseLog)
	{
		//stack overflow!?
		char realstring[1024+23+2];
		SYSTEMTIME st;
		GetLocalTime(&st);
		LPSTR tmp = realstring;
		tmp += wsprintf(tmp, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d:%2.2d    ", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		va_list list;
		va_start(list, lpszString);
		wvsprintf(tmp, lpszString, list);
		va_end(list);
		strcat(realstring, "\r\n");
		return _log(hFile, TRUE, fFlushBuffers, realstring);
	}
	return 1L;
}

int _logd(HANDLE hFile, int fUseLog, int fFlushBuffers, char *text)
{
	if(!fUseLog)
	{
		return 0;
	}

	//stack overflow in wsprintf!?
	char *temp_str;
	unsigned long bs;
	SYSTEMTIME st;

	GetLocalTime(&st);

	bs = strlen(text);
	temp_str = (char *) zalloc(bs + 26);
	if(!temp_str)
	{
		return 1;
	}

	wsprintf(temp_str, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d:%2.2d    ", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	strcpy(temp_str + 23, text);
	strcpy(temp_str + bs + 23, "\r\n");

	bs = (unsigned long) _log(hFile, TRUE, fFlushBuffers, temp_str);

	zfree(temp_str);

	return (int) bs;
}

BOOL _closelog(HANDLE hFile, BOOL fUseLog)
{
	return fUseLog?CloseHandle(hFile):TRUE;
}

DWORD _truncatetop(HANDLE hFile, DWORD dwSetPosHome)
{
	// add errors check!
	LPSTR lpBuffer = (LPSTR) malloc(TRUNCATE_BUFFER);
	if(lpBuffer)
	{
		DWORD dwBytesWritten, dwBytesRead, dwCurPos = 0;
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		while(dwSetPosHome + dwCurPos < dwFileSize)
		{
			dwBytesRead = dwFileSize-dwSetPosHome-dwCurPos < TRUNCATE_BUFFER?dwFileSize-dwSetPosHome-dwCurPos:TRUNCATE_BUFFER;
			SetFilePointer(hFile, dwSetPosHome+dwCurPos, NULL, FILE_BEGIN);
			ReadFile(hFile, lpBuffer, dwBytesRead, &dwBytesWritten, NULL);
			if(dwBytesRead != dwBytesWritten)
			{
				break;
			}
			SetFilePointer(hFile, dwCurPos, NULL, FILE_BEGIN);
			WriteFile(hFile, lpBuffer, dwBytesRead, &dwBytesWritten, NULL);
			if(dwBytesRead != dwBytesWritten)
			{
				break;
			}
			dwCurPos += dwBytesWritten;
		}
		free(lpBuffer);
		SetFilePointer(hFile, dwFileSize-dwSetPosHome, NULL, FILE_BEGIN);
		SetEndOfFile(hFile);
	}
	return SetFilePointer(hFile, 0, NULL, FILE_END);
}

BOOL _logonce(LPCSTR lpszLogFileName, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...)
{
	if(fUseLog)
	{
		HANDLE hLog;
		hLog = _openlog(lpszLogFileName, TRUE);
		if(hLog == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
		char realstring[1024];
		va_list list;
		va_start(list, lpszString);
		wvsprintf(realstring, lpszString, list);
		va_end(list);
		if(_log(hLog, TRUE, fFlushBuffers, realstring) == 0L)
		{
			_closelog(hLog, TRUE);
			return FALSE;
		}
		if(!_closelog(hLog, TRUE))
		{
			return FALSE;
		}
	}
	return TRUE;
}
