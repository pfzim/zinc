%define api.pure full
%error-verbose
%no-lines
%defines
//%define parser_class_name	"json_parser"

//%define api.prefix	yyjp
//%define namespace	"json_parser"

%lex-param		{void *scanner}
%parse-param	{void *scanner}
%parse-param	{json_parser_params *pp}

%code requires {
#include "json_vars.h"
}

%{

#include "json_parser.h"
#include "../zdbg.h"

//#define YYPARSE_PARAM pp
//#define YYPP ((cl_parser_params *) pp)
//#define YYLEX_PARAM pp

#define YYSTYPE json_value

#define ZL_ERROR(message) { yyerror(scanner, pp, message); YYABORT; }

void yyerror(void *scanner, json_parser_params *pp, const char *err);
int yylex(json_value *yylval, void *yyscanner);

int yylex_init(void  **ptr_yy_globals);
int yylex_destroy(void *yyscanner);
void yyset_extra(json_parser_params *user_defined, void *yyscanner);

%}

%left ':'

%token T_LABEL
%token T_CONST_DOUBLE
%token T_CONST_LONG
%token T_CONST_ULONG
%token T_CONST_STRING
%token T_TRUE
%token T_FALSE
%token T_NULL


%start json_data

%%



json_data
	:	object						{ pp->result = $1.obj_val; }
	|	/* empty */					{ pp->result = NULL; }
;

object
	:    '{' '}'					{ $$.flags = JS_OBJECT; $$.obj_val = NULL; }
	|    '{' members '}'			{ $$ = $2; }
;

members
	:    pair						{ $$ = $1; }
	|    pair ',' members			{ $$ = $1; $$.obj_val->next_node = $3.obj_val; }
;

pair
	:    label ':' value	
									{
										$$.flags = JS_OBJECT;
										$$.obj_val = NULL;
										if($3.flags & JS_OBJECT )
										{
											json_assign(&$$.obj_val, $1.str_val, NULL);
											$$.obj_val->child_node = $3.obj_val;
										}
										else
										{
											json_assign(&$$.obj_val, $1.str_val, json_dup0(&$3));
										}
										free_str($1.str_val);
									}
;

array
	:	'[' ']'						{ $$.flags = JS_ARRAY; $$.arr_val = NULL; }
    |	'[' elements ']'			{ $$ = $2; }
;

elements
	:    value						{ $$.flags = JS_ARRAY; $$.arr_val = NULL; json_array_push(&$$, json_dup0(&$1)); }
	|    elements ',' value			{ $$ = $1; json_array_push(&$$, json_dup0(&$3)); }
;

value
	:    T_CONST_STRING				{ $$ = $1; }
	|    T_CONST_LONG				{ $$ = $1; }
	|    T_CONST_ULONG				{ $$ = $1; }
	|    T_CONST_DOUBLE				{ $$ = $1; }
	|    object						{ $$ = $1; }
	|    array						{ $$ = $1; }
	|    T_TRUE						{ $$ = $1; }
	|    T_FALSE					{ $$ = $1; }
	|    T_NULL						{ $$ = $1; }
;

label
	:    T_CONST_STRING				{ $$ = $1; }
	|    T_LABEL					{ $$ = $1; }
	|    T_CONST_ULONG				{ $$.str_val = (char *) zalloc(33); _ultoa($1.ul_val, $$.str_val, 10); $$.flags = JS_STRING; }
	|    T_TRUE						{ $$.str_val = alloc_string("true"); $$.flags = JS_STRING; }
	|    T_FALSE					{ $$.str_val = alloc_string("false"); $$.flags = JS_STRING; }
	|    T_NULL						{ $$.str_val = alloc_string("null"); $$.flags = JS_STRING; }
;

%%









void yyerror(void *scanner, json_parser_params *pp, const char *err)
{
	pp->error_msg = cl_sprintf("json_parser: error at line %d: %s", pp->lineno, err);
}

int json_parse(char *code, char **error_msg, db_tree_node **json_tree)
{
	int ret;
	void *scanner;
	json_parser_params pp;

	memset(&pp, 0, sizeof(json_parser_params));

	pp.source_code = code;
	pp.sc_length = strlen(code);
	pp.lineno = 1;

	yylex_init(&scanner);
	yyset_extra(&pp, scanner);
	ret = yyparse(scanner, &pp);
	yylex_destroy(scanner);

	*json_tree = pp.result;
	
	if(error_msg)
	{
		*error_msg = pp.error_msg;
	}
	else if(ret)
	{
		free_str(pp.error_msg);
	}

	return ret;
}
