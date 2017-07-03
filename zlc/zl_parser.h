#ifndef	_BISON_H_
#define _BISON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zl_parser.hpp"
#include "zl_vars.h"
#include "zl_compiler.h"
#include "zl_opcodes.h"
#include "../utstrings.h"

//extern FILE *yyin;
//extern FILE *yyout;

//int zl_compile(unsigned char **hardcode, char *code, char **error_msg);
//int zl_compile(unsigned char **hardcode, unsigned char **data_table,/* unsigned long *vars_count,*/ zl_funcs_list *funcs, char *code, zl_names_map **vars_map, char **error_msg);
int zl_compile(unsigned char **hardcode, unsigned long *hard_code_size, char *code, char **error_msg,
	unsigned char **const_sect,
	unsigned long *const_size,
	unsigned char **data_sect,
	unsigned long *data_size,
	unsigned char **reloc_sect,
	unsigned long *reloc_size,
	unsigned char **import_sect,
	unsigned long *import_size,
	unsigned char **export_sect,
	unsigned long *export_size,
	unsigned char **map_sect,
	unsigned long *map_size
);

#endif // _BISON_H_