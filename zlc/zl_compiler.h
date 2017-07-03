#ifndef _ZCOMPILER_H_
#define _ZCOMPILER_H_

#include "zl_vars.h"
//#include "../zlist.h"

typedef struct _stack_buffer
{
	void *data;
	_stack_buffer *next_node;
} cl_stack_buffer;


typedef struct _cl_parser_params
{
	// source code
	char *source_code;
	unsigned long sc_offset;
	unsigned long sc_length;
	
	// hard code
	unsigned char *hard_code;
	unsigned long hc_fill;
	unsigned long hc_buffer_size;
	
	cl_stack_buffer *cl_stack;
	cl_stack_buffer *cl_loop_stack;

	char *error_msg;
	unsigned long error_code;
	unsigned long lineno;

	/* revision 1
	unsigned char *data_table;
	unsigned long data_fill;
	unsigned long data_buffer_size;
	zl_funcs_list *funcs_list;
	zl_names_map *vars_table;
	zl_names_map *jumps_table;
	zl_names_map *labels_table;
	zl_names_map *structs_table;
	zl_names_map *funcs_table;
	//*/

	//unsigned long current_jump;		// autoincrement
	//?unsigned long current_func;

	// revision 2
	unsigned long current_level;	// autoincrement
	unsigned long stack_size;		// function stack usage
	cl_var_node *vars_table;
	cl_data_node *data_table;
	cl_label_node *labels_table;	// inter compiler use only
	cl_label_node *funcs_table;
} cl_parser_params;

/*
typedef struct _zl_section_node
{
	unsigned long flags;		// параметры секции
	union
	{
		char *name;				// имя локальной переменной, функции или метки
		void *data;				// данные для секции const
	};
	union
	{
		unsigned long offset;	// смещение относительно hard_code для метки, функции или локальной переменной
		unsigned long size;		// размер данные для секции const
	};
	db_list_node *reference_offsets;
} zl_section_node;
//*/


//cl_var_node *cl_var_define(cl_var_node **vars_table, char *name, unsigned long level);
//cl_var_node *cl_var_define(cl_var_node **vars_table, cl_var_node *var_node);
cl_var_node *cl_var_define(cl_var_node **vars_table, char *name, unsigned long level);
cl_var_node *cl_var_find(cl_var_node *vars_table, char *name);
unsigned long cl_var_free_level(cl_var_node **vars_table, unsigned long level);
unsigned long cl_var_reference(cl_var_node *var_node, unsigned long offset);
cl_label_node *cl_label_define(cl_label_node **labels_table, char *name);
cl_label_node *cl_label_find(cl_label_node *labels_table, char *name);
unsigned long cl_label_reference(cl_label_node *label_node, unsigned long offset);
unsigned long cl_label_fix(cl_label_node *labels_table, unsigned char *hard_code);
unsigned long cl_label_free(cl_label_node **labels_table);
cl_data_node *cl_const_define(cl_data_node **data_table, char *data, unsigned long size, unsigned long reference);
cl_data_node *cl_const_free(cl_data_node **data_table);
//unsigned long cl_const_build_fix_free(cl_data_node *const_table, unsigned char *hard_code, unsigned char **const_section, unsigned long *size);
unsigned long cl_link_sections(
	unsigned long flags,
	cl_data_node *const_table,
	cl_var_node *vars_table,
	cl_label_node *funcs_table,
	unsigned char *hard_code,
	unsigned long hard_code_size,
	unsigned char **const_section,
	unsigned long *const_size,
	unsigned char **data_section,
	unsigned long *data_size,
	unsigned char **reloc_section,
	unsigned long *reloc_size,
	unsigned char **import_section,
	unsigned long *import_size,
	unsigned char **export_section,
	unsigned long *export_size,
	unsigned char **map_section,
	unsigned long *map_size
);

// глобальные переменные должны хранить размер и инициализационные данные

//db_list_node *cl_section_data_push(db_list_node **section, unsigned long flags, char *data, unsigned long size);
//db_list_node *cl_reference_add(db_list_node **refs, unsigned long offset);
//unsigned long cl_data_add(cl_parser_params *pp, unsigned char *data, unsigned long size);
//Строковые константы добавляются в таблицу данных и на выходе функция
//возвращает офсет занесённых данных относительно начала таблицы.

unsigned long cl_code_add(cl_parser_params *pp, unsigned char *data, unsigned long size);

/*
zl_names_map *cl_label_define(zl_names_map **labels_table, char *name, unsigned long offset);
//Метки при определении заносятся во временную таблицу (имя метки и её
//офсет относительно hardcode)

zl_names_map *cl_jump_define(zl_names_map **labels_table, char *name, unsigned long offset);
//по коду функция равна cl_label_define()
//Переходы на метки заносятся в таблицу аналогично самим меткам (имя метки
//и офсет на место в коде, где на ней ссылка).

void cl_names_free(zl_names_map **labels_table);
void cl_names_free(zl_names_map **labels_table);
#define cl_jumps_free cl_names_free
#define cl_labels_free cl_names_free

unsigned long cl_jump_fix(unsigned char *hardcode, zl_names_map *jumps_table, zl_names_map *labels_table);
//После окончания компиляции все места переходов на метки корректируются в
//соответствии с таблицей меток.

zl_names_map *cl_label_new(zl_names_map **jumps_table, unsigned long offset);
//Генерирует уникальное значение метки и помещает его в таблицу типа
//names_map. И возвращает указатель на структуру.
*/

void *cl_stack_push(cl_stack_buffer **cl_stack, void *data);
//Помещает значение в стек

void *cl_stack_pop(cl_stack_buffer **cl_stack);
//Возвращает значение из стека

void cl_stack_free(cl_stack_buffer **cl_stack);

/*
unsigned long cl_var_define(zl_names_map **vars_table, char *name, unsigned long flags, zl_names_map **out);
//Добавляет новую переменную в карту памяти.
//После компиляции из этой таблицы сформируется карта внешних переменных
//и два значения - количество внешних и внутренних переменных.

cl_var_node *cl_var_define(cl_var_node **vars_table, cl_var_node *var_node);
zl_names_map *cl_var_find(zl_names_map *vars_table, char *name, unsigned long *val);
zl_funcs_list *cl_func_find(zl_funcs_list *funcs, char *name, unsigned long *val);
unsigned long cl_find_var_or_func(zl_names_map *vars_table, zl_funcs_list *funcs, char *name);
*/

unsigned long cl_push(cl_parser_params *pp, unsigned char code);
unsigned long cl_push_dw(cl_parser_params *pp, unsigned long data);
unsigned long cl_push_op(cl_parser_params *pp, unsigned char code, zlval *arg1, zlval *arg2);

long cl_strtol(char *text, unsigned long len);
unsigned long cl_strtoul(char *text, unsigned long len);
double cl_strtod(char *text, unsigned long len);

char *cl_strdup(char *text);
char *cl_strndup(char *text, unsigned long len);
char *cl_sprintf(char *fmt, ...);

unsigned long cl_do_op(cl_parser_params *pp, unsigned char op, zlval *ss, zlval *s1, zlval *s2);

#endif // _ZCOMPILER_H_
