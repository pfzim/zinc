#ifndef _ZLOG_H_
#define _ZLOG_H_

//#pragma once
#include <windows.h>

#ifndef MAX_LOG_SIZE_KB
#define MAX_LOG_SIZE_KB					4096
#endif

#define TRUNCATE_BUFFER					32768

/*
class CLog
{
	public:
		enum Options
		{
			OPT_USELOG			= 0x0001,
			OPT_FLUSHBUFFERS	= 0x0002
		};

	protected:
		HANDLE hLog;
		DWORD dwOptions;
		//LPSTR lpFileName;
	
	public:
		CLog()
		{
			hLog = INVALID_HANDLE_VALUE;
			dwOptions = 0;
		};
		~CLog()
		{
			closelog();
		};

		HANDLE openlog(LPCSTR lpszLogFileName, DWORD dwOptions)
		{
			hLog = _openlog(lpszLogFileName, dwOptions & OPT_USELOG);
			return hLog;
		};
		DWORD log(LPCSTR lpszString)
		{
			return _log(hLog, lpszString);
		};
		DWORD log_ex(LPCSTR lpszString, ...)
		{
		};
		DWORD log_date(LPCSTR lpszString, ...)
		{
		};
		BOOL closelog()
		{
			if(hLog && (hLog != INVALID_HANDLE_VALUE))
			{
				CloseHandle(hLog);
				hLog = INVALID_HANDLE_VALUE;
			}
			return hLog;
		};
};
//*/

//open or create log file
HANDLE _openlog(LPCSTR lpszLogFileName, BOOL fUseLog);
//add string to log file [no "\r\n" add to end of string]
int _log(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString);
//add format string to log file [no "\r\n" add to end of string]
DWORD _logex(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...);
//add format string to log file ["\r\n" add to end of string]
DWORD _log_date(HANDLE hFile, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...);
int _logd(HANDLE hFile, int fUseLog, int fFlushBuffers, char *text);
// close log file
BOOL _closelog(HANDLE hFile, BOOL fUseLog);
DWORD _truncatetop(HANDLE hFile, DWORD dwSetPosHome);
//open log file, write formated string, close log
BOOL _logonce(LPCSTR lpszLogFileName, BOOL fUseLog, BOOL fFlushBuffers, LPCSTR lpszString, ...);

#endif //_ZLOG_H_
