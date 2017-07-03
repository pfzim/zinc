#ifndef _ZQUERY_H_
#define _ZQUERY_H_

#include "zlist.h"

#define CL_ORDER				SG_ORDER
#define CL_DESC					SG_DESC
#define CL_VISIBLE				SG_VISIBLE
#define CL_RIGHT				0x00400000

// deprecated values
#define CL_KEY					SG_KEY
#define CL_NUMBER				0x00800000		// store to array as number, otherwrise as string
#define CL_THOUSANDS			0x01000000
#define CL_DATE					0x02000000
#define CL_CURRENCY				0x04000000
#define CL_USER1				0x08000000
#define CL_USER2				0x10000000
#define CL_USER3				0x20000000
#define CL_USER4				0x40000000
#define CL_USER5				0x80000000

typedef struct _sql_query_node
{
	unsigned char id;
	unsigned long flags;
	char *db_col;
	db_list_node *db_cond;
	//char *value;
	void *user_data;
} sql_query_node;

typedef struct _sql_value_node
{
	unsigned long flags;
	char *value;
} sql_value_node;

typedef struct _lv_column_node
{
	unsigned char id;
	unsigned long flags;
	unsigned long cx;
	char *hu_name;
	void *user_data;
} lv_column_node;

typedef struct _sql_query_parts
{
	unsigned long flags;
	unsigned char count_names;
	unsigned long count_joins;
	unsigned char *offsets;
	unsigned char offsets_count;
	char *names;
	char *values;
	char *from;
	char *where;
	char *group;
	char *order;
	char *temp_name;
	db_list_node* values_list;
	int (*cb_print_data)(void *, char **);	// возвращает 1 и result, если требуется добавить значение в SQL запрос
	int (*cb_find_data)(void *, void *);	// list_walk функция
	//int (*cb_is_changed_data)(void *);
} sql_query_parts;

/*
typedef struct _select_dialog_info
{
	unsigned long flags;
	char *table_name;
	db_list_node *sql_query;
	db_list_node *lv_node;
	odbc_result *db_data;
	char *title;
	odbc_row *result;
	db_list_node *db_display;
	unsigned long display_count;
	CRITICAL_SECTION cs;
} select_dialog_info;
*/

int cb_lv_by_ud(void *data1, void *data2);
int cb_lv_by_id(void *data1, void *data2);
void cb_free_lv(void *data, void *data2);
int lv_free(db_list_node **db_node, void (*cb_free_user_data)(void *));
db_list_node *lv_column_add(db_list_node **db_node, unsigned char id, unsigned long flags, unsigned long cx, char *hu_name, void *user_data);
//int lv_build(db_list_node *db_node, HWND hwnd, unsigned long *order_ids, unsigned long count);
//int lv_build(db_list_node *db_node, HWND hwnd);

int cb_sql_query_by_key(void *data1, void *data2); // by key name
int cb_sql_query_by_ud(void *data1, void *data2); // by user_data
int cb_sql_query_by_id(void *data1, void *data2); // by column id
int sql_query_free(db_list_node **query, void (*cb_free_user_data)(void *));
db_list_node *sql_query_column_add(db_list_node **db_node, unsigned char id, unsigned long flags, char *db_col, db_list_node *db_cond, void *user_data);
db_list_node *sql_query_cond_add(db_list_node *col_node, unsigned long flags, char *value);
db_list_node *sql_query_cond_add(db_list_node *db_node, unsigned char id, unsigned long flags, char *value);
char *sql_query_build2(unsigned long flags, db_list_node *db_node, char *table_name, unsigned char *data_ids, unsigned char count);
char *sql_query_build3(unsigned long flags, db_list_node *db_node, char *table_name, unsigned char *data_ids, unsigned char count, db_list_node *values_list, int (*cb_print_data)(void *, char **), int (*cb_find_data)(void *, void *), char *additional);
char *sql_query_value(unsigned long flags, char *value);
db_list_node *sql_query_reset_conds(db_list_node *db_node);

//unsigned long sql_column_add(db_lv_data *db_tbl, unsigned long flags, char *db_col, char *db_cond, /*char *db_join_table, char *db_join_cond1, char *db_join_cond2,*/ unsigned long cx, char *xml_name, char *hu_name);
//void sql_filter_reset(db_lv_data *db_tbl);
//char *sql_query_from_table(unsigned long flags, db_lv_data *db_tbl, char *table_name, unsigned long *data_offsets, unsigned long *cols_offsets);

#endif //_ZQUERY_H_
