#ifndef _ZSOCKETS_H_
#define _ZSOCKETS_H_

//#pragma once

#include <winsock2.h>

typedef DWORD (WINAPI *CBSOCKETSFUNC) (DWORD);

typedef struct _BUFFSOCKET
{
	SOCKET s;
	char *pBuffer;
	unsigned long nPktLen;
	unsigned long nBufFill;
} BUFFSOCKET, *LPBUFFSOCKET;

int alloc_recvpkt(SOCKET s, char **buf, int *buflen);
int recv_file(SOCKET sock, char *lpszSaveAs, CBSOCKETSFUNC pSocketsFunc);
int send_file(SOCKET sock, char *lpszFileName, CBSOCKETSFUNC pSocketsFunc);

#endif //_ZSOCKETS_H_
