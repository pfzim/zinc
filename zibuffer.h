#ifndef _ZIBUFFER_H_
#define _ZIBUFFER_H_

#include <windows.h>

#define	BRW_FORWARD							0x00000000
#define	BRW_BACKWARD						0x00000001
#define	BRW_CENTERED						0x00000002

// invalid position in file
#define	BRW_INVALID_POINTER					0xFFFFFFFF //(-1)

typedef struct _EXTBUFFER
{
	HANDLE hFile;
	char *lpBuffer;
	unsigned long dwBufferSize;
	unsigned long dwFileSize;
	unsigned long dwFilePos;
	unsigned long dwBufferFill;
} EXTBUFFER, *LPEXTBUFFER;

HANDLE __initbufferedread(const char *szFileName, LPEXTBUFFER zBuffer, unsigned long dwBufferSize);
HANDLE __initbufferedwrite(const char *szFileName, LPEXTBUFFER zBuffer, unsigned long dwBufferSize);
unsigned long __donebufferedwrite(LPEXTBUFFER zBuffer);
void __destructbufferedreadwrite(LPEXTBUFFER zBuffer);
int __get(LPEXTBUFFER zBuffer, unsigned long dwFilePos);
int __getblock(EXTBUFFER *zb, char *out, unsigned long st, unsigned long en);
void __add(LPEXTBUFFER zBuffer, char szChar);
void __addblock(LPEXTBUFFER zBuffer, const char *szBuffer, unsigned long dwSize);
unsigned long __seek(EXTBUFFER *zBuffer, unsigned long dwFromPos, const char *lpszChars, int fErrorOnEof=TRUE);
unsigned long __seekcsp(EXTBUFFER *zBuffer, unsigned long dwFromPos, unsigned long *lpdwStartPos, unsigned long *lpdwEndPos, const char *lpszChars, int fErrorOnEof);
unsigned long __seekcsps(EXTBUFFER *zBuffer, unsigned long dwFromPos, unsigned long *lpdwStartPos, unsigned long *lpdwEndPos, const char *lpszChars, int fErrorOnEof);
unsigned long __seekstring(EXTBUFFER *zBuffer, unsigned long from_pos, const char *seek_string);
unsigned long __skipstring(EXTBUFFER *zBuffer, unsigned long dwFromPos);
unsigned long __skipblock(EXTBUFFER *zBuffer, unsigned long dwFromPos);
char *__alloc_getblock(EXTBUFFER *zBuffer, unsigned long dwStartPos, unsigned long dwEndPos);
//HANDLE __duplicatebufferedreadwrite(LPEXTBUFFER zBufferIn, LPEXTBUFFER zBufferOut, unsigned long dwBufferSize);

inline int __iseof(EXTBUFFER *zBuffer, unsigned long dwFilePos)
{
	return dwFilePos >= zBuffer->dwFileSize;
}

#endif //_ZIBUFFER_H_
