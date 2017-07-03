//#include "stdafx.h"								//pf_ziminski  [2004]
#include "zstylewnd.h"
#include "zcontrols.h"
#include <stdlib.h>

WNDPROC CDAMNStyleDlg::Init(HWND hwnd)
{
	FlatButtonSetColors(0x808080, 0x404040, 0xA0A0A0, 0x808080, 0x404040, 0x000000);
	FlatEditSetColors(0x808080, 0x808080);
	FlatStaticSetColors(0x808080, 0x505050);
	FlatButtonMake(hwnd);
	FlatEditMake(hwnd);
	FlatStaticMake(hwnd);

	return CWndObject::Init(hwnd);
}

LRESULT CDAMNStyleDlg::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		/*
		case WM_NCRBUTTONDOWN:
			{
				DWORD result = (DWORD) GetSystemMenu(hwnd, FALSE);
				if(result)
				{
					result = TrackPopupMenu((HMENU) result, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hwnd, NULL);
					if(result)
					{
						PostMessage(hwnd, WM_SYSCOMMAND, (WPARAM) result, (LPARAM) -1);
					}
				}
				return 0L;
			}
			//*/
		/*
		case WM_NCLBUTTONDBLCLK:
			if(IsOverButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				PostMessage(hwnd, WM_NCLBUTTONDOWN, wParam, lParam);
				return 0L;
			}
			break;
		case WM_NCLBUTTONDOWN:
			if(IsOverButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				dwOptions |= mouse_captured | pushed_close | pushed_maximize | pushed_minimize;
				PostMessage(hwnd, WM_NCPAINT, 0, NULL);
				return 0L;
			}
			break;
		case WM_NCLBUTTONUP:
			if(dwOptions & mouse_captured)
			{
				if(IsOverButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
				{
					//close
				}
				dwOptions &= ~(mouse_captured | pushed_close | pushed_maximize | pushed_minimize);
				PostMessage(hwnd, WM_NCPAINT, 0, NULL);
				return 0L;
			}
			break;
		case WM_NCMOUSEMOVE:
			if(dwOptions & mouse_captured)
			{
				if(!IsOverButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
				{
					dwOptions &= ~(mouse_captured | pushed_close | pushed_maximize | pushed_minimize);
					PostMessage(hwnd, WM_NCPAINT, 0, NULL);
				}
				return 0L;
			}
			break;
		*/
		case WM_NCACTIVATE:
		case WM_SETTEXT:
			{
				DWORD style;
				LRESULT result;
				
				style = GetWindowLong(m_hwnd, GWL_STYLE);
				SetWindowLong(m_hwnd, GWL_STYLE, style & ~WS_VISIBLE);
				result = CWndObject::WindowProc(uMsg, wParam, lParam);
				SetWindowLong(m_hwnd, GWL_STYLE, style);
				//InvalidateRect(hwnd, NULL, TRUE);
				PostMessage(m_hwnd, WM_NCPAINT, 0, NULL);
				return result;
			}
		case WM_NCPAINT:
			DrawTitleBar(GetForegroundWindow() == m_hwnd);
			return 0L; // must be 0L as ask in MSDN, but better work 1L
		case WM_DRAWITEM:
			FlatButtonDraw((LPDRAWITEMSTRUCT) lParam);
			return 1L;
		case WM_WINDOWPOSCHANGING:
			{
				#define lpwp ((LPWINDOWPOS) lParam)
				RECT WorkArea;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
				WorkArea.right -= lpwp->cx;
				WorkArea.bottom -= lpwp->cy;
				if(abs(WorkArea.left - lpwp->x) <= 10)
				{
					lpwp->x = WorkArea.left;
				}
				if(abs(WorkArea.right - lpwp->x) <= 10)
				{
					lpwp->x = WorkArea.right;
				}
				if(abs(WorkArea.top - lpwp->y) <= 10)
				{
					lpwp->y = WorkArea.top;
				}
				if(abs(WorkArea.bottom - lpwp->y) <= 10)
				{
					lpwp->y = WorkArea.bottom;
				}
			}
			break;
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
			//SetBkMode((HDC) wParam, TRANSPARENT);
			SetBkColor((HDC) wParam, 0x000000);
			SetTextColor((HDC) wParam, (COLORREF) IsWindowEnabled((HWND) lParam)?0xA0A0A0:0x808080);
			return (LRESULT) GetStockObject(BLACK_BRUSH);
	}
	return CWndObject::WindowProc(uMsg, wParam, lParam);
}

DWORD CDAMNStyleDlg::IsOverButton(DWORD dwX, DWORD dwY)
{
	if(!(GetWindowLong(m_hwnd, GWL_STYLE) & WS_CAPTION))
	{
		return 0L;
	}

	RECT rect;
	RECT clrect;

	GetWindowRect(m_hwnd, &rect);
	GetClientRect(m_hwnd, &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) (&clrect)+1);

	/*
	clrect.left = clrect.left - rect.left;
	clrect.top = clrect.top - rect.top;
	clrect.right = clrect.right - rect.left;
	clrect.bottom = clrect.bottom - rect.top;
	*/

	NONCLIENTMETRICS metrix;
	metrix.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrix, NULL);

	if(GetWindowLong(m_hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
	{
		metrix.iCaptionHeight = metrix.iSmCaptionHeight;
		metrix.iCaptionWidth = metrix.iSmCaptionWidth;
		metrix.lfCaptionFont = metrix.lfSmCaptionFont;
	}

	rect.left = clrect.right - metrix.iCaptionHeight+2;
	rect.top = clrect.top - metrix.iCaptionHeight + 1;
	rect.right = clrect.right -2;
	rect.bottom = clrect.top -3;

	POINT pt = {dwX, dwY};
	if(PtInRect(&rect, pt)) return 1L;

	rect.left -= metrix.iCaptionHeight-2;
	rect.right -= metrix.iCaptionHeight-2;
	if(PtInRect(&rect, pt)) return 1L;

	rect.left -= metrix.iCaptionHeight-4;
	rect.right -= metrix.iCaptionHeight-4;
	if(PtInRect(&rect, pt)) return 1L;

	return 0L;
}

void CDAMNStyleDlg::DrawTitleBar(BOOL fActive)
{
	HDC hdc;
	RECT rect;
	HPEN hPen, hPenOld;

	GetWindowRect(m_hwnd, &rect);
	
	hdc = GetWindowDC(m_hwnd);
	//hdc = GetDCEx(hwnd, (HRGN)wParam, DCX_INTERSECTRGN);
	//OutputDebug("GetDCEx: %d\n", GetLastError());

	RECT clrect;
	GetClientRect(m_hwnd, &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) (&clrect)+1);

	clrect.left = clrect.left - rect.left;
	clrect.top = clrect.top - rect.top;
	clrect.right = clrect.right - rect.left;
	clrect.bottom = clrect.bottom - rect.top;

	ExcludeClipRect(hdc, clrect.left, clrect.top, clrect.right, clrect.bottom);
	//ExcludeClipRect(hdc, clrect.left - rect.left, clrect.top - rect.top, clrect.right - rect.left, clrect.bottom - rect.top);

	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.top = 0;
	rect.left = 0;
	//FillRect(hdc, &rect, (HBRUSH) GetStockObject(WHITE_BRUSH));
	FillRect(hdc, &rect, (HBRUSH) GetStockObject(BLACK_BRUSH));

	//hPen = CreatePen(PS_SOLID, 0, 0x000000);
	hPen = CreatePen(PS_SOLID, 0, 0x808080);
	hPenOld = (HPEN) SelectObject(hdc, hPen);
	MoveToEx(hdc, 0, rect.bottom, NULL);
	LineTo(hdc, 0, 0);
	LineTo(hdc, rect.right-1, 0);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	//hPen = CreatePen(PS_SOLID, 0, 0x000000);
	hPen = CreatePen(PS_SOLID, 0, 0x404040);
	SelectObject(hdc, hPen);
	LineTo(hdc, rect.right-1, rect.bottom-1);
	LineTo(hdc, 0, rect.bottom-1);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);

	if(GetWindowLong(m_hwnd, GWL_STYLE) & WS_CAPTION)
	{
		NONCLIENTMETRICS metrix;
		metrix.cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrix, NULL);

		if(GetWindowLong(m_hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		{
			metrix.iCaptionHeight = metrix.iSmCaptionHeight;
			metrix.iCaptionWidth = metrix.iSmCaptionWidth;
			metrix.lfCaptionFont = metrix.lfSmCaptionFont;
		}

		rect.left = clrect.left;
		rect.top = clrect.top - metrix.iCaptionHeight - 1;
		rect.right = clrect.right;
		rect.bottom = clrect.top -1;

		FillRect(hdc, &rect, (HBRUSH) GetStockObject(DKGRAY_BRUSH));

		//рисуем иконку окна
		rect.left = rect.left + 2;
		DrawIconEx(hdc, rect.left, rect.top+1,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(102)),
			metrix.iCaptionWidth-2, metrix.iCaptionHeight-2, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);


		//выводим заголовок окна
		rect.left = rect.left + metrix.iSmCaptionWidth + 3;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, fActive?0xA0A0A0:0x808080);
		HFONT hFont = CreateFontIndirect(&metrix.lfCaptionFont);
		HFONT hFontOld = (HFONT) SelectObject(hdc, hFont);
		char lpstr[512];
		GetWindowText(m_hwnd, lpstr, 512);
		DrawText(hdc, lpstr, lstrlen(lpstr), &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);

		//metrix.iSmCaptionWidth = GetSystemMetrics(SM_CXSMSIZE);

		/*
		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth+2, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_close)?104:103)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);

		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth-metrix.iCaptionWidth+4, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_minimize)?108:107)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);

		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth-metrix.iCaptionWidth-metrix.iCaptionWidth+8, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_maximize)?106:105)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);
		*/
	}

	ReleaseDC(m_hwnd, hdc);
}

WNDPROC CStyledDlg::Init(HWND hwnd)
{
	FlatButtonSetColors(0x808080, 0x404040, 0xA0A0A0, 0x808080, 0x404040, 0x000000);
	FlatEditSetColors(0x808080, 0x808080);
	FlatStaticSetColors(0x808080, 0x505050);
	FlatButtonMake(hwnd);
	FlatEditMake(hwnd);
	FlatStaticMake(hwnd);

	return CDlgObject::Init(hwnd);
}

LRESULT CStyledDlg::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_NCACTIVATE:
			// Важно: на это сообщение надо возвращать нуль, чтобы окно могло терять фокус!
			DrawTitleBar(wParam);
			SetWindowLong(m_hwnd, DWL_MSGRESULT, 1L); // даем разрешение ОС на де/активацию окна
			return 1L; // сообщаем верхней оконной процедуре диалога, что мы обработали сообщение
		/*
		case WM_SETTEXT:
			/* не помню зачем такая сложность была нужна!?
			Затем, чтобы виндвс не рисовал свой вариант тулбара.
			{
				DWORD style;
				LRESULT result;
				
				style = GetWindowLong(m_hwnd, GWL_STYLE);
				SetWindowLong(m_hwnd, GWL_STYLE, style & ~WS_VISIBLE);
				result = CDlgObject::WindowProc(uMsg, wParam, lParam);
				SetWindowLong(m_hwnd, GWL_STYLE, style);
				//InvalidateRect(m_hwnd, NULL, TRUE);
				PostMessage(m_hwnd, WM_NCPAINT, 0, NULL);
				return result;
			}
			DrawTitleBar(wParam);
			return 1L;
		*/
		case WM_NCPAINT:
			DrawTitleBar(GetForegroundWindow() == m_hwnd);
			SetWindowLong(m_hwnd, DWL_MSGRESULT, 0L);
			return 1L;
			// Look SetWindowLong(DWL_DLGPROC) and DialogProc.
			// (Typically, the dialog box procedure should return TRUE if it processed the message)
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
			switch(wParam)
			{
				case HTMAXBUTTON:
				case HTMINBUTTON:
				case HTCLOSE:
					return 1L;
			}
			break;
		case WM_DRAWITEM:
			FlatButtonDraw((LPDRAWITEMSTRUCT) lParam);
			return 1L;
		case WM_WINDOWPOSCHANGING:
			{
				#define lpwp ((LPWINDOWPOS) lParam)
				RECT WorkArea;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
				WorkArea.right -= lpwp->cx;
				WorkArea.bottom -= lpwp->cy;
				if(abs(WorkArea.left - lpwp->x) <= 10)
				{
					lpwp->x = WorkArea.left;
				}
				if(abs(WorkArea.right - lpwp->x) <= 10)
				{
					lpwp->x = WorkArea.right;
				}
				if(abs(WorkArea.top - lpwp->y) <= 10)
				{
					lpwp->y = WorkArea.top;
				}
				if(abs(WorkArea.bottom - lpwp->y) <= 10)
				{
					lpwp->y = WorkArea.bottom;
				}
			}
			break; // call next WndProc
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
			//SetBkMode((HDC) wParam, TRANSPARENT);
			SetBkColor((HDC) wParam, FlatButtonGetBackgroundColor());
			SetTextColor((HDC) wParam, (COLORREF) IsWindowEnabled((HWND) lParam)?FlatButtonGetTextColor():FlatButtonGetTextDisabledColor());
			return (LRESULT) GetStockObject(BLACK_BRUSH);
	}
	return CDlgObject::WindowProc(uMsg, wParam, lParam);
}

LRESULT CStyledDlg::AfterCreate(WPARAM wParam, LPARAM lParam)
{
	FlatButtonSetColors(0x808080, 0x404040, 0xA0A0A0, 0x808080, 0x404040, 0x000000);
	FlatEditSetColors(0x808080, 0x808080);
	FlatStaticSetColors(0x808080, 0x505050);
	FlatButtonMake(m_hwnd);
	FlatEditMake(m_hwnd);
	FlatStaticMake(m_hwnd);

	return CDlgObject::AfterCreate(wParam, lParam);
}

DWORD CStyledDlg::IsOverButton(DWORD dwX, DWORD dwY)
{
	if(!(GetWindowLong(m_hwnd, GWL_STYLE) & WS_CAPTION))
	{
		return 0L;
	}

	RECT rect;
	RECT clrect;

	GetWindowRect(m_hwnd, &rect);
	GetClientRect(m_hwnd, &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) (&clrect)+1);

	/*
	clrect.left = clrect.left - rect.left;
	clrect.top = clrect.top - rect.top;
	clrect.right = clrect.right - rect.left;
	clrect.bottom = clrect.bottom - rect.top;
	*/

	NONCLIENTMETRICS metrix;
	metrix.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrix, NULL);

	if(GetWindowLong(m_hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
	{
		metrix.iCaptionHeight = metrix.iSmCaptionHeight;
		metrix.iCaptionWidth = metrix.iSmCaptionWidth;
		metrix.lfCaptionFont = metrix.lfSmCaptionFont;
	}

	rect.left = clrect.right - metrix.iCaptionHeight+2;
	rect.top = clrect.top - metrix.iCaptionHeight + 1;
	rect.right = clrect.right -2;
	rect.bottom = clrect.top -3;

	POINT pt = {dwX, dwY};
	if(PtInRect(&rect, pt)) return 1L;

	rect.left -= metrix.iCaptionHeight-2;
	rect.right -= metrix.iCaptionHeight-2;
	if(PtInRect(&rect, pt)) return 1L;

	rect.left -= metrix.iCaptionHeight-4;
	rect.right -= metrix.iCaptionHeight-4;
	if(PtInRect(&rect, pt)) return 1L;

	return 0L;
}

void CStyledDlg::DrawTitleBar(BOOL fActive)
{
	HDC hdc;
	RECT rect;
	HPEN hPen, hPenOld;

	GetWindowRect(m_hwnd, &rect);
	
	hdc = GetWindowDC(m_hwnd);
	//hdc = GetDCEx(hwnd, (HRGN)wParam, DCX_INTERSECTRGN);
	//OutputDebug("GetDCEx: %d\n", GetLastError());

	RECT clrect;
	GetClientRect(m_hwnd, &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) &clrect);
	ClientToScreen(m_hwnd, (LPPOINT) (&clrect)+1);

	clrect.left = clrect.left - rect.left;
	clrect.top = clrect.top - rect.top;
	clrect.right = clrect.right - rect.left;
	clrect.bottom = clrect.bottom - rect.top;

	ExcludeClipRect(hdc, clrect.left, clrect.top, clrect.right, clrect.bottom);
	//ExcludeClipRect(hdc, clrect.left - rect.left, clrect.top - rect.top, clrect.right - rect.left, clrect.bottom - rect.top);

	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.top = 0;
	rect.left = 0;
	//FillRect(hdc, &rect, (HBRUSH) GetStockObject(WHITE_BRUSH));
	FillRect(hdc, &rect, (HBRUSH) GetStockObject(BLACK_BRUSH));

	//hPen = CreatePen(PS_SOLID, 0, 0x000000);
	hPen = CreatePen(PS_SOLID, 0, 0x808080);
	hPenOld = (HPEN) SelectObject(hdc, hPen);
	MoveToEx(hdc, 0, rect.bottom, NULL);
	LineTo(hdc, 0, 0);
	LineTo(hdc, rect.right-1, 0);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	//hPen = CreatePen(PS_SOLID, 0, 0x000000);
	hPen = CreatePen(PS_SOLID, 0, 0x404040);
	SelectObject(hdc, hPen);
	LineTo(hdc, rect.right-1, rect.bottom-1);
	LineTo(hdc, 0, rect.bottom-1);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);

	if(GetWindowLong(m_hwnd, GWL_STYLE) & WS_CAPTION)
	{
		NONCLIENTMETRICS metrix;
		metrix.cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrix, NULL);

		if(GetWindowLong(m_hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		{
			metrix.iCaptionHeight = metrix.iSmCaptionHeight;
			metrix.iCaptionWidth = metrix.iSmCaptionWidth;
			metrix.lfCaptionFont = metrix.lfSmCaptionFont;
		}

		rect.left = clrect.left;
		rect.top = clrect.top - metrix.iCaptionHeight - 1;
		rect.right = clrect.right;
		rect.bottom = clrect.top -1;

		FillRect(hdc, &rect, (HBRUSH) GetStockObject(DKGRAY_BRUSH));

		//рисуем иконку окна
		rect.left = rect.left + 2;
		if(
			DrawIconEx(hdc, rect.left, rect.top+1,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(102)),
			metrix.iCaptionWidth-2, metrix.iCaptionHeight-2, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL)
		)
		{
			rect.left += metrix.iSmCaptionWidth;
		}


		//выводим заголовок окна
		rect.left += 3;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, fActive?0xA0A0A0:0x808080);
		HFONT hFont = CreateFontIndirect(&metrix.lfCaptionFont);
		HFONT hFontOld = (HFONT) SelectObject(hdc, hFont);
		char lpstr[512];
		GetWindowText(m_hwnd, lpstr, 512);
		DrawText(hdc, lpstr, lstrlen(lpstr), &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);

		//metrix.iSmCaptionWidth = GetSystemMetrics(SM_CXSMSIZE);

		/*
		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth+2, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_close)?104:103)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);

		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth-metrix.iCaptionWidth+4, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_minimize)?108:107)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);

		DrawIconEx(hdc, rect.right-metrix.iCaptionWidth-metrix.iCaptionWidth-metrix.iCaptionWidth+8, rect.top+2,
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE((dwOptions & pushed_maximize)?106:105)),
			metrix.iCaptionWidth-4, metrix.iCaptionHeight-4, 0, (HBRUSH) GetStockObject(BLACK_BRUSH), DI_NORMAL);
		*/
	}

	ReleaseDC(m_hwnd, hdc);
}
