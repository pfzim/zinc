#ifndef _ZJSON_H_
#define _ZJSON_H_

#include "zlist.h"

// JSON over db_tree_node

#define JS_TYPEMASK				0x0000FFFF
#define JS_STRING				0x00000001	// default
#define JS_LONG					0x00000002
#define JS_ULONG				0x00000004
#define JS_DOUBLE				0x00000008
#define JS_ARRAY				0x00000010
#define JS_OBJECT				0x00000020

typedef struct json_value
{
	unsigned long flags;
	union
	{
		db_list_node *arr_val;
		db_tree_node *obj_val;
		char *str_val;
		long l_val;
		unsigned long ul_val;
		double dbl_val;
	};
} json_value;


void cb_json_free(void *data);

json_value *json_val(unsigned long flags, void *data);

db_tree_node *json_set(db_tree_node **root_node, char *path, unsigned long flags, void *data);
db_tree_node *json_assign(db_tree_node **root_node, char *path, json_value *value);
json_value *json_find(db_tree_node *root_node, char *path);
int json_free(db_tree_node **root_node);

json_value *json_array_push(json_value *node, json_value *value);

char *json_print(db_tree_node *root_node);
char *json_print_fmt(db_tree_node *root_node);

json_value *json_dup0(json_value *value);

#endif //_ZJSON_H_
