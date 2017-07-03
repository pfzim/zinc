#ifndef _ZCONTROLS_H_
#define _ZCONTROLS_H_

#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include "zlist.h"

#define BTN_MOUSEOVER	0x0001
#define BTN_PUSHED		0x0002
#define BTN_EMPTY		0x0004


#define ZC_CAPTION_SIZE		255

#define ZC_KEY_STATES		(ZC_SHIFT_DOWN | ZC_CTRL_DOWN | ZC_ALT_DOWN)
#define ZC_MASK_STATES		0xFF000000

#define ZC_MOUSE_OVER		0x01000000	// указатель находится над контролом
#define ZC_MOUSE_DOWN		0x02000000	// нажата левая кнопка мыши
#define ZC_PUSHED			0x04000000	// кнопка нажата (нажат пробел)
#define ZC_ACTIVE			0x08000000	// фокус в окне
//#define ZC_SWAP_END			0x08000000	// за какой конец тянем мышкой (избавиться! можно определить по положению cur_char)
#define ZC_CARET_VISIBLE	0x10000000
#define ZC_SHIFT_DOWN		0x20000000	// нажат shift
#define ZC_CTRL_DOWN		0x40000000	// нажат ctrl
#define ZC_ALT_DOWN			0x80000000	// нажат alt

#define ZC_MASK_STYLES		0x00FFFFFF

#define ZC_TEXT_CENTER		0x00000001	//08
#define ZC_TEXT_RIGHT		0x00000002	//10
#define ZC_IMG_CENTER		0x00000004	//20
#define ZC_IMG_RIGHT		0x00000008	//40
#define ZC_DEFAULT			0x00000010	// def_push_button
#define ZC_PASSWORD			0x00000020
#define ZC_MULTILINE		0x00000040
#define ZC_WANTRETURN		0x00000080

#define FLD_STRING			0x00000000	// default?!?!
#define FLD_CHANGED			0x00000001
#define FLD_REQUIRE			0x00000002	// игнорируется, если установлено флаг FLD_CANEMPTY
#define FLD_CANEMPTY		0x00000004	// поле может быть пустым (в sql запрос добавляется name = '')
#define FLD_FLOAT			0x00000008
#define FLD_NUMBER			0x00000010

// Логика FLD_CANEMPTY и FLD_REQUIRE:
// - Если флаги не установлены, тогда поле считается не обязательным. Т.е. оно игнорируется, если не заполнено.
// - Если установлен флаг FLD_CANEMPTY, то поле заносится в БД даже если оно пустое.
// - Если установлен флаг FLD_REQUIRE, то выскакивает сообщение об ошибке, если оно пустое.

// Чем страшен флаг FLD_CANEMPTY:
// - Если ранее было введено какое-то значение и в последующем стёрто, то данное значение не 
// сотрётся в БД, т.к. поле будет проигнорировано.

//typedef int (*CCBF)(HWND, int);

typedef struct _dlg_fields_list
{
	char *name;				// field name in DB
	int item;				// control id
	unsigned long flags;
	unsigned long changed;
	int (*cb_read)(HWND, struct _dlg_fields_list *, void *);
} dlg_fields_list;

#define ZC_LEFT					0x00000001
#define ZC_TOP					0x00000002
#define ZC_RIGHT				0x00000004
#define ZC_BOTTOM				0x00000008

typedef struct _wnd_control_node
{
	int id;
	HWND hwnd;
	unsigned long flags;
	int cx1;
	int cy1;
	int cx2;
	int cy2;
} wnd_control_node;


void FlatButtonSetColors(COLORREF crLight, COLORREF crShadow, COLORREF crText, COLORREF crTextDisabled, COLORREF crHighLight, COLORREF crBackground);
COLORREF FlatButtonGetBackgroundColor();
COLORREF FlatButtonGetTextColor();
COLORREF FlatButtonGetTextDisabledColor();
void FlatButtonCreate(HWND hwnd);
void FlatButtonMake(HWND hwnd);
void FlatButtonDraw(LPDRAWITEMSTRUCT lpDrwItemStruct);

void FlatEditSetColors(COLORREF crLight, COLORREF crShadow);
void FlatEditCreate(HWND hwnd);
void FlatEditMake(HWND hwnd);

void FlatStaticSetColors(COLORREF crLight, COLORREF crShadow);
void FlatStaticCreate(HWND hwnd);
void FlatStaticMake(HWND hwnd);

void FlatTabSetColors(COLORREF crLight, COLORREF crShadow, COLORREF crText, COLORREF crHighLight, COLORREF crBackground);
void FlatTabCreate(HWND hwnd);

void RegisterMyControlClasses();

int read_dialog_controls(HWND hwnd, dlg_fields_list *efts, void *data);
int set_change_flag(dlg_fields_list *efts, int item);

// set or clear change flags for all items
int reset_change_flags(dlg_fields_list *efts, int clear_flag);

// edit control
int cb_read_ec(HWND hwnd, dlg_fields_list *efts, void *data);
// combobox control
int cb_read_cc(HWND hwnd, dlg_fields_list *efts, void *data);
// checkbox control
int cb_read_cb(HWND hwnd, dlg_fields_list *efts, void *data);
// treeview control
int cb_read_tc(HWND hwnd, dlg_fields_list *efts, void *data);
// data control
int cb_read_dc(HWND hwnd, dlg_fields_list *efts, void *data);

long SubClassTreeViewEdit(HWND hwnd);

LRESULT CALLBACK EditProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND zc_create(db_list_node **cl, unsigned long flags, RECT *cr, DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int cx1, int cy1, int cx2, int cy2, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
int cb_zc_move(void *data1, void *data2);
db_list_node *zc_reposition(db_list_node *cl, RECT *cr);
void cb_zc_free(void *data);
int zc_free(db_list_node **cl);

#endif //_ZCONTROLS_H_
