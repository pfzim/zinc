#ifndef _ZDBG_H_
#define _ZDBG_H_

#ifdef _DEBUG

#include <windows.h>
#include "zmessages.h"

#define ZASSERT(expr)				ZASSERTEX(__FILE__, __LINE__, expr)
#define ZASSERTEX(fn, ln, expr)		do{if(!(expr)) MessageBoxFmt(NULL, "ZASSERT", MB_OK, fn" (%d): "#expr, ln);}while(0)
#define ZTRACE						OutputDebugStringFmt

void OutputDebugStringFmt(const char *lpFormat, ...);

#else

#define ZASSERT(expr)				((void) 0)
#define ZASSERTEX(fn, ln, expr)		((void) 0)
#define ZTRACE()					((void) 0)

#endif //_DEBUG

#endif //_ZDBG_H_
