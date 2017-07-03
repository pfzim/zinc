#ifndef	_JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_parser.hpp"
//#include "json_vars.h"
#include "../zjson.h"
#include "../zlc/zl_compiler.h"
#include "../zlc/zl_opcodes.h"
#include "../utstrings.h"

//extern FILE *yyin;
//extern FILE *yyout;

//int zl_compile(unsigned char **hardcode, char *code, char **error_msg);
//int zl_compile(unsigned char **hardcode, unsigned char **data_table,/* unsigned long *vars_count,*/ zl_funcs_list *funcs, char *code, zl_names_map **vars_map, char **error_msg);
int json_parse(char *code, char **error_msg, db_tree_node **json_tree);

#endif // _JSON_PARSER_H_