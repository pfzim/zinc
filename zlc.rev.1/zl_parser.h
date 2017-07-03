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

int zl_compile(unsigned char **hardcode, unsigned char **data_table,/* unsigned long *vars_count,*/ zl_funcs_list *funcs, char *code, zl_names_map **vars_map, char **error_msg);

#endif // _BISON_H_