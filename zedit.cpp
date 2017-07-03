//#include "stdafx.h"								//pf_ziminski  [2004]
#include "zcontrols.h"
#include <commctrl.h>
#include <windowsx.h>
#include "utstrings.h"
#include "zdbg.h"

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL WM_MOUSELAST+1 
    // Message ID for IntelliMouse wheel
#endif

typedef struct _ZEDIT_ROWS
{
	unsigned long flags;
	unsigned long len;
	struct _ZEDIT_ROWS *next_node;
} ZEDIT_ROWS;

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

	unsigned long caret_x;			// absolute
	unsigned long caret_y;			// relative first_row
	unsigned long selection_start;	// index
	unsigned long selection_end;	// index
	unsigned long current_char;		// index
	unsigned long row_height;
	int tab;
	// multiline
	unsigned long total_rows;		// index
	unsigned long first_row;		// index
	unsigned long first_x;			// pixels
	//unsigned long current_row;		// index
	struct _ZEDIT_ROWS *rows;
} ZEDIT_DATA;

extern COLORREF crLight;
extern COLORREF crShadow;
extern COLORREF crText;
extern COLORREF crTextDisabled;
extern COLORREF crHighLight;
extern COLORREF crBackground;

extern COLORREF crEditLight;
extern COLORREF crEditShadow;

int parse_text(ZEDIT_DATA *zcd)
{
	char *ch;
	ZEDIT_ROWS *r;
	ZEDIT_ROWS *k;

	ch = zcd->caption;
	r = zcd->rows;
	zcd->total_rows = 1;
	r->len = 0;
	while(*ch)
	{
		r->len++;
		if(*ch == '\n')
		{
			zcd->total_rows++;
			if(!r->next_node)
			{
				r->next_node = (ZEDIT_ROWS *) zalloc(sizeof(ZEDIT_ROWS));
				r = r->next_node;
				r->next_node = NULL;
			}
			r->len = 0;
		}

		ch++;
	}

	while(r->next_node)
	{
		k = r->next_node;
		zfree(k);
		r->next_node = r->next_node->next_node;
	}

	//ZCD_DUMP(zcd);

	return 0;
}

unsigned long zedit_text_extent(HWND hwnd, ZEDIT_DATA *zcd, unsigned long s, unsigned long e)
{
	HDC hdc;
	HFONT hFontOld;
	unsigned long width;

	hdc = GetDC(hwnd);
	if(!hdc)
	{
		return 0;
	}

	hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
	//GetCharWidth32(hdc, wParam, wParam, &width);
	width = LOWORD(GetTabbedTextExtent(hdc, zcd->caption + s, e - s, 1, &zcd->tab));
	SelectObject(hdc, hFontOld);
	ReleaseDC(hwnd, hdc);

	return width;
}

ZEDIT_ROWS *zedit_find_row(ZEDIT_ROWS *r, unsigned long offset, unsigned long *i, unsigned long *j)
{
	*i = 0;
	*j = 0;
	while(r && ((*i + r->len) <= offset))
	{
		*i += r->len;
		(*j)++;
		r = r->next_node;
	}

	return r;
}

unsigned long zedit_char_form_point(HWND hwnd, ZEDIT_ROWS *r, int x, int y)
{
	return x;
}

unsigned long zedit_caret_move(HWND hwnd, ZEDIT_DATA *zcd, int scroll)
{
	HDC hdc;
	HFONT hFontOld;
	ZEDIT_ROWS *r;
	unsigned long i;
	unsigned long j;

	zcd->caret_y = zcd->crect.top;
	zcd->caret_x = zcd->crect.left;

	r = zcd->rows;
	j = 0;
	i = 0;
	while(r && (zcd->current_char > (i + r->len)))
	{
		if(j >= zcd->first_row)
		{
			zcd->caret_y += zcd->row_height;
		}

		i += r->len;
		r = r->next_node;
		j++;
	}

	if((zcd->current_char > 0) && (zcd->current_char == (i + r->len)) && (zcd->caption[zcd->current_char - 1] == '\n'))
	{
		zcd->caret_y += zcd->row_height;
	}
	else if(zcd->current_char > i)
	{
		hdc = GetDC(hwnd);
		if(!hdc)
		{
			return 0;
		}

		hFontOld = (HFONT) SelectObject(hdc, zcd->hFont);
		//GetCharWidth32(hdc, wParam, wParam, &width);
		zcd->caret_x += LOWORD(GetTabbedTextExtent(hdc, zcd->caption + i, zcd->current_char - i, 1, &zcd->tab));
		SelectObject(hdc, hFontOld);
		ReleaseDC(hwnd, hdc);
	}

	if(scroll)
	{
		zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

		if(zcd->caret_x < zcd->first_x)
		{
			if(zcd->caret_x < 30)
			{
				zcd->first_x = 0;
			}
			else
			{
				zcd->first_x = zcd->caret_x - 30;
			}
		}
		else if((signed) (zcd->caret_x - zcd->first_x) > zcd->crect.right)
		{
			zcd->first_x = zcd->caret_x - (zcd->crect.right - zcd->crect.left) + 30;
		}
		if(zcd->first_row > j)
		{
			zcd->first_row = j;
			zcd->caret_y = zcd->crect.top;
		}
		else
		{
			while((zcd->first_row < j)
				&& ((signed) (zcd->caret_y + (zcd->row_height)) > zcd->crect.bottom))
			{
				zcd->caret_y -= zcd->row_height;
				zcd->first_row++;
			}
		}

		if((zcd->flags & (ZC_CARET_VISIBLE | ZC_ACTIVE)) == ZC_ACTIVE)
		{
			zcd->flags |= ZC_CARET_VISIBLE;
		}
	}

	if(zcd->flags & ZC_CARET_VISIBLE)
	{
		ShowCaret(hwnd);
		SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
	}

	ZTRACE("    cc: %d   cx: %d   cy: %d   fx: %d   fr: %d   ss: %d   se: %d\n", zcd->current_char, zcd->caret_x, zcd->caret_y, zcd->first_x, zcd->first_row, zcd->selection_start, zcd->selection_end);

	return MAKELONG(zcd->caret_x, zcd->caret_y);
}

int zedit_paint_row(ZEDIT_DATA *zcd, HDC hdc, unsigned long offset)
{
	return 0;
}

LRESULT CALLBACK EditProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				zcd->rows = (ZEDIT_ROWS *) zalloc(sizeof(ZEDIT_ROWS));
				zcd->rows->len = 0;
				zcd->rows->next_node = NULL;

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
				//HFONT hFontOld;
				//HDC hdc;
				//RECT cr;
				//SIZE ts;

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

				zcd->tab = 30;
				zcd->row_height = 13;

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

				parse_text(zcd);

				/*
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
				*/

				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;
			}
			return 0L;
		//case WM_SIZE:
		//case WM_MOVE:
		//case WM_SHOWWINDOW:
		case WM_SETFONT:
			{
				if(wParam)
				{
					zcd->hFont = (HFONT) wParam;
				}

				if(LOWORD(lParam))
				{
					// здесь надо пересчитать позицию каретки
					zedit_caret_move(hwnd, zcd, 0);

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
				ZEDIT_ROWS *r;

				//ZTRACE("WM_PAINT\n");

				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					HideCaret(hwnd);
				}

				BeginPaint(hwnd, &ps);

				//IntersectClipRect(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
				IntersectClipRect(ps.hdc, zcd->crect.left, zcd->crect.top, zcd->crect.right, zcd->crect.bottom);

				hbrBack = (HBRUSH) SendMessage(GetParent(hwnd), WM_CTLCOLOREDIT, (WPARAM) ps.hdc, (LPARAM) hwnd);

				//if(ps.fErase)
				{
					FillRect(ps.hdc, &zcd->crect, hbrBack);
				}

				color_back = GetBkColor(ps.hdc);
				color_text = GetTextColor(ps.hdc);

				hFontOld = (HFONT) SelectObject(ps.hdc, zcd->hFont);

				r = zcd->rows;
				i = 0;
				j = 0;
				while(j < zcd->first_row)
				{
					i += r->len;
					r = r->next_node;
					j++;
				}

				SetBkColor(ps.hdc, color_back);
				SetTextColor(ps.hdc, color_text);

				y = zcd->crect.top;
				while(((signed) y < zcd->crect.bottom) && r)
				{
					if(zcd->selection_start != zcd->selection_end)
					{
						if((i >= zcd->selection_start) && (i <= zcd->selection_end))
						{
							SetBkColor(ps.hdc, RGB(10, 36, 106));
							SetTextColor(ps.hdc, RGB(255, 255, 255));
							if((i + r->len) <= zcd->selection_end)
							{
								// full row selected
								TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, r->len - 1, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
							}
							else
							{
								// start row selected
								x = LOWORD(TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, zcd->selection_end - i, 1, &zcd->tab, zcd->crect.left - zcd->first_x));
								SetBkColor(ps.hdc, color_back);
								SetTextColor(ps.hdc, color_text);
								TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x + x, y, zcd->caption + zcd->selection_end, i + r->len - zcd->selection_end, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
							}

						}
						else if((i < zcd->selection_start) && (i + r->len >= zcd->selection_start))
						{
							SetBkColor(ps.hdc, color_back);
							SetTextColor(ps.hdc, color_text);
							if((i + r->len) > zcd->selection_end)
							{
								// center of row selected
								x = LOWORD(TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, zcd->selection_start - i, 1, &zcd->tab, zcd->crect.left - zcd->first_x));
								SetBkColor(ps.hdc, RGB(10, 36, 106));
								SetTextColor(ps.hdc, RGB(255, 255, 255));
								x += LOWORD(TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x + x, y, zcd->caption + zcd->selection_start, zcd->selection_end - zcd->selection_start, 1, &zcd->tab, zcd->crect.left - zcd->first_x));
								SetBkColor(ps.hdc, color_back);
								SetTextColor(ps.hdc, color_text);
								TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x + x, y, zcd->caption + zcd->selection_end, i + r->len - zcd->selection_end, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
							}
							else
							{
								// end row selected
								x = LOWORD(TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, zcd->selection_start - i, 1, &zcd->tab, zcd->crect.left - zcd->first_x));
								SetBkColor(ps.hdc, RGB(10, 36, 106));
								SetTextColor(ps.hdc, RGB(255, 255, 255));
								TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x + x, y, zcd->caption + zcd->selection_start, i + r->len - zcd->selection_start, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
							}
						}
						else
						{
							// no this row selected
							TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, r->len, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
						}
					}
					else
					{
						TabbedTextOut(ps.hdc, zcd->crect.left - zcd->first_x, y, zcd->caption + i, (zcd->caption[i + r->len - 1] == '\n')?(r->len - 1):r->len, 1, &zcd->tab, zcd->crect.left - zcd->first_x);
					}
					y += zcd->row_height;
					i += r->len;
					r = r->next_node;
				}

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
				ZEDIT_ROWS *r;
				zfree(zcd->caption);

				while(zcd->rows)
				{
					r = zcd->rows;
					zcd->rows = zcd->rows->next_node;
					zfree(r);
				}
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
				unsigned long i;
				unsigned long j;
				int my;
				int mx;
				ZEDIT_ROWS *r;
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

					//j = zcd->current_char;
					zcd->current_char = 0;
					zcd->caret_x = zcd->crect.left;
					zcd->caret_y = zcd->crect.top;

					zedit_find_row(zcd->rows, zcd->current_char, &i, &j);
					//zcd->current_row = 0;
					r = zcd->rows;
					while(j < zcd->first_row)
					{
						zcd->current_char += r->len;
						r = r->next_node;
					}

					if(my < 0)
					{
						/*
						while((my < 0) && (zcd->current_row > 0))
						{
							zcd->current_row--;
							my += zcd->row_height;
							zcd->current_char -= zcd->row_heights[zcd->current_row] & 0xFFFFFF;
						}
						*/
					}
					else
					{
						while(r)
						{
							if((signed) (zcd->caret_y + (zcd->row_height)) > my)
							{
								break;
							}
							zcd->caret_y += zcd->row_height;
							zcd->current_char += r->len;
							r = r->next_node;
						}
					}

					// здесь есть ошибка: если mx меньше нуля!
					while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
					{
						if((signed)(zcd->caret_x - zcd->first_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2)) >= mx)
						{
							break;
						}
						zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
						zcd->current_char++;
					}
					//ZTRACE("SELECTION OLD: %d, %d (%d)\n", zcd->selection_start, zcd->selection_end, zcd->current_char);

					zedit_caret_move(hwnd, zcd, 1);

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

				zcd->flags |= ZC_ACTIVE | ZC_CARET_VISIBLE;
				if(zcd->flags & ZC_CARET_VISIBLE)
				{
					SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
					ShowCaret(hwnd);
				}
			}
			break;
		case WM_KILLFOCUS:
			{
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
			}
			break;

		case WM_SETTEXT:
			{
				strncpy_tiny(zcd->caption, (char *) lParam, zcd->bufsize-1);
				zcd->caplen = strlen(zcd->caption);

				// здесь надо обнулять значения структуры zcd

				parse_text(zcd);

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
				unsigned long i;
				unsigned long j;
				int repaint;
				ZEDIT_ROWS *r;

				repaint = 1;

				// устанавливаем фокус в окно, если неактивно
				if(~zcd->flags & ZC_ACTIVE)
				{
					repaint = 0;
					SetFocus(hwnd);
					//InvalidateRect(hwnd, NULL, FALSE);
				}

				i = zcd->current_char;
				zcd->current_char = 0;
				zcd->caret_x = zcd->crect.left;
				zcd->caret_y = zcd->crect.top;

				zedit_find_row(zcd->rows, zcd->current_char, &i, &j);
				r = zcd->rows;
				while(j < zcd->first_row)
				{
					zcd->current_char += r->len;
					r = r->next_node;
				}
				while(r)
				{
					if((signed) (zcd->caret_y + (zcd->row_height)) > GET_Y_LPARAM(lParam))
					{
						break;
					}
					zcd->caret_y += zcd->row_height;
					zcd->current_char += r->len;
					r = r->next_node;
				}

				while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
				{
					if((signed)(zcd->caret_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2) - zcd->first_x) >= GET_X_LPARAM(lParam))
					{
						break;
					}
					zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
					zcd->current_char++;
				}

				// here better select algorithm!?
				if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
				{
					if(zcd->selection_end == i)
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
					else if(zcd->selection_start == i)
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
					else if(zcd->current_char > i)
					{
						zcd->selection_start = i;
						zcd->selection_end = zcd->current_char;
					}
					else
					{
						zcd->selection_start = zcd->current_char;
						zcd->selection_end = i;
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

				zedit_caret_move(hwnd, zcd, 1);

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
			{
				unsigned long i;
				unsigned long j;

				ZTRACE("WM_MOUSEWHEEL: delta %d\n", (signed) wParam >> 16);

				zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

				if(((signed) wParam >> 16) < 0)
				{
					if(zcd->first_row < (zcd->total_rows - 1))
					{
						if(zcd->caret_y < (zcd->row_height))
						{
							zcd->caret_y = zcd->crect.top;
						}
						else
						{
							zcd->caret_y -= zcd->row_height;
						}
						zcd->first_row++;
					}
				}
				else
				{
					if(zcd->first_row > 0)
					{
						zcd->first_row--;
						zcd->caret_y += zcd->row_height;
					}
					if(zcd->first_row == j)
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
				else if((zcd->first_row > j) || ((zcd->caret_y + (zcd->row_height)) > zcd->crect.bottom))
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
			}
			break;
		case WM_KEYDOWN:
			{
				int repaint;
				unsigned long i;
				unsigned long j;
				ZEDIT_ROWS *r;

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

						zcd->current_char++;

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta

						//ZTRACE("FIRST X:  %d\n", zcd->first_x);
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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
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
						zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
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
						r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);
						i += r->len - 1;

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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
					}
				}
				/*
				else if(wParam == VK_UP)
				{
					if(((zcd->flags & ZC_KEY_STATES) != ZC_SHIFT_DOWN)
						&& (zcd->selection_start != zcd->selection_end))
					{
						zcd->selection_end = 0;
						zcd->selection_start = 0;
						repaint = 1;
					}

					zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

					if(j > 0)
					{
						i ^= zcd->current_char;
						zcd->current_char ^= i;
						i ^= zcd->current_char;

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caret_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2)) >= j)
							{
								break;
							}
							zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
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

					zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

					if(j < (zcd->total_rows - 1))
					{
						i ^= zcd->current_char;
						zcd->current_char ^= i;
						i ^= zcd->current_char;

						zcd->caret_y += zcd->row_height;

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caret_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2)) >= j)
							{
								break;
							}
							zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
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

					zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

					if(j > 0)
					{
						if(j > 5)
						{
							zcd->current_row -= 5;
						}
						else
						{
							zcd->current_row = 0;
						}
						i = zcd->current_char;
						zcd->current_char = 0;
						zcd->caret_y -= zcd->row_height;

						zcd->caret_y = zcd->crect.top;
						j = 0;
						r = zcd->rows;
						while(j < zcd->current_row)
						{
							zcd->current_char += r->len;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_height;
							}
							j++;
							r = r->next_node;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caret_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2)) >= j)
							{
								break;
							}
							zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta
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
						r = zcd->rows;
						while(j < zcd->current_row)
						{
							zcd->current_char += r->len;
							if(j >= zcd->first_row)
							{
								zcd->caret_y += zcd->row_height;
							}
							j++;
							r = r->next_node;
						}

						j = zcd->caret_x;
						zcd->caret_x = zcd->crect.left;
						while((zcd->current_char < zcd->caplen) && (zcd->caption[zcd->current_char] != '\n'))
						{
							if((zcd->caret_x + (zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1)/2)) >= j)
							{
								break;
							}
							zcd->caret_x += zedit_text_extent(hwnd, zcd, zcd->current_char, zcd->current_char + 1);
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

						zedit_caret_move(hwnd, zcd, 1);
						repaint = 1; //beta

						if(zcd->flags & ZC_CARET_VISIBLE)
						{
							//SetCaretPos(zcd->caret_x - zcd->first_x, zcd->caret_y);
						}
					}
				}
				*/
				else if(wParam == VK_DELETE) // delete char or current selection
				{
					if((zcd->flags & ZC_KEY_STATES) == ZC_SHIFT_DOWN)
					{
						SendMessage(hwnd, WM_CUT, 0, 0);
					}
					else
					{
						SendMessage(hwnd, WM_CLEAR, 0, 0);
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
				ZEDIT_ROWS *r;
				ZEDIT_ROWS *k;

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
				else if(wParam == 0x08)
				{
					if(zcd->selection_start != zcd->selection_end)  // some like DELETE key
					{
						zcd->current_char = zcd->selection_start;

						// ищем первый символ строки с которой начинается выделение и номер строки
						zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

						// объеденяем строки
						j = i;
						r = zcd->rows;
						while(r->next_node && ((i + r->len) <= zcd->selection_end))
						{
							r->len += r->next_node->len;
							k = r->next_node;
							r->next_node = r->next_node->next_node;
							zfree(k);
							zcd->total_rows--;
						}

						// удаляем текст
						memdel(zcd->caption, zcd->caplen + 1, zcd->selection_start, zcd->selection_end - zcd->selection_start);
						zcd->caplen -= zcd->selection_end - zcd->selection_start;

						r->len -= zcd->selection_end - zcd->selection_start;
						zcd->selection_start = 0;
						zcd->selection_end = 0;
					}
					else if(zcd->current_char)
					{
						zcd->current_char--;

						r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

						//if(zcd->caption[zcd->current_char] == '\n')
						if(r->next_node && (zcd->current_char == (i + r->len - 1)))
						{
							r->len += r->next_node->len;
							k = r->next_node;
							r->next_node = k->next_node;
							zfree(k);
							zcd->total_rows--;
						}

						r->len--;

						memdel(zcd->caption, zcd->caplen + 1, zcd->current_char, 1);
						zcd->caplen--;
					}
				}
				else // insert new char
				{
					if(zcd->selection_start != zcd->selection_end)
					{
						// удаляем и объединияем строки с учетом места для одного символа
						zcd->current_char = zcd->selection_start;

						r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

						while(r->next_node && ((i + r->len) <= zcd->selection_end))
						{
							r->len += r->next_node->len;
							k = r->next_node;
							r->next_node = r->next_node->next_node;
							zfree(k);
							zcd->total_rows--;
						}

						if(zcd->selection_end - zcd->selection_start > 1)
						{
							memdel(zcd->caption, zcd->caplen + 1, zcd->selection_start, zcd->selection_end - zcd->selection_start - 1);
							zcd->caplen -= zcd->selection_end - zcd->selection_start - 1;
						}

						r->len -= zcd->selection_end - zcd->selection_start;
						zcd->selection_start = 0;
						zcd->selection_end = 0;
					}
					else
					{
						if((zcd->caplen + 1) >= zcd->bufsize)
						{
							break;
						}
						memmov(zcd->caption, zcd->caplen + 1, zcd->current_char, 1);
						zcd->caplen++;
					}

					// split row
					if(((char) wParam == '\r') || ((char) wParam == '\n'))
					{
						zcd->caption[zcd->current_char] = '\n';

						r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

						j = r->len;
						ZTRACE("split row: cc: %d   rfc: %d   crl: %d\n", zcd->current_char, i, j);
						r->len = zcd->current_char - i + 1;

						k = (ZEDIT_ROWS *) zalloc(sizeof(ZEDIT_ROWS));
						k->next_node = r->next_node;
						r->next_node = k;
						zcd->total_rows++;
						k->len = (j + i - zcd->current_char);
					}
					else
					{
						zcd->caption[zcd->current_char] = (char) wParam;

						r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);
						r->len++;
					}

					zcd->current_char++;
				}

				zedit_caret_move(hwnd, zcd, 1);

				InvalidateRect(hwnd, NULL, TRUE);
				//ZCD_DUMP(zcd);
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

					if(uMsg == WM_COPY)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		case WM_CLEAR: // like DELETE key
			{
				unsigned long i;
				unsigned long j;
				ZEDIT_ROWS *r;
				ZEDIT_ROWS *k;

				// надо ещё пересчитывать высоту строк
				if(zcd->selection_start != zcd->selection_end)
				{
					zcd->current_char = zcd->selection_start;

					// ищем первый символ строки с которой начинается выделение и номер строки
					i = 0;
					j = 0;
					r = zcd->rows;
					while((j < zcd->total_rows) && ((i + r->len) <= zcd->current_char))
					{
						i += r->len;
						j++;
						r = r->next_node;
					}

					// объеденяем строки
					//zcd->current_row = j;
					j = i;
					while(r->next_node && ((i + r->len) < zcd->selection_end))
					{
						r->len += r->next_node->len;
						k = r->next_node;
						r->next_node = r->next_node->next_node;
						zfree(k);
						zcd->total_rows--;
					}

					// удаляем текст
					memdel(zcd->caption, zcd->caplen+1, zcd->selection_start, zcd->selection_end - zcd->selection_start);
					zcd->caplen -= zcd->selection_end - zcd->selection_start;

					r->len -= zcd->selection_end - zcd->selection_start;
					zcd->selection_start = 0;
					zcd->selection_end = 0;
				}
				else if(zcd->current_char < zcd->caplen)
				{
					r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

					if(zcd->caption[zcd->current_char] == '\n')
					{
						r->len += r->next_node->len;
						k = r->next_node;
						r->next_node = k->next_node;
						zfree(k);
						zcd->total_rows--;
					}

					r->len--;

					memdel(zcd->caption, zcd->caplen + 1, zcd->current_char, 1);
					zcd->caplen--;
					//ZCD_DUMP(zcd);
				}
				zedit_caret_move(hwnd, zcd, 1);

				InvalidateRect(hwnd, NULL, TRUE);
			}
			break;
		case WM_PASTE:
			{
				char *temp_str;
				//char *ch;
				HGLOBAL hglb;
				unsigned long in_size;
				unsigned long mov_size;
				unsigned long i;
				unsigned long j;
				ZEDIT_ROWS *r;
				ZEDIT_ROWS *k;

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
								// удаляем и объединияем строки с учетом места для одного символа
								zcd->current_char = zcd->selection_start;
								i = 0;
								j = 0;
								r = zcd->rows;
								while((j < zcd->total_rows) && ((i + r->len) <= zcd->current_char))
								{
									i += r->len;
									j++;
									r = r->next_node;
								}

								j = i;
								while(r->next_node && ((i + r->len) < zcd->selection_end))
								{
									r->len += r->next_node->len;
									k = r->next_node;
									r->next_node = r->next_node->next_node;
									zfree(k);
									zcd->total_rows--;
								}

								if(zcd->selection_end - zcd->selection_start > in_size)
								{
									mov_size = zcd->selection_end - zcd->selection_start - in_size;
									memdel(zcd->caption, zcd->caplen + 1, zcd->selection_start, mov_size);
									zcd->caplen -= mov_size;
									mov_size = 0;
								}
								else
								{
									mov_size = in_size - zcd->selection_end + zcd->selection_start;
								}

								r->len -= zcd->selection_end - zcd->selection_start;
								zcd->selection_start = 0;
								zcd->selection_end = 0;
							}

							ZTRACE("clen: %d,   cch: %d,    mov_size: %d\n", zcd->caplen, zcd->current_char, mov_size);

							if(mov_size)
							{
								memmov(zcd->caption, zcd->caplen + 1, zcd->current_char, mov_size);
								zcd->caplen += mov_size;
							}

							r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);
							r->len += in_size;

							memcpy(&zcd->caption[zcd->current_char], temp_str, in_size);

							r = zedit_find_row(zcd->rows, zcd->current_char, &i, &j);

							j = zcd->current_char + in_size;

							while((zcd->current_char < j) && (zcd->total_rows < (zcd->bufsize/4)))
							{
								if(zcd->caption[zcd->current_char] == '\r')
								{
									zcd->caption[zcd->current_char] = ' ';
								}

								if(zcd->caption[zcd->current_char] == '\n')
								{
									mov_size = r->len;
									ZTRACE("split row : %d -> %d + %d\n", mov_size, zcd->current_char - i + 1, mov_size - zcd->current_char + i - 1);
									r->len = zcd->current_char - i + 1;

									k = (ZEDIT_ROWS *) zalloc(sizeof(ZEDIT_ROWS));
									k->next_node = r->next_node;
									r->next_node = k;
									//zcd->current_row++;
									zcd->total_rows++;

									k->len = mov_size + i - zcd->current_char - 1;
									r = k;
									i = zcd->current_char + 1;
								}

								zcd->current_char++;
							}

							//ZCD_DUMP(zcd);

							zedit_caret_move(hwnd, zcd, 1);

							InvalidateRect(hwnd, NULL, TRUE);
						}

						GlobalUnlock(hglb);
					}
				}
				CloseClipboard();

				//ZCD_DUMP(zcd);
			}
			break;

		case EM_SETSEL:
			{
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

				zcd->current_char = zcd->selection_end;

				zedit_caret_move(hwnd, zcd, 1);

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

