%{

#include "zl_parser.h"

//#define YYPARSE_PARAM pp
//#define YYPP ((cl_parser_params *) pp)
//#define YYLEX_PARAM pp

#define YYSTYPE zlval

#define ZL_ERROR(message) { yyerror(scanner, pp, message); YYABORT; }

void yyerror(void *scanner, cl_parser_params *pp, char *err);
int yylex(zlval *yylval, void *yyscanner);
     
int yylex_init(void  **ptr_yy_globals);
int yylex_destroy(void *yyscanner) ;
void yyset_extra(cl_parser_params *user_defined, void *yyscanner);

%}

%pure_parser
%error-verbose

%lex-param		{void *scanner}
%parse-param	{void *scanner}
%parse-param	{cl_parser_params *pp}

%left ','
%left '=' T_OR_ASSIGN T_XOR_ASSIGN T_SHL_ASSIGN T_SHR_ASSIGN T_ADD_ASSIGN T_MOD_ASSIGN T_DIV_ASSIGN T_MUL_ASSIGN T_SUB_ASSIGN T_AND_ASSIGN
%left '?' ':'
%left T_OR
%left T_AND
%left '|'
%left '^'
%left '&'
%nonassoc T_EQ T_NE
%nonassoc '<' T_LE '>' T_GE
%left T_SHL T_SHR
%left '+' '-'
%left '*' '/' '%'
%right '!' '~' T_INC T_DEC '@'
%nonassoc T_PLUS T_MINUS T_POINTER
%right '['
%token T_IF
%left T_ELSE

%token '.'
%token T_PTR_OP

%token T_WHILE T_DO T_BREAK T_CONTINUE T_FOR

%token T_LABEL
%token T_CONSTANT_DOUBLE
%token T_CONSTANT_LONG
%token T_CONSTANT_STRING

%token T_VOID
%token T_CHAR
%token T_SHORT
%token T_INT
%token T_LONG
%token T_FLOAT
%token T_DOUBLE
%token T_SIGNED
%token T_UNSIGNED
%token T_STRUCT
%token T_STRUCT_DEFINED
%token T_ELLIPSIS
%token T_STRING
%token T_EXTERN
%token T_VAR
%token T_IMPORT
%token T_FROM

%token T_GOTO
%token T_SIZEOF
%token T_RETURN

%token T_REGISTER
%token T_OPERATOR

%token T_ASM

%left T_PARAM_SEPARATOR

%token T_NOP
%token T_DBG_PRINT_REGS

%token T_FUNCTION

%start program_list

%%

program_list
	: program program_list
	| /* empty */
;

program
	: declaration_statement
	| declaration_function
;

statement_list
	: statement statement_list
	| /* empty */
;
statement
	: compound_statement
	| expression_statement
	| buildin_statement
	| selection_statement
	| labeled_statement
	| jump_statement
	| declaration_statement
	| assembler_statement
;

compound_statement
	: '{'
		{
			pp->current_level++;
		}
		statement_list '}'
		{
			cl_var_free_level(&pp->vars_table, pp->current_level);
			pp->current_level--;
		}
;

//*
assembler_statement
	: T_ASM assembler_command_list '}'
;

assembler_command_list
	: assembler_command '\n' assembler_command_list
	| labeled_statement assembler_command_list
	| assembler_command
;

assembler_command
	: T_GOTO T_LABEL
		{
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, $2.string);
			free_str($2.string);

			if(!label)
			{
				ZL_ERROR("internal error");
			}

			cl_push(pp, (unsigned char) $1.value);
			cl_label_reference(label, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_OPERATOR operator_parameter ',' operator_parameter		{ cl_push_op(pp, (unsigned char) $1.value, &$2, &$4); }
	| T_OPERATOR operator_parameter 							{ cl_push_op(pp, (unsigned char) $1.value, &$2, NULL) }
	| T_OPERATOR 												{ cl_push_op(pp, (unsigned char) $1.value, NULL, NULL) }
	| T_LABEL '(' assembler_argument_list ')'
		{
			cl_label_node *func;

			func = cl_label_find(pp->funcs_table, $1.string);
			if(!func)
			{
				ZL_ERROR("undeclared identifier");
			}

			if(func->flags & ZLF_FUNC_INTERNAL)
			{
				cl_push(pp, OP_CALL); // call near
			}
			else
			{
				ZL_ERROR("unsupported far call");
			}

			cl_label_reference(func, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| /* empty */
;

/*
assembler_command_list
	: assembler_command assembler_command_list
	| / * empty * /
;

assembler_command
	: labeled_statement
	| T_GOTO T_LABEL '\n'										{ cl_push(pp, (unsigned char) $1.value); cl_jump_define(&pp->jumps_table, $2.string, pp->hc_fill); free_str($2.string); cl_push_dw(pp, 0) }
	| T_OPERATOR operator_parameter ',' operator_parameter '\n'	{ cl_push_op(pp, (unsigned char) $1.value, &$2, &$4); }
	| T_OPERATOR operator_parameter '\n'						{ cl_push_op(pp, (unsigned char) $1.value, &$2, NULL) }
	| T_OPERATOR '\n'											{ cl_push_op(pp, (unsigned char) $1.value, NULL, NULL) }
	| T_LABEL '(' call_argument_list ')' '\n'					{ cl_push_op(pp, OP_CALL, &$1, NULL); free_str($1.string); }
	| '\n'
;
*/

assembler_argument_list
	: operator_parameter ',' assembler_argument_list			{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $3.value); }
	| operator_parameter										{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value); }
	| /* empty */
;

operator_parameter
	: T_REGISTER												{ $$ = $1; $$.flags = ARG_REG }
	| '[' T_REGISTER ']'										{ $$ = $1; $$.flags = ARG_PREG }
	| T_CONSTANT_LONG											{ $$ = $1; $$.flags = ARG_IMM }
//	| T_CONSTANT_DOUBLE											{ $$ = $1; $$.flags = ARG_IMM8 }
//	| T_CONSTANT_STRING											{ $$ = $1; $$.flags = ARG_DATA; }
/*
	| T_LABEL
		{
			$$ = $1;
			$$.flags = ARG_MEM;
			$$.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, $1.string);
			free_str($1.string);
			if($$.uvalue == 1) ZL_ERROR("undeclared identifier");
		}
	| '[' T_LABEL ']'
		{
			$$ = $1;
			$$.flags = ARG_PMEM;
			$$.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, $2.string);
			free_str($2.string);
			if($$.uvalue == 1) ZL_ERROR("undeclared identifier");
		}
*/
;
//*/

expression_statement
	: ';'
	| expression ';'											{ cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); }
;

buildin_statement
	: T_NOP ';'													{ cl_push(pp, OP_NOP); }
	| T_DBG_PRINT_REGS ';'										{ cl_push(pp, OP_DBG_PRINT); }
	| T_BREAK ';'
		{
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack) ZL_ERROR("break not in loop");

			cl_label_reference((cl_label_node *) pp->cl_loop_stack->next_node->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_CONTINUE ';'
		{
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack || !pp->cl_loop_stack->next_node) ZL_ERROR("continue not in loop");

			cl_label_reference((cl_label_node *) pp->cl_loop_stack->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_BREAK T_CONSTANT_LONG ';'
		{
			cl_stack_buffer *lsb;
			lsb = pp->cl_loop_stack;
			$2.value = $2.value*2-1;
			while($2.value--)
			{
				lsb = lsb->next_node;
				if(!lsb) ZL_ERROR("break not in loop");
			}
			cl_push(pp, OP_JMP);

			cl_label_reference((cl_label_node *) lsb->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_CONTINUE T_CONSTANT_LONG ';'
		{
			cl_stack_buffer *lsb;
			lsb = pp->cl_loop_stack;
			$2.value = ($2.value-1)*2;
			while($2.value--)
			{
				lsb = lsb->next_node;
				if(!lsb) ZL_ERROR("continue not in loop");
			}
			cl_push(pp, OP_JMP);

			cl_label_reference((cl_label_node *) lsb->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_RETURN ';'
		{
			cl_label_node *lb_label;

			lb_label = cl_label_find(pp->labels_table, "@exit_func");

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_label, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_RETURN assignment_expression ';'
		{
			cl_label_node *lb_label;

			lb_label = cl_label_find(pp->labels_table, "@exit_func");

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_JMP);
			cl_label_reference(lb_label, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
;

selection_statement
	: T_IF '(' assignment_expression ')'
		{
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		statement
		else_statement
		{
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		}
	| T_WHILE
		{
			cl_label_node *lb_exit, *lb_condition;
			lb_exit = cl_label_define(&pp->labels_table, NULL);
			lb_condition = cl_label_define(&pp->labels_table, NULL);
			lb_condition->offset = pp->hc_fill;
			lb_condition->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
		}
		'(' assignment_expression ')'
		{
			cl_label_node *lb_exit;
			lb_exit= (cl_label_node *) pp->cl_loop_stack->next_node->data;
			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_exit, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		statement
		{
			cl_label_node *lb_exit, *lb_condition;
			lb_condition = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_condition, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		}
	| T_DO
		{
			cl_label_node *lb_exit, *lb_condition, *lb_start;
			lb_exit = cl_label_define(&pp->labels_table, NULL);
			lb_condition = cl_label_define(&pp->labels_table, NULL);
			lb_start = cl_label_define(&pp->labels_table, NULL);
			lb_start->offset = pp->hc_fill;
			lb_start->flags = ZLF_DEFINED;

			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
			cl_stack_push(&pp->cl_stack, lb_start);				// start loop
		}
		statement
		T_WHILE
		{
			cl_label_node *lb_condition;
			lb_condition = (cl_label_node *) pp->cl_loop_stack->data;
			lb_condition->offset = pp->hc_fill;
			lb_condition->flags = ZLF_DEFINED;
		}
		'(' assignment_expression ')'
		{
			cl_label_node *lb_exit, *lb_condition;
			lb_condition = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_label_reference((cl_label_node *) cl_stack_pop(&pp->cl_stack), pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		}';'
	| T_FOR '(' expression ';'
		{
			cl_label_node *lb_exit, *lb_loop, *lb_condition;
			lb_exit = cl_label_define(&pp->labels_table, NULL);
			lb_loop = cl_label_define(&pp->labels_table, NULL);

			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit
			cl_stack_push(&pp->cl_loop_stack, lb_loop);			// loop

			cl_push(pp, OP_POP_REG);
			cl_push(pp, REG_EAX);

			lb_condition = cl_label_define(&pp->labels_table, NULL);
			lb_condition->offset = pp->hc_fill;
			lb_condition->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_stack, lb_condition);
		}
		expression ';'
		{
			cl_label_node *lb_loop, *lb_exit, *lb_statement;

			lb_statement = cl_label_define(&pp->labels_table, NULL);
			lb_loop = (cl_label_node *) pp->cl_loop_stack->data;
			lb_exit = (cl_label_node *) pp->cl_loop_stack->next_node->data;

			cl_stack_push(&pp->cl_stack, lb_statement);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_label_reference(lb_statement, pp->hc_fill);
			cl_push_dw(pp, 0);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_exit, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_loop->offset = pp->hc_fill;
			lb_loop->flags = ZLF_DEFINED;
		}
		expression ')'
		{
			cl_label_node *lb_condition, *lb_statement;
			lb_statement = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_condition = (cl_label_node *) cl_stack_pop(&pp->cl_stack);

			cl_push(pp, OP_POP_REG);
			cl_push(pp, REG_EAX);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_condition, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_statement->offset = pp->hc_fill;
			lb_statement->flags = ZLF_DEFINED;
		}
		statement
		{
			cl_label_node *lb_exit, *lb_loop;
			lb_loop = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_loop, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		}
;

else_statement
	: /* empty */
	| T_ELSE
		{
			cl_label_node *lb_skip, *lb_else;
			lb_else = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_define(&pp->labels_table, NULL);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			lb_else->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_stack, lb_skip);
		}
		statement
;

labeled_statement
	: T_LABEL ':' /*statement*/
		{
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, $1.string);
			free_str($1.string);

			if(label->flags & ZLF_DEFINED)
			{
				ZL_ERROR("label redefined");
			}

			label->offset = pp->hc_fill;
			label->flags = ZLF_DEFINED;
		}
;

jump_statement
	: T_GOTO T_LABEL ';'
		{
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, $2.string);
			free_str($2.string);

			if(!label)
			{
				ZL_ERROR("internal error");
			}

			cl_push(pp, (unsigned char) $1.value);
			cl_label_reference(label, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
;

declaration_function
	: T_IMPORT T_CONSTANT_LONG T_LABEL '(' T_CONSTANT_LONG ')' ';'
		{
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, $3.string);
			free_str($3.string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_DEFINED;
			if(!$2.uvalue)
			{
				func->flags |= ZLF_STDCALL;
			}
			func->library = NULL;
			func->params = $5.uvalue;
		}
	| T_IMPORT T_CONSTANT_LONG T_LABEL '(' T_CONSTANT_LONG ')' T_FROM T_CONSTANT_STRING ';'
		{
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, $3.string);
			free_str($3.string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_DEFINED;
			if(!$2.uvalue)
			{
				func->flags |= ZLF_STDCALL;
			}
			func->library = $8.string;
			func->params = $5.uvalue;
		}
/*
	| T_FUNCTION T_LABEL '(' function_args ')' ';' // это требуетс€ чтобы знать кака€ функци€ - внутренн€€ или внешн€€
		{
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, $2.string);
			free_str($2.string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_FUNC_INTERNAL;
			func->library = NULL;
		}
*/
	| T_FUNCTION T_LABEL '('
		{
			pp->current_level++;
		}
		function_args ')'
		{
			cl_label_node *func;

			cl_label_define(&pp->labels_table, "@exit_func");

			func = cl_label_define(&pp->funcs_table, $2.string);
			free_str($2.string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_FUNC_INTERNAL | ZLF_DEFINED;
			func->offset = pp->hc_fill;
			//func->params_size = $4.size;

			pp->current_level--;
			pp->stack_size = 4;

			//cl_push(pp, OP_DBG_PRINT);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBP);
			cl_push(pp, OP_MOV_REG_REG); cl_push(pp, REG_EBP); cl_push(pp, REG_ESP);
			cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_ESP);
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
			cl_push_dw(pp, 0);
			//cl_push(pp, OP_NOP);
		}
		compound_statement
		{
			cl_label_node *label;
			unsigned long fix_point;

			label = cl_label_find(pp->labels_table, "@exit_func");

			label->offset = pp->hc_fill;
			label->flags = ZLF_DEFINED;

			//cl_push(pp, OP_NOP);
			pp->stack_size -= 4;
			cl_push(pp, OP_SUB_REG_IMM); cl_push(pp, REG_ESP); cl_push_dw(pp, pp->stack_size);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EBP);
			//cl_push(pp, OP_DBG_PRINT);
			cl_push(pp, OP_RET);
			cl_push(pp, OP_EOF);

			//cl_push(pp, OP_NOP);
			//cl_push(pp, OP_NOP);
			//cl_push(pp, OP_NOP);

			fix_point = (unsigned long) cl_stack_pop(&pp->cl_stack);

			pp->hard_code[fix_point] = (char)((unsigned long) (pp->stack_size & 0xFF));
			pp->hard_code[fix_point+1] = (char)((unsigned long) ((pp->stack_size >> 8) & 0xFF));
			pp->hard_code[fix_point+2] = (char)((unsigned long) ((pp->stack_size >> 16) & 0xFF));
			pp->hard_code[fix_point+3] = (char)((unsigned long) (pp->stack_size >> 24));

			if(cl_label_fix(pp->labels_table, pp->hard_code))
			{
				ZL_ERROR("error, jump to undefined label");
			}

			cl_label_free(&pp->labels_table);
		}
;

function_args
	: function_arg
		{
			$$.value = 8;
			$1.var->offset = $$.value;
			$1.var->flags |= ZLF_PARAM;
		}
	| function_arg ',' function_args
		{
			$$.value = $3.value + 4;
			$1.var->offset = $$.value;
			$1.var->flags |= ZLF_PARAM;
		}
	| /* empty - function does not have arguments */
;

function_arg
	: declaration_specifiers
		{
			if(($1.flags & ZLF_EXTERNAL) && (pp->current_level > 0))
			{
				ZL_ERROR("external variable must be defined global");
			}

			if(!($1.flags & ZLF_TYPE))
			{
				$1.flags |= ZLF_INT;	// use default type
			}

			cl_stack_push(&pp->cl_stack, (void *) $1.flags);
		}
		declarator
		{
			if($3.var->flags & ZLF_AUTOARRAY)
			{
				//ZL_ERROR("unknown size");
				/*
				$3.var->flags += 0x01000000;
				$3.var->rows--;
				if(!$3.var->rows)
				{
					$3.var->flags &= ~ZLF_ARRAY;
				}
				*/
				$3.var->size[1] = 1;
			}

			if($3.var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				//$3.var->flags += 0x01000000;
				//$3.var->size[0] = 4;

				level = $3.var->rows;

				$3.var->size[level] *= $3.var->size[0];
				level--;

				while(level)
				{
					$3.var->size[level] *= $3.var->size[level+1];
					level--;
				}
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = $3.var->rows;

			while(dc < uc)
			{
				x = $3.var->size[dc];
				$3.var->size[dc] = $3.var->size[uc];
				$3.var->size[uc] = x;

				dc++;
				uc--;
			}

			$3.var->references = 0;

			$$ = $3;

			cl_stack_pop(&pp->cl_stack);
		}
;

declaration_statement
	: declaration_specifiers ';'								{ /* nothing to do */ }
	| declaration_specifiers
		{
			if(($1.flags & ZLF_EXTERNAL) && (pp->current_level > 0))
			{
				ZL_ERROR("external variable must be defined global");
			}

			if(!($1.flags & ZLF_TYPE))
			{
				$1.flags |= ZLF_INT;	// use default type
			}

			cl_stack_push(&pp->cl_stack, (void *) $1.flags);
		}
		init_declarator_list ';'
		{
			cl_stack_pop(&pp->cl_stack);
		}
;

declaration_specifiers
	: type_specifier											{ $$.flags = $1.flags }
	| type_specifier declaration_specifiers
		{
			$$.flags = $1.flags | $2.flags;
			if(($$.flags & (ZLF_SIGNED | ZLF_UNSIGNED)) == (ZLF_SIGNED | ZLF_UNSIGNED))
			{
				ZL_ERROR("signed/unsigned keywords mutually exclusive");
			}

			if(($$.flags & (ZLF_EXTERNAL | ZLF_INTERNAL)) == (ZLF_EXTERNAL | ZLF_INTERNAL))
			{
				ZL_ERROR("internal/external keywords mutually exclusive");
			}

			if(($$.flags & (ZLF_SHORT | ZLF_LONG)) == (ZLF_SHORT | ZLF_LONG))
			{
				ZL_ERROR("short/long keywords mutually exclusive");
			}

			if(($$.flags & (ZLF_SHORT | ZLF_LONG)) && ($$.flags & ZLF_TYPE) && !($$.flags & ZLF_INT))
			{
				ZL_ERROR("short/long keywords can not be used with none 'int' type");
			}

			if(($$.flags & (ZLF_SIGNED | ZLF_UNSIGNED)) && ($$.flags & ZLF_TYPE) && !($$.flags & (ZLF_INT|ZLF_CHAR)))
			{
				ZL_ERROR("signed/unsigned keywords can not be used with none 'int/char' type ");
			}

			if(($1.flags & ZLF_TYPE) && ($2.flags & ZLF_TYPE))
			{
				ZL_ERROR("type specifier followed by type specifier is illegal");
			}
		}
;

init_declarator_list
	: init_declarator											{ /* nothing to do */ }
	| init_declarator_list ',' init_declarator					{ /* nothing to do */ }
;

init_declarator
	: declarator
		{
			if($1.var->flags & ZLF_AUTOARRAY)
			{
				ZL_ERROR("unknown size");
			}

			if($1.var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				level = $1.var->rows;

				$1.var->size[level] *= $1.var->size[0];
				level--;

				while(level)
				{
					$1.var->size[level] *= $1.var->size[level+1];
					level--;
				}

				//$1.var->size[0] = $1.var->size[1];
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = $1.var->rows;

			while(dc < uc)
			{
				x = $1.var->size[dc];
				$1.var->size[dc] = $1.var->size[uc];
				$1.var->size[uc] = x;

				dc++;
				uc--;
			}

			if((~$1.var->flags & ZLF_EXTERNAL) && ($1.var->level != 0))
			{
				$1.var->offset = pp->stack_size;
				pp->stack_size += $1.var->size[$1.var->rows];
				if($1.var->size[$1.var->rows] % 4)
				{
					pp->stack_size += 4 - ($1.var->size[$1.var->rows] % 4);
				}
			}

			$1.var->references = 0;

			/*
			if(!cl_var_define(&pp->vars_table, &$1.var))
			{
				ZL_ERROR("redefinition");
			}
			*/
		}
	| declarator '='
		{
			cl_var_node *var;

			if($1.var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				level = $1.var->rows;

				$1.var->size[level] *= $1.var->size[0];
				level--;

				while(level)
				{
					$1.var->size[level] *= $1.var->size[level+1];
					level--;
				}

				//$1.var->size[0] = $1.var->size[1];
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = $1.var->rows;

			while(dc < uc)
			{
				x = $1.var->size[dc];
				$1.var->size[dc] = $1.var->size[uc];
				$1.var->size[uc] = x;

				dc++;
				uc--;
			}

			$1.var->references = 0;

			var = $1.var;
			/*
			var = cl_var_define(&pp->vars_table, &$1.var);
			if(!var)
			{
				ZL_ERROR("redefinition");
			}
			*/

			cl_stack_push(&pp->cl_stack, (void *) var);

			if($1.var->flags & ZLF_EXTERNAL)
			{
				cl_push(pp, OP_PUSH_PMEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else if($1.var->level == 0) // global var
			{
				cl_push(pp, OP_PUSH_MEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else // local var
			{
				var->offset = pp->stack_size;

				cl_push(pp, OP_MOV_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EBP);
				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, var->offset);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
			}
		}
		initializer
		{
			cl_var_node *var;

			var = (cl_var_node *) pp->cl_stack->data;
			if(var->flags & ZLF_AUTOARRAY)
			{
				if(($4.size % var->size[var->rows-1]) > 0)
				{
					var->size[var->rows] = var->size[var->rows-1] - ($4.size % var->size[var->rows-1]) + $4.size;
				}
				else
				{
					var->size[var->rows] = $4.size;
				}
				// debug
				//printf("%s = (%u - (%u %% %u) + %u) = %u\n", $1.var->name, $1.var->size[$1.var->rows-1], $4.size, $1.var->size[$1.var->rows-1], $4.size, $1.var->size[$1.var->rows]);
				//for(int i = 0; i <= $1.var->rows; i++)
				//{
				//	printf("[%u]", $1.var->size[i]);
				//}
				//printf("\n");
			}
			else if(var->flags & ZLF_ARRAY)
			{
				if(var->size[var->rows] < $4.size)
				{
					ZL_ERROR("too many initializers");
				}
			}

			if((~var->flags & ZLF_EXTERNAL) && (var->level != 0))
			{
				pp->stack_size += var->size[var->rows];
				if($1.var->size[$1.var->rows] % 4)
				{
					pp->stack_size += 4 - ($1.var->size[$1.var->rows] % 4);
				}
			}

			cl_stack_pop(&pp->cl_stack);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		}
;

pointer
	: '*'														{ $$.flags = 0x01000000 }
	| '*' pointer												{ $$.flags = 0x01000000 + $2.flags }
;

declarator
	: pointer direct_declarator
		{
			$$ = $2;
			//memcpy(&$$, &$2, sizeof($$));

			$$.var->flags += $1.flags;
			$$.var->size[0] = 4;
		}
	| direct_declarator
		{
			$$ = $1;

			if($$.var->flags & ZLF_VOID)
			{
				ZL_ERROR("illegal use of type 'void'");
			}
		}
;

direct_declarator
	: T_LABEL
		{
			$$.var = cl_var_define(&pp->vars_table, $1.string, pp->current_level);

			if(!$$.var)
			{
				ZL_ERROR("redefinition");
			}

			$$.var->name = $1.string;
			$$.var->flags = (unsigned long) pp->cl_stack->data;
			$$.var->level = pp->current_level;
			$$.var->rows = 0;
			$$.var->size[0] = 4; // default var size

			switch($$.var->flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					$$.var->size[0] = 1;
					break;
				case ZLF_INT:
					if($$.var->flags & ZLF_SHORT)
					{
						$$.var->size[0] = 2;
					}
					break;
				case ZLF_DOUBLE:
					$$.var->size[0] = 8;
					break;
			}
		}
	| direct_declarator '[' const_expr ']'
		{
			$$ = $1;

			if($1.var->flags & ZLF_ARRAY)
			{
				//ZL_ERROR("sorry, multi-level arrays not yet supported!");
			}
			
			if($3.uvalue == 0)
			{
				ZL_ERROR("cannot allocate an array of constant size 0.");
			}

			$$.var->flags |= ZLF_ARRAY;
			$$.var->rows++;
			$$.var->size[$$.var->rows] = $3.uvalue; // кол-во элементов в строке массива
			// уровнем выше делать перерасчЄт кол-ва элементов в размер строки массива
		}
	| direct_declarator '[' ']'
		{
			// last array level with auto size
			$$ = $1;

			if($1.var->flags & ZLF_ARRAY)
			{
				ZL_ERROR("unknown size, missing subscript!");
			}

			$$.var->flags |= ZLF_ARRAY | ZLF_AUTOARRAY;
			$$.var->rows++;
			$$.var->size[$$.var->rows] = 0; // set after initialize
		}
	| '(' declarator ')'
		{
			$$ = $2;
		}
;

initializer
	: assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			$$.size = 4;

			if(!(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_POINTER))
			{
				switch(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						$$.size = 1;
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_SHORT)
						{
							$$.size = 2;
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						$$.size = 8;
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);
		}
	| '{' initializer_list '}'
		{
			$$ = $2;
		}
;

initializer_list
	: initializer
		{
			//if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_AUTOARRAY)
			//{
				//((zl_names_map *) pp->cl_stack->data)->elements->size += var_size;
			//}
		}
	| initializer ','
		{
			if(~((cl_var_node *) pp->cl_stack->data)->flags & ZLF_ARRAY)
			{
				ZL_ERROR("too many initializers");
			}

			// check array for overflow here or error!

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);

			if($1.size > 1)
			{
				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_EAX); cl_push_dw(pp, $1.size);
			}
			else
			{
				cl_push(pp, OP_INC_REG); cl_push(pp, REG_EAX);
			}

			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);

			//if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_AUTOARRAY)
			{
				//((zl_names_map *) pp->cl_stack->data)->elements->size += var_size;
			}
		}
		initializer_list
		{
			$$.size = $1.size + $4.size;
		}
;

/*
struct_specifier
	: T_STRUCT T_LABEL '{' struct_declaration_list '}'			{ free_str($2.string); }
;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
;

specifier_qualifier_list
	: type_specifier
	| type_specifier specifier_qualifier_list
;

struct_declarator_list
	: declarator
	| struct_declarator_list ',' declarator
;
//*/

type_specifier
	: T_VOID													{ $$.flags = ZLF_VOID }
	| T_CHAR													{ $$.flags = ZLF_CHAR }
	| T_SHORT													{ $$.flags = ZLF_SHORT }
	| T_INT														{ $$.flags = ZLF_INT }
	| T_LONG													{ $$.flags = ZLF_LONG }
	| T_FLOAT													{ $$.flags = ZLF_FLOAT }
	| T_DOUBLE													{ $$.flags = ZLF_DOUBLE }
	| T_SIGNED													{ $$.flags = ZLF_SIGNED }
	| T_UNSIGNED												{ $$.flags = ZLF_UNSIGNED }
	| T_EXTERN													{ $$.flags = ZLF_EXTERNAL }
	| T_VAR														{ $$.flags = ZLF_INTERNAL }
//	| struct_specifier											{ $$.flags = ZLF_STRUCT }
//	| T_LABEL													{ cl_find_in_struct_list() ;$$.flags = ZLF_STRUCT; free_str($1.string); }
;

expression
	: assignment_expression
	| assignment_expression ',' expression						{ cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); }
;															
															
argument_expression_list									
	: /* empty */												{ $$.size = 0; }
	| assignment_expression										{ $$.size = 4; }
	| assignment_expression ',' argument_expression_list		{ $$.size = 4 + $3.size; }
;															
															
assignment_expression										
	: expr													
	| unary_expression '=' assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
		}
	| unary_expression T_ADD_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
		}
	| unary_expression T_SUB_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_SUB_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_MUL_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_DIV_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_DIV_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_MOD_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_MOD_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_SHL_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_SHL_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_SHR_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_SHR_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_AND_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_AND_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_XOR_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_XOR_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
	| unary_expression T_OR_ASSIGN assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!($1.flags & ZLF_POINTER))
			{
				switch($1.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if($1.flags & ZLF_SHORT)
						{
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_OR_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
		}
;

unary_expression
	: T_LABEL													
		{
			// push address of value to stack

			cl_var_node *var;

			$$ = $1;

			var = cl_var_find(pp->vars_table, $1.string);
			free_str($1.string);
			if(!var)
			{
				ZL_ERROR("undeclared identifier");
			}

			//memcpy(&$$.var, var, sizeof(cl_var_node));
			$$.var = var;
			$$.flags = var->flags;
			$$.rows = var->rows;

			if($$.flags & ZLF_EXTERNAL)
			{
				cl_push(pp, OP_PUSH_PMEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else if($$.var->level == 0) // global var
			{
				cl_push(pp, OP_PUSH_MEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else // local var
			{
				cl_push(pp, OP_MOV_REG_REG); cl_push(pp, REG_EBX); cl_push(pp, REG_EBP);
				if(var->flags & ZLF_PARAM)
				{
					cl_push(pp, OP_SUB_REG_IMM);
				}
				else
				{
					cl_push(pp, OP_ADD_REG_IMM);
				}
				cl_push(pp, REG_EBX); cl_push_dw(pp, var->offset);

				if(($$.flags & (ZLF_PARAM | ZLF_ARRAY)) == (ZLF_PARAM | ZLF_ARRAY))
				{
					// ссылка на массив переданна€ через параметр функции
					cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_EBX);
				}

				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
			}
		}
	| unary_expression '[' assignment_expression ']'
		{
			// pop address
			// add to address offset
			// push address this address to stack

			// ul *v = {0,1,2};
			// v[1]; //= 1

			$$.flags = $1.flags;

			if($$.flags & ZLF_ARRAY)
			{
				unsigned long ptr_step;
				ptr_step = 4;
				
				$$.rows--;

				if($$.rows)
				{
					ptr_step = $$.var->size[$$.rows];
				}
				else
				{
					$$.flags &= ~ZLF_ARRAY;

					if(!($$.flags & ZLF_POINTER))
					{
						ptr_step = $$.var->size[0];
					}
				}

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				if(ptr_step > 1)
				{
					cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, ptr_step);
					cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
				}
				cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			}
			else if($$.flags & ZLF_POINTER)
			{
				$$.flags -= 0x01000000;

				if(!($$.flags & ZLF_POINTER))
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							$$.var->size[0] = 1;
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								$$.var->size[0] = 2;
							}
							break;
						case ZLF_DOUBLE:
							$$.var->size[0] = 8;
							break;
					}
				}

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, $$.var->size[0]);
				cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
				cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			}
			else
			{
				ZL_ERROR("subscript requires array or pointer type");
			}
		}
// end comment */
//	| unary_expression '.' T_LABEL
//	| unary_expression T_PTR_OP T_LABEL
//	| cast_expression											// not work here
;

expr
//	: const_expr												{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value) }
	: T_CONSTANT_LONG											{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value) }
//	| T_CONSTANT_DOUBLE											{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value) }
	| T_CONSTANT_STRING											
		{
			cl_push(pp, OP_PUSH_OFFSET);
			cl_const_define(&pp->data_table, $1.string, $1.size, pp->hc_fill); // добавл€ем ссылку на эти данные
			free_str($1.string);
			cl_push_dw(pp, 0);						// ставим заглушку
		}
	| '(' expr ')'												{ $$ = $2; }
	| expr '+' expr												{ cl_do_op(pp, OP_ADD_STK_STK, &$$, &$1, &$3) }
	| expr '-' expr												{ cl_do_op(pp, OP_SUB_STK_STK, &$$, &$1, &$3) }
	| expr '*' expr												{ cl_do_op(pp, OP_MUL_STK_STK, &$$, &$1, &$3) }
	| expr '/' expr												{ cl_do_op(pp, OP_DIV_STK_STK, &$$, &$1, &$3) }
	| expr '|' expr												{ cl_do_op(pp, OP_OR_STK_STK, &$$, &$1, &$3) }
	| expr '&' expr												{ cl_do_op(pp, OP_AND_STK_STK, &$$, &$1, &$3) }
	| expr '%' expr												{ cl_do_op(pp, OP_MOD_STK_STK, &$$, &$1, &$3) }
	| expr '^' expr												{ cl_do_op(pp, OP_XOR_STK_STK, &$$, &$1, &$3) }
	| expr T_SHL expr											{ cl_do_op(pp, OP_SHL_STK_STK, &$$, &$1, &$3) }
	| expr T_SHR expr											{ cl_do_op(pp, OP_SHR_STK_STK, &$$, &$1, &$3) }
	| expr '>' expr												{ cl_do_op(pp, OP_G_STK_STK, &$$, &$1, &$3) }
	| expr '<' expr												{ cl_do_op(pp, OP_L_STK_STK, &$$, &$1, &$3) }
	| expr T_GE expr											{ cl_do_op(pp, OP_GE_STK_STK, &$$, &$1, &$3) }
	| expr T_LE expr											{ cl_do_op(pp, OP_LE_STK_STK, &$$, &$1, &$3) }
	| expr T_EQ expr											{ cl_do_op(pp, OP_E_STK_STK, &$$, &$1, &$3) }
	| expr T_NE expr											{ cl_do_op(pp, OP_NE_STK_STK, &$$, &$1, &$3) }
	| expr T_OR
		{
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			//lb_skip->offset = pp->hc_fill;
			//lb_skip->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JNZ_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		}
		expr
		{
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		}
	| expr T_AND
		{
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			//lb_skip->offset = pp->hc_fill;
			//lb_skip->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		}
		expr
		{
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		}
	| expr '?'
		{
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		expr ':'
		{
			cl_label_node *lb_skip, *lb_else;
			lb_else = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_define(&pp->labels_table, NULL);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			lb_else->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_stack, lb_skip);
		}
		expr
		{
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		}
	| '~' expr													{ $$ = $2; cl_push(pp, OP_BNOT_STK); }
	| '!' expr													{ $$ = $2; cl_push(pp, OP_LNOT_STK); }
	| '+' expr	%prec T_PLUS									{ $$ = $2; }
	| '-' expr	%prec T_MINUS
		{
			$$ = $2;
			$$.flags &= ~ZLF_UNSIGNED;
			$$.flags |= ZLF_SIGNED;
			cl_push(pp, OP_MINUS_STK);
		}
	| '&' unary_expression	%prec T_POINTER						{ /* nothing to do */ }
	| '*' expr	%prec T_POINTER
		{
			$$ = $2;
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			if(!($2.flags & ZLF_POINTER))
			{
				ZL_ERROR("illegal indirection");
			}

			$$.flags -= 0x01000000;
		}
	| unary_expression T_INC
		{
			if($1.flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);

			if($1.flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if(($1.flags & ZLF_POINTER) == 0x01000000)
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								ptr_step = 2;
							}
							break;
						case ZLF_DOUBLE:
							ptr_step = 8;
							break;
					}
				}

				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, ptr_step);
			}
			else
			{
				cl_push(pp, OP_INC_REG); cl_push(pp, REG_ECX);
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
		}
	| unary_expression T_DEC
		{
			if($1.flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);

			if($1.flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if(($1.flags & ZLF_POINTER) == 0x01000000)
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								ptr_step = 2;
							}
							break;
						case ZLF_DOUBLE:
							ptr_step = 8;
							break;
					}
				}

				cl_push(pp, OP_SUB_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, ptr_step);
			}
			else
			{
				cl_push(pp, OP_DEC_REG); cl_push(pp, REG_ECX);
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
		}
	| T_INC unary_expression
		{
			if($2.flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);

			if($2.flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if(($1.flags & ZLF_POINTER) == 0x01000000)
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								ptr_step = 2;
							}
							break;
						case ZLF_DOUBLE:
							ptr_step = 8;
							break;
					}
				}

				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, ptr_step);
			}
			else
			{
				cl_push(pp, OP_INC_REG); cl_push(pp, REG_ECX);
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);
		}
	| T_DEC unary_expression
		{
			if($2.flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);

			if($2.flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if(($1.flags & ZLF_POINTER) == 0x01000000)
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								ptr_step = 2;
							}
							break;
						case ZLF_DOUBLE:
							ptr_step = 8;
							break;
					}
				}

				cl_push(pp, OP_SUB_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, ptr_step);
			}
			else
			{
				cl_push(pp, OP_DEC_REG); cl_push(pp, REG_ECX);
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);
		}
	| T_LABEL '(' argument_expression_list ')'
		{
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, $1.string);
			free_str($1.string);

			if(~func->flags & ZLF_DEFINED)
			{
				func->flags = ZLF_FUNC_INTERNAL;
				func->library = NULL;
			}

			// - OR - then function must be predefined before call
			/*
			func = cl_label_find(pp->funcs_table, $1.string);
			free_str($1.string);
			if(!func)
			{
				ZL_ERROR("undeclared identifier");
			}
			*/

			if(func->flags & ZLF_FUNC_INTERNAL)
			{
				cl_push(pp, OP_CALL); // call near
			}
			else
			{
				cl_push(pp, OP_RCALL_FAR); // rcall far
			}
			cl_label_reference(func, pp->hc_fill);
			cl_push_dw(pp, 0);
			if($3.size)
			{
				cl_push(pp, OP_SUB_REG_IMM); cl_push(pp, REG_ESP); cl_push_dw(pp, $3.size);
			}
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
		}
	| unary_expression
		{
			// get value from address and push
			if(!($1.flags & (ZLF_STRUCT | ZLF_ARRAY)))
			{
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);

				if(!($1.flags & ZLF_POINTER))
				{
					switch($1.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, 0x000000FF);
							cl_push(pp, OP_AND_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
							break;
						case ZLF_INT:
							if($1.flags & ZLF_SHORT)
							{
								cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, 0x0000FFFF);
								cl_push(pp, OP_AND_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
							}
							break;
						case ZLF_DOUBLE:
							ZL_ERROR("sorry, double not yet supported");
							break;
					}
				}

				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			}
		}
	| T_SIZEOF '(' declaration_specifiers ')'
		{
			$$.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			unsigned long var_size;
			switch($3.flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if($3.flags & ZLF_SHORT)
					{
						var_size = 2;
					}
					else
					{
						var_size = 4;
					}
					break;
				case ZLF_DOUBLE:
					var_size = 8;
					break;
			}

			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, var_size);
		}
	| T_SIZEOF '('
		{
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		}
		unary_expression ')'
		{
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
			$$.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $4.var->size[$4.rows]);
		}
//*/
//	| '(' specifier_qualifier_list ')' unary_expression
//	| unary_expression '(' argument_expression_list ')'
;

const_expr
	: T_CONSTANT_LONG											{ /* nop */ }
//	| T_CONSTANT_DOUBLE											{ /* nop */ }
//	| T_CONSTANT_STRING											{ /* incorrect */ }
	| '(' const_expr ')'										{ /* nop */ }
	| const_expr '+' const_expr									{ $$.value = $1.value + $3.value; }
	| const_expr '-' const_expr									{ $$.value = $1.value - $3.value; }
	| const_expr '*' const_expr									{ $$.value = $1.value * $3.value; }
	| const_expr '/' const_expr									{ $$.value = $1.value / $3.value; }
	| const_expr '|' const_expr									{ $$.value = $1.value | $3.value; }
	| const_expr '&' const_expr									{ $$.value = $1.value & $3.value; }
	| const_expr '%' const_expr									{ $$.value = $1.value % $3.value; }
	| const_expr '^' const_expr									{ $$.value = $1.value ^ $3.value; }
	| const_expr T_SHL const_expr								{ $$.value = $1.value << $3.value; }
	| const_expr T_SHR const_expr								{ $$.value = $1.value >> $3.value; }
	| const_expr '>' const_expr									{ $$.value = $1.value < $3.value; }
	| const_expr '<' const_expr									{ $$.value = $1.value > $3.value; }
	| const_expr T_GE const_expr								{ $$.value = $1.value >= $3.value; }
	| const_expr T_LE const_expr								{ $$.value = $1.value <= $3.value; }
	| const_expr T_EQ const_expr								{ $$.value = $1.value == $3.value; }
	| const_expr T_NE const_expr								{ $$.value = $1.value != $3.value; }
	| const_expr T_OR const_expr								{ $$.value = $1.value || $3.value; }
	| const_expr T_AND const_expr								{ $$.value = $1.value && $3.value; }
	| const_expr '?' const_expr ':' const_expr					{ $$.value = $1.value ? $3.value : $5.value; }
	| '~' const_expr											{ $$.value = ~$2.value; }
	| '!' const_expr											{ $$.value = !$2.value; }
	| '+' const_expr	%prec T_PLUS							{ /* nothing to do */ }
	| '-' const_expr	%prec T_MINUS							{ $$.value = -$2.value; }
	| T_SIZEOF '(' declaration_specifiers ')'
		{
			unsigned long var_size;
			switch($3.flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if($3.flags & ZLF_SHORT)
					{
						var_size = 2;
					}
					else
					{
						var_size = 4;
					}
					break;
				case ZLF_DOUBLE:
					var_size = 8;
					break;
			}

			$$.value = var_size;
		}
	| T_SIZEOF '('
		{
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		}
		unary_expression ')'
		{
			$$.value = $4.var->size[$4.rows];
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
		}
;







%%









void yyerror(void *scanner, cl_parser_params *pp, char *err)
{
	pp->error_msg = cl_sprintf("zlc: error at line %d: %s", pp->lineno, err);
}

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
)
{
	int ret;
	void *scanner;
	cl_parser_params pp;
	cl_label_node *func;

	memset(&pp, 0, sizeof(cl_parser_params));

	pp.source_code = code;
	pp.sc_length = strlen(code);
	pp.lineno = 1;


	func = cl_label_define(&pp.funcs_table, "main");

	func->flags = ZLF_FUNC_INTERNAL;
	func->library = NULL;
	
	cl_push(&pp, OP_CALL);
	cl_label_reference(func, pp.hc_fill);
	cl_push_dw(&pp, 0);
	cl_push(&pp, OP_EOF);

	yylex_init(&scanner);
	yyset_extra(&pp, scanner);
	ret = yyparse(scanner, &pp);
	yylex_destroy(scanner);

	cl_push(&pp, OP_EOF);

	if(error_msg)
	{
		*error_msg = pp.error_msg;
	}
	else if(ret)
	{
		free_str(pp.error_msg);
	}

	*hard_code_size = pp.hc_fill;

	if(!ret && cl_label_fix(pp.funcs_table, pp.hard_code))
	{
		ret = 1;
		if(error_msg)
		{
			*error_msg = alloc_string("zlc: error, jump to undefined label");
		}
	}


	cl_link_sections(0, pp.data_table, pp.vars_table, pp.funcs_table, pp.hard_code, pp.hc_fill,
		const_sect, const_size,
		data_sect, data_size,
		reloc_sect, reloc_size,
		import_sect, import_size,
		export_sect, export_size,
		map_sect, map_size);

	cl_label_free(&pp.labels_table);
	cl_label_free(&pp.funcs_table);
	cl_const_free(&pp.data_table);
	cl_var_free_level(&pp.vars_table, 0);

	cl_stack_free(&pp.cl_stack);
	cl_stack_free(&pp.cl_loop_stack);

	*hardcode = pp.hard_code;

	return ret;
}

