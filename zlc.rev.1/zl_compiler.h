#ifndef _ZCOMPILER_H_
#define _ZCOMPILER_H_

#include "zl_vars.h"

typedef struct _stack_buffer
{
	void *data;
	_stack_buffer *next_node;
} cl_stack_buffer;

typedef struct _cl_parser_params
{
	char *source_code;
	unsigned long sc_offset;
	unsigned long sc_length;
	unsigned char *hard_code;
	unsigned long hc_fill;
	unsigned long hc_buffer_size;
	unsigned char *data_table;
	unsigned long data_fill;
	unsigned long data_buffer_size;
	zl_funcs_list *funcs_list;
	zl_names_map *vars_table;
	zl_names_map *jumps_table;
	zl_names_map *labels_table;
	zl_names_map *structs_table;
	cl_stack_buffer *cl_stack;
	cl_stack_buffer *cl_loop_stack;
	char *error_msg;
	unsigned long error_code;
	unsigned long lineno;
} cl_parser_params;

unsigned long cl_data_add(cl_parser_params *pp, unsigned char *data, unsigned long size);
//��������� ��������� ����������� � ������� ������ � �� ������ �������
//���������� ����� ��������� ������ ������������ ������ �������.

unsigned long cl_code_add(cl_parser_params *pp, unsigned char *data, unsigned long size);

zl_names_map *cl_label_define(zl_names_map **labels_table, char *name, unsigned long offset);
//����� ��� ����������� ��������� �� ��������� ������� (��� ����� � �
//����� ������������ hardcode)

zl_names_map *cl_jump_define(zl_names_map **labels_table, char *name, unsigned long offset);
//�� ���� ������� ����� cl_label_define()
//�������� �� ����� ��������� � ������� ���������� ����� ������ (��� �����
//� ����� �� ����� � ����, ��� �� ��� ������).

void cl_names_free(zl_names_map **labels_table);
void cl_names_free(zl_names_map **labels_table);
#define cl_jumps_free cl_names_free
#define cl_labels_free cl_names_free

unsigned long cl_jump_fix(unsigned char *hardcode, zl_names_map *jumps_table, zl_names_map *labels_table);
//����� ��������� ���������� ��� ����� ��������� �� ����� �������������� �
//������������ � �������� �����.

zl_names_map *cl_label_new(zl_names_map **jumps_table, unsigned long offset);
//���������� ���������� �������� ����� � �������� ��� � ������� ����
//names_map. � ���������� ��������� �� ���������.

void *cl_stack_push(cl_stack_buffer **cl_stack, void *data);
//�������� �������� � ����

void *cl_stack_pop(cl_stack_buffer **cl_stack);
//���������� �������� �� �����

void cl_stack_free(cl_stack_buffer **cl_stack);

//cl_push(hardcode, OP_CALL)
//��������� ���

//cl_push_dw(hardcode, (unsigned long) 0)
//��������� ���

unsigned long cl_var_define(zl_names_map **vars_table, char *name, unsigned long flags, zl_names_map **out);
//��������� ����� ���������� � ����� ������.
//����� ���������� �� ���� ������� ������������ ����� ������� ����������
//� ��� �������� - ���������� ������� � ���������� ����������.

zl_names_map *cl_var_find(zl_names_map *vars_table, char *name, unsigned long *val);
zl_funcs_list *cl_func_find(zl_funcs_list *funcs, char *name, unsigned long *val);
unsigned long cl_find_var_or_func(zl_names_map *vars_table, zl_funcs_list *funcs, char *name);

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
