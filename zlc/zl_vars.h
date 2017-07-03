#ifndef _VARS_H_
#define _VARS_H_


// move to cl
typedef struct _cl_var_node
{
	char *name;
	unsigned long flags;
	unsigned long level;
	unsigned long offset;
	// for arrays
	unsigned long rows;
	unsigned long size[1024]; //1024
	// global vars
	unsigned long references;
	unsigned long reference_offsets[1024]; //1024

	struct _cl_var_node *next_node;
} cl_var_node;

typedef struct _cl_label_node
{
	char *name;
	char *library;
	unsigned long flags;
	//??unsigned long level;
	unsigned long offset;
	unsigned long references;
	unsigned long reference_offsets[1024];
	unsigned long params;

	struct _cl_label_node *next_node;
} cl_label_node;

typedef struct _cl_data_node
{
	char *data;
	unsigned long size;
	unsigned long references;
	unsigned long reference_offsets[1024];

	struct _cl_data_node *next_node;
} cl_data_node;

typedef struct _zl_map_section
{
	unsigned long flags;
	char name[100];
	char library[256];
} zl_map_section;

typedef struct _zl_export_section
{
	unsigned long offset;
	unsigned long params;
	char name[100];
} zl_export_section;

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
	void *proc;
} zl_funcs_list;

/* revision 1
typedef struct _zl_funcs_list
{
	char *name;
	unsigned long params;
	void *proc;
	unsigned long flags;		// кто чистит стек? вызываемая функция или вызывающая.
} zl_funcs_list;
*/

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
	//char *name;					// он используется? удалить?
	//union
	//{
	//	struct _zl_val *next_node;
	//	struct _zl_names_map *var_info;
	//};

	// revision 2
	struct _cl_var_node *var; // для облегчения структуры переделать в *указатель
	unsigned long flags;
	unsigned long rows;
} zlval;

//zlval *zl_val_alloc(zlval **zl_tree);
//zlval *zl_val_find(zlval *zl_tree, char *name);
//unsigned long zl_val_free(zlval **zl_tree);


#endif // _VARS_H_