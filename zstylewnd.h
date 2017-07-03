#ifndef _ZSTYLEWND_H_
#define _ZSTYLEWND_H_

//#pragma once

#include "zwindow.h"

class CDAMNStyleDlg: public CWndObject
{
	protected:
		DWORD dwOptions;

		enum Options
		{
			mouse_captured	= 0x0001,
			pushed_close	= 0x0002,
			pushed_maximize	= 0x0003,
			pushed_minimize	= 0x0004,
		};
	
		void DrawTitleBar(BOOL fActive);
		DWORD IsOverButton(DWORD dwX, DWORD dwY);

	public:
		WNDPROC Init(HWND hwnd);
		LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class CStyledDlg: public CDlgObject
{
	protected:
		DWORD dwOptions;

		enum Options
		{
			mouse_captured	= 0x0001,
			pushed_close	= 0x0002,
			pushed_maximize	= 0x0003,
			pushed_minimize	= 0x0004,
		};

		void DrawTitleBar(BOOL fActive);
		DWORD IsOverButton(DWORD dwX, DWORD dwY);

	public:
		virtual WNDPROC Init(HWND hwnd);
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT AfterCreate(WPARAM wParam, LPARAM lParam);
};


#endif //_ZSTYLEWND_H_
