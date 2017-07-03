//#include "stdafx.h"									//dEmon (c) 2007
#include "zdbg.h"

#ifdef _DEBUG

#include <stdio.h>
#include <stdarg.h>
#include "zalloc.h"
#include "snprintf.h"

void OutputDebugStringFmt(const char *lpFormat, ...)
{
	va_list list;
	char *out;

	va_start(list, lpFormat);
	vasprintf(&out, lpFormat, list);
	va_end(list);

	OutputDebugString(out);

	zfree(out);
}

#endif //_DEBUG
