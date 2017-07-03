#ifndef _JSON_VARS_H_
#define _JSON_VARS_H_

#include "../zlist.h"

typedef struct _json_parser_params
{
	// source code
	char *source_code;
	unsigned long sc_offset;
	unsigned long sc_length;
	
	char *error_msg;
	unsigned long error_code;
	unsigned long lineno;

	db_tree_node *result;
} json_parser_params;


#endif // _JSON_VARS_H_