#ifndef _ZMESSAGES_H_
#define _ZMESSAGES_H_

#include <windows.h>

BOOL SetDlgItemTextEx(HWND hwnd, int nIDDlgItem, LPCSTR lpFormat, ...);
BOOL SetWindowTextEx(HWND hwnd, LPCSTR lpFormat, ...);
LRESULT SetTextMessageEx(HWND hwnd, LPCSTR lpFormat, ...);
int MessageBoxFmt(HWND hWnd, LPCTSTR lpCaption, UINT uType, LPCSTR lpFormat, ...);
char * AllocGetDlgItemText(HWND hwnd, int nitem);
char * AllocGetWindowText(HWND hwnd);
void SetStatusBarText(HWND hwndStatusBar, unsigned int nPart, LPCTSTR strings, ...);
void AppendText(HWND hwnd, LPCTSTR strings, ...);

#endif //_ZMESSAGES_H_