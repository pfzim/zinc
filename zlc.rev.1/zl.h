#ifndef _ZL_H_
#define _ZL_H_

#include "zl_vars.h"
#include "zl_compiler.h"

unsigned long zl_set(void **maptable, zl_names_map *vars_map, char *name, void *ext_var);
unsigned long zl_memtable_make(void ***mem_table, unsigned long **vars_table, unsigned long count);
unsigned long zl_memtable_make2(void ***mem_table, zl_names_map *vars_map);
unsigned long zl_free(unsigned char **hardcode, void ***mem_table, unsigned char **data_table, zl_names_map **mapvars);
unsigned long zl_execute(unsigned char *hardcode, unsigned char *datatable, void **maptable, zl_funcs_list *fn_list);
unsigned long zl_decompile(unsigned char *hardcode);

#endif // _ZL_H_
