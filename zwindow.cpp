//#include "stdafx.h"								//pf_ziminski  [2004]
#include "zdbg.h"
#include "zalloc.h"
#include "zwindow.h"
//#include <malloc.h>

typedef struct _CLASSESLIST
{
	_CLASSESLIST *next;
	CWndObject *object;
} CLASSESLIST, *LPCLASSESLIST;

/*
class CClassesList
{
	protected:
		static LPCLASSESLIST lpClassesList;

	public:
		CClassesList()
			{
				lpClassesList = NULL;
			};
		~CClassesList()
			{
				LPCLASSESLIST temp_node = lpClassesList;
				LPCLASSESLIST del_node;
				while(temp_node)
				{
					del_node = temp_node;
					temp_node = temp_node->next;
					free(del_node);
				}
			};
		void clslist_add_node(LPVOID lpObject)
			{
				LPCLASSESLIST new_node = (LPCLASSESLIST) malloc(sizeof(CLASSESLIST));

				if(!new_node) return;

				new_node ->next = NULL;
				new_node->object = (CWndObject*) lpObject;

				if(lpClassesList)
				{
					LPCLASSESLIST temp_node = lpClassesList;
					while(temp_node)
					{
						temp_node = temp_node->next;
					}

					temp_node->next = new_node;
				}
				else
				{
					lpClassesList = new_node;
				}
			};
		void clslist_free_node(LPVOID lpObject)
			{
				if(lpClassesList)
				{
					LPCLASSESLIST del_node;
					if(lpClassesList->object == lpObject)
					{
						del_node = lpClassesList;
						lpClassesList = lpClassesList->next;
					}
					else
					{
						LPCLASSESLIST temp_node = lpClassesList;
						while(temp_node->next->object != lpObject)
						{
							temp_node = temp_node->next;
						}

						del_node = temp_node->next;
						temp_node->next = temp_node->next->next;
					}

					free(del_node);
				}
			};
};
//*/

LPCLASSESLIST lpClassesList = NULL;

LPCLASSESLIST clslist_add_node(LPVOID lpObject);
void clslist_free_node(LPVOID lpObject);

// moved to zwindow.h
//LRESULT CALLBACK AfxWndProcBase(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CWndObject::CWndObject()
{
	m_hwnd = NULL;
	m_WndProcOld = NULL;
	clslist_add_node(this);
}

WNDPROC CWndObject::Init(HWND hwnd)
{
	if(IsWindow(hwnd))
	{
		m_hwnd = hwnd;
		m_WndProcOld = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) ::AfxWndProcBase);

		return m_WndProcOld;
	}

	return 0L;
};

CWndObject::~CWndObject()
{
	if(IsWindow(m_hwnd))
	{
		SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG) m_WndProcOld);
	}
	clslist_free_node(this);
};

LRESULT CWndObject::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if(uMsg == WM_NCPAINT) ZTRACE("DBG: 0. CWndObject::WindowProc()\n");
	if(m_WndProcOld)
	{
		return CallWindowProc(m_WndProcOld, m_hwnd, uMsg, wParam, lParam);
	}

	return 0L;
}

CDlgObject::CDlgObject()
{
	nExitCode = 0;
}

CDlgObject::~CDlgObject()
{
	if(m_hwnd)
	{
		DestroyWindow(m_hwnd);
	}
	//CWndObject::~CWndObject(); // не требуется. вызывается автоматически
}

WNDPROC CDlgObject::Init(HWND hwnd)
{
	if(IsWindow(hwnd))
	{
		m_hwnd = hwnd;
		m_WndProcOld = (WNDPROC) SetWindowLong(hwnd, DWL_DLGPROC, (LONG) ::AfxWndProcBase);

		// получается так:
		// Оконная процедура устанавливается DefDialogProc,
		// а она уже вызывает AfxWndProcBase.
		// Следовательно, если сообщение было обработано в AfxWndProcBase,
		// то нужно вернуть значение TRUE.
		// Look SetWindowLong(DWL_DLGPROC) and DialogProc.
		// (Typically, the dialog box procedure should return TRUE if it processed the message)
		// Чтобы не было различий в возвращаемых значениях оконными процедурами
		// при использовании Create или Init - переопределяем Init для установки
		// DWL_DLGPROC вместо GWL_WNDPROC.


		return m_WndProcOld;
	}

	return 0L;
};

HWND CDlgObject::Create(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent)
{
	// Замечание: Диалоговый окна всегда имеют оконную процедуру верхнего уровня,
	// которая в свою очередь вызывает пользовательскую процедуру окна.
	// Поэтому полноценного перехвата сообщений получаться не может.
	// Например WM_NCPAINT после пользовательской процедуры будет отрабатываться
	// стандартной процедурой.

	//m_WndProcOld = DefDlgProc;
	//m_WndProcOld = DefWindowProc;

	// метод 1: не полноценный перехват (различия в возвращаемых значениях на сообщения)
	m_hwnd = CreateDialog(hInstance, lpTemplate, hWndParent, (DLGPROC) AfxWndProcBase);
	// Правильное поведение оконной процедуры:
	// Если наша процедура обработала сообщение и не трубуется дальнейщая обработка
	// верхней процедурой, то делаем return 1L;
	// Для ОС возвращаем значение на обработанной сообщение
	// через SetWindowLong(m_hwnd, DWL_MSGRESULT, 0L);
	
	// метод 2: полноценный перехват (возвращаемые значения как для обычных окон)
	//m_hwnd = CreateDialog(hInstance, lpTemplate, hWndParent, (DLGPROC) NULL);
	//m_WndProcOld = (WNDPROC) SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG) ::AfxWndProcBase);
	ZTRACE("DBG: WndProc DefDlgProc: 0x%.8X, GWL_WNDPROC: 0x%.8X, DWL_DLGPROC: 0x%.8X, AfxBase: 0x%.8X\n", DefDlgProc, GetWindowLong(m_hwnd, GWL_WNDPROC), GetWindowLong(m_hwnd, DWL_DLGPROC), AfxWndProcBase);
	if(m_hwnd)
	{
		AfterCreate(0L, NULL);
	}

	return m_hwnd;
}

HWND CDlgObject::CreateParam(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent, LPARAM lParam)
{
	//m_WndProcOld = DefDlgProc;
	//m_WndProcOld = DefWindowProc;

	m_hwnd = CreateDialogParam(hInstance, lpTemplate, hWndParent, (DLGPROC) AfxWndProcBase, lParam);
	//m_hwnd = CreateDialog(hInstance, lpTemplate, hWndParent, (DLGPROC) NULL);
	//m_WndProcOld = (WNDPROC) SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG) ::AfxWndProcBase);
	ZTRACE("DBG: WndProc DefDlgProc: 0x%.8X, GWL_WNDPROC: 0x%.8X, DWL_DLGPROC: 0x%.8X, afx: 0x%.8X\n", DefDlgProc, GetWindowLong(m_hwnd, GWL_WNDPROC), GetWindowLong(m_hwnd, DWL_DLGPROC), AfxWndProcBase);
	if(m_hwnd)
	{
		AfterCreate(0L, lParam);
	}

	return m_hwnd;
}

int CDlgObject::Do()
{
	if(m_hwnd)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		MessageLoop();
		//m_hwnd = NULL; // обнуляется в WM_DESTROY
	}
	return nExitCode;
}

void CDlgObject::MessageLoop()
{
	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0 ) > 0)
	{
		//здесь вообще непонятно что делать!
		if(!IsWindow(m_hwnd) || !IsDialogMessage(m_hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CDlgObject::Show(int cmd)
{
	if(m_hwnd)
	{
		ShowWindow(m_hwnd, cmd);
	}
}

void CDlgObject::Destroy()
{
	if(m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;
	}
}

void CDlgObject::EndDialog(int nCode)
{
	nExitCode = nCode;
	Destroy();
}

LRESULT CDlgObject::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		/* this messages skipped
		case WM_INITDIALOG:
			MessageBox(NULL, _T("WM_INITDIALOG"), _T("Message received"), MB_OK);
			break;
		case WM_CREATE:
			MessageBox(NULL, _T("WM_CREATE"), _T("Message received"), MB_OK);
			break;
		*/

		case WM_COMMAND:
			return OnCommand(wParam, lParam);
		case WM_DESTROY:
			return OnDestroy(wParam, lParam);
	}

	//if(uMsg == WM_NCPAINT) ZTRACE("DBG: 1. CDlgObject::WindowProc()\n");
	return CWndObject::WindowProc(uMsg, wParam, lParam);
}

LRESULT CDlgObject::AfterCreate(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

LRESULT CDlgObject::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CWndObject::WindowProc(WM_COMMAND, wParam, lParam);
}

LRESULT CDlgObject::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return CWndObject::WindowProc(WM_DESTROY, wParam, lParam);
}

LPCLASSESLIST clslist_add_node(LPVOID lpObject)
{
	LPCLASSESLIST new_node = (LPCLASSESLIST) zalloc(sizeof(CLASSESLIST));

	if(!new_node) return 0L;

	new_node ->next = NULL;
	new_node->object = (CWndObject*) lpObject;

	if(lpClassesList)
	{
		LPCLASSESLIST temp_node = lpClassesList;
		while(temp_node->next)
		{
			temp_node = temp_node->next;
		}

		temp_node->next = new_node;
	}
	else
	{
		lpClassesList = new_node;
	}

	return new_node;
}

void clslist_free_node(LPVOID lpObject)
{
	if(lpClassesList)
	{
		LPCLASSESLIST del_node;
		if(lpClassesList->object == lpObject)
		{
			del_node = lpClassesList;
			lpClassesList = lpClassesList->next;
		}
		else
		{
			LPCLASSESLIST temp_node = lpClassesList;
			while(temp_node->next->object != lpObject)
			{
				temp_node = temp_node->next;
			}

			del_node = temp_node->next;
			temp_node->next = temp_node->next->next;
		}

		zfree(del_node);
	}
}

LRESULT CALLBACK AfxWndProcBase(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPCLASSESLIST lpList = lpClassesList;

	while(lpList)
	{
		if(lpList->object->GetHandle() == hwnd)
		{
			//*
			unsigned long ret = lpList->object->WindowProc(uMsg, wParam, lParam);
			//ZTRACE("DBG: AfxWndProcBase: processed message 0x%.4X with result code 0x%.2X\n", uMsg, ret);
			return ret;
			//*/
			//return lpList->object->WindowProc(uMsg, wParam, lParam);
		}

		lpList = lpList->next;
	}

	ZTRACE("DBG: AfxWndProcBase: Skipped message 0x%.8X\n", uMsg);
	return 0L; // may be DefWindowProc?
}

