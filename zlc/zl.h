#ifndef _ZL_H_
#define _ZL_H_

#include "zl_vars.h"
#include "zl_compiler.h"

//unsigned long zl_set(void **maptable, zl_names_map *vars_map, char *name, void *ext_var);
unsigned long zl_set(unsigned char *reloc_section, zl_map_section *map_section, unsigned long count, char *name, void *ext_var);
//unsigned long zl_memtable_make(void ***mem_table, unsigned long **vars_table, unsigned long count);
//unsigned long zl_memtable_make2(void ***mem_table, zl_names_map *vars_map);
//unsigned long zl_free(unsigned char **hardcode, void ***mem_table, unsigned char **data_table, zl_names_map **mapvars);
unsigned long zl_free(unsigned char *hardcode, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect, unsigned char *export_sect, unsigned char *map_sect);
unsigned long zl_offset(char *name, zl_export_section *export_section, unsigned long count);
//unsigned long zl_call(unsigned long offset, unsigned char *hardcode, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect);
//unsigned long zl_execute(unsigned long *regs, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect);
//unsigned long zl_execute(unsigned char *hardcode, unsigned char *datatable, void **maptable, zl_funcs_list *fn_list);
//unsigned long zl_execute(unsigned char *hardcode, unsigned char *data_sect, unsigned char *const_sect, unsigned char *reloc_sect, unsigned char *import_sect);
unsigned long zl_init(unsigned long offset, unsigned char *hardcode, unsigned long *stack, unsigned long *regs, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect);
unsigned long zl_push(unsigned long *regs, unsigned long value);
unsigned long zl_pop(unsigned long *regs, unsigned long value);
unsigned long zl_call(unsigned long *regs);
unsigned long zl_execute(unsigned long *regs);
unsigned long zl_decompile(unsigned char *hardcode, unsigned long hard_code_size);
unsigned long zl_unload_modules(unsigned long *modules);
unsigned long zl_load_functions(unsigned char *import_section, zl_map_section *map_section, unsigned long count, zl_funcs_list *fn_list, unsigned long **modules);
//unsigned long zl_load_functions(unsigned char *import_section, zl_map_section *map_section, unsigned long count, zl_funcs_list *fn_list);

#endif // _ZL_H_
