//#include "stdafx.h"								//pf_ziminski  [2004]
#include "zcontrols.h"
#include <commctrl.h>
#include <windowsx.h>
#include "utstrings.h"
#include "zmessages.h"
#include "zdbg.h"

/** FLAT BUTTON CODE **********************************************\
case WM_INITDIALOG:
	FlatButtonSetColors(0x808080, 0x404040, 0xA0A0A0, 0x808080, 0x404040, 0x000000);
	FlatEditSetColors(0x808080, 0x404040);
	FlatStaticSetColors(0x808080, 0x404040);
	FlatButtonMake(hwnd);
	FlatEditMake(hwnd);
	FlatStaticMake(hwnd);
...
case WM_DRAWITEM:
	if(((LPDRAWITEMSTRUCT) lParam)->CtlType == ODT_BUTTON)
	{
		FlatButtonDraw((LPDRAWITEMSTRUCT) lParam);
	}
	return 1L;

case WM_CTLCOLORLISTBOX:
case WM_CTLCOLORSCROLLBAR:
case WM_CTLCOLORBTN:
case WM_CTLCOLOREDIT:
case WM_CTLCOLORSTATIC:
	SetBkMode((HDC) wParam, TRANSPARENT);
	SetTextColor((HDC) wParam, (COLORREF) IsWindowEnabled((HWND) lParam)?0xA0A0A0:0x808080);
case WM_CTLCOLORDLG:
	return (LRESULT) GetStockObject(BLACK_BRUSH);
\** FLAT BUTTON CODE **********************************************/

//
// Custom control process messages:
//
//	WM_NCCREATE			- инициализация контрола (надо ли рисовать?)
//	WM_NCCALCSIZE		- инициализация размеров (надо ли рисовать?)
//	WM_CREATE			- инициализация (надо ли рисовать?)
//	WM_SIZE				- после изменения размера
//	WM_MOVE				- после перемещения
//	WM_SHOWWINDOW		- показать/скрыть контрол
//	WM_SETFONT			- установка шрифта, который будет использовать контрол
//	WM_GETDLGCODE		- какие сообщения (клавиши) будет обрабатывать контрол
//	
//	WM_UPDATEUISTATE	- Показывать фокус и быстрые клавижи или нет при нажатии любой клавиши (win2k or later)
//	
//	WM_PAINT			- рисуем (что?)
//	WM_NCPAINT			- рисуем (а здесь что?)
//	WM_ERASEBKGND		- рисуем фон (как?)
//	
//	WM_DESTROY			- удаляем (что?)
//	WM_NCDESTROY		- удаляем (а здесь что?)
//

typedef struct _HYPER_LINK_DATA
{
	char caption[ZC_CAPTION_SIZE+1];
	unsigned long caplen;
	RECT crect;
	RECT tr;
	HFONT hFont;
	HFONT hFontActive;
	HPEN hPenFocus;
	unsigned long flags;
} HYPER_LINK_DATA;

typedef struct _ZCONTROL_DATA
{
	char caption[ZC_CAPTION_SIZE+1];
	unsigned long caplen;
	RECT crect;
	RECT tr;
	HFONT hFont;
	HPEN hPenFocus;
	unsigned long flags;
} ZCONTROL_DATA;

typedef struct _ZEDIT_DATA
{
	char *caption;
	unsigned long caplen;
	unsigned long bufsize;
	RECT wrect;
	RECT crect;
	HFONT hFont;
	HPEN hPenFocus;
	unsigned long flags;

	unsigned char *char_widths;		// ширина каждого символа
	unsigned long total_rows;
	unsigned long caret_x;			// absolute
	unsigned long caret_y;			// relative first_row
	unsigned long selection_start;
	unsigned long selection_end;
	unsigned long current_char;
	// multiline
	unsigned long *row_heights;		// LO_BYTE - максимальная высота строки, HI_TRIBYTE - длинна строки
	unsigned long first_row;
	unsigned long first_x;
	unsigned long current_row;
} ZEDIT_DATA;

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL WM_MOUSELAST+1 
    // Message ID for IntelliMouse wheel
#endif

#define	calc_control_size(zcd, ts)\
				do {\
					if(zcd->flags & ZC_TEXT_RIGHT)\
					{\
						zcd->tr.left = zcd->crect.right - ts.cx - 1;\
					}\
					else if(zcd->flags & ZC_TEXT_CENTER)\
					{\
						zcd->tr.left = (zcd->crect.right + zcd->crect.left - ts.cx - 1)/2;\
					}\
					else\
					{\
						zcd->tr.left = 0;\
					}\
					\
					zcd->tr.top = (zcd->crect.bottom + zcd->crect.top - ts.cy)/2;\
					zcd->tr.right = zcd->tr.left + ts.cx;\
					zcd->tr.bottom = zcd->tr.top + ts.cy;\
					\
					if(zcd->tr.left < 0)\
					{\
						zcd->tr.left = 0;\
					}\
					\
					if(zcd->tr.top < 0)\
					{\
						zcd->tr.top = 0;\
					}\
					\
					if(zcd->tr.right >= zcd->crect.right)\
					{\
						zcd->tr.right = zcd->crect.right - 1;\
					}\
					\
					if(zcd->tr.bottom >= zcd->crect.bottom)\
					{\
						zcd->tr.bottom = zcd->crect.bottom - 1;\
					}\
				} while(0)

#define zedit_scroll_screen(zcd)\
				do {\
					if(zcd->caret_x < zcd->first_x)\
					{\
						if(zcd->caret_x < 30)\
						{\
							zcd->first_x = 0;\
						}\
						else\
						{\
							zcd->first_x = zcd->caret_x - 30;\
						}\
					}\
					else if((zcd->caret_x - zcd->first_x) > zcd->crect.right)\
					{\
						zcd->first_x = zcd->caret_x - (zcd->crect.right - zcd->crect.left) + 30;\
					}\
					if(zcd->first_row > zcd->current_row)\
					{\
						zcd->first_row = zcd->current_row;\
						zcd->caret_y = zcd->crect.top;\
					}\
					else\
					{\
						while((zcd->first_row < zcd->current_row)\
							&& ((signed) (zcd->caret_y + (zcd->row_heights[zcd->current_row] >> 24)) > zcd->crect.bottom))\
						{\
							zcd->caret_y -= zcd->row_heights[zcd->first_row] >> 24;\
							zcd->first_row++;\
						}\
					}\
					ZTRACE("    cc: %d   cr: %d   cx: %d   cy: %d   fx: %d   fr: %d\n", zcd->current_char, zcd->current_row, zcd->caret_x, zcd->caret_y, zcd->first_x, zcd->first_row);\
					if((zcd->flags & (ZC_CARET_VISIBLE | ZC_ACTIVE)) == ZC_ACTIVE)\
					{\
						ShowCaret(hwnd);\
						zcd->flags |= ZC_CARET_VISIBLE;\
					}\
				} while(0)

#ifdef _DEBUG
#define ZCD_DUMP(zcd)\
				do {\
					unsigned long j;\
					unsigned long i;\
					ZTRACE("ZCONTROL DATA DUMP:\n");\
					ZTRACE("    cc: %d   cr: %d   cx: %d   cy: %d   fx: %d   fr: %d\n", zcd->current_char, zcd->current_row, zcd->caret_x, zcd->caret_y, zcd->first_x, zcd->first_row);\
					i = 0;\
					j = 0;\
					while(j < zcd->total_rows)\
					{\
						i += zcd->row_heights[j] & 0xFFFFFF;\
						ZTRACE("    %d: H %d   L%d\n", j, zcd->row_heights[j] >> 24, zcd->row_heights[j] & 0xFFFFFF);\
						j++;\
					}\
					ZTRACE("    TOTAL SIZE: sum(%d) == %d\n", i, zcd->caplen);\
					/*ZTRACE("    DATA:\n%s\n", zcd->caption);*/\
				} while(0)
#else
#define ZCD_DUMP(zcd)	((void) 0)
#endif

COLORREF crLight			= GetSysColor(COLOR_BTNHIGHLIGHT);
COLORREF crShadow			= GetSysColor(COLOR_BTNSHADOW);
COLORREF crText				= GetSysColor(COLOR_BTNTEXT);
COLORREF crTextDisabled		= GetSysColor(COLOR_GRAYTEXT);
COLORREF crHighLight		= GetSysColor(COLOR_BTNTEXT);
COLORREF crBackground		= GetSysColor(COLOR_BTNFACE);

COLORREF crEditLight		= GetSysColor(COLOR_BTNHIGHLIGHT);
COLORREF crEditShadow		= GetSysColor(COLOR_BTNSHADOW);

COLORREF crStaticLight		= GetSysColor(COLOR_BTNHIGHLIGHT);
COLORREF crStaticShadow		= GetSysColor(COLOR_BTNSHADOW);

COLORREF crTabLight			= GetSysColor(COLOR_BTNHIGHLIGHT);
COLORREF crTabShadow		= GetSysColor(COLOR_BTNSHADOW);
COLORREF crTabText			= GetSysColor(COLOR_BTNTEXT);
COLORREF crTabHighLight		= GetSysColor(COLOR_BTNTEXT);
COLORREF crTabBackground	= GetSysColor(COLOR_WINDOW);

WNDPROC FlatButtonOldProc;
WNDPROC FlatEditOldProc;
WNDPROC FlatStaticOldProc;
WNDPROC FlatTabOldProc;

LRESULT CALLBACK FlatButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FlatEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FlatStaticProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FlatTabProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int parse_text(ZEDIT_DATA *zcd, HDC hDC)
{
	char *ch;
	unsigned long i;
	unsigned long j;
	unsigned long len;
	int width;

	ch = zcd->caption;
	i = 0;
	j = 0;
	len = 0;
	while(*ch)
	{
		len++;
		if(*ch != '\n')
		{
			if(*ch != '\t')
			{
				GetCharWidth32(hDC, (unsigned char) *ch, (unsigned char) *ch, &width);
				zcd->char_widths[i] = (unsigned char) width;
			}
			else
			{
				zcd->char_widths[i] = 10;
			}
		}
		else
		{
			zcd->char_widths[i] = 0;
			zcd->row_heights[j] = len & 0xFFFFFF;
			zcd->row_heights[j] |= 13<<24;
			j++;
			zcd->row_heights[j] = 0;
			len = 0;
		}

		i++;
		ch++;
	}
	zcd->row_heights[j] = len & 0xFFFFFF;
	zcd->row_heights[j] |= 13<<24;
	zcd->total_rows = j+1;

	ZCD_DUMP(zcd);

	return 0;
}

void FlatButtonSetColors(COLORREF crLight, COLORREF crShadow, COLORREF crText, COLORREF crTextDisabled, COLORREF crHighLight, COLORREF crBackground)
{
	::crLight = crLight;
	::crShadow = crShadow;
	::crText = crText;
	::crTextDisabled = crTextDisabled;
	::crHighLight = crHighLight;
	::crBackground = crBackground;
}

COLORREF FlatButtonGetBackgroundColor()
{
	return crBackground;
}

COLORREF FlatButtonGetTextColor()
{
	return crText;
}

COLORREF FlatButtonGetTextDisabledColor()
{
	return crTextDisabled;
}

void FlatButtonCreate(HWND hwnd)
{
	FlatButtonOldProc = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) FlatButtonProc);
	SetWindowLong(hwnd, GWL_STYLE, (GetWindowLong(hwnd, GWL_STYLE) & ~0x0F) | BS_OWNERDRAW);
}

LRESULT CALLBACK FlatButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				ClientToScreen(hwnd, &pt);
				if(WindowFromPoint(pt) != hwnd)
				{
					SetWindowLong(hwnd, GWL_USERDATA, GetWindowLong(hwnd, GWL_USERDATA) & ~BTN_MOUSEOVER);
					InvalidateRect(hwnd, NULL, FALSE);
				}
				else if(!(GetWindowLong(hwnd, GWL_USERDATA) & BTN_MOUSEOVER))
				{
					TRACKMOUSEEVENT tm;
					tm.cbSize = sizeof(TRACKMOUSEEVENT);
					tm.dwFlags = TME_LEAVE;
					tm.hwndTrack = hwnd;
					tm.dwHoverTime = 0;
					if(_TrackMouseEvent(&tm))
					{
						SetWindowLong(hwnd, GWL_USERDATA, GetWindowLong(hwnd, GWL_USERDATA) | BTN_MOUSEOVER);
						InvalidateRect(hwnd, NULL, FALSE);
					}
				}
			}
			break;
		case WM_MOUSELEAVE:
			if(GetWindowLong(hwnd, GWL_USERDATA) & BTN_MOUSEOVER)
			{
				SetWindowLong(hwnd, GWL_USERDATA, GetWindowLong(hwnd, GWL_USERDATA) & ~BTN_MOUSEOVER);
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case WM_LBUTTONDBLCLK:
		    PostMessage(hwnd, WM_LBUTTONDOWN, wParam, lParam);
			return 0;
		case WM_LBUTTONDOWN:
			SetWindowLong(hwnd, GWL_USERDATA, GetWindowLong(hwnd, GWL_USERDATA) | BTN_PUSHED);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_LBUTTONUP:
			SetWindowLong(hwnd, GWL_USERDATA, (GetWindowLong(hwnd, GWL_USERDATA) & ~BTN_PUSHED));
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		/*
		case WM_ERASEBKGND:
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				FillRect((HDC) wParam, &rc, (HBRUSH) GetCurrentObject((HDC) wParam, OBJ_BRUSH));
				return 1L;
			}
		//*/
	}

	return CallWindowProc(FlatButtonOldProc, hwnd, uMsg, wParam, lParam);
}

void FlatButtonDraw(LPDRAWITEMSTRUCT lpDrwItemStruct)
{
	char lpstr[512];
	HGDIOBJ hObj, hObjOld;

	hObj = CreateSolidBrush(crBackground);
	FillRect(lpDrwItemStruct->hDC, &lpDrwItemStruct->rcItem, (HBRUSH) hObj);
	DeleteObject(hObj);

	DWORD dwState = GetWindowLong(lpDrwItemStruct->hwndItem, GWL_USERDATA);
	//if(dwState & (BTN_MOUSEOVER|BTN_PUSHED))
	if(dwState & BTN_MOUSEOVER)
	{
		//hObj = CreatePen(PS_SOLID, 0, ((dwState & (BTN_MOUSEOVER|BTN_PUSHED)) == (BTN_MOUSEOVER|BTN_PUSHED))?crShadow:crLight);
		hObj = CreatePen(PS_SOLID, 0, (dwState & BTN_PUSHED)?crShadow:crLight);
		hObjOld = (HPEN) SelectObject(lpDrwItemStruct->hDC, hObj);
		MoveToEx(lpDrwItemStruct->hDC, 0, lpDrwItemStruct->rcItem.bottom-1, NULL);
		LineTo(lpDrwItemStruct->hDC, 0, 0);
		LineTo(lpDrwItemStruct->hDC, lpDrwItemStruct->rcItem.right-1, 0);
		SelectObject(lpDrwItemStruct->hDC, hObjOld);
		DeleteObject(hObj);
		//hObj = CreatePen(PS_SOLID, 0, ((dwState & (BTN_MOUSEOVER|BTN_PUSHED)) == (BTN_MOUSEOVER|BTN_PUSHED))?crLight:crShadow);
		hObj = CreatePen(PS_SOLID, 0, (dwState & BTN_PUSHED)?crLight:crShadow);
		SelectObject(lpDrwItemStruct->hDC, hObj);
		LineTo(lpDrwItemStruct->hDC, lpDrwItemStruct->rcItem.right-1, lpDrwItemStruct->rcItem.bottom-1);
		LineTo(lpDrwItemStruct->hDC, 0, lpDrwItemStruct->rcItem.bottom-1);
		SelectObject(lpDrwItemStruct->hDC, hObjOld);
		DeleteObject(hObj);
	}
	if(lpDrwItemStruct->itemState & ODS_FOCUS)
	{
		hObj = CreatePen(PS_SOLID, 0, crHighLight);
		hObjOld = (HPEN) SelectObject(lpDrwItemStruct->hDC, hObj);
		MoveToEx(lpDrwItemStruct->hDC, 3, 3, NULL);
		LineTo(lpDrwItemStruct->hDC, lpDrwItemStruct->rcItem.right-4, 3);
		LineTo(lpDrwItemStruct->hDC, lpDrwItemStruct->rcItem.right-4, lpDrwItemStruct->rcItem.bottom-4);
		LineTo(lpDrwItemStruct->hDC, 3, lpDrwItemStruct->rcItem.bottom-4);
		LineTo(lpDrwItemStruct->hDC, 3, 3);
		SelectObject(lpDrwItemStruct->hDC, hObjOld);
		DeleteObject(hObj);
	}

	GetWindowText(lpDrwItemStruct->hwndItem, lpstr, 512);
	SetTextColor(lpDrwItemStruct->hDC, (COLORREF) (lpDrwItemStruct->itemState & ODS_DISABLED)?crTextDisabled:crText);
	SetBkMode(lpDrwItemStruct->hDC, TRANSPARENT);
	if(lpDrwItemStruct->itemState & ODS_DEFAULT)
	{
		LOGFONT logfont;
		GetObject(GetCurrentObject(lpDrwItemStruct->hDC, OBJ_FONT), sizeof(LOGFONT), &logfont);
		logfont.lfUnderline = TRUE;
		logfont.lfWeight = FW_BOLD;
		hObj = CreateFontIndirect(&logfont);
		hObjOld = SelectObject(lpDrwItemStruct->hDC, hObj);
	}
	//DrawText(lpDrwItemStruct->hDC, lpstr, lstrlen(lpstr), &lpDrwItemStruct->rcItem, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//lpDrwItemStruct->rcItem.left+((lpDrwItemStruct->rcItem.right-lpDrwItemStruct->rcItem.left)-sz.cx)/2,
	//lpDrwItemStruct->rcItem.top+((lpDrwItemStruct->rcItem.bottom-lpDrwItemStruct->rcItem.top)-sz.cy)/2,
	SIZE sz;
	GetTextExtentPoint32(lpDrwItemStruct->hDC, lpstr, lstrlen(lpstr), &sz);
	ExtTextOut(lpDrwItemStruct->hDC, 
		(lpDrwItemStruct->rcItem.right-sz.cx)/2+(((dwState & (BTN_MOUSEOVER|BTN_PUSHED)) == (BTN_MOUSEOVER|BTN_PUSHED))?1:0),
		(lpDrwItemStruct->rcItem.bottom-sz.cy)/2+(((dwState & (BTN_MOUSEOVER|BTN_PUSHED)) == (BTN_MOUSEOVER|BTN_PUSHED))?1:0),
		ETO_CLIPPED, &lpDrwItemStruct->rcItem, lpstr, lstrlen(lpstr), NULL);
	if(lpDrwItemStruct->itemState & ODS_DEFAULT)
	{
		SelectObject(lpDrwItemStruct->hDC, hObjOld);
	}
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	char lptemp[7];
	if(GetClassName(hwnd, lptemp, 7) != 0)
	{
		if((lParam == 1) && (lstrcmpi(lptemp, "Button") == 0) &&
			((GetWindowLong(hwnd, GWL_STYLE) & 0x0F) <= 1)) //check for button
		{
			FlatButtonCreate(hwnd);
		}
		else if((lParam == 2) && (lstrcmpi(lptemp, "Edit") == 0))
		{
			FlatEditCreate(hwnd);
		}
		else if((lParam == 3) && (lstrcmpi(lptemp, "Static") == 0))
		{
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			if((dwStyle & SS_SUNKEN) ||
				((dwStyle & SS_TYPEMASK) == SS_ETCHEDFRAME) || 
				((dwStyle & SS_TYPEMASK) == SS_ETCHEDHORZ) ||
				((dwStyle & SS_TYPEMASK) == SS_ETCHEDVERT) ||
				((dwStyle & SS_TYPEMASK) == SS_BLACKFRAME))
			{
				FlatStaticCreate(hwnd);
			}
		}
	}
	return TRUE;
}

void FlatButtonMake(HWND hwnd)
{
	EnumChildWindows(hwnd, EnumChildProc, 1);
}

// EDIT ************************************************************
void FlatEditMake(HWND hwnd)
{
	EnumChildWindows(hwnd, EnumChildProc, 2);
}

void FlatEditSetColors(COLORREF crLight, COLORREF crShadow)
{
	::crEditLight = crLight;
	::crEditShadow = crShadow;
}

void FlatEditCreate(HWND hwnd)
{
	FlatEditOldProc = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) FlatEditProc);
}

LRESULT CALLBACK FlatEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_NCPAINT:
			{
				DWORD dwStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
				if(dwStyle & (WS_EX_CLIENTEDGE|WS_EX_STATICEDGE))
				{
					RECT rect;
					HGDIOBJ hObj, hObjOld;
					HDC hdc = GetWindowDC(hwnd);
					GetWindowRect(hwnd, &rect);

					rect.right -= rect.left+((dwStyle & WS_EX_CLIENTEDGE)?1:0);
					rect.bottom -= rect.top+((dwStyle & WS_EX_CLIENTEDGE)?1:0);
					rect.left = ((dwStyle & WS_EX_CLIENTEDGE)?1:0);
					rect.top = ((dwStyle & WS_EX_CLIENTEDGE)?1:0);

					hObj = CreatePen(PS_SOLID, 0, crEditShadow);
					hObjOld = (HPEN) SelectObject(hdc, hObj);
					MoveToEx(hdc, rect.left, rect.bottom-1, NULL);
					LineTo(hdc, rect.left, rect.top);
					LineTo(hdc, rect.right-1, rect.top);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					hObj = CreatePen(PS_SOLID, 0, crEditLight);
					SelectObject(hdc, hObj);
					LineTo(hdc, rect.right-1, rect.bottom-1);
					LineTo(hdc, rect.left, rect.bottom-1);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					ReleaseDC(hwnd, hdc);
					return 0L;
				}
			}
			break;
	}

	return CallWindowProc(FlatEditOldProc, hwnd, uMsg, wParam, lParam);
}

// STATIC ************************************************************
void FlatStaticMake(HWND hwnd)
{
	EnumChildWindows(hwnd, EnumChildProc, 3);
}

void FlatStaticSetColors(COLORREF crLight, COLORREF crShadow)
{
	::crStaticLight = crLight;
	::crStaticShadow = crShadow;
}

void FlatStaticCreate(HWND hwnd)
{
	FlatStaticOldProc = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) FlatStaticProc);
}

LRESULT CALLBACK FlatStaticProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_NCPAINT:
			{
				DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
				if(	((dwStyle & SS_TYPEMASK) != SS_ETCHEDFRAME) &&
					((dwStyle & SS_TYPEMASK) != SS_BLACKFRAME))
				{
					RECT rect;
					HGDIOBJ hObj, hObjOld;
					HDC hdc = GetWindowDC(hwnd);
					GetWindowRect(hwnd, &rect);
					rect.right -= rect.left;
					rect.bottom -= rect.top;
					rect.left = 0;
					rect.top = 0;

					hObj = CreatePen(PS_SOLID, 0, crStaticShadow);
					hObjOld = (HPEN) SelectObject(hdc, hObj);
					MoveToEx(hdc, 0, rect.bottom-1, NULL);
					LineTo(hdc, 0, 0);
					LineTo(hdc, rect.right-1, 0);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					hObj = CreatePen(PS_SOLID, 0, crStaticLight);
					SelectObject(hdc, hObj);
					LineTo(hdc, rect.right-1, rect.bottom-1);
					LineTo(hdc, 0, rect.bottom-1);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					ReleaseDC(hwnd, hdc);
					return 0L;
				}
			}
			break;
		case WM_PAINT:
			{
				DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
				if(	((dwStyle & SS_TYPEMASK) == SS_ETCHEDFRAME) ||
					((dwStyle & SS_TYPEMASK) == SS_BLACKFRAME))
				{
					PAINTSTRUCT ps;
					HDC hdc;
					hdc = wParam?(HDC)wParam:BeginPaint(hwnd, &ps);

					RECT rect;
					HGDIOBJ hObj, hObjOld;
					GetWindowRect(hwnd, &rect);
					rect.right -= rect.left;
					rect.bottom -= rect.top;
					rect.left = 0;
					rect.top = 0;

					hObj = CreatePen(PS_SOLID, 0, crStaticShadow);
					hObjOld = (HPEN) SelectObject(hdc, hObj);
					MoveToEx(hdc, 0, rect.bottom-1, NULL);
					LineTo(hdc, 0, 0);
					LineTo(hdc, rect.right-1, 0);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					hObj = CreatePen(PS_SOLID, 0, crStaticShadow);
					SelectObject(hdc, hObj);
					LineTo(hdc, rect.right-1, rect.bottom-1);
					LineTo(hdc, 0, rect.bottom-1);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					if(!wParam) EndPaint(hwnd, &ps);
					return 0L;
				}
			}
			break;
	}

	return CallWindowProc(FlatStaticOldProc, hwnd, uMsg, wParam, lParam);
}

// TAB *************************************************************
void FlatTabSetColors(COLORREF crLight, COLORREF crShadow, COLORREF crText, COLORREF crHighLight, COLORREF crBackground)
{
	::crTabLight = crLight;
	::crTabShadow = crShadow;
	::crTabText = crText;
	::crTabHighLight = crHighLight;
	::crTabBackground = crBackground;
}

void FlatTabCreate(HWND hwnd)
{
	FlatTabOldProc = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) FlatTabProc);
}

LRESULT CALLBACK FlatTabProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT:
			{
				RECT rect;
				HGDIOBJ hObj, hObjOld;
				PAINTSTRUCT ps;

				HDC hdc = BeginPaint(hwnd, &ps);

				TCITEM ti;
				char lpstr[512];
				DWORD curfocus = TabCtrl_GetCurFocus(hwnd);
				DWORD cursel = TabCtrl_GetCurSel(hwnd);
				DWORD count = TabCtrl_GetItemCount(hwnd);
				while(count--)
				{
					TabCtrl_GetItemRect(hwnd, count, &rect);
					ti.mask = TCIF_TEXT;
					ti.pszText = lpstr;
					ti.cchTextMax = 512;
					TabCtrl_GetItem(hwnd, count, &ti);

					if(count == cursel)
					{
						rect.top = 0;
					}
					hObj = CreatePen(PS_SOLID, 0, crTabLight);
					hObjOld = (HPEN) SelectObject(hdc, hObj);
					MoveToEx(hdc, rect.left, rect.bottom-1, NULL);
					LineTo(hdc, rect.left, rect.top);
					LineTo(hdc, rect.right-1, rect.top);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);

					hObj = CreatePen(PS_SOLID, 0, crTabShadow);
					SelectObject(hdc, hObj);
					LineTo(hdc, rect.right-1, rect.bottom-1);
					SelectObject(hdc, hObjOld);
					DeleteObject(hObj);
					
					SIZE sz;
					SetBkMode(hdc, TRANSPARENT);
					SetTextColor(hdc, crTabText);
					hObj = (HGDIOBJ) SendMessage(hwnd, WM_GETFONT, 0, 0L);
					hObjOld = SelectObject(hdc, hObj);
					LineTo(hdc, rect.right-1, rect.bottom-1);
					GetTextExtentPoint32(hdc, lpstr, lstrlen(lpstr), &sz);
					ExtTextOut(hdc, 
						rect.left+((rect.right-rect.left)-sz.cx)/2,
						rect.top+((rect.bottom-rect.top)-sz.cy)/2,
						ETO_CLIPPED, &rect, lpstr, lstrlen(lpstr), NULL);
					SelectObject(hdc, hObjOld);

					if(count == curfocus)
					{
						hObj = CreatePen(PS_SOLID, 0, crTabHighLight);
						hObjOld = (HPEN) SelectObject(hdc, hObj);
						MoveToEx(hdc, rect.left+3, rect.top+3, NULL);
						LineTo(hdc, rect.right-4, rect.top+3);
						LineTo(hdc, rect.right-4, rect.bottom-4);
						LineTo(hdc, rect.left+3, rect.bottom-4);
						LineTo(hdc, rect.left+3, rect.top+3);
						SelectObject(hdc, hObjOld);
						DeleteObject(hObj);
					}
				}

				GetClientRect(hwnd, &rect);
				RECT rect_tab;
				CopyRect(&rect_tab, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect_tab);
				rect.top = rect_tab.top-2;

				if(cursel >= 0)
				{
					TabCtrl_GetItemRect(hwnd, cursel, &rect_tab);
				}

				hObj = CreatePen(PS_SOLID, 0, crTabLight);
				hObjOld = (HPEN) SelectObject(hdc, hObj);
				MoveToEx(hdc, rect.left, rect.bottom-1, NULL);
				LineTo(hdc, rect.left, rect.top);
				if(cursel >= 0)
				{
					LineTo(hdc, rect_tab.left+1, rect.top);
					MoveToEx(hdc, rect_tab.right-1, rect.top, NULL);
					LineTo(hdc, rect.right-1, rect.top);
				}
				else
				{
					LineTo(hdc, rect.right-1, rect.top);
				}
				SelectObject(hdc, hObjOld);
				DeleteObject(hObj);

				hObj = CreatePen(PS_SOLID, 0, crTabShadow);
				SelectObject(hdc, hObj);
				LineTo(hdc, rect.right-1, rect.bottom-1);
				LineTo(hdc, rect.left, rect.bottom-1);
				SelectObject(hdc, hObjOld);
				DeleteObject(hObj);

				EndPaint(hwnd, &ps);

				return 0L;
			}
		case WM_ERASEBKGND:
			{
				RECT rc;
				HGDIOBJ hObj;
				GetClientRect(hwnd, &rc);
				hObj = CreateSolidBrush(crTabBackground);
				FillRect((HDC) wParam, &rc, (HBRUSH) hObj);
				DeleteObject(hObj);
				return 1L;
			}
	}

	return CallWindowProc(FlatTabOldProc, hwnd, uMsg, wParam, lParam);
}

// HyperLink control *************************************************************

LRESULT CALLBACK HyperLinkProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HYPER_LINK_DATA *hld;

	hld = (HYPER_LINK_DATA *) GetWindowLong(hwnd, 0);

	switch(uMsg)
	{
		// *** SYSTEM MESSAGES ***

		case WM_NCCREATE:
			{
				//ZTRACE("WM_NCCREATE\n");
				hld = (HYPER_LINK_DATA *) zalloc(sizeof(HYPER_LINK_DATA));
				if(!hld)
				{
					return 0L;
				}
				memset(hld, 0, sizeof(HYPER_LINK_DATA));
				SetWindowLong(hwnd, 0, (long) hld);
			}
			return 1L;
		//case WM_NCCALCSIZE:
		case WM_CREATE:
			{
				LOGFONT lf;
				LOGBRUSH lb;
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				hld->flags = ((LPCREATESTRUCT) lParam)->style & ZC_MASK_STYLES;

				if(isempty((char *) ((LPCREATESTRUCT) lParam)->lpszName))
				{
					strncpy_tiny(hld->caption, "HyperLink control v.0.01 (c)oded by dEmon", ZC_CAPTION_SIZE);
				}
				else
				{
					strncpy_tiny(hld->caption, ((LPCREATESTRUCT) lParam)->lpszName, ZC_CAPTION_SIZE);
				}
				hld->caplen = strlen(hld->caption);

				hld->hFont = (HFONT) GetStockObject(SYSTEM_FONT);
				GetObject(hld->hFont, sizeof(lf), &lf);
				lf.lfUnderline = TRUE;
				hld->hFontActive = CreateFontIndirect(&lf);

				lb.lbColor = RGB(128, 128, 128);
				lb.lbHatch = 0;
				lb.lbStyle = BS_SOLID;
				hld->hPenFocus = ExtCreatePen(PS_GEOMETRIC | PS_DOT | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 1, &lb, 0, NULL);

				GetClientRect(hwnd, &hld->crect);
				
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, hld->hFont);
					GetTextExtentPoint32(hdc, hld->caption, hld->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					if(hld->flags & ZC_TEXT_RIGHT)
					{
						hld->tr.left = hld->crect.right - ts.cx - 1;
					}
					else if(hld->flags & ZC_TEXT_CENTER)
					{
						hld->tr.left = (hld->crect.right + hld->crect.left - ts.cx - 1)/2;
					}
					else
					{
						hld->tr.left = 0;
					}

					hld->tr.top = (hld->crect.bottom + hld->crect.top - ts.cy)/2;
					hld->tr.right = hld->tr.left + ts.cx;
					hld->tr.bottom = hld->tr.top + ts.cy;

					if(hld->tr.left < 0)
					{
						hld->tr.left = 0;
					}

					if(hld->tr.top < 0)
					{
						hld->tr.top = 0;
					}

					if(hld->tr.right >= hld->crect.right)
					{
						hld->tr.right = hld->crect.right - 1;
					}

					if(hld->tr.bottom >= hld->crect.bottom)
					{
						hld->tr.bottom = hld->crect.bottom - 1;
					}
				}
			}
			return 0L;
		//case WM_SIZE:
		//case WM_MOVE:
		//case WM_SHOWWINDOW:
		case WM_SETFONT:
			{
				HFONT hFontOld;
				HDC hdc;
				RECT cr;
				SIZE ts;

				if(wParam)
				{
					LOGFONT lf;

					if(hld->hFontActive)
					{
						DeleteObject(hld->hFontActive);
					}

					hld->hFont = (HFONT) wParam;
					GetObject((void *) wParam, sizeof(lf), &lf);
					lf.lfUnderline = TRUE;
					hld->hFontActive = CreateFontIndirect(&lf);
				}

				GetClientRect(hwnd, &cr);

				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, hld->hFont);
					GetTextExtentPoint32(hdc, hld->caption, hld->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					if(hld->flags & ZC_TEXT_RIGHT)
					{
						hld->tr.left = cr.right - ts.cx - 1;
					}
					else if(hld->flags & ZC_TEXT_CENTER)
					{
						hld->tr.left = (cr.right + cr.left - ts.cx - 1)/2;
					}
					else
					{
						hld->tr.left = 0;
					}

					hld->tr.top = (cr.bottom + cr.top - ts.cy)/2;
					hld->tr.right = hld->tr.left + ts.cx;
					hld->tr.bottom = hld->tr.top + ts.cy;

					if(hld->tr.left < 0)
					{
						hld->tr.left = 0;
					}

					if(hld->tr.top < 0)
					{
						hld->tr.top = 0;
					}

					if(hld->tr.right >= cr.right)
					{
						hld->tr.right = cr.right - 1;
					}

					if(hld->tr.bottom >= cr.bottom)
					{
						hld->tr.bottom = cr.bottom - 1;
					}
				}

				if(LOWORD(lParam))
				{
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			break;
		case WM_GETFONT:
			return (long) hld->hFont;
		case WM_GETDLGCODE:
			return (((GetFocus() == hwnd) || (hld->flags & ZC_DEFAULT))?DLGC_DEFPUSHBUTTON:DLGC_UNDEFPUSHBUTTON) | DLGC_BUTTON;

		case WM_PAINT:
			{
				// рисуем с учётом установленного шрифта и фокуса
				RECT cr;
				PAINTSTRUCT ps;
				HFONT hFontOld;
				HPEN hPenOld;
				//SIZE ts;

				GetClientRect(hwnd, &cr);
				
				BeginPaint(hwnd, &ps);

				IntersectClipRect(ps.hdc, cr.left, cr.top, cr.right, cr.bottom);

				FillRect(ps.hdc, &cr, (HBRUSH) SendMessage(GetParent(hwnd), WM_CTLCOLORSTATIC, (WPARAM) ps.hdc, (LPARAM) hwnd));

				//SetTextAlign(ps.hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP); // is default
				//SetBkColor(ps.hdc, 0); // set by WM_CTLCOLORSTATIC
				if(hld->flags & (ZC_MOUSE_OVER | ZC_PUSHED))
				{
					SetTextColor(ps.hdc, RGB(255, 0, 0));
					hFontOld = (HFONT) SelectObject(ps.hdc, hld->hFontActive);
				}
				else
				{
					SetTextColor(ps.hdc, RGB(0, 0, 255));
					hFontOld = (HFONT) SelectObject(ps.hdc, hld->hFont);
				}

				TextOut(ps.hdc, hld->tr.left, hld->tr.top, hld->caption, hld->caplen);
				SelectObject(ps.hdc, hFontOld);

				if(GetFocus() == hwnd)
				{
					hPenOld = (HPEN) SelectObject(ps.hdc, hld->hPenFocus);
					MoveToEx(ps.hdc, hld->tr.left, hld->tr.top, NULL);
					LineTo(ps.hdc, hld->tr.right, hld->tr.top);
					LineTo(ps.hdc, hld->tr.right,hld->tr.bottom);
					LineTo(ps.hdc, hld->tr.left, hld->tr.bottom);
					LineTo(ps.hdc, hld->tr.left, hld->tr.top);
					SelectObject(ps.hdc, hPenOld);
				}

				EndPaint(hwnd, &ps);

				//ZTRACE("WM_PAINT: %d %d %d %d\n", cr.left, cr.top, cr.right, cr.bottom);
			}
			return 0L;
		//case WM_NCPAINT:
		//case WM_ERASEBKGND:
		case WM_DESTROY:
			{
				DeleteObject(hld->hPenFocus);
				DeleteObject(hld->hFontActive);
			}
			break;
		case WM_NCDESTROY:
			{
				zfree(hld);
			}
			break;

		//case WM_SETCURSOR: // proccess by default wndproc
			//ZTRACE("WM_SETCURSOR\n");
			//SetCursor(LoadCursor(NULL, IDC_HAND));
			// maybe capture mouse need here?
			//return 0L;

		case WM_NCHITTEST:
			{
				POINT pt;

				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				ScreenToClient(hwnd, &pt);

				if((pt.x >= hld->tr.left)
					&& (pt.x <= hld->tr.right)
					&& (pt.y >= hld->tr.top)
					&& (pt.y <= hld->tr.bottom))
				{
					//ZTRACE("WM_NCHITTEST: HTCLIENT\n");
					return HTCLIENT;
				}
				//ZTRACE("WM_NCHITTEST: HTTRANSPARENT\n");
				return HTTRANSPARENT; // вместо сенд мессадж ту парент виндов
			}
		case WM_MOUSEMOVE:
			{
				RECT crect;
				//POINT pt;

				//ZTRACE("WM_MOUSEMOVE %d, %d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				//if(GetCapture() != hwnd)
				//{
				//	SetCapture(hwnd);
					//ZTRACE("HyperLink: mouse captured\n");
				//}

				//GetWindowRect(hwnd, &crect);
				//GetCursorPos(&pt);
				GetClientRect(hwnd, &crect);

				if((GET_X_LPARAM(lParam) >= hld->tr.left)
					&& (GET_X_LPARAM(lParam) <= hld->tr.right)
					&& (GET_Y_LPARAM(lParam) >= hld->tr.top)
					&& (GET_Y_LPARAM(lParam) <= hld->tr.bottom))
				{
					if(ZC_MOUSE_OVER & ~hld->flags)
					{
						//SetCursor((HCURSOR) GetClassLongPtr(hwnd, GCLP_HCURSOR));
						hld->flags |= ZC_MOUSE_OVER;
						InvalidateRect(hwnd, NULL, FALSE);
						SetCapture(hwnd);
						// maybe capture here???
						//ZTRACE("WM_MOUSEMOVE: %d %d: %d %d %d %d\n", pt.x, pt.y, rect.left, rect.top, rect.right, rect.bottom);
						//ZTRACE("HyperLink: mouse over\n");
					}
				}
				else
				{
					if(hld->flags & ZC_MOUSE_OVER)
					{
						hld->flags &= ~ZC_MOUSE_OVER;
						//SetCursor(LoadCursor(NULL, IDC_ARROW));
						InvalidateRect(hwnd, NULL, FALSE);
						//ZTRACE("HyperLink: mouse out\n");
					}

					if((ZC_MOUSE_DOWN & ~hld->flags) 
					/*
					&& ((GET_X_LPARAM(lParam) < crect.left)
					|| (GET_X_LPARAM(lParam) > crect.right)
					|| (GET_Y_LPARAM(lParam) < crect.top)
					|| (GET_Y_LPARAM(lParam) > crect.bottom))
					*/
					)
					{
						ReleaseCapture();
						//ZTRACE("HyperLink: capture released\n");
					}
				}
			}
			break;

		// *** USER MESSAGES ***

		case WM_KILLFOCUS:
			hld->flags &= ~ZC_MASK_STATES;
			if(GetCapture() == hwnd)
			{
				ReleaseCapture();
			}
		case WM_SETFOCUS:
			InvalidateRect(hwnd, NULL, FALSE);
			break;

		case WM_SETTEXT:
			{
				HFONT hFontOld;
				HDC hdc;
				RECT cr;
				SIZE ts;

				strncpy_tiny(hld->caption, (char *) lParam, ZC_CAPTION_SIZE);
				hld->caplen = strlen(hld->caption);

				GetClientRect(hwnd, &cr);
				
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, hld->hFont);
					GetTextExtentPoint32(hdc, hld->caption, hld->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					if(hld->flags & ZC_TEXT_RIGHT)
					{
						hld->tr.left = cr.right - ts.cx - 1;
					}
					else if(hld->flags & ZC_TEXT_CENTER)
					{
						hld->tr.left = (cr.right + cr.left - ts.cx - 1)/2;
					}
					else
					{
						hld->tr.left = 0;
					}

					hld->tr.top = (cr.bottom + cr.top - ts.cy)/2;
					hld->tr.right = hld->tr.left + ts.cx;
					hld->tr.bottom = hld->tr.top + ts.cy;

					if(hld->tr.left < 0)
					{
						hld->tr.left = 0;
					}

					if(hld->tr.top < 0)
					{
						hld->tr.top = 0;
					}

					if(hld->tr.right >= cr.right)
					{
						hld->tr.right = cr.right - 1;
					}

					if(hld->tr.bottom >= cr.bottom)
					{
						hld->tr.bottom = cr.bottom - 1;
					}
				}

				InvalidateRect(hwnd, NULL, FALSE);
			}
			return 1L;

		case WM_GETTEXT:
			{
				strncpy_tiny((char *) lParam, hld->caption, wParam-1);
			}
		case WM_GETTEXTLENGTH: // not including the terminating null character
			return hld->caplen;

		case WM_LBUTTONDOWN:
			SetFocus(hwnd);
			if(hld->flags & ZC_MOUSE_OVER)
			{
				hld->flags |= ZC_MOUSE_DOWN;
			}
			return 0L;
		case WM_LBUTTONUP:
			{
				//ZTRACE("WM_LBUTTONUP\n");
				if(GetCapture() == hwnd)
				{
					ReleaseCapture();
					//ZTRACE("HyperLink: capture released\n");
				}

				SetCursor(LoadCursor(NULL, IDC_ARROW));

				if((hld->flags & (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) == (ZC_MOUSE_OVER | ZC_MOUSE_DOWN))
				{
					//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
					// здесь немножко неправильно, родительского окна может и не быть
					SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
				}

				hld->flags &= ~(ZC_MOUSE_OVER | ZC_MOUSE_DOWN);
			}
			break;
		case WM_KEYDOWN:
			if(wParam == VK_RETURN)
			{
				//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
				SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
			}
			else if((wParam == VK_SPACE) && (~hld->flags & ZC_PUSHED))
			{
				hld->flags |= ZC_PUSHED;
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case WM_KEYUP:
			if((wParam == VK_SPACE) && (hld->flags & ZC_PUSHED))
			{
				//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
				hld->flags &= ~ZC_PUSHED;
				InvalidateRect(hwnd, NULL, FALSE);
				SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
			}
			//ZTRACE("WM_KEYUP: %d\n", wParam);
			break;
		/* fake message
		case WM_KEYDOWN:
			ZTRACE("WM_KEYDOWN: %c\n", wParam);
			break;
		case 0x128:
			{
				SYSTEMTIME st;
				GetSystemTime(&st);
				ZTRACE("WM_UPDATEUISTATE: %X, %X\n", LOWORD(wParam), HIWORD(wParam));
			}
			break;
		//*/
		default:
			/*
			{
				SYSTEMTIME st;
				GetSystemTime(&st);
				ZTRACE("WM_UNKNOWN: %X\n", uMsg);
			}
			//*/
			return CallWindowProc(DefWindowProc, hwnd, uMsg, wParam, lParam);
	}

	return 0L;
}

LRESULT CALLBACK ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ZCONTROL_DATA *zcd;

	zcd = (ZCONTROL_DATA *) GetWindowLong(hwnd, 0);

	switch(uMsg)
	{
		// *** SYSTEM MESSAGES ***

		case WM_NCCREATE:
			{
				//ZTRACE("WM_NCCREATE\n");
				zcd = (ZCONTROL_DATA *) zalloc(sizeof(ZCONTROL_DATA));
				if(!zcd)
				{
					return 0L;
				}
				memset(zcd, 0, sizeof(ZCONTROL_DATA));
				SetWindowLong(hwnd, 0, (long) zcd);
			}
			return 1L;
		//case WM_NCCALCSIZE:
		case WM_CREATE:
			{
				//LOGFONT lf;
				LOGBRUSH lb;
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				zcd->flags = ((LPCREATESTRUCT) lParam)->style & ZC_MASK_STYLES;

				if(isempty((char *) ((LPCREATESTRUCT) lParam)->lpszName))
				{
					strncpy_tiny(zcd->caption, "Button control v.0.01 (c)oded by dEmon", ZC_CAPTION_SIZE);
				}
				else
				{
					strncpy_tiny(zcd->caption, ((LPCREATESTRUCT) lParam)->lpszName, ZC_CAPTION_SIZE);
				}
				zcd->caplen = strlen(zcd->caption);

				zcd->hFont = (HFONT) GetStockObject(SYSTEM_FONT);

				lb.lbColor = RGB(128, 128, 128);
				lb.lbHatch = 0;
				lb.lbStyle = BS_SOLID;
				zcd->hPenFocus = ExtCreatePen(PS_GEOMETRIC | PS_DOT | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 1, &lb, 0, NULL);
				/*
				zcd->hPenFocus = CreatePen(PS_SOLID, 0, 0x404040);
				//*/

				GetClientRect(hwnd, &zcd->crect);
				
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					calc_control_size(zcd, ts);
				}
			}
			return 0L;
		//case WM_SIZE:
		//case WM_MOVE:
		//case WM_SHOWWINDOW:
		case WM_SETFONT:
			{
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				if(wParam)
				{
					zcd->hFont = (HFONT) wParam;
				}
				
				//GetClientRect(hwnd, &cr);
				
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					calc_control_size(zcd, ts);
				}

				if(LOWORD(lParam))
				{
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			break;
		case WM_GETFONT:
			return (long) zcd->hFont;
		case WM_GETDLGCODE:
			return (((GetFocus() == hwnd) || (zcd->flags & ZC_DEFAULT))?DLGC_DEFPUSHBUTTON:DLGC_UNDEFPUSHBUTTON) | DLGC_BUTTON;

		case WM_PAINT:
			{
				// рисуем с учётом установленного шрифта и фокуса
				PAINTSTRUCT ps;
				HFONT hFontOld;
				HPEN hPenOld;

				BeginPaint(hwnd, &ps);

				FillRect(ps.hdc, &zcd->crect, (HBRUSH) SendMessage(GetParent(hwnd), WM_CTLCOLORSTATIC, (WPARAM) ps.hdc, (LPARAM) hwnd));

				//SetTextAlign(ps.hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP); // is default
				//SetBkColor(ps.hdc, 0); // set by WM_CTLCOLORSTATIC
				//SetTextColor(ps.hdc, RGB(0, 0, 255));
				hFontOld = (HFONT) SelectObject(ps.hdc, zcd->hFont);

				TextOut(ps.hdc, 
					zcd->tr.left + ((((zcd->flags & (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) == (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) || (zcd->flags & ZC_PUSHED))?1:0),
					zcd->tr.top + ((((zcd->flags & (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) == (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) || (zcd->flags & ZC_PUSHED))?1:0),
					zcd->caption, zcd->caplen);
				SelectObject(ps.hdc, hFontOld);

				if(GetFocus() == hwnd)
				{
					hPenOld = (HPEN) SelectObject(ps.hdc, zcd->hPenFocus);
					MoveToEx(ps.hdc, 3, 3, NULL);
					LineTo(ps.hdc, zcd->crect.right-4, 3);
					LineTo(ps.hdc, zcd->crect.right-4, zcd->crect.bottom-4);
					LineTo(ps.hdc, 3, zcd->crect.bottom-4);
					LineTo(ps.hdc, 3, 3);
					SelectObject(ps.hdc, hPenOld);
				}

				if(zcd->flags & (ZC_MOUSE_OVER | ZC_PUSHED))
				{
					HPEN hPen;
					hPen = CreatePen(PS_SOLID, 0, (zcd->flags & (ZC_MOUSE_DOWN | ZC_PUSHED))?crShadow:crLight);
					hPenOld = (HPEN) SelectObject(ps.hdc, hPen);
					MoveToEx(ps.hdc, 0, zcd->crect.bottom-1, NULL);
					LineTo(ps.hdc, 0, 0);
					LineTo(ps.hdc, zcd->crect.right-1, 0);
					SelectObject(ps.hdc, hPenOld);
					DeleteObject(hPen);

					hPen = CreatePen(PS_SOLID, 0, (zcd->flags & (ZC_MOUSE_DOWN | ZC_PUSHED))?crLight:crShadow);
					SelectObject(ps.hdc, hPen);
					LineTo(ps.hdc, zcd->crect.right-1, zcd->crect.bottom-1);
					LineTo(ps.hdc, 0, zcd->crect.bottom-1);
					SelectObject(ps.hdc, hPenOld);
					DeleteObject(hPen);
				}

				EndPaint(hwnd, &ps);

				//ZTRACE("WM_PAINT: %d %d %d %d\n", cr.left, cr.top, cr.right, cr.bottom);
			}
			return 0L;
		//case WM_NCPAINT:
		//case WM_ERASEBKGND:
		case WM_DESTROY:
			{
				DeleteObject(zcd->hPenFocus);
			}
			break;
		case WM_NCDESTROY:
			{
				zfree(zcd);
			}
			break;

		//case WM_SETCURSOR: // proccess by default wndproc
			//ZTRACE("WM_SETCURSOR\n");
			//SetCursor(LoadCursor(NULL, IDC_HAND));
			// maybe capture mouse need here?
		//	return 0L;

		case WM_NCHITTEST:
			//ZTRACE("WM_NCHITTEST\n");
			return HTCLIENT;
			//break;
		case WM_MOUSEMOVE:
			{
				//RECT crect;
				//POINT pt;

				//ZTRACE("WM_MOUSEMOVE %d, %d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				//if(GetCapture() != hwnd)
				//{
				//	SetCapture(hwnd);
					//ZTRACE("HyperLink: mouse captured\n");
				//}

				//GetWindowRect(hwnd, &crect);
				//GetCursorPos(&pt);
				//GetClientRect(hwnd, &crect);

				// мышь внутри контрола?
				if((GET_X_LPARAM(lParam) >= zcd->crect.left)
					&& (GET_X_LPARAM(lParam) <= zcd->crect.right)
					&& (GET_Y_LPARAM(lParam) >= zcd->crect.top)
					&& (GET_Y_LPARAM(lParam) <= zcd->crect.bottom))
				{
					// мышь не захвачена?
					if(ZC_MOUSE_OVER & ~zcd->flags)
					{
						zcd->flags |= ZC_MOUSE_OVER;
						InvalidateRect(hwnd, NULL, FALSE);
						SetCapture(hwnd);
						// maybe capture here???
						//ZTRACE("WM_MOUSEMOVE: %d %d: %d %d %d %d\n", pt.x, pt.y, rect.left, rect.top, rect.right, rect.bottom);
						//ZTRACE("HyperLink: mouse over\n");
					}
				}
				else
				{
					// мышь захвачена и около контрола?
					if(zcd->flags & ZC_MOUSE_OVER)
					{
						zcd->flags &= ~ZC_MOUSE_OVER;
						InvalidateRect(hwnd, NULL, FALSE);
						//ZTRACE("HyperLink: mouse out\n");
					}

					// кнопка не зажата?
					if(ZC_MOUSE_DOWN & ~zcd->flags) 
					{
						ReleaseCapture();
						//ZTRACE("HyperLink: capture released\n");
					}
				}
			}
			break;

		// *** USER MESSAGES ***

		case WM_KILLFOCUS:
			zcd->flags &= ~ZC_MASK_STATES;
			if(GetCapture() == hwnd)
			{
				ReleaseCapture();
			}
		case WM_SETFOCUS:
			InvalidateRect(hwnd, NULL, FALSE);
			break;

		case WM_SETTEXT:
			{
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				strncpy_tiny(zcd->caption, (char *) lParam, ZC_CAPTION_SIZE);
				zcd->caplen = strlen(zcd->caption);

				//GetClientRect(hwnd, &cr);
				
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					calc_control_size(zcd, ts);
				}

				InvalidateRect(hwnd, NULL, FALSE);
			}
			return 1L;

		case WM_GETTEXT:
			{
				strncpy_tiny((char *) lParam, zcd->caption, wParam-1);
			}
		case WM_GETTEXTLENGTH:
			return zcd->caplen;

		case WM_LBUTTONDOWN:
			SetFocus(hwnd);
			zcd->flags |= ZC_MOUSE_DOWN;
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_LBUTTONUP:
			{
				//ZTRACE("WM_LBUTTONUP\n");
				if(GetCapture() == hwnd)
				{
					ReleaseCapture();
					//ZTRACE("HyperLink: capture released\n");
				}

				if((zcd->flags & (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) == (ZC_MOUSE_OVER | ZC_MOUSE_DOWN))
				{
					//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
					SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
				}

				zcd->flags &= ~(ZC_MOUSE_OVER | ZC_MOUSE_DOWN);
			}
			break;
		case WM_KEYDOWN:
			if((wParam == VK_SPACE) && (~zcd->flags & ZC_PUSHED))
			{
				zcd->flags |= ZC_PUSHED;
				InvalidateRect(hwnd, NULL, FALSE);
			}
			else if(wParam == VK_RETURN)
			{
				//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
				SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
			}
			break;
		case WM_KEYUP:
			if((wParam == VK_SPACE) && (zcd->flags & ZC_PUSHED))
			{
				//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
				SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
				zcd->flags &= ~ZC_PUSHED;
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		default:
			return CallWindowProc(DefWindowProc, hwnd, uMsg, wParam, lParam);
	}

	return 0L;
}


unsigned long zedit_scroll(ZEDIT_DATA *zcd, unsigned long flags)
{
	unsigned long changed;
	changed = 0;

	return changed;
}

unsigned long zedit_move(ZEDIT_DATA *zcd, unsigned long offset, unsigned long flags)
{
	unsigned long prev_offset;
	prev_offset = 0;
	// ZE_LEFT
	// ZE_RIGHT
	// ZE_PAGE_DOWN
	// ZE_PAGE_UP
	// ZE_ABSOLUTE
	return prev_offset;
}

unsigned long zedit_select(ZEDIT_DATA *zcd, unsigned long prev_offset)
{
	unsigned long changed;
	changed = 0;

	return changed;
}

unsigned long zedit_replace(ZEDIT_DATA *zcd, char *data, unsigned long data_size, unsigned long offset, unsigned long flags)
{
	unsigned long prev_offset;
	prev_offset = 0;
	// ZE_FOLLOW_CURSOR
	// ZE_REPLACE_AT_CURSOR
	return prev_offset;
}

unsigned long zedit_current_row(ZEDIT_DATA *zcd)
{
	unsigned long i;

	i = zcd->current_char;

	if(i)
	{
		i--;
		while(i && (zcd->caption[i] != '\n'))
		{
			i--;
		}
	}

	return i;
}

unsigned long zedit_previous_row(ZEDIT_DATA *zcd)
{
	unsigned long i;

	i = zedit_current_row(zcd);

	if(i)
	{
		i--;
		while(i && (zcd->caption[i] != '\n'))
		{
			i--;
		}
	}

	return i;
}

unsigned long zedit_next_row(ZEDIT_DATA *zcd)
{
	unsigned long i;

	i = zcd->current_char;

	while((i < zcd->caplen) && (zcd->caption[i] != '\n'))
	{
		i++;
	}

	if(i < zcd->caplen)
	{
		return i + 1;
	}
	else
	{
		return 0;
	}
}

unsigned long zedit_current_height(ZEDIT_DATA *zcd, unsigned long offset)
{
	do
	{
		// calc height here
		offset++;
	}
	while((offset < zcd->caplen) && (zcd->caption[offset] != '\n'));

	return offset;
}

LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ZEDIT_DATA *zcd;

	zcd = (ZEDIT_DATA *) GetWindowLong(hwnd, 0);

	switch(uMsg)
	{
		// *** SYSTEM MESSAGES ***

		case WM_NCCREATE:
			{
				//ZTRACE("WM_NCCREATE\n");
				zcd = (ZEDIT_DATA *) zalloc(sizeof(ZEDIT_DATA));
				if(!zcd)
				{
					return 0L;
				}
				memset(zcd, 0, sizeof(ZEDIT_DATA));

				zcd->bufsize = 1048576;

				zcd->caption = (char *) zalloc(zcd->bufsize);
				zcd->char_widths = (unsigned char *) zalloc(zcd->bufsize);
				zcd->row_heights = (unsigned long *) zalloc(zcd->bufsize); // max rows = (zcd->bufsize / 4) = 262144

				SetWindowLong(hwnd, 0, (long) zcd);
			}
			return 1L;
		case WM_NCCALCSIZE:
			{
				if(wParam)
				{
					ZTRACE("WM_NCCALCSIZE:   wParam: %d !!!!!!!!!!\n", wParam);
				}
			}	
			return 0L;
		case WM_CREATE:
			{
				//LOGFONT lf;
				LOGBRUSH lb;
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				zcd->flags = ((LPCREATESTRUCT) lParam)->style & ZC_MASK_STYLES;

				if(isempty((char *) ((LPCREATESTRUCT) lParam)->lpszName))
				{
					strncpy_tiny(zcd->caption, "Edit control v.0.01 (c)oded by dEmon", zcd->bufsize-1);
				}
				else
				{
					strncpy_tiny(zcd->caption, ((LPCREATESTRUCT) lParam)->lpszName, zcd->bufsize-1);
				}
				zcd->caplen = strlen(zcd->caption);

				zcd->hFont = (HFONT) GetStockObject(SYSTEM_FONT);

				lb.lbColor = RGB(128, 128, 128);
				lb.lbHatch = 0;
				lb.lbStyle = BS_SOLID;
				zcd->hPenFocus = NULL; //ExtCreatePen(PS_GEOMETRIC | PS_DOT | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 1, &lb, 0, NULL);
				/*
				zcd->hPenFocus = CreatePen(PS_SOLID, 0, 0x404040);
				//*/

				GetClientRect(hwnd, &zcd->wrect);
				zcd->crect.left = zcd->wrect.left + 2;
				zcd->crect.top = zcd->wrect.top + 2;
				zcd->crect.right = zcd->wrect.right - 2;
				zcd->crect.bottom = zcd->wrect.bottom - 2;

				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					parse_text(zcd, hdc);
					SelectObject(hdc, hFontOld);

					ReleaseDC(hwnd, hdc);

					//calc_control_size(zcd, ts);
				}

				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;
			}
			return 0L;
		//case WM_SIZE:
		//case WM_MOVE:
		//case WM_SHOWWINDOW:
		case WM_SETFONT:
			{
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				SIZE ts;

				if(wParam)
				{
					zcd->hFont = (HFONT) wParam;
				}

				//GetClientRect(hwnd, &cr);

				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					parse_text(zcd, hdc);
					SelectObject(hdc, hFontOld);

					ReleaseDC(hwnd, hdc);

					//calc_control_size(zcd, ts);
				}
				// здесь надо пересчитать позицию каретки

				if(LOWORD(lParam))
				{
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			break;
		case WM_GETFONT:
			return (long) zcd->hFont;
		case WM_GETDLGCODE:
			{
				//ZTRACE("zedit: WM_GETDLGCODE\n");
				return (zcd->flags & (ZC_WANTRETURN | ZC_MULTILINE))
					?(DLGC_HASSETSEL| DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTALLKEYS)
					:(DLGC_HASSETSEL| DLGC_WANTARROWS | DLGC_WANTCHARS);
			}

		case WM_NCPAINT:
			{
				HDC hdc;
				HPEN hPenOld;
				HPEN hPen;

				ZTRACE("WM_NCPAINT\n");
				/*
				if(wParam == 1)
				{
					hdc = GetDC(hwnd);
				}
				else
				{
					hdc = GetDCEx(hwnd, (HRGN) wParam, DCX_WINDOW | DCX_INTERSECTRGN);
				}
				*/
				hdc = GetDC(hwnd);
				if(hdc)
				{
					hPen = CreatePen(PS_SOLID, 0, crLight);
					hPenOld = (HPEN) SelectObject(hdc, hPen);
					MoveToEx(hdc, 0, zcd->wrect.bottom-1, NULL);
					LineTo(hdc, 0, 0);
					LineTo(hdc, zcd->wrect.right-1, 0);
					LineTo(hdc, zcd->wrect.right-1, zcd->wrect.bottom-1);
					LineTo(hdc, 0, zcd->wrect.bottom-1);
					SelectObject(hdc, hPenOld);
					DeleteObject(hPen);

					ReleaseDC(hwnd, hdc);
				}
				/*
				else
				{
					ZTRACE("WM_NCPAINT: error!!!!\n");
				}
				*/
			}
			return 0L;
		case WM_PAINT:
			{
				// рисуем с учётом установленного шрифта и фокуса
				PAINTSTRUCT ps;
				HFONT hFontOld;
				HBRUSH hbrBack;
				COLORREF color_back;
				COLORREF color_text;
				unsigned long i;
				unsigned long j;
				unsigned long x;
				unsigned long y;

				//ZTRACE("WM_PAINT\n");

				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					HideCaret(hwnd);
				}

				BeginPaint(hwnd, &ps);

				hbrBack = (HBRUSH) SendMessage(GetParent(hwnd), WM_CTLCOLOREDIT, (WPARAM) ps.hdc, (LPARAM) hwnd);

				//if(ps.fErase)
				{
					FillRect(ps.hdc, &zcd->crect, hbrBack);
				}

				color_back = GetBkColor(ps.hdc);
				color_text = GetTextColor(ps.hdc);

				hFontOld = (HFONT) SelectObject(ps.hdc, zcd->hFont);

				i = 0;
				j = 0;
				while(j < zcd->first_row)
				{
					i += zcd->row_heights[j] & 0xFFFFFF;
					j++;
				}

				if((zcd->selection_start != zcd->selection_end) && (i > zcd->selection_start) && (i < zcd->selection_end))
				{
					SetBkColor(ps.hdc, RGB(10, 36, 106));
					SetTextColor(ps.hdc, RGB(255, 255, 255));
				}

				y = zcd->crect.top;
				//while((j < zcd->total_rows))
				while((j < zcd->total_rows) && ((signed) (y + (zcd->row_heights[j] >> 24)) < zcd->crect.bottom))
				{
					x = zcd->crect.left;
					while((i < zcd->caplen) && ((signed) (x + zcd->char_widths[i] - zcd->first_x) < zcd->crect.right) && (zcd->caption[i] != '\n'))
					{
						if(zcd->selection_start != zcd->selection_end)
						{
							if(i == zcd->selection_start)
							{
								SetBkColor(ps.hdc, RGB(10, 36, 106));
								SetTextColor(ps.hdc, RGB(255, 255, 255));
							}
							else if(i == zcd->selection_end)
							{
								SetBkColor(ps.hdc, color_back);
								SetTextColor(ps.hdc, color_text);
							}
						}

						if(x > zcd->first_x)
						{
							if(zcd->caption[i] != '\t')
							{
								TextOut(ps.hdc, x - zcd->first_x, y, zcd->caption + i, 1);
							}
							else
							{
								//FillRect();
							}
						}
						x += zcd->char_widths[i];
						i++;
					}

					while((i < zcd->caplen) && (zcd->caption[i] != '\n'))
					{
						if(zcd->selection_start != zcd->selection_end)
						{
							if(i == zcd->selection_start)
							{
								SetBkColor(ps.hdc, RGB(10, 36, 106));
								SetTextColor(ps.hdc, RGB(255, 255, 255));
							}
							else if(i == zcd->selection_end)
							{
								SetBkColor(ps.hdc, color_back);
								SetTextColor(ps.hdc, color_text);
							}
						}

						i++;
					}

					if(zcd->selection_start != zcd->selection_end)
					{
						if(i == zcd->selection_start)
						{
							SetBkColor(ps.hdc, RGB(10, 36, 106));
							SetTextColor(ps.hdc, RGB(255, 255, 255));
						}
						else if(i == zcd->selection_end)
						{
							SetBkColor(ps.hdc, color_back);
							SetTextColor(ps.hdc, color_text);
						}
					}
					y += zcd->row_heights[j] >> 24;
					i++;
					j++;
				}

				SelectObject(ps.hdc, hFontOld);

				SetBkColor(ps.hdc, color_back);
				SetTextColor(ps.hdc, color_text);

				EndPaint(hwnd, &ps);
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					ShowCaret(hwnd);
				}

				//ZTRACE("WM_PAINT: %d %d %d %d\n", cr.left, cr.top, cr.right, cr.bottom);
			}
			return 0L;
		//case WM_NCPAINT:
		case WM_ERASEBKGND:
			{
				FillRect((HDC) wParam, &zcd->crect, (HBRUSH) SendMessage(GetParent(hwnd), WM_CTLCOLOREDIT, wParam, (LPARAM) hwnd));
			}
			return 1L;
		case WM_DESTROY:
			{
				//DeleteObject(zcd->hPenFocus);
			}
			break;
		case WM_NCDESTROY:
			{
				zfree(zcd->caption);
				zfree(zcd->char_widths);
				zfree(zcd->row_heights);
				zfree(zcd);
			}
			break;

		//case WM_SETCURSOR: // proccess by default wndproc
			//ZTRACE("WM_SETCURSOR\n");
			//SetCursor(LoadCursor(NULL, IDC_IBEAM));
			// maybe capture mouse need here?
		//	return 0L;

		case WM_NCHITTEST:
			{
				POINT pt;

				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				ScreenToClient(hwnd, &pt);

				if((pt.x >= zcd->crect.left)
					&& (pt.x <= zcd->crect.right)
					&& (pt.y >= zcd->crect.top)
					&& (pt.y <= zcd->crect.bottom))
				{
					//ZTRACE("WM_NCHITTEST: HTCLIENT\n");
					return HTCLIENT;
				}
				else
				{
					//ZTRACE("WM_NCHITTEST: HTBORDER\n");
					return HTBORDER;
				}
			}
		case WM_MOUSEMOVE:
			{
				unsigned long j;
				int my;
				int mx;
				//RECT crect;
				//POINT pt;

				//ZTRACE("WM_MOUSEMOVE %d, %d\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				//if(GetCapture() != hwnd)
				//{
				//	SetCapture(hwnd);
					//ZTRACE("HyperLink: mouse captured\n");
				//}

				//GetWindowRect(hwnd, &crect);
				//GetCursorPos(&pt);
				//GetClientRect(hwnd, &crect);

				if(zcd->flags & ZC_MOUSE_DOWN)
				{
					/*
					zcd->caret_x = zcd->crect.left;
					zcd->current_char = 0;
					while(zcd->current_char < zcd->caplen)
					{
						if((signed) (zcd->caret_x + (zcd->char_widths[zcd->current_char]/2)) >= GET_X_LPARAM(lParam))
						{
							break;
						}
						zcd->caret_x += zcd->char_widths[zcd->current_char];
						zcd->current_char++;
					}
					*/

					mx = GET_X_LPARAM(lParam);
					my = GET_Y_LPARAM(lParam);

					j = zcd->current_char;
					zcd->current_char = 0;
					zcd->caret_x = zcd->crect.left;
					zcd->caret_y = zcd->crect.top;

					zcd->current_row = 0;
					while(zcd->current_row < zcd->first_row)
					{
						zcd->current_char += zcd->row_heights[zcd->current_row] & 0xFFFFFF;
						zcd->current_row++;
					}

					if(my < 0)
					{
						while((my < 0) && (zcd->current_row > 0))
						{
							zcd->current_row--;
							my += zcd->row_heights[zcd->current_row] >> 24;
							zcd->current_char -= zcd->row_heights[zcd->current_row] & 0xFFFFFF;
						}
					}
					else
					{
						while(zcd->current_row < (zcd->total_rows - 1))
						{
							if((signed) (zcd->caret_y + (zcd->row_heights[zcd->current_row] >> 24)) > my)
							{
								break;
							}
							zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
							zcd->current_char += zcd->row_heights[zcd->current_row] & 0xFFFFFF;
							zcd->current_row++;
						}
					}

					// здесь есть ошибка: если mx меньше нуля!
					while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
					{
						if((zcd->caption[zcd->current_char] == '\n')
							|| ((signed)(zcd->caret_x - zcd->first_x + (zcd->char_widths[zcd->current_char]/2)) >= mx))
						{
							break;
						}
						zcd->caret_x += zcd->char_widths[zcd->current_char];
						zcd->current_char++;
					}
					//ZTRACE("SELECTION OLD: %d, %d (%d)\n", zcd->selection_start, zcd->selection_end, zcd->current_char);

					zedit_scroll_screen(zcd);

					if(zcd->flags & ZC_CARET_VISIBLE)
					{
						SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
					}

					//if(zcd->flags & ZC_SWAP_END)
					if(zcd->selection_end == j)
					{
						if(zcd->selection_start > zcd->current_char)
						{
							zcd->selection_end = zcd->selection_start;
							zcd->selection_start = zcd->current_char;
							//zcd->flags &= ~ZC_SWAP_END;
						}
						else
						{
							zcd->selection_end = zcd->current_char;
						}
					}
					else
					{
						if(zcd->selection_end < zcd->current_char)
						{
							zcd->selection_start = zcd->selection_end;
							zcd->selection_end = zcd->current_char;
							//zcd->flags |= ZC_SWAP_END;
						}
						else
						{
							zcd->selection_start = zcd->current_char;
						}
					}

					/*
					if(zcd->selection_start > zcd->current_char)
					{
						if(zcd->flags & ZC_SWAP_END)
						{
							zcd->selection_end = zcd->selection_start;
						}
						zcd->selection_start = zcd->current_char;
						zcd->flags &= ~ZC_SWAP_END;
					}
					else if(zcd->selection_end < zcd->current_char)
					{
						if(~zcd->flags & ZC_SWAP_END)
						{
							zcd->selection_start = zcd->selection_end;
						}
						zcd->selection_end = zcd->current_char;
						zcd->flags |= ZC_SWAP_END;
					}
					else
					{
						if(zcd->flags & ZC_SWAP_END)
						{
							zcd->selection_end = zcd->current_char;
						}
						else
						{
							zcd->selection_start = zcd->current_char;
						}
					}
					*/

					//ZTRACE("SELECTION: %d, %d\n", zcd->selection_start, zcd->selection_end);

					InvalidateRect(hwnd, NULL, FALSE);
				}

				if((GET_X_LPARAM(lParam) >= zcd->crect.left)
					&& (GET_X_LPARAM(lParam) <= zcd->crect.right)
					&& (GET_Y_LPARAM(lParam) >= zcd->crect.top)
					&& (GET_Y_LPARAM(lParam) <= zcd->crect.bottom))
				{
					if(ZC_MOUSE_OVER & ~zcd->flags)
					{
						zcd->flags |= ZC_MOUSE_OVER;
						//InvalidateRect(hwnd, NULL, FALSE);
						SetCapture(hwnd);
						//SetCursor((HCURSOR) GetClassLongPtr(hwnd, GCLP_HCURSOR));
						// maybe capture here???
						//ZTRACE("WM_MOUSEMOVE: %d %d: %d %d %d %d\n", pt.x, pt.y, rect.left, rect.top, rect.right, rect.bottom);
						//ZTRACE("HyperLink: mouse over\n");
					}
				}
				else
				{
					if(zcd->flags & ZC_MOUSE_OVER)
					{
						zcd->flags &= ~ZC_MOUSE_OVER;
						//InvalidateRect(hwnd, NULL, FALSE);
						//ZTRACE("HyperLink: mouse out\n");
					}

					if(ZC_MOUSE_DOWN & ~zcd->flags) 
					{
						//SetCursor(LoadCursor(NULL, IDC_ARROW));
						ReleaseCapture();
						//ZTRACE("HyperLink: capture released\n");
					}
				}
			}
			break;

		// *** USER MESSAGES ***

		case WM_SETFOCUS:
			{
				//ZTRACE("WM_SETFOCUS\n");
				InvalidateRect(hwnd, NULL, FALSE);

				CreateCaret(hwnd, NULL, 1, 13);
				/*
				zcd->caret_x = zcd->crect.left;
				//zcd->caret_y = zcd->crect.top;
				for(unsigned long i = 0; i < zcd->current_char; i++)
				{
					zcd->caret_x += zcd->char_widths[i];
				}
				*/
				zcd->flags |= ZC_ACTIVE | ZC_CARET_VISIBLE;
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
					ShowCaret(hwnd);
				}
			}
			break;
		case WM_KILLFOCUS:
			//ZTRACE("WM_KILLFOCUS\n");
			if(GetCapture() == hwnd)
			{
				ReleaseCapture();
			}
			zcd->flags &= ~ZC_MASK_STATES; // reset flags
			InvalidateRect(hwnd, NULL, FALSE);
			if(zcd->flags & ZC_CARET_VISIBLE)
			{
				HideCaret(hwnd);
			}
			DestroyCaret();
			break;

		case WM_SETTEXT:
			{
				HFONT hFontOld;
				HDC hdc;
				//RECT cr;
				//SIZE ts;

				strncpy_tiny(zcd->caption, (char *) lParam, zcd->bufsize-1);
				zcd->caplen = strlen(zcd->caption);

				// здесь надо обнулять значения структуры zcd

				//GetClientRect(hwnd, &cr);

				hdc = GetDC(hwnd);
				if(hdc)
				{
					hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
					//GetTextExtentPoint32(hdc, zcd->caption, zcd->caplen, &ts);
					parse_text(zcd, hdc);
					SelectObject(hdc, hFontOld);
			
					ReleaseDC(hwnd, hdc);

					//calc_control_size(zcd, ts);
				}
				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y); // only if have focus!
				}

				InvalidateRect(hwnd, NULL, FALSE);
			}
			return 1L;

		case WM_GETTEXT:
			{
				strncpy_tiny((char *) lParam, zcd->caption, (zcd->caplen > wParam)?wParam-1:zcd->caplen);
				// here must return value is the number of chars copied
			}
		case WM_GETTEXTLENGTH:
			// not including the terminating null character
			return zcd->caplen;

		case WM_LBUTTONDOWN:
			{
				unsigned long j;
				int repaint;

				repaint = 1;

				// устанавливаем фокус в окно, если неактивно
				if(~zcd->flags & ZC_ACTIVE)
				{
					repaint = 0;
					SetFocus(hwnd);
					//InvalidateRect(hwnd, NULL, FALSE);
				}

				j = zcd->current_char;
				zcd->current_char = 0;
				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;

				zcd->current_row = 0;
				while(zcd->current_row < zcd->first_row)
				{
					zcd->current_char += zcd->row_heights[zcd->current_row] & 0xFFFFFF;
					zcd->current_row++;
				}
				while(zcd->current_row < (zcd->total_rows - 1))
				{
					if((signed) (zcd->caret_y + (zcd->row_heights[zcd->current_row] >> 24)) > GET_Y_LPARAM(lParam))
					{
						break;
					}
					zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
					zcd->current_char += zcd->row_heights[zcd->current_row] & 0xFFFFFF;
					zcd->current_row++;
				}

				while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
				{
					if((zcd->caption[zcd->current_char] == '\n')
						|| ((signed)(zcd->caret_x - zcd->first_x + (zcd->char_widths[zcd->current_char]/2)) >= GET_X_LPARAM(lParam)))
					{
						break;
					}
					zcd->caret_x += zcd->char_widths[zcd->current_char];
					zcd->current_char++;
				}

				// here better select algorithm!?
				if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
				{
					if(zcd->selection_end == j)
					{
						if(zcd->selection_start > zcd->current_char)
						{
							zcd->selection_end = zcd->selection_start;
							zcd->selection_start = zcd->current_char;
						}
						else
						{
							zcd->selection_end = zcd->current_char;
						}
					}
					else if(zcd->selection_start == j)
					{
						if(zcd->selection_end < zcd->current_char)
						{
							zcd->selection_start = zcd->selection_end;
							zcd->selection_end = zcd->current_char;
						}
						else
						{
							zcd->selection_start = zcd->current_char;
						}
					}
					else if(zcd->current_char > j)
					{
						zcd->selection_start = j;
						zcd->selection_end = zcd->current_char;
					}
					else
					{
						zcd->selection_start = zcd->current_char;
						zcd->selection_end = j;
					}
				}
				else
				{
					if(zcd->selection_start == zcd->selection_end)
					{
						repaint = 0;
					}
					zcd->selection_start = zcd->current_char;
					zcd->selection_end = zcd->current_char;
				}


				zedit_scroll_screen(zcd);
				/*
				if((zcd->flags & (ZC_CARET_VISIBLE | ZC_ACTIVE)) == ZC_ACTIVE)
				{
					ShowCaret(hwnd);
					zcd->flags |= ZC_CARET_VISIBLE;
				}
				*/

				//if(repaint)
				{
					InvalidateRect(hwnd, NULL, FALSE);
				}

				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
				}
				zcd->flags |= ZC_MOUSE_DOWN;
			}
			break;
		case WM_LBUTTONUP:
			{
				//ZTRACE("WM_LBUTTONUP\n");
				if(GetCapture() == hwnd)
				{
					ReleaseCapture();
					//ZTRACE("HyperLink: capture released\n");
				}

				if((zcd->flags & (ZC_MOUSE_OVER | ZC_MOUSE_DOWN)) == (ZC_MOUSE_OVER | ZC_MOUSE_DOWN))
				{
					//ZTRACE("HyperLink: send BN_CLICKED to parent\n");
					SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED), (LPARAM) hwnd);
				}

				zcd->flags &= ~(ZC_MOUSE_OVER | ZC_MOUSE_DOWN);
			}
			break;
		case WM_MOUSEWHEEL:
			ZTRACE("WM_MOUSEWHEEL: delta %d\n", (signed) wParam >> 16);
			if(((signed) wParam >> 16) < 0)
			{
				if(zcd->first_row < (zcd->total_rows - 1))
				{
					if(zcd->caret_y < (zcd->row_heights[zcd->first_row] >> 24))
					{
						zcd->caret_y = zcd->crect.top;
					}
					else
					{
						zcd->caret_y -= zcd->row_heights[zcd->first_row] >> 24;
					}
					zcd->first_row++;
				}
			}
			else
			{
				if(zcd->first_row > 0)
				{
					zcd->first_row--;
					zcd->caret_y += zcd->row_heights[zcd->first_row] >> 24;
				}
				if(zcd->first_row == zcd->current_row)
				{
					zcd->caret_y = zcd->crect.top;
				}
			}

			if(((zcd->caret_x - zcd->first_x) < zcd->crect.left) || ((zcd->caret_x - zcd->first_x) > zcd->crect.right))
			{
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					HideCaret(hwnd);
				}
				zcd->flags &= ~ZC_CARET_VISIBLE;
			}
			else if((zcd->first_row > zcd->current_row) || ((zcd->caret_y + (zcd->row_heights[zcd->current_row] >> 24)) > zcd->crect.bottom))
			{
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					HideCaret(hwnd);
				}
				zcd->flags &= ~ZC_CARET_VISIBLE;
			}
			else if(~zcd->flags & ZC_CARET_VISIBLE)
			{
				zcd->flags |= ZC_CARET_VISIBLE;
				ShowCaret(hwnd);
			}
			if(zcd->flags & ZC_CARET_VISIBLE)
			{
				SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
			}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_KEYDOWN:
			{
				int repaint;
				unsigned long i;
				unsigned long j;

				//ZTRACE("WM_KEYDOWN: 0x%.8X\n", wParam);

				repaint = 0;
				if(wParam == VK_RIGHT) // move right
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						// здесь должны быть не нули, а новая позиция курсора
						// иначе получется лажа при SHIFT + MOUSE_CLICK
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_char < zcd->caplen)
					{
						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_end == zcd->current_char)
							{
								zcd->selection_end++;
							}
							else if(zcd->selection_start == zcd->current_char)
							{
								zcd->selection_start++;
							}
							else
							{
								zcd->selection_start = zcd->current_char;
								zcd->selection_end = zcd->current_char + 1;
							}
							repaint = 1;
						}
						if(zcd->caption[zcd->current_char] == '\n')
						{
							zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
							zcd->current_row++;
							zcd->caret_x = zcd->crect.left;
						}
						else
						{
							zcd->caret_x += zcd->char_widths[zcd->current_char];
						}
						zcd->current_char++;

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						//ZTRACE("FIRST X:  %d\n", zcd->first_x);

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_LEFT) // move left
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_char > 0)
					{
						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_start == zcd->current_char)
							{
								zcd->selection_start--;
							}
							else if(zcd->selection_end == zcd->current_char)
							{
								zcd->selection_end--;
							}
							else
							{
								zcd->selection_end = zcd->current_char;
								zcd->selection_start = zcd->current_char - 1;
							}
							repaint = 1;
						}
						zcd->current_char--;
						if(zcd->caption[zcd->current_char] == '\n')
						{
							zcd->caret_y -= zcd->row_heights[zcd->current_row] >> 24;
							zcd->current_row--;
							zcd->caret_x = zcd->crect.left;
							j = 0;
							i = 0;
							while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) < zcd->current_char))
							{
								i += zcd->row_heights[j] & 0xFFFFFF;
								//zcd->caret_y += zcd->row_heights[j] >> 24;
								j++;
							}

							while(i < zcd->current_char)
							{
								zcd->caret_x += zcd->char_widths[i];
								i++;
							}
						}
						else
						{
							zcd->caret_x -= zcd->char_widths[zcd->current_char];
						}

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_HOME) // move current line start
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}

					if((zcd->current_char > 0) && (zcd->caption[zcd->current_char-1] != '\n'))
					{
						i = zcd->current_char;
						while((i > 0) && (zcd->caption[i-1] != '\n'))
						{
							i--;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_start == zcd->selection_end)
							{
								zcd->selection_end = zcd->current_char;
								zcd->selection_start = i;
							}
							else if(zcd->selection_start != zcd->current_char)
							{
								if(i < zcd->selection_start)
								{
									zcd->selection_end = zcd->selection_start;
									zcd->selection_start = i;
								}
								else
								{
									zcd->selection_end = i;
								}
							}
							else
							{
								zcd->selection_start = i;
							}
							repaint = 1;
						}

						zcd->current_char = i;
						zcd->caret_x = zcd->crect.left;

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_END) // move current line end
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
					{
						i = zcd->current_char;
						while((i < zcd->caplen) && (zcd->caption[i] != '\n'))
						{
							zcd->caret_x += zcd->char_widths[i];
							i++;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_start == zcd->selection_end)
							{
								zcd->selection_start = zcd->current_char;
								zcd->selection_end = i;
							}
							else if(zcd->selection_end != zcd->current_char)
							{
								if(i > zcd->selection_end)
								{
									zcd->selection_start = zcd->selection_end;
									zcd->selection_end = i;
								}
								else
								{
									zcd->selection_start = i;
								}
							}
							else
							{
								zcd->selection_end = i;
							}
							repaint = 1;
						}
						zcd->current_char = i;
						/*
						for(;zcd->current_char < zcd->caplen; zcd->current_char++)
						{
							zcd->caret_x += zcd->char_widths[zcd->current_char];
						}
						*/

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_UP)
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_row > 0)
					{
						zcd->current_row--;
						i = zcd->current_char;
						zcd->current_char = 0;
						zcd->caret_y -= zcd->row_heights[zcd->current_row] >> 24;

						j = 0;
						while(j < zcd->current_row)
						{
							zcd->current_char += zcd->row_heights[j] & 0xFFFFFF;
							j++;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caption[zcd->current_char] == '\n')
								|| ((zcd->caret_x + (zcd->char_widths[zcd->current_char]/2)) >= j))
							{
								break;
							}
							zcd->caret_x += zcd->char_widths[zcd->current_char];
							zcd->current_char++;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_start == i)
							{
								zcd->selection_start = zcd->current_char;
							}
							else if(zcd->selection_end == i)
							{
								if(zcd->current_char < zcd->selection_start)
								{
									zcd->selection_end = zcd->selection_start;
									zcd->selection_start = zcd->current_char;
								}
								else
								{
									zcd->selection_end = zcd->current_char;
								}
							}
							else
							{
								zcd->selection_end = i;
								zcd->selection_start = zcd->current_char;
							}
							repaint = 1;
						}

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_DOWN)
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_row < (zcd->total_rows - 1))
					{
						i = zcd->current_char;
						zcd->current_char = 0;
						zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
						zcd->current_row++;

						j = 0;
						while(j < zcd->current_row)
						{
							zcd->current_char += zcd->row_heights[j] & 0xFFFFFF;
							j++;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caption[zcd->current_char] == '\n')
								|| ((zcd->caret_x + (zcd->char_widths[zcd->current_char]/2)) >= j))
							{
								break;
							}
							zcd->caret_x += zcd->char_widths[zcd->current_char];
							zcd->current_char++;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_end == i)
							{
								zcd->selection_end = zcd->current_char;
							}
							else if(zcd->selection_start == i)
							{
								if(zcd->current_char > zcd->selection_end)
								{
									zcd->selection_start = zcd->selection_end;
									zcd->selection_end = zcd->current_char;
								}
								else
								{
									zcd->selection_start = zcd->current_char;
								}
							}
							else
							{
								zcd->selection_start = i;
								zcd->selection_end = zcd->current_char;
							}
							repaint = 1;
						}

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_PRIOR)
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_row > 0)
					{
						if(zcd->current_row > 5)
						{
							zcd->current_row -= 5;
						}
						else
						{
							zcd->current_row = 0;
						}
						i = zcd->current_char;
						zcd->current_char = 0;
						zcd->caret_y -= zcd->row_heights[zcd->current_row] >> 24;

						zcd->caret_y = zcd->crect.top;
						j = 0;
						while(j < zcd->current_row)
						{
							zcd->current_char += zcd->row_heights[j] & 0xFFFFFF;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
							}
							j++;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caption[zcd->current_char] == '\n')
								|| ((zcd->caret_x + (zcd->char_widths[zcd->current_char]/2)) >= j))
							{
								break;
							}
							zcd->caret_x += zcd->char_widths[zcd->current_char];
							zcd->current_char++;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_start == i)
							{
								zcd->selection_start = zcd->current_char;
							}
							else if(zcd->selection_end == i)
							{
								if(zcd->current_char < zcd->selection_start)
								{
									zcd->selection_end = zcd->selection_start;
									zcd->selection_start = zcd->current_char;
								}
								else
								{
									zcd->selection_end = zcd->current_char;
								}
							}
							else
							{
								zcd->selection_end = i;
								zcd->selection_start = zcd->current_char;
							}
							repaint = 1;
						}

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_NEXT)
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}
					if(zcd->current_row < (zcd->total_rows - 1))
					{
						i = zcd->current_char;
						zcd->current_char = 0;
						//zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
						if((zcd->current_row + 5) < zcd->total_rows)
						{
							zcd->current_row += 5;
						}
						else
						{
							zcd->current_row = zcd->total_rows - 1;
						}

						zcd->caret_y = zcd->crect.top;
						j = 0;
						while(j < zcd->current_row)
						{
							zcd->current_char += zcd->row_heights[j] & 0xFFFFFF;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;
							}
							j++;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caption[zcd->current_char] == '\n')
								|| ((zcd->caret_x + (zcd->char_widths[zcd->current_char]/2)) >= j))
							{
								break;
							}
							zcd->caret_x += zcd->char_widths[zcd->current_char];
							zcd->current_char++;
						}

						if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
						{
							if(zcd->selection_end == i)
							{
								zcd->selection_end = zcd->current_char;
							}
							else if(zcd->selection_start == i)
							{
								if(zcd->current_char > zcd->selection_end)
								{
									zcd->selection_start = zcd->selection_end;
									zcd->selection_end = zcd->current_char;
								}
								else
								{
									zcd->selection_start = zcd->current_char;
								}
							}
							else
							{
								zcd->selection_start = i;
								zcd->selection_end = zcd->current_char;
							}
							repaint = 1;
						}

						zedit_scroll_screen(zcd);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				else if(wParam == VK_DELETE) // delete char or current selection
				{
					if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
					{
						SendMessage(hwnd, WM_CUT, 0, 0);
					}
					else
					{
						// надо ещё пересчитывать высоту строк
						if(zcd->selection_start != zcd->selection_end)
						{
							zcd->current_char = zcd->selection_start;
							zcd->caret_y = zcd->crect.top;
							i = 0;
							j = 0;
							while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) <= zcd->current_char))
							{
								i += zcd->row_heights[j] & 0xFFFFFF;
								if(j >= zcd->first_row)
								{
									zcd->caret_y += zcd->row_heights[j] >> 24;
								}
								j++;
							}

							zcd->current_row = j;
							j = i;
							while(i < zcd->selection_end)
							{
								if(zcd->caption[i] == '\n')
								{
									zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF)) & 0xFFFFFF);
									if((zcd->current_row + 2) < zcd->total_rows)
									{
										memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
									}
									zcd->total_rows--;
								}
								i++;
							}

							strdel(&zcd->caption[zcd->selection_start], zcd->selection_end - zcd->selection_start);
							memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->selection_start, zcd->selection_end - zcd->selection_start);
							zcd->caplen -= zcd->selection_end - zcd->selection_start;
							zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - zcd->selection_end + zcd->selection_start) & 0xFFFFFF);
							zcd->selection_end = 0;
							zcd->selection_start = 0;
							zcd->caret_x = zcd->crect.left;
							while(j < zcd->current_char)
							{
								zcd->caret_x += zcd->char_widths[j];
								j++;
							}

							zedit_scroll_screen(zcd);
							repaint = 1; //beta

							if(zcd->flags & ZC_CARET_VISIBLE)
							{
								SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
							}
							InvalidateRect(hwnd, NULL, TRUE);
						}
						else if(zcd->current_char < zcd->caplen)
						{
							if(zcd->caption[zcd->current_char] == '\n')
							{
								zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF) - 1) & 0xFFFFFF);
								if((zcd->current_row + 2) < zcd->total_rows)
								{
									memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
								}
								zcd->total_rows--;
							}
							else
							{
								zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - 1) & 0xFFFFFF);
							}
							strdel(&zcd->caption[zcd->current_char], 1);
							memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->current_char, 1);
							zcd->caplen--;
							InvalidateRect(hwnd, NULL, TRUE);
							ZCD_DUMP(zcd);
						}
					}
				}
				else if(((zcd->flags & ZC_KEY_STATES) == ZC_CTRL_DOWN) && (wParam == VK_INSERT))
				{
					SendMessage(hwnd, WM_COPY, 0, 0);
				}
				else if(((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN) && (wParam == VK_INSERT))
				{
					SendMessage(hwnd, WM_PASTE, 0, 0);
				}
				else if(wParam == VK_SHIFT)
				{
					zcd->flags |= ZC_SHIFT_DOWN;
				}
				else if(wParam == VK_MENU)
				{
					zcd->flags |= ZC_ALT_DOWN;
				}
				else if(wParam == VK_CONTROL)
				{
					zcd->flags |= ZC_CTRL_DOWN;
				}

				if(repaint)
				{
					InvalidateRect(hwnd, NULL, TRUE);
				}
				//ZTRACE("SELECTION: %d, %d\n", zcd->selection_start, zcd->selection_end);
			}
			return 0L;
		case WM_KEYUP:
			// здесь таится ошибка. клавиша будет отжата,
			// но сообщение WM_KEYUP не придёт, т.к. окно может
			// потерять фокус ввода. но эта ошибка исправлена в
			// WM_KILLFOCUS

			//BOOL control;
			//control = GetKeyState(VK_CONTROL) & 0x8000;
			//shift = GetKeyState(VK_SHIFT) & 0x8000;

			if(wParam == VK_SHIFT)
			{
				zcd->flags &= ~ZC_SHIFT_DOWN;
			}
			else if(wParam == VK_MENU)
			{
				zcd->flags &= ~ZC_ALT_DOWN;
			}
			else if(wParam == VK_CONTROL)
			{
				zcd->flags &= ~ZC_CTRL_DOWN;
			}
			break;
		case WM_CHAR:
			{
				unsigned long i;
				unsigned long j;

				ZTRACE("WM_CHAR: 0x%.2X\n", (unsigned char) wParam);

				if(wParam == 0x18) // Ctrl+X
				{
					SendMessage(hwnd, WM_CUT, 0, 0);
					break;
				}
				else if(wParam == 0x03) // Ctrl+C
				{
					SendMessage(hwnd, WM_COPY, 0, 0);
					break;
				}
				else if(wParam == 0x16) // Ctrl+V
				{
					SendMessage(hwnd, WM_PASTE, 0, 0);
					break;
				}
				else if(wParam == 0x08) // like DELETE key
				{
					if(zcd->selection_start != zcd->selection_end)
					{
						zcd->current_char = zcd->selection_start;
						zcd->caret_y = zcd->crect.top;
						i = 0;
						j = 0;
						while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) <= zcd->current_char))
						{
							i += zcd->row_heights[j] & 0xFFFFFF;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_heights[j] >> 24;
							}
							j++;
						}

						zcd->current_row = j;
						j = i;
						while(i < zcd->selection_end)
						{
							if(zcd->caption[i] == '\n')
							{
								zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF)) & 0xFFFFFF);
								if((zcd->current_row + 2) < zcd->total_rows)
								{
									memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
								}
								zcd->total_rows--;
							}
							i++;
						}

						strdel(&zcd->caption[zcd->selection_start], zcd->selection_end - zcd->selection_start);
						memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->selection_start, zcd->selection_end - zcd->selection_start);
						zcd->caplen -= zcd->selection_end - zcd->selection_start;
						zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - zcd->selection_end + zcd->selection_start) & 0xFFFFFF);
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						zcd->caret_x = zcd->crect.left;
						while(j < zcd->current_char)
						{
							zcd->caret_x += zcd->char_widths[j];
							j++;
						}
					}
					else if(zcd->current_char)
					{
						zcd->current_char--;
						if(zcd->caption[zcd->current_char] == '\n')
						{
							zcd->current_row--;

							zcd->caret_y -= zcd->row_heights[zcd->current_row] >> 24;
							zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF) - 1) & 0xFFFFFF);
							zcd->caret_x = zcd->crect.left;
							j = 0;
							i = 0;
							while(j < zcd->current_row)
							{
								i += zcd->row_heights[j] & 0xFFFFFF;
								//zcd->caret_y += zcd->row_heights[j] >> 24;
								j++;
							}

							while(i < zcd->current_char)
							{
								zcd->caret_x += zcd->char_widths[i];
								i++;
							}

							if((zcd->current_row + 2) < zcd->total_rows)
							{
								memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
							}
							zcd->total_rows--;
						}
						else
						{
							zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - 1) & 0xFFFFFF);
							zcd->caret_x -= zcd->char_widths[zcd->current_char];
						}
						strdel(&zcd->caption[zcd->current_char], 1);
						memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->current_char, 1);
						zcd->caplen--;
					}
				}
				else // insert new char
				{
					HDC hdc;
					HFONT hFontOld;
					int width;

					if(zcd->selection_start != zcd->selection_end)
					{
						// удаляем и объединияем строки с учетом места для одного символа
						zcd->current_char = zcd->selection_start;
						zcd->caret_y = zcd->crect.top;
						i = 0;
						j = 0;
						while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) <= zcd->current_char))
						{
							i += zcd->row_heights[j] & 0xFFFFFF;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_heights[j] >> 24;
							}
							j++;
						}

						zcd->current_row = j;
						j = i;
						while(i < zcd->selection_end)
						{
							if(zcd->caption[i] == '\n')
							{
								zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF)) & 0xFFFFFF);
								if((zcd->current_row + 2) < zcd->total_rows)
								{
									memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
								}
								zcd->total_rows--;
							}
							i++;
						}

						if(zcd->selection_end - zcd->selection_start > 1)
						{
							strdel(&zcd->caption[zcd->selection_start], zcd->selection_end - zcd->selection_start - 1);
							memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->selection_start, zcd->selection_end - zcd->selection_start - 1);
							zcd->caplen -= zcd->selection_end - zcd->selection_start - 1;
						}

						zcd->caret_x = zcd->crect.left;
						while(j < zcd->current_char)
						{
							zcd->caret_x += zcd->char_widths[j];
							j++;
						}
						zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - zcd->selection_end + zcd->selection_start) & 0xFFFFFF);
						zcd->selection_start = 0;
						zcd->selection_end = 0;
					}
					else
					{
						if((zcd->caplen + 1) >= zcd->bufsize)
						{
							break;
						}
						strmov_bytes(&zcd->caption[zcd->current_char], 1);
						//memmov(zcd->caption, zcd->caplen+1, zcd->current_char, 1);
						memmov(zcd->char_widths, zcd->caplen, zcd->current_char, 1);
						zcd->caplen++;
					}

					// split row
					if((((char) wParam == '\r') || ((char) wParam == '\n')) && (zcd->total_rows < (zcd->bufsize/4)))
					{
						zcd->caption[zcd->current_char] = '\n';

						j = 0;
						i = 0;
						while(j < zcd->current_row)
						{
							i += zcd->row_heights[j] & 0xFFFFFF;
							j++;
						}

						j = zcd->row_heights[zcd->current_row] & 0xFFFFFF;
						ZTRACE("split row: cc: %d   rfc: %d   crl: %d\n", zcd->current_char, i, j);
						zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | ((zcd->current_char - i + 1) & 0xFFFFFF);

						zcd->caret_x = zcd->crect.left;
						zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;

						zcd->current_row++;
						if(zcd->current_row < zcd->total_rows)
						{
							memmov(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), zcd->current_row * sizeof(unsigned long), sizeof(unsigned long));
						}
						zcd->total_rows++;
						zcd->row_heights[zcd->current_row] = (13 << 24) | ((j + i - zcd->current_char) & 0xFFFFFF);
						zcd->char_widths[zcd->current_char] = 0;

						// repaint in beta
						while((zcd->first_row < zcd->current_row)
							&& (signed) (zcd->caret_y + (zcd->row_heights[zcd->current_row] >> 24)) > (zcd->crect.bottom))
						{
							zcd->caret_y -= zcd->row_heights[zcd->first_row] >> 24;
							zcd->first_row++;
						}
					}
					else
					{
						zcd->caption[zcd->current_char] = (char) wParam;

						width = 10;
						zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + 1) & 0xFFFFFF);

						if((char) wParam != '\t')
						{
							hdc = GetDC(hwnd);
							if(hdc)
							{
								hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
								GetCharWidth32(hdc, wParam, wParam, &width);
								SelectObject(hdc, hFontOld);
						
								ReleaseDC(hwnd, hdc);
							}
						}
						zcd->char_widths[zcd->current_char] = (unsigned char) width;
						zcd->caret_x += zcd->char_widths[zcd->current_char];
					}

					zcd->current_char++;
				}

				zedit_scroll_screen(zcd);

				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
				}
				InvalidateRect(hwnd, NULL, TRUE);
				ZCD_DUMP(zcd);
			}
			break;
		case WM_CUT:
		case WM_COPY:
			{
				if(zcd->selection_start != zcd->selection_end)
				{
					char *temp_str;
					unsigned long *locale;
					HGLOBAL hglbCopy;

					if(!OpenClipboard(hwnd))
					{
						break;
					}

					EmptyClipboard();

					hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 4);
					if(!hglbCopy)
					{
						CloseClipboard();
						break;
					}

					locale = (unsigned long *) GlobalLock(hglbCopy);
					*locale = 0x00000419;
					GlobalUnlock(hglbCopy);
					SetClipboardData(CF_LOCALE, hglbCopy);

					if(zcd->selection_start != zcd->selection_end)
					{
						hglbCopy = GlobalAlloc(GMEM_MOVEABLE, zcd->selection_end - zcd->selection_start + 1);
					}
					else
					{
						hglbCopy = GlobalAlloc(GMEM_MOVEABLE, zcd->caplen + 1);
					}

					if(!hglbCopy)
					{
						CloseClipboard();
						break;
					}

					temp_str = (char *) GlobalLock(hglbCopy);
					if(zcd->selection_start != zcd->selection_end)
					{
						memcpy(temp_str, zcd->caption + zcd->selection_start, zcd->selection_end - zcd->selection_start);
						temp_str[zcd->selection_end - zcd->selection_start] = 0;
					}
					else
					{
						memcpy(temp_str, zcd->caption, zcd->caplen + 1);
					}
					GlobalUnlock(hglbCopy);

					SetClipboardData(CF_TEXT, hglbCopy);

					CloseClipboard();
				}

				if(uMsg == WM_COPY)
				{
					break;
				}
			}
		case WM_CLEAR: // like DELETE key
			{
				if(zcd->selection_start != zcd->selection_end)
				{
					unsigned long j;
					unsigned long i;

					zcd->current_char = zcd->selection_start;
					zcd->caret_y = zcd->crect.top;
					i = 0;
					j = 0;
					while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) <= zcd->current_char))
					{
						i += zcd->row_heights[j] & 0xFFFFFF;
						if(j >= zcd->first_row)
						{
							zcd->caret_y += zcd->row_heights[j] >> 24;
						}
						j++;
					}

					zcd->current_row = j;
					j = i;
					while(i < zcd->selection_end)
					{
						if(zcd->caption[i] == '\n')
						{
							zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF)) & 0xFFFFFF);
							if((zcd->current_row + 2) < zcd->total_rows)
							{
								memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
							}
							zcd->total_rows--;
						}
						i++;
					}

					strdel(&zcd->caption[zcd->selection_start], zcd->selection_end - zcd->selection_start);
					memdel((char *) zcd->char_widths, zcd->caplen+1, zcd->selection_start, zcd->selection_end - zcd->selection_start);
					zcd->caplen -= zcd->selection_end - zcd->selection_start;
					zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - zcd->selection_end + zcd->selection_start) & 0xFFFFFF);
					zcd->selection_end = 0;
					zcd->selection_start = 0;
					zcd->caret_x = zcd->crect.left;
					while(j < zcd->current_char)
					{
						zcd->caret_x += zcd->char_widths[j];
						j++;
					}

					zedit_scroll_screen(zcd);

					if(zcd->flags & ZC_CARET_VISIBLE)
					{
						SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
					}
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			break;
		case WM_PASTE:
			{
				char *temp_str;
				char *ch;
				HGLOBAL hglb;
				HDC hdc;
				HFONT hFontOld;
				unsigned long in_size;
				unsigned long mov_size;
				unsigned long i;
				unsigned long j;
				int width;

				if(!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(hwnd))
				{
					break;
				}
 
				hglb = GetClipboardData(CF_TEXT);
				if(hglb)
				{
					temp_str = (char *) GlobalLock(hglb);
					if(temp_str)
					{
						in_size = strlen(temp_str);
						mov_size = in_size;
						if((zcd->caplen - (zcd->selection_end - zcd->selection_start) + in_size) < zcd->bufsize)
						{
							if(zcd->selection_start != zcd->selection_end)
							{
								/*
								zcd->current_char = zcd->selection_end;
								while(zcd->current_char > zcd->selection_start)
								{
									zcd->caret_x -= zcd->char_widths[zcd->current_char];
									zcd->current_char--;
								}

								if(zcd->selection_end - zcd->selection_start > in_size)
								{
									mov_size = zcd->selection_end - zcd->selection_start - in_size;
									strdel(&zcd->caption[zcd->selection_start], mov_size);
									memdel(zcd->char_widths, zcd->caplen, zcd->selection_start, mov_size);
									zcd->caplen -= mov_size;
									mov_size = 0;
								}
								else
								{
									mov_size = in_size - zcd->selection_end + zcd->selection_start;
								}
								zcd->selection_start = 0;
								zcd->selection_end = 0;
								*/

								zcd->current_char = zcd->selection_start;
								zcd->caret_y = zcd->crect.top;
								i = 0;
								j = 0;
								while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) <= zcd->current_char))
								{
									i += zcd->row_heights[j] & 0xFFFFFF;
									if(j >= zcd->first_row)
									{
										zcd->caret_y += zcd->row_heights[j] >> 24;
									}
									j++;
								}

								zcd->current_row = j;
								j = i;
								while(i < zcd->selection_end)
								{
									if(zcd->caption[i] == '\n')
									{
										ZTRACE("row merge: %d + %d = %d\n", (zcd->row_heights[zcd->current_row] & 0xFFFFFF), (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF), (zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF));
										zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + (zcd->row_heights[zcd->current_row+1] & 0xFFFFFF)) & 0xFFFFFF);
										// оптимизировать: вынести за границу цикла
										if((zcd->current_row + 2) < zcd->total_rows)
										{
											memdel(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), (zcd->current_row + 1) * sizeof(unsigned long), sizeof(unsigned long));
										}
										zcd->total_rows--;
									}
									i++;
								}

								if(zcd->selection_end - zcd->selection_start > in_size)
								{
									mov_size = zcd->selection_end - zcd->selection_start - in_size;
									strdel(&zcd->caption[zcd->selection_start], mov_size);
									memdel(zcd->char_widths, zcd->caplen, zcd->selection_start, mov_size);
									zcd->caplen -= mov_size;
									mov_size = 0;
								}
								else
								{
									mov_size = in_size - zcd->selection_end + zcd->selection_start;
								}

								zcd->caret_x = zcd->crect.left;
								while(j < zcd->current_char)
								{
									zcd->caret_x += zcd->char_widths[j];
									j++;
								}
								zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) - zcd->selection_end + zcd->selection_start) & 0xFFFFFF);
								zcd->selection_start = 0;
								zcd->selection_end = 0;
							}

							ZTRACE("clen: %d,   cch: %d,    mov_size: %d\n", zcd->caplen, zcd->current_char, mov_size);

							if(mov_size)
							{
								memmov(zcd->caption, zcd->caplen+1, zcd->current_char, mov_size);
								memmov(zcd->char_widths, zcd->caplen, zcd->current_char, mov_size);
								zcd->caplen += mov_size;
							}

							zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | (((zcd->row_heights[zcd->current_row] & 0xFFFFFF) + in_size) & 0xFFFFFF);

							memcpy(&zcd->caption[zcd->current_char], temp_str, in_size);

							j = 0;
							i = 0;
							while(j < zcd->current_row)
							{
								i += zcd->row_heights[j] & 0xFFFFFF;
								j++;
							}

							j = zcd->current_char + in_size;

							while((zcd->current_char < j) && (zcd->total_rows < (zcd->bufsize/4)))
							{
								if(zcd->caption[zcd->current_char] == '\r')
								{
									zcd->caption[zcd->current_char] = ' ';
								}

								if(zcd->caption[zcd->current_char] == '\n')
								{
									mov_size = zcd->row_heights[zcd->current_row] & 0xFFFFFF;
									ZTRACE("split row %d: %d -> %d + %d\n", zcd->current_row, mov_size, zcd->current_char - i + 1, mov_size - zcd->current_char + i - 1);
									zcd->row_heights[zcd->current_row] = (zcd->row_heights[zcd->current_row] & 0xFF000000) | ((zcd->current_char - i + 1) & 0xFFFFFF);

									zcd->caret_x = zcd->crect.left;
									zcd->caret_y += zcd->row_heights[zcd->current_row] >> 24;

									zcd->current_row++;
									if(zcd->current_row < zcd->total_rows)
									{
										memmov(zcd->row_heights, zcd->total_rows * sizeof(unsigned long), zcd->current_row * sizeof(unsigned long), sizeof(unsigned long));
									}
									zcd->total_rows++;
									zcd->row_heights[zcd->current_row] = (13 << 24) | ((mov_size + i - zcd->current_char - 1) & 0xFFFFFF);
									zcd->char_widths[zcd->current_char] = 0;
									i = zcd->current_char + 1;
								}

								zcd->current_char++;
							}

							ZCD_DUMP(zcd);

							zcd->current_char -= in_size;

							hdc = GetDC(hwnd);
							if(hdc)
							{
								hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
								ch = temp_str;
								while(*ch)
								{
									if(*ch != '\t')
									{
										GetCharWidth32(hdc, (unsigned char) *ch, (unsigned char) *ch, &width);
										zcd->char_widths[zcd->current_char] = (unsigned char) width;
									}
									else
									{
										zcd->char_widths[zcd->current_char] = 10;
									}
									if(*ch == '\n')
									{
										zcd->caret_x = zcd->crect.left;
									}
									else
									{
										zcd->caret_x += width;
									}
									zcd->current_char++;
									ch++;
								}
								SelectObject(hdc, hFontOld);
						
								ReleaseDC(hwnd, hdc);
							}

							zedit_scroll_screen(zcd);

							if(zcd->flags & ZC_CARET_VISIBLE)
							{
								SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
							}

							InvalidateRect(hwnd, NULL, TRUE);
						}

						GlobalUnlock(hglb);
					}
				}
				CloseClipboard();

				ZCD_DUMP(zcd);
			}
			break;
		case EM_SETSEL:
			{
				unsigned long j;
				unsigned long i;

				if((wParam == -1) || (wParam > zcd->caplen))
				{
					zcd->selection_start = 0;
					zcd->selection_end = 0;
				}
				else
				{
					zcd->selection_start = (unsigned long) wParam;
					if((lParam == -1) || ((unsigned long) lParam > zcd->caplen))
					{
						zcd->selection_end = zcd->caplen;
					}
					else if(wParam >= (unsigned long) lParam)
					{
						zcd->selection_start = 0;
						zcd->selection_end = 0;
					}
					else
					{
						zcd->selection_end = (unsigned long) lParam;
					}
				}

				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;
				zcd->current_char = zcd->selection_end;
				j = 0;
				i = 0;
				while((j < zcd->total_rows) && ((i + zcd->row_heights[j] & 0xFFFFFF) < zcd->current_char))
				{
					i += zcd->row_heights[j] & 0xFFFFFF;
					zcd->caret_y += zcd->row_heights[j] >> 24;
					j++;
				}

				zcd->current_row = j;
				while(i < zcd->current_char)
				{
					zcd->caret_x += zcd->char_widths[i];
					i++;
				}

				zedit_scroll_screen(zcd);

				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
				}
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case EM_CANUNDO:
			// not supported
			return 0L;
		default:
			return CallWindowProc(DefWindowProc, hwnd, uMsg, wParam, lParam);
	}

	return 0L;
}

// В общем, примерно такая идея с суперконтролом:
/*
LRESULT CALLBACK SuperControlProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ZSUPERCONROL_DATA *zcd;

	zcd = (ZSUPERCONROL_DATA *) GetWindowLong(hwnd, 0);

	switch(uMsg)
	{
		case WM_NCCREATE:
		case WM_CREATE:
		case WM_ERASEBKGND:
			return 1L;
		case WM_PAINT:
			break;
		case WM_MOUSEMOVE:
			{
				ZCONTROL_NODE temp_node;

				temp_node = zcd->controls;
				while(temp_node)
				{
					if(PtInRect(&temp_node->wrect, &pt))
					{
						if(~temp_node->flags & ZC_MOUSEOVER)
						{
							temp_node->flasg |= ZC_MOUSEOVER;
							zsc_send_message(zcd->controls, id, ZM_MOUSEOVER, 0, 0);
						}
						zsc_send_message(zcd->controls, id, ZM_MOUSEMOVE, 0, lParam);
					}
					else if(temp_node->flags & ZC_MOUSEOVER)
					{
						temp_node->flasg &= ~ZC_MOUSEOVER;
						zsc_send_message(zcd->controls, id, ZM_MOUSELEAVE, 0, 0);
					}

					temp_node = temp_node->next_node;
				}

				if(!PtInRect(zcd->wrect, &pt))
				{
					if(GetCapture() == hwnd)
					{
						ReleaseCapture();
					}
				}
				else if(GetCapture() != hwnd)
				{
					SetCapture(hwnd);
				}
			}
			break;
		case WM_KEYDOWN:
			{
				zsc_send_message(zcd->active_control, id, ZM_KEYDOWN, 0, 0);
			}
			break;
		case WM_CHAR:
			{
				zsc_send_message(zcd->active_control, id, ZM_CHAR, 0, 0);
			}
			break;
		case ZM_ADDCONTROL:
			{
				zsc_send_message(zcd->controls, id, ZM_CALCSIZE, 0, 0);
				zsc_send_message(zcd->controls, id, ZM_CREATE, 0, 0);
			}
			break;
		case ZM_DELCONTROL:
			{
				zsc_send_message(zcd->controls, id, ZM_DESTROY, 0, 0);
			}
			break;
		default:
			return CallWindowProc(DefWindowProc, hwnd, uMsg, wParam, lParam);
	}

	return 0L;
}
*/

// registering my custom controls
void RegisterMyControlClasses()
{
	ATOM atm;
	WNDCLASSEX wce;
	
	wce.cbSize = sizeof(WNDCLASSEX);
	wce.cbClsExtra = 0;
	wce.cbWndExtra = sizeof(HYPER_LINK_DATA *);
	wce.lpfnWndProc = HyperLinkProc;
	wce.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS | CS_PARENTDC; // | CS_PARENTDC; // | CS_GLOBALCLASS | CS_OWNDC;
	wce.lpszClassName = "zhyperlink";
	wce.hInstance = NULL; //GetModuleHandle(NULL);
	wce.hIcon = NULL;
	wce.hIconSm = NULL;
	wce.hCursor = LoadCursor(NULL, IDC_HAND);
	/*
	if(!wce.hCursor)
	{
		wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	}
	*/
	wce.hbrBackground = NULL;
	wce.lpszMenuName = NULL;

	atm = RegisterClassEx(&wce);

	ZTRACE("DBG: RegisterClassEx: 0x%.8X GetLastError: %d\n", atm, GetLastError());

	wce.cbWndExtra = sizeof(ZCONTROL_DATA *);
	wce.lpfnWndProc = ButtonProc;
	wce.lpszClassName = "zbutton";
	wce.hCursor = NULL;

	atm = RegisterClassEx(&wce);

	ZTRACE("DBG: RegisterClassEx: 0x%.8X GetLastError: %d\n", atm, GetLastError());

	wce.cbWndExtra = sizeof(ZCONTROL_DATA *);
	wce.lpfnWndProc = EditProc2;
	wce.lpszClassName = "zedit";
	wce.hCursor = LoadCursor(NULL, IDC_IBEAM);

	atm = RegisterClassEx(&wce);

	ZTRACE("DBG: RegisterClassEx: 0x%.8X GetLastError: %d\n", atm, GetLastError());
}

// move to: zcontrols or zmessages. checked! - ok
int read_dialog_controls(HWND hwnd, dlg_fields_list *efts, void *data)
{
	unsigned long i;

	i = 0;
	while(efts[i].name)
	{
		// validate and read control
		if(efts[i].cb_read && efts[i].cb_read(hwnd, &efts[i], data))
		{
			// read failed
			return 1;
		}

		i++;
	}

	return 0;
}

int set_change_flag(dlg_fields_list *efts, int item)
{
	unsigned long i;

	i = 0;
	while(efts[i].name)
	{
		if(efts[i].item == item)
		{
			efts[i].changed |= FLD_CHANGED;
			return 0;
		}
		i++;
	}

	return 1;
}

int reset_change_flags(dlg_fields_list *efts, int clear_flag)
{
	unsigned long i;

	i = 0;
	while(efts[i].name)
	{
		if(clear_flag)
		{
			efts[i].changed &= ~FLD_CHANGED;
		}
		else
		{
			efts[i].changed |= FLD_CHANGED;
		}
		i++;
	}

	return 0;
}

// read edit controls
int cb_read_ec(HWND hwnd, dlg_fields_list *efts, void *data)
{
	char *temp_str;
	int exit_code;

	exit_code = 0;

	temp_str = trim(AllocGetDlgItemText(hwnd, efts->item));

	// числовое значение
	if((efts->changed & FLD_NUMBER) && (isinteger(killspaces(temp_str)) == -1))
	{
		free_str(temp_str);

		MessageBox(hwnd, "Неправильное числовое значение!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
		SetFocus(GetDlgItem(hwnd, efts->item));
		return 1;
	}

	// значение с плавающей точкой
	else if((efts->changed & FLD_FLOAT) && (isfloat(killspaces(temp_str), 1) == -1))
	{
		free_str(temp_str);

		MessageBox(hwnd, "Неправильное значение с плавающей точкой!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
		SetFocus(GetDlgItem(hwnd, efts->item));
		return 1;
	}

	// поле пустое и быть им не может?
	if(isempty(temp_str) && (~efts->changed & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->changed & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не заполнено обязательное поле!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->changed & FLD_CHANGED)
	{
		if(isempty(temp_str) && (efts->changed & (FLD_FLOAT | FLD_NUMBER)))
		{
			sql_query_add((db_list_node **) data, efts->name, "0", efts->flags);
		}
		else
		{
			sql_query_add((db_list_node **) data, efts->name, temp_str, efts->flags);
		}
	}

	free_str(temp_str);

	return exit_code;
}

// read combo box controls
int cb_read_cc(HWND hwnd, dlg_fields_list *efts, void *data)
{
	int exit_code;
	int cur_sel;

	exit_code = 0;

	cur_sel = SendDlgItemMessage(hwnd, efts->item, CB_GETCURSEL, 0, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->changed & FLD_CANEMPTY))
	if(((cur_sel == CB_ERR) || (cur_sel == 0)) && (~efts->changed & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->changed & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение из выпадающего списка!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->changed & FLD_CHANGED)
	{
		//sql_query_add((db_list_node **) data, efts->name, (char *) ((cur_sel != CB_ERR)?SendDlgItemMessage(hwnd, efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
		sql_query_add((db_list_node **) data, efts->name, (char *) (((cur_sel != CB_ERR) && (cur_sel != 0))?SendDlgItemMessage(hwnd, efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
	}

	return exit_code;
}

// read check box controls: return string "1" if checked, otherwise "0"
int cb_read_cb(HWND hwnd, dlg_fields_list *efts, void *data)
{
	// поле было отредактировано?
	if(efts->changed & FLD_CHANGED)
	{
		sql_query_add((db_list_node **) data, efts->name, (SendDlgItemMessage(hwnd, efts->item, BM_GETCHECK, 0, 0L) == BST_CHECKED)?"1":"0", efts->flags);
	}

	return 0;
}

// read tree view controls
int cb_read_tc(HWND hwnd, dlg_fields_list *efts, void *data)
{
	int exit_code;
	long cur_sel;

	exit_code = 0;

	cur_sel = SendDlgItemMessage(hwnd, efts->item, TVM_GETNEXTITEM, TVGN_CARET, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->changed & FLD_CANEMPTY))
	if((cur_sel == 0) && (~efts->changed & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->changed & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение в дереве!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->changed & FLD_CHANGED)
	{
		//sql_query_add((db_list_node **) data, efts->name, (char *) ((cur_sel != CB_ERR)?SendDlgItemMessage(hwnd, efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
		if(cur_sel != 0)
		{
			TVITEM tvItem;
			
			tvItem.mask = TVIF_HANDLE | TVIF_PARAM;
			tvItem.hItem = (HTREEITEM) cur_sel;
			SendDlgItemMessage(hwnd, efts->item, TVM_GETITEM, 0, (LPARAM) &tvItem);

			cur_sel = tvItem.lParam;
		}

		sql_query_add((db_list_node **) data, efts->name, (char *) cur_sel, efts->flags);
	}

	return exit_code;
}

// read date controls
int cb_read_dc(HWND hwnd, dlg_fields_list *efts, void *data)
{
	char *temp_str;
	int exit_code;
	int failed;
	SYSTEMTIME date;

	exit_code = 0;

	failed = SendDlgItemMessage(hwnd, efts->item, DTM_GETSYSTEMTIME, 0, (LPARAM) &date);

	if((failed != GDT_VALID) && (~efts->changed & FLD_CANEMPTY))
	{
		if(efts->changed & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не заполнено обязательное поле с датой!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, efts->item));
			exit_code = 1;
		}
	}
	else if(efts->changed & FLD_CHANGED)
	{
		if(failed == GDT_VALID)
		{
			temp_str = alloc_string_ex("%.4d-%.2d-%.2d", date.wYear, date.wMonth, date.wDay);
			sql_query_add((db_list_node **) data, efts->name, temp_str, efts->flags);
			free_str(temp_str);
		}
		else
		{
			sql_query_add((db_list_node **) data, efts->name, NULL, SG_NULL);
		}
	}

	return exit_code;
}

LRESULT CALLBACK TreeViewEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC OldWndProc;

	if(uMsg == WM_GETDLGCODE)
	{
		return DLGC_WANTALLKEYS;
	}

	OldWndProc = (WNDPROC) GetWindowLong(hwnd, GWL_USERDATA);
	if(!OldWndProc)
	{
		ZTRACE("Error: Null WndProc for TreeView! Rewrite this function with using global list variable!\n");
		return 0L;
	}

	return CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);
}

long SubClassTreeViewEdit(HWND hwnd)
{
	HWND hwndEdit;
	WNDPROC OldWndProc;

	hwndEdit = (HWND) SendMessage(hwnd, TVM_GETEDITCONTROL, 0, 0L);
	if(hwndEdit)
	{
		OldWndProc = (WNDPROC) SetWindowLong(hwndEdit, GWL_WNDPROC, (LONG) TreeViewEditProc);
		SetWindowLong(hwndEdit, GWL_USERDATA, (LONG) OldWndProc);

		return (long) OldWndProc;
	}

	return 0L;
}

HWND zc_create(db_list_node **cl, unsigned long flags, RECT *cr, DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int cx1, int cy1, int cx2, int cy2, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	int x1;
	int y1;
	int x2;
	int y2;

	//RECT cr;

	wnd_control_node *temp_node;

	temp_node = (wnd_control_node *) zalloc(sizeof(wnd_control_node));
	if(temp_node)
	{
		temp_node->cx1 = cx1;
		temp_node->cy1 = cy1;
		temp_node->cx2 = cx2;
		temp_node->cy2 = cy2;
		temp_node->flags = flags;
		temp_node->id = (int) hMenu;

		//GetClientRect(hWndParent, &cr);

		if((flags & (ZC_LEFT | ZC_RIGHT)) == 0)
		{
			flags |= ZC_LEFT;
		}

		if((flags & (ZC_TOP | ZC_BOTTOM)) == 0)
		{
			flags |= ZC_TOP;
		}

		if(flags & ZC_LEFT)
		{
			x1 = cr->left + cx1;
		}
		
		if(flags & ZC_RIGHT)
		{
			x2 = cr->right - cx2;
		}
		else
		{
			x2 = x1 + cx2;
		}

		if(~flags & ZC_LEFT)
		{
			x1 = x2 - cx1;
		}

		if(flags & ZC_TOP)
		{
			y1 = cr->top + cy1;
		}

		if(flags & ZC_BOTTOM)
		{
			y2 = cr->bottom - cy2;
		}
		else
		{
			y2 = y1 + cy2;
		}

		if(~flags & ZC_TOP)
		{
			y1 = y2 - cy1;
		}

		temp_node->hwnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x1, y1, x2 - x1, y2 - y1, hWndParent, hMenu, hInstance, lpParam);

		if(!temp_node->hwnd)
		{
			zfree(temp_node);
			return 0;
		}

		SendMessage(temp_node->hwnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), TRUE);

		list_add(cl, temp_node);

		return temp_node->hwnd;
	}

	return 0;
}

int cb_zc_move(void *data1, void *data2)
{
	int x1;
	int y1;
	int x2;
	int y2;

	if((((wnd_control_node *) data1)->flags & (ZC_LEFT | ZC_RIGHT)) == 0)
	{
		((wnd_control_node *) data1)->flags |= ZC_LEFT;
	}

	if((((wnd_control_node *) data1)->flags & (ZC_TOP | ZC_BOTTOM)) == 0)
	{
		((wnd_control_node *) data1)->flags |= ZC_TOP;
	}

	if(((wnd_control_node *) data1)->flags & ZC_LEFT)
	{
		x1 = ((RECT *) data2)->left + ((wnd_control_node *) data1)->cx1;
	}

	if(((wnd_control_node *) data1)->flags & ZC_RIGHT)
	{
		x2 = ((RECT *) data2)->right - ((wnd_control_node *) data1)->cx2;
	}
	else
	{
		x2 = x1 + ((wnd_control_node *) data1)->cx2;
	}

	if(~((wnd_control_node *) data1)->flags & ZC_LEFT)
	{
		x1 = x2 - ((wnd_control_node *) data1)->cx1;
	}

	if(((wnd_control_node *) data1)->flags & ZC_BOTTOM)
	{
		y2 = ((RECT *) data2)->bottom - ((wnd_control_node *) data1)->cy2;
	}

	if(((wnd_control_node *) data1)->flags & ZC_TOP)
	{
		y1 = ((RECT *) data2)->top + ((wnd_control_node *) data1)->cy1;
	}

	if(~((wnd_control_node *) data1)->flags & ZC_BOTTOM)
	{
		y2 = y1 + ((wnd_control_node *) data1)->cy2;
	}

	if(~((wnd_control_node *) data1)->flags & ZC_TOP)
	{
		y1 = y2 - ((wnd_control_node *) data1)->cy1;
	}

	MoveWindow(((wnd_control_node *) data1)->hwnd, x1, y1, x2 - x1, y2 - y1, TRUE);

	return 0;
}

db_list_node *zc_reposition(db_list_node *cl, RECT *cr)
{
	return list_walk(cl, cb_zc_move, cr);
}

void cb_zc_free(void *data)
{
	DestroyWindow(((wnd_control_node *) data)->hwnd);
	zfree(data);
}

int zc_free(db_list_node **cl)
{
	return list_free(cl, cb_zc_free);
}

