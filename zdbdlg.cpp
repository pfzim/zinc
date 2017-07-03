//#include "stdafx.h"								//pf_ziminski  [2004]
#include "zdbdlg.h"
#include <commctrl.h>
#include <windowsx.h>
#include "utstrings.h"
#include "zmessages.h"
#include "zdbg.h"
#include <stdio.h>

// read edit controls
int ncb_read_ec(db_dialog_node *efts, HWND hwnd, void *data)
{
	char *temp_str;
	int exit_code;

	exit_code = 0;

	temp_str = trim(AllocGetDlgItemText(hwnd, (int) efts->item));

	// числовое значение
	if((efts->flags & FLD_NUMBER) && (isinteger(killspaces(temp_str)) == -1))
	{
		free_str(temp_str);

		MessageBox(hwnd, "Неправильное числовое значение!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
		SetFocus(GetDlgItem(hwnd, (int) efts->item));
		return 1;
	}

	// значение с плавающей точкой
	else if((efts->flags & FLD_FLOAT) && (isfloat(killspaces(temp_str), 1) == -1))
	{
		free_str(temp_str);

		MessageBox(hwnd, "Неправильное значение с плавающей точкой!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
		SetFocus(GetDlgItem(hwnd, (int) efts->item));
		return 1;
	}

	// поле пустое и быть им не может?
	if(isempty(temp_str) && (~efts->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не заполнено обязательное поле!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->flags & FLD_CHANGED)
	{
		if(isempty(temp_str) && (efts->flags & (FLD_FLOAT | FLD_NUMBER)))
		{
			sql_query_add((db_list_node **) data, efts->db_col, "0", SG_RAWVAL);
		}
		else
		{
			sql_query_add((db_list_node **) data, efts->db_col, temp_str, SG_STRING);
		}
	}

	free_str(temp_str);

	return exit_code;
}

// read combo box controls
int ncb_read_cc(db_dialog_node *efts, HWND hwnd, void *data)
{
	int exit_code;
	int cur_sel;

	exit_code = 0;

	cur_sel = SendDlgItemMessage(hwnd, (int) efts->item, CB_GETCURSEL, 0, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->flags & FLD_CANEMPTY))
	if(((cur_sel == CB_ERR) || (cur_sel == 0)) && (~efts->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение из выпадающего списка!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->flags & FLD_CHANGED)
	{
		//sql_query_add((db_list_node **) data, efts->db_col, (char *) ((cur_sel != CB_ERR)?SendDlgItemMessage(hwnd, (int) efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
		sql_query_add((db_list_node **) data, efts->db_col, (char *) (((cur_sel != CB_ERR) && (cur_sel != 0))?SendDlgItemMessage(hwnd, (int) efts->item, CB_GETITEMDATA, cur_sel, 0L):0), (efts->flags & FLD_STRING)?SG_STRING:SG_NUMBER);
	}

	return exit_code;
}

// read check box controls: return string "1" if checked, otherwise "0"
int ncb_read_cb(db_dialog_node *efts, HWND hwnd, void *data)
{
	// поле было отредактировано?
	if(efts->flags & FLD_CHANGED)
	{
		sql_query_add((db_list_node **) data, efts->db_col, (SendDlgItemMessage(hwnd, (int) efts->item, BM_GETCHECK, 0, 0L) == BST_CHECKED)?"1":"0", SG_STRING);
	}

	return 0;
}

// read tree view controls
int ncb_read_tc(db_dialog_node *efts, HWND hwnd, void *data)
{
	int exit_code;
	long cur_sel;

	exit_code = 0;

	cur_sel = SendDlgItemMessage(hwnd, (int) efts->item, TVM_GETNEXTITEM, TVGN_CARET, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->flags & FLD_CANEMPTY))
	if((cur_sel == 0) && (~efts->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(efts->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение в дереве!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) efts->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(efts->flags & FLD_CHANGED)
	{
		//sql_query_add((db_list_node **) data, efts->db_col, (char *) ((cur_sel != CB_ERR)?SendDlgItemMessage(hwnd, (int) efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
		if(cur_sel != 0)
		{
			TVITEM tvItem;
			
			tvItem.mask = TVIF_HANDLE | TVIF_PARAM;
			tvItem.hItem = (HTREEITEM) cur_sel;
			SendDlgItemMessage(hwnd, (int) efts->item, TVM_GETITEM, 0, (LPARAM) &tvItem);

			cur_sel = tvItem.lParam;
		}

		sql_query_add((db_list_node **) data, efts->db_col, (char *) cur_sel, (efts->flags & FLD_STRING)?SG_STRING:SG_NUMBER);
	}

	return exit_code;
}

// read date controls
int ncb_read_dc(db_dialog_node *efts, HWND hwnd, void *data)
{
	char *temp_str;
	int exit_code;
	int failed;
	SYSTEMTIME date;

	exit_code = 0;

	failed = SendDlgItemMessage(hwnd, (int) efts->item, DTM_GETSYSTEMTIME, 0, (LPARAM) &date);

	if((failed != GDT_VALID) && (~efts->flags & FLD_CANEMPTY))
	{
		if(efts->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не заполнено обязательное поле с датой!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) efts->item));
			exit_code = 1;
		}
	}
	else if(efts->flags & FLD_CHANGED)
	{
		if(failed == GDT_VALID)
		{
			temp_str = alloc_string_ex("%.4d-%.2d-%.2d", date.wYear, date.wMonth, date.wDay);
			sql_query_add((db_list_node **) data, efts->db_col, temp_str, SG_STRING);
			free_str(temp_str);
		}
		else
		{
			sql_query_add((db_list_node **) data, efts->db_col, NULL, SG_NULL);
		}
	}

	return exit_code;
}


int cb_change_dc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_NOTIFY)
		&& (((LPNMHDR)lParam)->code == DTN_DATETIMECHANGE)
		&& (((LPNMHDR)lParam)->hwndFrom == GetDlgItem(hwnd, (int) dn->item))
	)
	{
		return 0;
	}

	return -1;
}

int cb_change_tc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_NOTIFY)
		&& (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
		&& (((LPNMHDR)lParam)->hwndFrom == GetDlgItem(hwnd, (int) dn->item))
	)
	{
		return 0;
	}

	return -1;
}

int cb_change_ec(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND)
		&& (HIWORD(wParam) == EN_CHANGE)
		&& (LOWORD(wParam) == (int) dn->item)
	)
	{
		return 0;
	}

	return -1;
}

int cb_change_cc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND)
		&& (HIWORD(wParam) == CBN_SELCHANGE)
		&& (LOWORD(wParam) == (int) dn->item)
	)
	{
		return 0;
	}

	return -1;
}

void cb_print_dc(db_dialog_node *dn, HWND hwnd, char *value)
{
	SYSTEMTIME sdate;

	if(isempty(value))
	{
		SendDlgItemMessage(hwnd, (int) dn->item, DTM_SETSYSTEMTIME, GDT_NONE, (LPARAM) 0);
	}
	else
	{
		memset(&sdate, 0, sizeof(SYSTEMTIME));
		sdate.wYear = (short) _ipa(value, 0, "-");
		sdate.wMonth = (short) _ipa(value, 1, "-");
		sdate.wDay = (short) _ipa(value, 2, "-");
		SendDlgItemMessage(hwnd, (int) dn->item, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM) &sdate);
	}
}

void cb_print_cc(db_dialog_node *dn, HWND hwnd, char *value)
{
	int j, k;
	unsigned long data;

	k = SendDlgItemMessage(hwnd, dn->item, CB_GETCOUNT, 0, 0L);
	data = strtoul(value, NULL, 10);

	for(j = 0; j < k; j++)
	{
		if((unsigned long) SendDlgItemMessage(hwnd, dn->item, CB_GETITEMDATA, j, 0L) == data)
		{
			SendDlgItemMessage(hwnd, dn->item, CB_SETCURSEL, j, 0L);
			break;
		}
	}

	// old style - select by position
	//SendDlgItemMessage(hwnd, dn->item, CB_SETCURSEL, strtoul(value, NULL, 10), 0L);
}

void cb_print_ec(db_dialog_node *dn, HWND hwnd, char *value)
{
	char *temp_str;

	if(dn->flags & (FLD_NUMBER | FLD_FLOAT))
	{
		temp_str = format_number(value);
		SetDlgItemText(hwnd, dn->item, temp_str);
		free_str(temp_str);
	}
	else
	{
		SetDlgItemText(hwnd, dn->item, value);
	}
}

// здесь нестыковка: cb_print_data поддерживает только цифры, а cb_read_data поддерживает и строки
void cb_print_data(db_dialog_node *dn, HWND hwnd, char *value)
{
	dn->data = (void *) strtoul(value, NULL, 10);
}

int cb_read_data(db_dialog_node *dn, HWND hwnd, void *data)
{
	if(!dn->data && (~dn->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(dn->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) dn->item));
			return 1;
		}
	}

	if(dn->flags & FLD_CHANGED)
	{
		if(dn->flags & (FLD_NUMBER | FLD_FLOAT))
		{
			sql_query_add((db_list_node **) data, dn->db_col, (char *) dn->data, SG_NUMBER);
		}
		else //if(dn->flags & FLD_STRING)
		{
			sql_query_add((db_list_node **) data, dn->db_col, (char *) dn->data, SG_STRING);
		}
/*
#ifdef _DEBUG
		else
		{
			MessageBox(hwnd, "Ошибка в коде программы: Не указан тип данных!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}
#endif
*/
	}

	return 0;
}

void cb_print_ptr(db_dialog_node *dn, HWND hwnd, char *value)
{
	if(dn->flags & (FLD_NUMBER | FLD_FLOAT))
	{
		*(void **) dn->data = (void *) strtoul(value, NULL, 10);
	}
	else //if(dn->flags & FLD_STRING)
	{
		ZTRACE("cb_print_ptr: %s\n", value);
		*(void **) dn->data = (void *) alloc_string(value);
	}
}

int cb_read_ptr(db_dialog_node *dn, HWND hwnd, void *data)
{
	if(!dn->data && (~dn->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(dn->flags & FLD_REQUIRE)
		{
			MessageBox(hwnd, "Не выбрано значение!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			SetFocus(GetDlgItem(hwnd, (int) dn->item));
			return 1;
		}
	}

	if(dn->flags & FLD_CHANGED)
	{
		if(dn->flags & (FLD_NUMBER | FLD_FLOAT))
		{
			sql_query_add((db_list_node **) data, dn->db_col, (char *) (*(void **) dn->data), SG_NUMBER);
		}
		else //if(dn->flags & FLD_STRING)
		{
			sql_query_add((db_list_node **) data, dn->db_col, (char *) (*(void **) dn->data), SG_STRING);
		}
/*
#ifdef _DEBUG
		else
		{
			MessageBox(hwnd, "Ошибка в коде программы: Не указан тип данных!", "Ошибка!", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}
#endif
*/
	}

	return 0;
}


int sql_dlg_change_control(db_dialog *dbd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	unsigned long j;

	j = 0;
	while(j < dbd->fields_count)
	{
		if(dbd->fields[j]->cb_change && !dbd->fields[j]->cb_change(dbd->fields[j], dbd->hwnd, uMsg, wParam, lParam))
		{
			dbd->fields[j]->flags |= FLD_CHANGED;
			return 0;
		}

		j++;
	}

	return -1;
}

int sql_dlg_create(db_dialog **dbd, HWND hwnd, char *name, void *user_data)
{
	*dbd = (db_dialog *) zalloc(sizeof(db_dialog));
	if(*dbd)
	{
		(*dbd)->flags = 0;
		(*dbd)->hwnd = hwnd;
		(*dbd)->name = alloc_string(name);
		//(*dbd)->key = alloc_string(key);
		(*dbd)->data = user_data;
		(*dbd)->fields_count = 0;
		return 0;
	}

	return -1;
}

void sql_dlg_free(db_dialog **db_tbl)
{
	if(*db_tbl)
	{
		while((*db_tbl)->fields_count)
		{
			(*db_tbl)->fields_count--;
			free_str((*db_tbl)->fields[(*db_tbl)->fields_count]->db_col);
			//free_str((*db_tbl)->fields[(*db_tbl)->fields_count]->db_cond);
			free_str((*db_tbl)->fields[(*db_tbl)->fields_count]->db_join_cond1);
			free_str((*db_tbl)->fields[(*db_tbl)->fields_count]->db_join_cond2);
			free_str((*db_tbl)->fields[(*db_tbl)->fields_count]->db_join_table);
			if((*db_tbl)->fields[(*db_tbl)->fields_count]->cb_free)
			{
				(*db_tbl)->fields[(*db_tbl)->fields_count]->cb_free((*db_tbl)->fields[(*db_tbl)->fields_count]->data);
			}
			zfree((*db_tbl)->fields[(*db_tbl)->fields_count]);
		}

		free_str((*db_tbl)->name);
		//free_str((*db_tbl)->key);
		//free_str((*db_tbl)->data);
		zfree(*db_tbl);

		*db_tbl = NULL;
	}
}

unsigned long sql_dlg_field_add(db_dialog *db_tbl, unsigned long flags, int item, char *db_col, char *db_join_table, char *db_join_cond1, char *db_join_cond2, void (*cb_write)(struct _db_dialog_node *, HWND, char *), int (*cb_change)(struct _db_dialog_node *, HWND, UINT, WPARAM, LPARAM), int (*cb_read)(struct _db_dialog_node *, HWND, void *), void (*cb_free)(void *), void *data)
{
	if(db_tbl->fields_count < 64)
	{
		db_tbl->fields[db_tbl->fields_count] = (db_dialog_node *) zalloc(sizeof(db_dialog_node));
		if(db_tbl->fields[db_tbl->fields_count])
		{
			db_tbl->fields[db_tbl->fields_count]->flags = flags;
			db_tbl->fields[db_tbl->fields_count]->db_col = alloc_string(db_col);
			db_tbl->fields[db_tbl->fields_count]->item = item;
			db_tbl->fields[db_tbl->fields_count]->db_join_table = alloc_string(db_join_table);
			db_tbl->fields[db_tbl->fields_count]->db_join_cond1 = alloc_string(db_join_cond1);
			db_tbl->fields[db_tbl->fields_count]->db_join_cond2 = alloc_string(db_join_cond2);
			db_tbl->fields[db_tbl->fields_count]->cb_change = cb_change;
			db_tbl->fields[db_tbl->fields_count]->cb_write = cb_write;
			db_tbl->fields[db_tbl->fields_count]->cb_read = cb_read;
			db_tbl->fields[db_tbl->fields_count]->cb_free = cb_free;
			db_tbl->fields[db_tbl->fields_count]->data = data;
			//db_tbl->fields_count++;

			return db_tbl->fields_count++; //идентификатор первой записи совпадает с кодом ошибки 0
		}
	}

	return 0;
}

char *sql_dlg_select_query(db_dialog *db_tbl, char *db_col, void *value, unsigned long flags)
{
	unsigned long j;
	unsigned long i;
	char *temp_str;
	char *temp_slct;
	char *temp_from;
	char *temp_whre;

	j = 0;
	temp_str = nullstring;
	temp_slct = alloc_string("SELECT ");
	temp_from = alloc_string(" FROM ");
	temp_whre = nullstring;
	alloc_strcat(&temp_from, db_tbl->name);
	alloc_strcat(&temp_from, " AS m");

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		if(!isempty(db_tbl->fields[i]->db_join_table))
		{
			temp_str = alloc_string_ex("j%d.`%s`", i, db_tbl->fields[i]->db_col);

			// add to select
			if(i)
			{
				alloc_strcat(&temp_slct, ", ");
			}
			alloc_strcat(&temp_slct, temp_str);

			j++;

			free_str(temp_str);
			temp_str = alloc_string_ex(" LEFT JOIN %s AS j%d ON j%d.`%s`=m.`%s`", db_tbl->fields[i]->db_join_table, i, i, db_tbl->fields[i]->db_join_cond1, db_tbl->fields[i]->db_join_cond2);
			alloc_strcat(&temp_from, temp_str);
			free_str(temp_str);
		}
		else
		{
			// make col name
			temp_str = alloc_string_ex("m.`%s`", db_tbl->fields[i]->db_col);

			// add to select
			if(i)
			{
				alloc_strcat(&temp_slct, ", ");
			}
			alloc_strcat(&temp_slct, temp_str);

			free_str(temp_str);
		}
	}

	alloc_strcat(&temp_slct, temp_from);
	free_str(temp_from);

	if(!isempty(db_col))
	{
		alloc_strcat(&temp_slct, " WHERE m.`");
		alloc_strcat(&temp_slct, db_col);
		alloc_strcat(&temp_slct, "`");

		if(db_tbl->flags & (SG_LIKE | SG_BEGINS | SG_CONTAINE | SG_ENDS))
		{
			alloc_strcat(&temp_slct, " LIKE ");
		}
		else if(db_tbl->flags & SG_LSS)
		{
			alloc_strcat(&temp_slct, "<");
		}
		else if(db_tbl->flags & SG_GTR)
		{
			alloc_strcat(&temp_slct, ">");
		}
		else if(db_tbl->flags & SG_LEQ)
		{
			alloc_strcat(&temp_slct, "<=");
		}
		else if(db_tbl->flags & SG_GEQ)
		{
			alloc_strcat(&temp_slct, ">=");
		}
		else if(db_tbl->flags & SG_NEQ)
		{
			alloc_strcat(&temp_slct, "<>");
		}
		else
		{
			alloc_strcat(&temp_slct, "=");
		}

		switch(flags & SG_TYPE_MASK)
		{
			case SG_STRING:
				temp_str = alloc_string((char *) value);
				alloc_dbescape(&temp_str);
				if(db_tbl->flags & SG_PASSWORD)
				{
					alloc_strcat(&temp_slct, "PASSWORD('");
				}
				else
				{
					alloc_strcat(&temp_slct, "'");
				}
				if(db_tbl->flags & SG_ENDS)
				{
					alloc_strcat(&temp_slct, "%");
				}
				alloc_strcat(&temp_slct, temp_str);
				if(db_tbl->flags & SG_BEGINS)
				{
					alloc_strcat(&temp_slct, "%");
				}
				if(db_tbl->flags & SG_PASSWORD)
				{
					alloc_strcat(&temp_slct, "')");
				}
				else
				{
					alloc_strcat(&temp_slct, "'");
				}
				free_str(temp_str);
				break;
			case SG_RAWVAL:
				alloc_strcat(&temp_slct, (char *) value);
				break;
			case SG_NUMBER:
				temp_str = alloc_string_ex("%d", (char *) value);
				alloc_strcat(&temp_slct, temp_str);
				free_str(temp_str);
				break;
			case SG_DEFAULT:
				alloc_strcat(&temp_slct, "DEFAULT");
				break;
			default:
				alloc_strcat(&temp_slct, "NULL");
		}
	}

	alloc_strcat(&temp_slct, " LIMIT 1");

	return temp_slct;
}

int sql_dlg_print(db_dialog *db_tbl, odbc_result *res)
{
	unsigned long i;
	unsigned long j;

	if(!res || (res->num_rows <= 0) || (res->num_cols < (short) db_tbl->fields_count))
	{
		return -1;
	}

	for(j = 0; j < res->num_rows; j++)
	{
		i = 0;
		while(i < db_tbl->fields_count)
		{
			if(db_tbl->fields[i]->cb_write)
			{
				db_tbl->fields[i]->cb_write(db_tbl->fields[i], db_tbl->hwnd, res->rows[j]->data + res->rows[j]->offsets[i]);
			}

			i++;
		}
	}

	return 0;
}

int sql_dlg_update_query(db_dialog *db_tbl, void *data)
{
	unsigned long i;

	i = 0;
	while(i < db_tbl->fields_count)
	{
		// validate and read control
		if(db_tbl->fields[i]->cb_read && db_tbl->fields[i]->cb_read(db_tbl->fields[i], db_tbl->hwnd, data))
		{
			// read failed
			return 1;
		}

		i++;
	}

	return 0;
}

int sql_dlg_reset_change_flags(db_dialog *db_tbl, int clear_flag)
{
	unsigned long i;

	i = 0;
	while(i < db_tbl->fields_count)
	{
		if(clear_flag)
		{
			db_tbl->fields[i]->flags &= ~FLD_CHANGED;
		}
		else
		{
			db_tbl->fields[i]->flags |= FLD_CHANGED;
		}

		i++;
	}

	return 0;
}

int sql_dlg_is_changed(db_dialog *db_tbl)
{
	unsigned long i;

	i = 0;
	while(i < db_tbl->fields_count)
	{
		if(db_tbl->fields[i]->flags & FLD_CHANGED)
		{
			return 1;
		}

		i++;
	}

	return 0;
}

/* *********************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************
********************************************************************************************************************* */

int cb_vl_by_id(void *data1, void *data2)
{
	return ((value_node *) data1)->id == (unsigned long) data2;
}

inline void vl_free_data(value_node *data)
{
	if(data->flags & DF_BYNAME)
	{
		free_str(data->name);
	}

	switch(data->flags & DT_TYPEMASK)
	{
		case DT_STRING:
		case DT_RAWSTRING:
			free_str(data->str_val);
			break;
	}
}

void cb_vl_free(void *data, void *data2)
{
	vl_free_data((value_node *) data);

	zfree(data);
}

int cb_vl_dup(void *data1, void *data2)
{
	value_node *temp_node;

	temp_node = (value_node *) zalloc(sizeof(value_node));
	if(temp_node)
	{
		memcpy(temp_node, data1, sizeof(value_node));
		if(((value_node *) data1)->flags & DF_BYNAME)
		{
			temp_node->name = alloc_string(((value_node *) data1)->name);
		}

		switch(((value_node *) data1)->flags & DT_TYPEMASK)
		{
			case DT_STRING:
			case DT_RAWSTRING:
				temp_node->str_val = alloc_string(((value_node *) data1)->str_val);
				break;
		}

		list_add((db_list_node **) data2, temp_node);

		return 0;
	}

	return 1;
}

int cb_vl_mark(void *data1, void *data2)
{
	if(data2)
	{
		((value_node *) data1)->flags |= DF_CHANGED;
	}
	else
	{
		((value_node *) data1)->flags &= ~DF_CHANGED;
	}

	return 0;
}

db_list_node *value_list_duplicate(db_list_node *data_list)
{
	db_list_node *dup_data;

	dup_data = NULL;

	list_walk(data_list, cb_vl_dup, &dup_data);

	return dup_data;
}

inline void value_node_set(value_node *node, unsigned long flags, unsigned long id, void *data)
{
	memset(node, 0, sizeof(value_node));

	node->flags = flags;
	node->id = id;

	if(data)
	{
		switch(flags & DT_TYPEMASK)
		{
			case DT_DATETIME:
				node->datetime_val.date = ((unsigned long *) data)[0];
				node->datetime_val.time = ((unsigned long *) data)[1];
				break;
			case DT_CURRENCY:
				node->i64_val = *(__int64 *) data;
				break;
			case DT_DOUBLE:
				node->dbl_val = *(double *) data;
				break;
			case DT_STRING:
			case DT_RAWSTRING:	// specific sql type
				node->str_val = alloc_string((char *) data);
				break;
			default:
				node->p_val = data;
		}
	}
}

inline value_node *value_node_new(unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = (value_node *) zalloc(sizeof(value_node));
	if(temp_node)
	{
		value_node_set(temp_node, flags, id, data);
	}

	return temp_node;
}

value_node *value_list_add(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = value_node_new(flags, id, data);
	if(temp_node)
	{
		list_add(data_list, temp_node);
	}

	return temp_node;
}

// overwrite existing node without reallocate
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = (value_node *) list_extract(list_walk(*data_list, cb_vl_by_id, (void *) id));
	if(temp_node)
	{
		vl_free_data(temp_node);
		value_node_set(temp_node, flags, id, data);
		//ZTRACE("value_list_set: %d!\n", id);
	}
	else
	{
		temp_node = value_node_new(flags, id, data);
		if(temp_node)
		{
			list_add(data_list, temp_node);
		}
	}

	return temp_node;
}

/*
// always create (alloc) new node (not overwrite if exist)
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;
	db_list_node *exist_node;

	temp_node = value_node_new(flags, id, data);
	if(temp_node)
	{
		exist_node = list_walk(*data_list, cb_vl_by_id, (void *) id);
		if(exist_node)
		{
			cb_vl_free(exist_node->data, NULL);
			exist_node->data = temp_node;
		}
		else
		{
			list_add(data_list, temp_node);
		}
	}

	return temp_node;
}
*/

int cb_vl_print_sql(void *data, char **result)
{
	char *temp_str;

	*result = nullstring;

	if(!data || (~((value_node *) data)->flags & DF_CHANGED))
	{
		return 0;
	}

	if(((value_node *) data)->flags & DT_STRING)
	{
		if(((value_node *) data)->str_val)
		{
			*result = alloc_string("'");
			temp_str = alloc_string(((value_node *) data)->str_val);
			alloc_dbescape(&temp_str);
			alloc_strcat(result, temp_str);
			free_str(temp_str);
			alloc_strcat(result, "'");
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else if(((value_node *) data)->flags & DT_LONG)
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_ltoa(((value_node *) data)->l_val, *result, 10);
		}
	}
	else if(((value_node *) data)->flags & DT_DATE)
	{
		if(((value_node *) data)->datetime_val.date)
		{
			*result = alloc_string_ex("'%.4d-%.2d-%.2d'", zyear(((value_node *) data)->datetime_val.date), zmonth(((value_node *) data)->datetime_val.date), zday(((value_node *) data)->datetime_val.date));
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else if(((value_node *) data)->flags & DT_DOUBLE)	// 8 byte
	{
		*result = (char *) zalloc(256);
		if(*result)
		{
			_snprintf(*result, 255, "%.4f", ((value_node *) data)->dbl_val);
		}
	}
	else if(((value_node *) data)->flags & DT_CURRENCY)	// 8 byte
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_i64toa(((value_node *) data)->i64_val, *result, 10);
		}
	}
	else if(((value_node *) data)->flags & DT_DATETIME)	// 8 byte
	{
		if(((value_node *) data)->datetime_val.date)
		{
			*result = alloc_string_ex("'%.4d-%.2d-%.2d %.2d:%.2d:%.2d'", zyear(((value_node *) data)->datetime_val.date), zmonth(((value_node *) data)->datetime_val.date), zday(((value_node *) data)->datetime_val.date), zhour(((value_node *) data)->datetime_val.time), zminute(((value_node *) data)->datetime_val.time), zsecond(((value_node *) data)->datetime_val.time));
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else if(((value_node *) data)->flags & DT_RAWSTRING)	// строка без кавычек, для функций типа NOW()
	{
		if(((value_node *) data)->str_val)
		{
			*result = alloc_string(((value_node *) data)->str_val);
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else // DT_ULONG
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_ultoa(((value_node *) data)->ul_val, *result, 10);
		}
	}

	return 1;
}


// for controls, that direct change data - mark data as changed *********************************************************
int cb_mark_changed(dialog_info *dlg, control_node *ctrl)
{
	int failed;

	if(ctrl->flags & FLD_CHANGED)
	{
		((value_node *) ctrl->data)->flags |= DF_CHANGED;
	}
	else
	{
		((value_node *) ctrl->data)->flags &= ~DF_CHANGED;
	}

	failed = 0;

	if((ctrl->flags & (FLD_REQUIRE | FLD_CANEMPTY)) == FLD_REQUIRE)
	{
		if(((value_node *) ctrl->data)->flags & DT_STRING)
		{
			if(isempty(((value_node *) ctrl->data)->str_val))
			{
				failed = 1;
			}
		}
		else if(((value_node *) ctrl->data)->flags & DT_LONG)
		{
			if(((value_node *) ctrl->data)->l_val == 0)
			{
				failed = 1;
			}
		}
		else if(((value_node *) ctrl->data)->flags & DT_DATE)
		{
			if(((value_node *) ctrl->data)->datetime_val.date == 0)
			{
				failed = 1;
			}
		}
		else if(((value_node *) ctrl->data)->flags & DT_DOUBLE)	// 8 byte
		{
			if(((value_node *) ctrl->data)->dbl_val == 0)
			{
				failed = 1;
			}
		}
		else if(((value_node *) ctrl->data)->flags & DT_CURRENCY)	// 8 byte
		{
			if(((value_node *) ctrl->data)->i64_val == 0)
			{
				failed = 1;
			}
		}
		else if(((value_node *) ctrl->data)->flags & DT_DATETIME)	// 8 byte
		{
			if(((value_node *) ctrl->data)->datetime_val.date == 0)
			{
				failed = 1;
			}
		}
		else // DT_ULONG
		{
			if(((value_node *) ctrl->data)->ul_val == 0)
			{
				failed = 1;
			}
		}
	}

	if(failed)
	{
		MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Не заполнено обязательное поле (%d)!", ctrl->id);
		SetFocus(GetDlgItem(dlg->hwnd, (int) ctrl->item));
	}

	return failed;
}

// edit controls ********************************************************************************************************
int cb_change_ec3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND)
		&& (HIWORD(wParam) == EN_CHANGE)
		&& (LOWORD(wParam) == (int) ctrl->item)
	)
	{
		return 0;
	}

	return -1;
}

void cb_print_ec3(dialog_info *dlg, control_node *ctrl)
{
	char *temp_str;
	char temp_buf[256];

	if(!ctrl->data)
	{
		return;
	}

	if(((value_node *) ctrl->data)->flags & DT_STRING)
	{
		SetDlgItemText(dlg->hwnd, ctrl->item, ((value_node *) ctrl->data)->str_val);
	}
	else if(((value_node *) ctrl->data)->flags & DT_LONG)
	{
		_ltoa(((value_node *) ctrl->data)->l_val, temp_buf, 10);
		temp_str = format_number(temp_buf);
		SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
		free_str(temp_str);
	}
	else if(((value_node *) ctrl->data)->flags & DT_DATE)
	{
		if(((value_node *) ctrl->data)->datetime_val.date)
		{
			temp_str = alloc_string_ex("%.2d.%.2d.%.4d", zday(((value_node *) ctrl->data)->datetime_val.date), zmonth(((value_node *) ctrl->data)->datetime_val.date), zyear(((value_node *) ctrl->data)->datetime_val.date));
			SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
			free_str(temp_str);
		}
		else
		{
			SetDlgItemText(dlg->hwnd, ctrl->item, "");
		}
	}
	else if(((value_node *) ctrl->data)->flags & DT_DATETIME)	// 8 byte
	{
		if(((value_node *) ctrl->data)->datetime_val.date)
		{
			temp_str = alloc_string_ex("%.2d.%.2d.%.4d %.2d:%.2d:%.2d", 
											zday(((value_node *) ctrl->data)->datetime_val.date),
											zmonth(((value_node *) ctrl->data)->datetime_val.date),
											zyear(((value_node *) ctrl->data)->datetime_val.date),
											zhour(((value_node *) ctrl->data)->datetime_val.time),
											zminute(((value_node *) ctrl->data)->datetime_val.time),
											zsecond(((value_node *) ctrl->data)->datetime_val.time));
			SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
			free_str(temp_str);
		}
		else
		{
			SetDlgItemText(dlg->hwnd, ctrl->item, "");
		}
	}
	else if(((value_node *) ctrl->data)->flags & DT_DOUBLE)	// 8 byte
	{
		_snprintf(temp_buf, 255, "%.4f", ((value_node *) ctrl->data)->dbl_val);
		temp_str = format_number(temp_buf);
		SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
		free_str(temp_str);
	}
	else if(((value_node *) ctrl->data)->flags & DT_CURRENCY)	// 8 byte
	{
		/*
		_i64toa(((value_node *) ctrl->data)->i64_1 / 10000, temp_buf, 10);
		temp_str = alloc_string(temp_buf);
		alloc_strcat(&temp_str, ".");
		if(((value_node *) ctrl->data)->i64_1 < 0)
		{
			_i64toa(-((value_node *) ctrl->data)->i64_1 % 10000, temp_buf, 10);
		}
		else
		{
			_i64toa(((value_node *) ctrl->data)->i64_1 % 10000, temp_buf, 10);
		}
		alloc_strcat(&temp_str, temp_buf);
		*/

		//temp_str = format_number(temp_buf);
		currencytostr(((value_node *) ctrl->data)->i64_val, temp_buf);
		SetDlgItemText(dlg->hwnd, ctrl->item, temp_buf);
		//temp_str = alloc_currency2str(((value_node *) ctrl->data)->i64_1, 1, "рубль", "рубля", "рублей", "копейка", "копейки", "копеек");
		//SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
		//free_str(temp_str);
	}
	else // DT_ULONG
	{
		_ultoa(((value_node *) ctrl->data)->ul_val, temp_buf, 10);
		temp_str = format_number(temp_buf);
		SetDlgItemText(dlg->hwnd, ctrl->item, temp_str);
		free_str(temp_str);
	}
}

int cb_read_ec3(dialog_info *dlg, control_node *ctrl)
{
	char *temp_str;
	int exit_code;

	exit_code = 0;

	((value_node *) ctrl->data)->flags &= ~DF_CHANGED;

	temp_str = trim(AllocGetDlgItemText(dlg->hwnd, (int) ctrl->item));

	// числовое значение
	if((((value_node *) ctrl->data)->flags & (DT_LONG | DT_ULONG)) && (isinteger(killspaces(temp_str)) == -1))
	{
		free_str(temp_str);

		MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Неправильное числовое значение в поле (%d)!", ctrl->id);
		SetFocus(GetDlgItem(dlg->hwnd, (int) ctrl->item));
		return 1;
	}

	// значение с плавающей точкой
	else if((((value_node *) ctrl->data)->flags & (DT_DOUBLE | DT_CURRENCY)) && (isfloat(killspaces(temp_str), 1) == -1))
	{
		free_str(temp_str);

		MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Неправильное значение с плавающей точкой в поле (%d)!", ctrl->id);
		SetFocus(GetDlgItem(dlg->hwnd, (int) ctrl->item));
		return 1;
	}

	// поле пустое и быть им не может?
	if(isempty(temp_str) && (~ctrl->flags & FLD_CANEMPTY)) // при этом уловии не удалится предыдущее значение
	{
		// обязательное поле?
		if(ctrl->flags & FLD_REQUIRE)
		{
			MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Не заполнено обязательное поле (%d)!", ctrl->id);
			SetFocus(GetDlgItem(dlg->hwnd, (int) ctrl->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(ctrl->flags & FLD_CHANGED)
	{
		((value_node *) ctrl->data)->flags |= DF_CHANGED;

		if(((value_node *) ctrl->data)->flags & DT_STRING)
		{
			free_str(((value_node *) ctrl->data)->str_val);
			((value_node *) ctrl->data)->str_val = alloc_string(temp_str);
		}
		else if(((value_node *) ctrl->data)->flags & DT_LONG)
		{
			((value_node *) ctrl->data)->l_val = strtol(temp_str, NULL, 10);
		}
		else if(((value_node *) ctrl->data)->flags & DT_DOUBLE)	// 8 byte
		{
			((value_node *) ctrl->data)->dbl_val = strtod(temp_str, NULL);
		}
		else if(((value_node *) ctrl->data)->flags & DT_CURRENCY)	// 8 byte
		{
			((value_node *) ctrl->data)->i64_val = strtocurrency(temp_str);
			//ZTRACE("!!!!!!strtocurrency: %d\n", ((value_node *) ctrl->data)->);
		}
		else if(((value_node *) ctrl->data)->flags & DT_DATETIME)	// 8 byte
		{
			((value_node *) ctrl->data)->datetime_val.date = strtodate2(temp_str);
			((value_node *) ctrl->data)->datetime_val.time = strtotime2(temp_str);
		}
		else if(((value_node *) ctrl->data)->flags & DT_DATE)
		{
			((value_node *) ctrl->data)->datetime_val.date = strtodate(temp_str);
		}
		else // DT_ULONG
		{
			((value_node *) ctrl->data)->ul_val = strtoul(temp_str, NULL, 10);
		}
	}

	free_str(temp_str);

	return exit_code;
}


// combo box controls ***************************************************************************************************
int cb_change_co3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND)
		&& (HIWORD(wParam) == CBN_SELCHANGE)
		&& (LOWORD(wParam) == (int) ctrl->item)
	)
	{
		return 0;
	}

	return -1;
}

void cb_print_co3(dialog_info *dlg, control_node *ctrl)
{
	int j, k;
	unsigned long data;

	if(!ctrl->data)
	{
		return;
	}

	if(((value_node *) ctrl->data)->flags & DT_ULONG)
	{
		data = ((value_node *) ctrl->data)->ul_val;

		k = SendDlgItemMessage(dlg->hwnd, ctrl->item, CB_GETCOUNT, 0, 0L);
		for(j = 0; j < k; j++)
		{
			if((unsigned long) SendDlgItemMessage(dlg->hwnd, ctrl->item, CB_GETITEMDATA, j, 0L) == data)
			{
				SendDlgItemMessage(dlg->hwnd, ctrl->item, CB_SETCURSEL, j, 0L);
				break;
			}
		}
	}
	else
	{
		ZTRACE("cb_print: invalid data type\n");
	}

	// old style - select by position
	//SendDlgItemMessage(hwnd, dn->item, CB_SETCURSEL, strtoul(value, NULL, 10), 0L);
}

int cb_read_co3(dialog_info *dlg, control_node *ctrl)
{
	int exit_code;
	int cur_sel;

	exit_code = 0;

	((value_node *) ctrl->data)->flags &= ~DF_CHANGED;

	cur_sel = SendDlgItemMessage(dlg->hwnd, ctrl->item, CB_GETCURSEL, 0, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->flags & FLD_CANEMPTY))
	if(((cur_sel == CB_ERR) || (cur_sel == 0)) && (~ctrl->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(ctrl->flags & FLD_REQUIRE)
		{
			MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Не выбрано значение из выпадающего списка (%d)!", ctrl->id);
			SetFocus(GetDlgItem(dlg->hwnd, ctrl->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(ctrl->flags & FLD_CHANGED)
	{
		((value_node *) ctrl->data)->flags |= DF_CHANGED;
		if(((value_node *) ctrl->data)->flags & DT_ULONG)
		{
			((value_node *) ctrl->data)->ul_val = (unsigned long) (((cur_sel != CB_ERR) && (cur_sel != 0))?SendDlgItemMessage(dlg->hwnd, ctrl->item, CB_GETITEMDATA, cur_sel, 0L):0);
		}
		else
		{
			ZTRACE("cb_read: invalid data type\n");
		}
	}

	return exit_code;
}

// check box controls ***************************************************************************************************
int cb_change_cb3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND)
		&& (HIWORD(wParam) == BN_CLICKED)
		&& (LOWORD(wParam) == (int) ctrl->item)
	)
	{
		return 0;
	}

	return -1;
}

void cb_print_cb3(dialog_info *dlg, control_node *ctrl)
{
	int j, k;
	unsigned long data;

	if(!ctrl->data)
	{
		return;
	}

	if(((value_node *) ctrl->data)->flags & DT_ULONG)
	{
		CheckDlgButton(dlg->hwnd, ctrl->item, ((value_node *) ctrl->data)->ul_val?BST_CHECKED:BST_UNCHECKED);
	}
	else
	{
		ZTRACE("cb_print: invalid data type\n");
	}

	// old style - select by position
	//SendDlgItemMessage(hwnd, dn->item, CB_SETCURSEL, strtoul(value, NULL, 10), 0L);
}

// return string "1" if checked, otherwise "0"
int cb_read_cb3(dialog_info *dlg, control_node *ctrl)
{
	((value_node *) ctrl->data)->flags &= ~DF_CHANGED;

	// поле было отредактировано?
	if(ctrl->flags & FLD_CHANGED)
	{
		((value_node *) ctrl->data)->flags |= DF_CHANGED;
		if(((value_node *) ctrl->data)->flags & DT_ULONG)
		{
			((value_node *) ctrl->data)->ul_val = (unsigned long) ((SendDlgItemMessage(dlg->hwnd, ctrl->item, BM_GETCHECK, 0, 0L) == BST_CHECKED)?1:0);
		}
		else
		{
			ZTRACE("cb_read: invalid data type\n");
		}
	}

	return 0;
}

// read tree view controls **********************************************************************************************
int cb_change_tc3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_NOTIFY)
		&& (((LPNMHDR) lParam)->code == TVN_SELCHANGED)
		&& (((LPNMHDR) lParam)->hwndFrom == GetDlgItem(dlg->hwnd, (int) ctrl->item))
		//OR && (((LPNMHDR) lParam)->idFrom == (int) ctrl->item)
	)
	{
		return 0;
	}

	return -1;
}

int cb_read_tc3(dialog_info *dlg, control_node *ctrl)
{
	int exit_code;
	long cur_sel;

	exit_code = 0;

	((value_node *) ctrl->data)->flags &= ~DF_CHANGED;

	cur_sel = SendDlgItemMessage(dlg->hwnd, ctrl->item, TVM_GETNEXTITEM, TVGN_CARET, 0L);

	// поле пустое и быть им не может?
	//if((cur_sel == CB_ERR) && (~efts->flags & FLD_CANEMPTY))
	if((cur_sel == 0) && (~ctrl->flags & FLD_CANEMPTY))
	{
		// обязательное поле?
		if(ctrl->flags & FLD_REQUIRE)
		{
			MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Не выбрано значение в дереве (%d)!", ctrl->id);
			SetFocus(GetDlgItem(dlg->hwnd, ctrl->item));
			exit_code = 1;
		}
	}
	// поле было отредактировано?
	else if(ctrl->flags & FLD_CHANGED)
	{
		//sql_query_add((db_list_node **) data, efts->db_col, (char *) ((cur_sel != CB_ERR)?SendDlgItemMessage(hwnd, (int) efts->item, CB_GETITEMDATA, cur_sel, 0L):0), efts->flags);
		if(cur_sel != 0)
		{
			TVITEM tvItem;
			
			tvItem.mask = TVIF_HANDLE | TVIF_PARAM;
			tvItem.hItem = (HTREEITEM) cur_sel;
			SendDlgItemMessage(dlg->hwnd, ctrl->item, TVM_GETITEM, 0, (LPARAM) &tvItem);

			cur_sel = tvItem.lParam;
		}

		((value_node *) ctrl->data)->flags |= DF_CHANGED;
		if(((value_node *) ctrl->data)->flags & DT_ULONG)
		{
			((value_node *) ctrl->data)->ul_val = cur_sel;
		}
		else
		{
			ZTRACE("cb_read: invalid data type\n");
		}
	}

	return exit_code;
}

// read date controls ***************************************************************************************************
int cb_change_dc3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_NOTIFY)
		&& (((LPNMHDR) lParam)->code == DTN_DATETIMECHANGE)
		&& (((LPNMHDR) lParam)->hwndFrom == GetDlgItem(dlg->hwnd, (int) ctrl->item))
		//OR && (((LPNMHDR) lParam)->idFrom == (int) ctrl->item)
	)
	{
		return 0;
	}

	return -1;
}

void cb_print_dc3(dialog_info *dlg, control_node *ctrl)
{
	SYSTEMTIME sdate;

	SendDlgItemMessage(dlg->hwnd, ctrl->item, DTM_SETSYSTEMTIME, GDT_NONE, (LPARAM) 0);

	if(!ctrl->data)
	{
		return;
	}

	if(((value_node *) ctrl->data)->flags & DT_DATE)
	{
		if(((value_node *) ctrl->data)->datetime_val.date)
		{
			memset(&sdate, 0, sizeof(SYSTEMTIME));
			sdate.wYear = (short) zyear(((value_node *) ctrl->data)->datetime_val.date);
			sdate.wMonth = (short) zmonth(((value_node *) ctrl->data)->datetime_val.date);
			sdate.wDay = (short) zday(((value_node *) ctrl->data)->datetime_val.date);
			SendDlgItemMessage(dlg->hwnd, ctrl->item, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM) &sdate);
		}
	}
	else
	{
		ZTRACE("cb_print: invalid data type\n");
	}
}

int cb_read_dc3(dialog_info *dlg, control_node *ctrl)
{
	int exit_code;
	int failed;
	SYSTEMTIME date;

	exit_code = 0;

	((value_node *) ctrl->data)->flags &= ~DF_CHANGED;

	failed = SendDlgItemMessage(dlg->hwnd, ctrl->item, DTM_GETSYSTEMTIME, 0, (LPARAM) &date);

	if((failed != GDT_VALID) && (~ctrl->flags & FLD_CANEMPTY))
	{
		if(ctrl->flags & FLD_REQUIRE)
		{
			MessageBoxFmt(dlg->hwnd, "Ошибка!", MB_OK | MB_ICONEXCLAMATION, "Не заполнено обязательное поле с датой (%d)!", ctrl->id);
			SetFocus(GetDlgItem(dlg->hwnd, ctrl->item));
			exit_code = 1;
		}
	}
	else if(ctrl->flags & FLD_CHANGED)
	{
		((value_node *) ctrl->data)->flags |= DF_CHANGED;
		if(((value_node *) ctrl->data)->flags & DT_DATE)
		{
			if(failed == GDT_VALID)
			{
				((value_node *) ctrl->data)->datetime_val.date = zdate(date.wDay, date.wMonth, date.wYear);
			}
			else
			{
				((value_node *) ctrl->data)->datetime_val.date = 0;
			}
		}
		else
		{
			ZTRACE("cb_print: invalid data type\n");
		}
	}

	return exit_code;
}

//  *********************************************************************************************************************





int dialog_info_create(dialog_info **dbd, HWND hwnd, db_list_node **fill_data)
{
	*dbd = (dialog_info *) zalloc(sizeof(dialog_info));
	if(*dbd)
	{
		(*dbd)->flags = 0;
		(*dbd)->hwnd = hwnd;
		(*dbd)->saved_data = fill_data;
		(*dbd)->actual_data = value_list_duplicate(*fill_data);
		(*dbd)->fields_count = 0;

		return 0;
	}

	return -1;
}

db_list_node *dialog_info_free(dialog_info **db_tbl)
{
	db_list_node *actual_data;

	if(*db_tbl)
	{
		while((*db_tbl)->fields_count)
		{
			(*db_tbl)->fields_count--;
			/*
			if((*db_tbl)->fields[(*db_tbl)->fields_count]->cb_free)
			{
				(*db_tbl)->fields[(*db_tbl)->fields_count]->cb_free((*db_tbl)->fields[(*db_tbl)->fields_count]->data);
			}
			*/
			zfree((*db_tbl)->fields[(*db_tbl)->fields_count]);
		}

		list_free(&(*db_tbl)->actual_data, cb_vl_free, NULL);

		actual_data = *(*db_tbl)->saved_data;		// return last saved data
		
		zfree(*db_tbl);

		*db_tbl = NULL;

		return actual_data;
	}

	return NULL;
}

unsigned long dialog_control_assign(dialog_info *db_tbl, unsigned long flags, unsigned long id, int item, int (*cb_change)(struct _dialog_info *, struct _control_node *, UINT, WPARAM, LPARAM), void (*cb_write)(struct _dialog_info *, struct _control_node *), int (*cb_read)(struct _dialog_info *, struct _control_node *))
{
	if(db_tbl->fields_count < 64)
	{
		db_tbl->fields[db_tbl->fields_count] = (control_node *) zalloc(sizeof(control_node));
		if(db_tbl->fields[db_tbl->fields_count])
		{
			db_tbl->fields[db_tbl->fields_count]->flags = flags;
			db_tbl->fields[db_tbl->fields_count]->id = id;
			db_tbl->fields[db_tbl->fields_count]->item = item;
			db_tbl->fields[db_tbl->fields_count]->cb_change = cb_change;
			db_tbl->fields[db_tbl->fields_count]->cb_write = cb_write;
			db_tbl->fields[db_tbl->fields_count]->cb_read = cb_read;
			if(id)
			{
				db_tbl->fields[db_tbl->fields_count]->data = list_extract(list_walk(db_tbl->actual_data, cb_vl_by_id, (void *) id));
			}
			else
			{
				db_tbl->fields[db_tbl->fields_count]->data = NULL;
			}
			//db_tbl->fields[db_tbl->fields_count]->cb_free = cb_free;

			return db_tbl->fields_count++; //идентификатор первой записи совпадает с кодом ошибки 0
		}
	}

	return 0;
}

void dialog_control_mark_changed(dialog_info *db_tbl, int fset)
{
	unsigned long i;

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		if(fset)
		{
			db_tbl->fields[i]->flags |= FLD_CHANGED;
		}
		else
		{
			db_tbl->fields[i]->flags &= ~FLD_CHANGED;
		}
	}
}

int dialog_control_change(dialog_info *dbd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	unsigned long i;

	for(i = 0; i < dbd->fields_count; i++)
	{
		if(dbd->fields[i]->cb_change && !dbd->fields[i]->cb_change(dbd, dbd->fields[i], uMsg, wParam, lParam))
		{
			dbd->fields[i]->flags |= FLD_CHANGED;
			return 0;
		}
	}

	return -1;
}

int dialog_control_print(dialog_info *db_tbl)
{
	unsigned long i;

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		if(db_tbl->fields[i]->cb_write)
		{
			db_tbl->fields[i]->cb_write(db_tbl, db_tbl->fields[i]);
		}
	}

	return 0;
}

int dialog_control_read(dialog_info *db_tbl)
{
	unsigned long i;

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		// validate and read control
		if(db_tbl->fields[i]->cb_read && db_tbl->fields[i]->cb_read(db_tbl, db_tbl->fields[i]))
		{
			// read failed
			return 1;
		}
	}

	return 0;
}

control_node *dialog_control_find(dialog_info *db_tbl, unsigned long id)
{
	unsigned long i;

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		if(db_tbl->fields[i]->id == id)
		{
			return db_tbl->fields[i];
		}
	}

	return NULL;
}

control_node *dialog_control_data_set(dialog_info *db_tbl, unsigned long id, unsigned long flags, void *data)
{
	control_node *temp_node;

	temp_node = dialog_control_find(db_tbl, id);

	if(temp_node)
	{
		temp_node->flags |= FLD_CHANGED;
		temp_node->data = value_list_set(&db_tbl->actual_data, flags, id, data);
		if(temp_node->cb_write)
		{
			temp_node->cb_write(db_tbl, temp_node);
		}
	}

	return temp_node;
}

control_node *dialog_control_print1(dialog_info *db_tbl, unsigned long id)
{
	control_node *temp_node;

	temp_node = dialog_control_find(db_tbl, id);

	if(temp_node)
	{
		if(temp_node->cb_write)
		{
			temp_node->cb_write(db_tbl, temp_node);
		}
	}

	return temp_node;
}

int dialog_control_undo(dialog_info *db_tbl)
{
	unsigned long i;

	list_free(&db_tbl->actual_data, cb_vl_free, NULL);
	db_tbl->actual_data = value_list_duplicate(*db_tbl->saved_data);

	for(i = 0; i < db_tbl->fields_count; i++)
	{
		db_tbl->fields[i]->data = list_extract(list_walk(db_tbl->actual_data, cb_vl_by_id, (void *) db_tbl->fields[i]->id));
		if(db_tbl->fields[i]->cb_write)
		{
			db_tbl->fields[i]->cb_write(db_tbl, db_tbl->fields[i]);
		}
		db_tbl->fields[i]->flags &= ~FLD_CHANGED;
	}
	
	//db_tbl->flags &= ~DLG_CHANGED; // надо ли это делать здесь?

	return 0;
}

int dialog_control_save(dialog_info *db_tbl)
{
	list_free(db_tbl->saved_data, cb_vl_free, NULL);
	*db_tbl->saved_data = value_list_duplicate(db_tbl->actual_data);
	dialog_control_mark_changed(db_tbl, 0);
	//db_tbl->undo_data[для всех]->flags &= ~DF_CHANGED;	// реализовано в cb_read_*

	//db_tbl->flags &= ~DLG_CHANGED; // надо ли это делать здесь?
	
	return 0;
}

