#ifndef _ZWINDOW_H_
#define _ZWINDOW_H_

//#pragma once
#include <windows.h>

class CWndObject
{
	protected:
		HWND m_hwnd;
		WNDPROC m_WndProcOld;

	public:
		CWndObject();
		~CWndObject();

		virtual WNDPROC Init(HWND hwnd);
		HWND GetHandle() const { return m_hwnd; };
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class CDlgObject: public CWndObject
{
	protected:
		int nExitCode;
	public:
		CDlgObject();
		~CDlgObject();
		virtual WNDPROC CDlgObject::Init(HWND hwnd);
		HWND Create(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent);
		HWND CreateParam(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent, LPARAM lParam);
		int Do();
		void Destroy();
		void Show(int cmd);
		void EndDialog(int nCode);
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT AfterCreate(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		virtual void MessageLoop();
};

LRESULT CALLBACK AfxWndProcBase(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //_ZWINDOW_H_