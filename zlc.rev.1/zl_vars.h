#ifndef _VARS_H_
#define _VARS_H_

typedef struct _zl_names_map
{
	char *name;
	unsigned long flags;
	unsigned long offset;
	unsigned long size;
	struct _zl_names_map *elements;
	struct _zl_names_map *struct_el;
	struct _zl_names_map *next_node;
} zl_names_map;

typedef struct _zl_funcs_list
{
	char *name;
	unsigned long params;
	void *proc;
	unsigned long flags;		// кто чистит стек? вызываемая функция или вызывающая.
} zl_funcs_list;

typedef struct _zl_opcodes_list
{
	unsigned char opcode;
	unsigned long group;
	unsigned long arg1;
	unsigned long arg2;
} zl_opcodes_list;

typedef struct _zl_val
{
	union
	{
		signed long value;
		unsigned long uvalue;
		double dvalue;
		float fvalue;
		void *data;
		char *string;
		//signed __int64 value64;
		//unsigned __int64 uvalue64;
	};
	unsigned long size;
	unsigned long fill;
	char *name;
	unsigned long flags;
	union
	{
		struct _zl_val *next_node;
		struct _zl_names_map *var_info;
	};
} zlval;

zlval *zl_val_alloc(zlval **zl_tree);
zlval *zl_val_find(zlval *zl_tree, char *name);
unsigned long zl_val_free(zlval **zl_tree);


#endif // _VARS_H_