#ifndef _ZDBDLG_H_
#define _ZDBDLG_H_

// database dialogs

#pragma comment(lib, "comctl32.lib")

#include "zcontrols.h"
#include "zodbc.h"

#define DLG_CHANGED				0x00000001

#define DT_TYPEMASK				0x00FFF000
#define DT_STRING				0x00001000	// default
#define DT_LONG					0x00002000
#define DT_ULONG				0x00004000	// TODO: сделать этот тит по умолчанию!
#define DT_DOUBLE				0x00008000
#define DT_CURRENCY				0x00010000
#define DT_DATE					0x00020000
#define DT_DATETIME				0x00040000
#define DT_DATA					0x00080000
#define DT_RAWSTRING			0x00100000
#define DT_USERDATA				0x00200000
#define DT_RESERVED1			0x00400000
#define DT_RESERVED2			0x00800000

#define DF_FILLED				0x01000000	// флаг указывает на заполненность поля
#define DF_CHANGED				0x02000000	// флаг указывает, что данные были изменены
#define DF_BYNAME				0x04000000	// в качестве идентификатора используется name, вместо id
#define DF_USER1				0x08000000
#define DF_USER2				0x10000000
#define DF_USER3				0x20000000
#define DF_USER4				0x40000000
#define DF_USER5				0x80000000

//#define DT_NEWTYPE			0x00000000
//#define FLD_THOUSANDS			0x00000001


typedef struct _db_dialog_node
{
	unsigned long flags;
	int item;
	char *db_col;
	//char *db_cond;
	char *db_join_table;
	char *db_join_cond1;
	char *db_join_cond2;
	int (*cb_change)(struct _db_dialog_node *, HWND, UINT, WPARAM, LPARAM);
	void (*cb_write)(struct _db_dialog_node *, HWND, char *);
	int (*cb_read)(struct _db_dialog_node *, HWND, void *);
	void (*cb_free)(void *);
	void *data;
} db_dialog_node;

typedef struct _db_dialog
{
	unsigned long flags;
	HWND hwnd;
	char *name;
	//char *key;
	void *data;
	unsigned long fields_count;
	db_dialog_node *fields[64];
} db_dialog;

typedef struct _value_node
{
	union
	{
		unsigned long id;
		char *name;
	};
	unsigned long flags;		// data type like (DT_NUMBER, DT_FLOAT)
	union
	{
		void *p_val;			// pointer to any data (how to free? by type in flags?)
		char *str_val;			// pointer to string
		unsigned long ul_val;	// unsigned long
		long l_val;				// long
		double dbl_val;			// double
		__int64 i64_val;		// int64
		struct _data_node
		{
			void *ptr;			// pointer to any data
			unsigned long size;	// size of data
		} data_val;				// pointer to any data with size
		struct _date_time_value
		{
			unsigned long date;	// date zdate()
			unsigned long time;	// time ztime()
		} datetime_val;			// datatime
		struct _64bit_value
		{
			unsigned long hi32;
			unsigned long lo32;
		} val64;				// 64bit value as two unsigned long values
	};
} value_node;

typedef struct _control_node
{
	unsigned long id;
	unsigned long flags;	// like (FLD_REQUIRE, FLD_CANEMPTY)
	int item;
	int (*cb_change)(struct _dialog_info *, struct _control_node *, UINT, WPARAM, LPARAM);
	void (*cb_write)(struct _dialog_info *, struct _control_node *);
	int (*cb_read)(struct _dialog_info *, struct _control_node *);
	//void (*cb_free)(struct _dialog_info *, struct _control_node *); Здесь он не нужен, т.к. с данными
	// работают функции созданные пользователем. Соответсвенно известно как очищать данные. Требовалась в
	// предыдущей версии для очистки user_data
	void *data;		// pointer to this control actual_data in list
} control_node;

typedef struct _dialog_info
{
	unsigned long flags;
	HWND hwnd;
	unsigned long fields_count;
	control_node *fields[64];
	db_list_node *actual_data;				// list of control's data (initially == saved_date);
	db_list_node **saved_data;				// list of control's data
} dialog_info;

typedef struct _db_parsed_table
{
	unsigned long flags[256];		// тип данных и другие флаги
	unsigned char data_map[256];	// id колонок
	unsigned long cols;				// кол-во колонок
	unsigned long rows;				// кол-во строк
	char ***data;					// матрица ссылок на данные
} db_parsed_table;


int ncb_read_ec(db_dialog_node *efts, HWND hwnd, void *data);	// edit control
int ncb_read_cc(db_dialog_node *efts, HWND hwnd, void *data);	// combobox
int ncb_read_cb(db_dialog_node *efts, HWND hwnd, void *data);	// chekbox
int ncb_read_tc(db_dialog_node *efts, HWND hwnd, void *data);	// tree view
int ncb_read_dc(db_dialog_node *efts, HWND hwnd, void *data);	// date picker
int cb_change_dc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int cb_change_tc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int cb_change_ec(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int cb_change_cc(db_dialog_node *dn, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void cb_print_cc(db_dialog_node *dn, HWND hwnd, char *value);
void cb_print_dc(db_dialog_node *dn, HWND hwnd, char *value);
void cb_print_ec(db_dialog_node *dn, HWND hwnd, char *value);
void cb_print_data(db_dialog_node *dn, HWND hwnd, char *value);
int cb_read_data(db_dialog_node *dn, HWND hwnd, void *data);
void cb_print_ptr(db_dialog_node *dn, HWND hwnd, char *value);
int cb_read_ptr(db_dialog_node *dn, HWND hwnd, void *data);

int sql_dlg_change_control(db_dialog *dbd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int sql_dlg_create(db_dialog **dbd, HWND hwnd, char *name, void *user_data);
void sql_dlg_free(db_dialog **db_tbl);
unsigned long sql_dlg_field_add(db_dialog *db_tbl, unsigned long flags, int item, char *db_col, char *db_join_table, char *db_join_cond1, char *db_join_cond2, void (*cb_write)(struct _db_dialog_node *, HWND, char *), int (*cb_change)(struct _db_dialog_node *, HWND, UINT, WPARAM, LPARAM), int (*cb_read)(struct _db_dialog_node *, HWND, void *), void (*cb_free)(void *), void *data);
char *sql_dlg_select_query(db_dialog *db_tbl, char *db_col, void *value, unsigned long flags);
int sql_dlg_print(db_dialog *db_tbl, odbc_result *res);
int sql_dlg_update_query(db_dialog *db_tbl, void *data);
int sql_dlg_reset_change_flags(db_dialog *db_tbl, int clear_flag);
int sql_dlg_is_changed(db_dialog *db_tbl);


int cb_vl_by_id(void *data1, void *data2);
void cb_vl_free(void *data, void *data2);
int cb_vl_print_sql(void *data, char **result);
int cb_vl_dup(void *data1, void *data2);
int cb_vl_mark(void *data1, void *data2);		// 1=mark, 0=unmark with flag DF_CHANGED

db_list_node *value_list_duplicate(db_list_node *data_list);
value_node *value_list_add(db_list_node **data_list, unsigned long flags, unsigned long id, void *data);
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data);


int cb_mark_changed(dialog_info *dlg, control_node *ctrl);

int cb_change_ec3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam);
void cb_print_ec3(dialog_info *dlg, control_node *ctrl);
int cb_read_ec3(dialog_info *dlg, control_node *ctrl);

int cb_change_co3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam);
void cb_print_co3(dialog_info *dlg, control_node *ctrl);
int cb_read_co3(dialog_info *dlg, control_node *ctrl);

int cb_change_cb3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam);
void cb_print_cb3(dialog_info *dlg, control_node *ctrl);
int cb_read_cb3(dialog_info *dlg, control_node *ctrl);

int cb_change_tc3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam);
int cb_read_tc3(dialog_info *dlg, control_node *ctrl);

int cb_change_dc3(dialog_info *dlg, control_node *ctrl, UINT uMsg, WPARAM wParam, LPARAM lParam);
void cb_print_dc3(dialog_info *dlg, control_node *ctrl);
int cb_read_dc3(dialog_info *dlg, control_node *ctrl);


int dialog_info_create(dialog_info **dbd, HWND hwnd, db_list_node **fill_data);
db_list_node *dialog_info_free(dialog_info **db_tbl);
unsigned long dialog_control_assign(dialog_info *db_tbl, unsigned long flags, unsigned long id, int item, int (*cb_change)(struct _dialog_info *, struct _control_node *, UINT, WPARAM, LPARAM), void (*cb_write)(struct _dialog_info *, struct _control_node *), int (*cb_read)(struct _dialog_info *, struct _control_node *));
void dialog_control_mark_changed(dialog_info *db_tbl, int fset);
int dialog_control_change(dialog_info *dbd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int dialog_control_print(dialog_info *db_tbl);
int dialog_control_read(dialog_info *db_tbl);
control_node *dialog_control_find(dialog_info *db_tbl, unsigned long id);
control_node *dialog_control_print1(dialog_info *db_tbl, unsigned long id);
control_node *dialog_control_data_set(dialog_info *db_tbl, unsigned long id, unsigned long flags, void *data);
int dialog_control_undo(dialog_info *db_tbl);
int dialog_control_save(dialog_info *db_tbl);

int parsed_table_bsort(db_list_node **db_list, db_parsed_table *db_table, unsigned char id);
int parsed_table_bsort(db_parsed_table *db_table, unsigned char id);
int parsed_table_qsort(db_parsed_table *db_table, unsigned char id, unsigned char direction);
int parsed_table_qsort(db_list_node **disp_list, db_parsed_table *db_table, unsigned char id, unsigned char direction);
// rename parsed_table_filter -> parsed_table_col_filter
int parsed_table_filter(db_list_node **db_list, db_parsed_table *db_table, unsigned char id, unsigned long flags, void *data);
int parsed_table_data_filter(db_list_node **db_list, db_parsed_table *db_table, unsigned long flags, void *data);
void parsed_table_free(db_parsed_table **db_data);
void parse_db_result(odbc_result *res, db_parsed_table *db_data);
void cb_table_free(void *data, void *data2);


#endif //_ZDBDLG_H_
