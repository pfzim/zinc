//#include "stdafx.h"								//pf_ziminski  [2003]
#include "zmessages.h"
#include "zalloc.h"
#include "snprintf.h"
#include "utstrings.h"
#include <commctrl.h>

BOOL SetDlgItemTextEx(HWND hwnd, int nIDDlgItem, LPCSTR lpFormat, ...)
{
	va_list list;
	char *out;
	int ret_code;

	va_start(list, lpFormat);
	vasprintf(&out, lpFormat, list);
	va_end(list);
	ret_code = SetDlgItemText(hwnd, nIDDlgItem, out);

	zfree(out);

	return ret_code;
}

BOOL SetWindowTextEx(HWND hwnd, LPCSTR lpFormat, ...)
{
	va_list list;
	char *out;
	int ret_code;

	va_start(list, lpFormat);
	vasprintf(&out, lpFormat, list);
	va_end(list);

	ret_code = SetWindowText(hwnd, out);

	zfree(out);

	return ret_code;
}

LRESULT SetTextMessageEx(HWND hwnd, LPCSTR lpFormat, ...)
{
	va_list list;
	char *out;
	int ret_code;

	va_start(list, lpFormat);
	vasprintf(&out, lpFormat, list);
	va_end(list);

	ret_code = SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM) out);

	zfree(out);

	return ret_code;
}

int MessageBoxFmt(HWND hWnd, LPCTSTR lpCaption, UINT uType, LPCSTR lpFormat, ...)
{
	va_list list;
	char *out;
	int ret_code;

	va_start(list, lpFormat);
	vasprintf(&out, lpFormat, list);
	va_end(list);

	ret_code = MessageBox(hWnd, out, lpCaption, uType);

	zfree(out);

	return ret_code;
}

// move to zmessages: code checked - ok
char * AllocGetDlgItemText(HWND hwnd, int nitem)
{
	int count = GetWindowTextLength(GetDlgItem(hwnd, nitem))+1;

	if(count == 1)
	{
		return nullstring;
	}

	char *text = (char *) zalloc(count);
	if(text)
	{
		GetDlgItemText(hwnd, nitem, text, count);
	}
	else
	{
		return nullstring;
	}
	
	return text;
}

char * AllocGetWindowText(HWND hwnd)
{
	int count = GetWindowTextLength(hwnd)+1;

	if(count == 1)
	{
		return nullstring;
	}

	char *text = (char *) zalloc(count);
	if(text)
	{
		GetWindowText(hwnd, text, count);
	}
	else
	{
		return nullstring;
	}
	
	return text;
}

void SetStatusBarText(HWND hwndStatusBar, unsigned int nPart, LPCTSTR strings, ...)
{
	va_list list;
	char *out;

	va_start(list, strings);
	vasprintf(&out, strings, list);
	va_end(list);

	SendMessage(hwndStatusBar, SB_SETTEXT, (WPARAM) nPart, (LPARAM) out);

	zfree(out);
}

void AppendText(HWND hwnd, LPCTSTR strings, ...)
{
	va_list list;
	char *out;
	int ndx;

	va_start(list, strings);
	vasprintf(&out, strings, list);
	va_end(list);

	ndx = GetWindowTextLength(hwnd);
	SendMessage(hwnd, EM_SETSEL, (WPARAM) ndx, (LPARAM) ndx);

	SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM) out);

	zfree(out);
}

