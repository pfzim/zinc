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

%token T_GOTO
%token T_SIZEOF

%token T_REGISTER
%token T_OPERATOR

%token T_ASM

%left T_PARAM_SEPARATOR

%token T_NOP

%start statement_list

%%

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
	: '{' statement_list '}'
;

assembler_statement
	: T_ASM assembler_command_list '}'
;

assembler_command_list
	: assembler_command '\n' assembler_command_list
	| labeled_statement assembler_command_list
	| assembler_command
;

assembler_command
	: T_GOTO T_LABEL											{ cl_push(pp, (unsigned char) $1.value); cl_jump_define(&pp->jumps_table, $2.string, pp->hc_fill); free_str($2.string); cl_push_dw(pp, 0) }
	| T_OPERATOR operator_parameter ',' operator_parameter		{ cl_push_op(pp, (unsigned char) $1.value, &$2, &$4); }
	| T_OPERATOR operator_parameter 							{ cl_push_op(pp, (unsigned char) $1.value, &$2, NULL) }
	| T_OPERATOR 												{ cl_push_op(pp, (unsigned char) $1.value, NULL, NULL) }
	| T_LABEL '(' assembler_argument_list ')' 					{ cl_push_op(pp, OP_CALL, &$1, NULL); free_str($1.string); }
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
	| T_CONSTANT_LONG											{ $$ = $1; $$.flags = ARG_IMM }
//	| T_CONSTANT_DOUBLE											{ $$ = $1; $$.flags = ARG_IMM8 }
	| T_CONSTANT_STRING											{ $$ = $1; $$.flags = ARG_DATA; }
	| T_LABEL													{ $$ = $1; $$.flags = ARG_MEM; $$.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, $1.string); free_str($1.string); if($$.uvalue == 1) ZL_ERROR("undeclared identifier");}
	| '[' T_LABEL ']'											{ $$ = $1; $$.flags = ARG_PMEM; $$.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, $2.string); free_str($2.string); if($$.uvalue == 1) ZL_ERROR("undeclared identifier"); }
	| '[' T_REGISTER ']'										{ $$ = $1; $$.flags = ARG_PREG }
;

expression_statement
	: ';'
	| expression ';'											{ cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); }
;

buildin_statement
	: T_NOP ';'													{ cl_push(pp, OP_NOP); }
	| T_BREAK ';'
		{
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack) ZL_ERROR("break not in loop");
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) pp->cl_loop_stack->next_node->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
	| T_CONTINUE ';'
		{
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack || !pp->cl_loop_stack->next_node) ZL_ERROR("continue not in loop");
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) pp->cl_loop_stack->data)->name, pp->hc_fill);
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
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) lsb->data)->name, pp->hc_fill);
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
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) lsb->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
;

selection_statement
	: T_IF '(' assignment_expression ')'
		{
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, 0);
			cl_stack_push(&pp->cl_stack, lb_skip);

			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			//cl_push(pp, OP_TEST_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		statement
		else_statement
		{
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			//cl_label_define(&pp->labels_table, ((zl_names_map *)cl_stack_pop(&pp->cl_stack))->name, pp->hc_fill);
		}
	| T_WHILE
		{
			zl_names_map *lb_exit, *lb_condition;
			lb_exit = cl_label_new(&pp->labels_table, 0);
			lb_condition = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
		}
		'(' assignment_expression ')'
		{
			zl_names_map *lb_exit;
			lb_exit= (zl_names_map *) pp->cl_loop_stack->next_node->data;
			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_exit->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		statement
		{
			zl_names_map *lb_exit, *lb_condition;
			lb_condition = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_condition->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
		}
	| T_DO
		{
			zl_names_map *lb_exit, *lb_condition, *lb_start;
			lb_exit = cl_label_new(&pp->labels_table, 0);
			lb_condition = cl_label_new(&pp->labels_table, 0);
			lb_start = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
			cl_stack_push(&pp->cl_stack, lb_start);				// start loop
		}
		statement
		T_WHILE
		{
			zl_names_map *lb_condition;
			lb_condition = (zl_names_map *) pp->cl_loop_stack->data;
			lb_condition->offset = pp->hc_fill;
		}
		'(' assignment_expression ')'
		{
			zl_names_map *lb_exit, *lb_condition;
			lb_condition = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) cl_stack_pop(&pp->cl_stack))->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
		}';'
	| T_FOR '(' expression ';'
		{
			zl_names_map *lb_exit, *lb_loop, *lb_condition;
			lb_exit = cl_label_new(&pp->labels_table, 0);
			lb_loop = cl_label_new(&pp->labels_table, 0);

			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit
			cl_stack_push(&pp->cl_loop_stack, lb_loop);			// loop

			cl_push(pp, OP_POP_REG);
			cl_push(pp, REG_EAX);

			lb_condition = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_stack, lb_condition);
		}
		expression ';'
		{
			zl_names_map *lb_loop, *lb_exit, *lb_statement;

			lb_statement = cl_label_new(&pp->labels_table, 0);
			lb_loop = (zl_names_map *) pp->cl_loop_stack->data;
			lb_exit = (zl_names_map *) pp->cl_loop_stack->next_node->data;

			cl_stack_push(&pp->cl_stack, lb_statement);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_statement->name, pp->hc_fill);
			cl_push_dw(pp, 0);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_exit->name, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_loop->offset = pp->hc_fill;
		}
		expression ')'
		{
			zl_names_map *lb_condition, *lb_statement;
			lb_statement = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_condition = (zl_names_map *) cl_stack_pop(&pp->cl_stack);

			cl_push(pp, OP_POP_REG);
			cl_push(pp, REG_EAX);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_condition->name, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_statement->offset = pp->hc_fill;
		}
		statement
		{
			zl_names_map *lb_exit, *lb_loop;
			lb_loop = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_loop->name, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_exit->offset = pp->hc_fill;
		}
;

else_statement
	: /* empty */
	| T_ELSE
		{
			zl_names_map *lb_skip, *lb_else;
			lb_else = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_new(&pp->labels_table, 0);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			cl_stack_push(&pp->cl_stack, lb_skip);
		}
		statement
;

labeled_statement
	: T_LABEL ':' /*statement*/
		{
			zl_names_map *label;
			label = cl_label_define(&pp->labels_table, $1.string, pp->hc_fill);
			free_str($1.string);
			if(!label)
			{
				ZL_ERROR("label redefined");
			}
		}
;

jump_statement
	: T_GOTO T_LABEL ';'										{ cl_push(pp, (unsigned char) $1.value); cl_jump_define(&pp->jumps_table, $2.string, pp->hc_fill); free_str($2.string); cl_push_dw(pp, 0) }
;

declaration_statement
	: declaration_specifiers ';'								{ /* nothing to do */ }
	| declaration_specifiers
		{
			if(!($1.flags & ZLF_TYPE))
			{
				$1.flags |= ZLF_INT;
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
			if(($1.flags & ZLF_AUTOARRAY))
			{
				ZL_ERROR("unknown size");
			}

			if($1.flags & ZLF_ARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long var_size;
				var_size = 4;

				if(!($1.flags & ZLF_POINTER))
				{
					switch($1.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if($1.flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							ZL_ERROR("sorry, double not yet supported");
							break;
					}
				}

				back_loop = 1;
				el = $1.var_info->elements;
				while(el->next_node)
				{
					back_loop++;
					el = el->next_node;				
				}

				back_loop--;

				el->size *= var_size;
				//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);

				if(back_loop > 0)
				{
					while(back_loop)
					{
						back_loop--;
						i = back_loop;
						el = $1.var_info->elements;
						while(i)
						{
							i--;
							el = el->next_node;
						}
						el->size *= el->next_node->size;
						//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);
					}
				}

				//$$.var_info->elements->size 

				/*
				printf("(%u) %s", $1.var_info->size, $1.var_info->name);
				el = $1.var_info->elements;
				while(el)
				{
					printf("[%u]", el->size);
					el = el->next_node;				
				}
				printf("\n");
				//*/
			}
		}
	| declarator '='
		{
			cl_stack_push(&pp->cl_stack, (void *) $1.var_info);
			cl_push(pp, OP_PUSH_PMEM); cl_push_dw(pp, $1.value*4);
		}
		initializer
		{
			$1.flags = ((zl_names_map *) pp->cl_stack->data)->flags;
			if($1.flags & ZLF_AUTOARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long level_size;
				unsigned long var_size;
				var_size = 4;

				if(!($1.flags & ZLF_POINTER))
				{
					switch($1.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if($1.flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							ZL_ERROR("sorry, double not yet supported");
							break;
					}
				}

				back_loop = 1;
				el = $1.var_info->elements;
				while(el->next_node)
				{
					back_loop++;
					//level_size *= el->size;
					el = el->next_node;				
				}

				back_loop--;

				if(back_loop > 0)
				{
					el->size *= var_size;
					//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);
					level_size = el->size;
					back_loop--;
					while(back_loop)
					{
						i = back_loop;
						el = $1.var_info->elements;
						while(i)
						{
							i--;
							el = el->next_node;
						}
						el->size *= el->next_node->size;
						level_size = el->size;
						//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);

						back_loop--;
					}
				}
				else
				{
					level_size = $1.var_info->elements->size;
				}

				/*
				el = $1.var_info->elements->next_node;
				level_size = 1;
				while(el)
				{
					level_size *= el->size;
					el = el->next_node;
				}
				//*/

				// sizeof level 0 = var_size * level 0
				// sizeof level 1 = sizeof level 0 * level 1
				// sizeof level 2 = sizeof level 2 * level 1
				// ...
				// size of array = size of level N-1 * level N

				//printf("elements->size = %u\n", $1.var_info->elements->size);
				$1.var_info->size = (level_size)?(level_size * ($1.var_info->elements->size / level_size) + (($1.var_info->elements->size % level_size)?(level_size):(0))):($1.var_info->elements->size);
				$1.var_info->elements->size = $1.var_info->size;
				//$1.var_info->elements->size = $1.var_info->size / (level_size * var_size) + (($1.var_info->size % (level_size * var_size))?1:0);
				//$1.var_info->size += (($1.var_info->size % (level_size * var_size))?((level_size*var_size) - $1.var_info->size % (level_size * var_size)):0);
				//printf("level_size = %u\n", level_size);
				//printf("size = %u\n", $1.var_info->size);
				//printf("elements->size = %u\n", $1.var_info->elements->size);
				//ZL_ERROR("debug");

				/*
				printf("%s", $1.var_info->name);
				el = $1.var_info->elements;
				while(el)
				{
					printf("[%u]", el->size);
					el = el->next_node;				
				}
				printf("\n");
				//*/
			}
			else if($1.flags & ZLF_ARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long level_size;
				unsigned long var_size;
				var_size = 4;

				if(!($1.flags & ZLF_POINTER))
				{
					switch($1.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if($1.flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							ZL_ERROR("sorry, double not yet supported");
							break;
					}
				}

				back_loop = 1;
				el = $1.var_info->elements;
				while(el->next_node)
				{
					back_loop++;
					//level_size *= el->size;
					el = el->next_node;				
				}

				back_loop--;

				if(back_loop > 0)
				{
					el->size *= var_size;
					level_size = el->size;
					//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);
					back_loop--;
					while(back_loop)
					{
						i = back_loop;
						el = $1.var_info->elements;
						while(i)
						{
							i--;
							el = el->next_node;
						}
						el->size *= el->next_node->size;
						level_size += el->size;
						//printf("%s[%u] = %u\n", $1.var_info->name, back_loop, el->size);

						back_loop--;
					}
				}
				else
				{
					level_size = $1.var_info->elements->size;
				}

				//$$.var_info->elements->size 
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
			$$.flags += $1.flags;
			$$.var_info->flags = $$.flags;
			
			if($$.flags & ZLF_ARRAY)
			{
				unsigned long var_size;
				var_size = 4;

				switch($$.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						var_size = 1;					
						break;
					case ZLF_INT:
						if($$.flags & ZLF_SHORT)
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

				$$.var_info->size = ($$.var_info->size/var_size)*4;
			}
			else
			{
				$$.var_info->size = 4;
			}
		}
	| direct_declarator
		{
			$$ = $1;

			if($$.flags & ZLF_VOID)
			{
				ZL_ERROR("illegal use of type 'void'");
			}
		}
;

direct_declarator
	: T_LABEL
		{
			$$.flags = (unsigned long) pp->cl_stack->data;
			$$.value = cl_var_define(&pp->vars_table, $1.string, $$.flags, &$$.var_info);
			free_str($1.string);
			if(!$$.value) ZL_ERROR("redefinition");
			$$.value--;

			unsigned long var_size;
			var_size = 4;

			switch($$.flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if($$.flags & ZLF_SHORT)
					{
						var_size = 2;
					}
					break;
				case ZLF_DOUBLE:
					var_size = 8;
					break;
			}

			$$.var_info->size = var_size;
		}
	| direct_declarator '[' const_expr ']'
		{
			$$ = $1;

			if($1.flags & ZLF_ARRAY)
			{
				//ZL_ERROR("sorry, multi-level arrays not yet supported!");
			}
			
			if($3.uvalue == 0)
			{
				ZL_ERROR("cannot allocate an array of constant size 0.");
			}

			$$.flags |= ZLF_ARRAY;
			$$.var_info->flags = $$.flags;
			$$.var_info->size *= $3.uvalue;
			
			zl_names_map *level;
			cl_var_define(&$$.var_info->elements, NULL, 0, &level);

			level->size = $3.uvalue;
		}
	| direct_declarator '[' ']'
		{
			// last array level with auto size
			$$ = $1;

			if($1.flags & ZLF_ARRAY)
			{
				ZL_ERROR("unknown size, missing subscript!");
			}

			$$.flags |= ZLF_ARRAY | ZLF_AUTOARRAY;
			$$.var_info->flags = $$.flags;

			cl_var_define(&$$.var_info->elements, NULL, 0, NULL);

			//$$.var_info->size = 0;
			$$.var_info->elements->size = 0;
		}
//	| '(' declarator ')'										{ $$ = $2; }
;

initializer
	: assignment_expression
		{
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_POINTER))
			{
				switch(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_SHORT)
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
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);
		}
	| '{' initializer_list '}'
;

initializer_list
	: initializer
		{
			if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_AUTOARRAY)
			{
				unsigned long var_size;
				var_size = 4;

				if(!(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_POINTER))
				{
					switch(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							ZL_ERROR("sorry, double not yet supported");
							break;
					}
				}

				//((zl_names_map *) pp->cl_stack->data)->size += var_size;
				((zl_names_map *) pp->cl_stack->data)->elements->size += var_size;
				//((zl_names_map *) pp->cl_stack->data)->elements->size++;
			}
		}
	| initializer ','
		{
			if(~((zl_names_map *) pp->cl_stack->data)->flags & ZLF_ARRAY)
			{
				ZL_ERROR("too many initializers");								
			}

			// check array for overflow here or error!

			unsigned long var_size;
			var_size = 4;

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);

			if(!(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_POINTER))
			{
				switch(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						var_size = 1;
						break;
					case ZLF_INT:
						if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_SHORT)
						{
							var_size = 2;
						}
						break;
					case ZLF_DOUBLE:
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}
			if(var_size > 1)
			{
				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_EAX); cl_push_dw(pp, var_size);
			}
			else
			{
				cl_push(pp, OP_INC_REG); cl_push(pp, REG_EAX);
			}

			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);

			if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_AUTOARRAY)
			{
				//((zl_names_map *) pp->cl_stack->data)->size += var_size;
				((zl_names_map *) pp->cl_stack->data)->elements->size += var_size;
				//((zl_names_map *) pp->cl_stack->data)->elements->size++;
			}

		}
		initializer_list
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
	: /* empty */											
	| assignment_expression										//{ printf("arg: %d\n", $1.value); }
	| assignment_expression ',' argument_expression_list		//{ printf("arg: %d\n", $1.value); }
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
			$$ = $1;

			$$.var_info = cl_var_find(pp->vars_table, $1.string, &$$.uvalue);
			free_str($1.string);
			if(!$$.var_info)
			{
				ZL_ERROR("undeclared identifier");
			}

			$$.flags = $$.var_info->flags;
			$$.size = $$.var_info->size;
			
			if($$.flags & ZLF_ARRAY)
			{
				$$.var_info = $$.var_info->elements;
			}

			cl_push(pp, OP_PUSH_PMEM); cl_push_dw(pp, $$.uvalue);
		}
	| unary_expression '[' assignment_expression ']'
		{
			// pop address
			// add to address offset
			// push address this address to stack

			// ul *v = {0,1,2};
			// v[1]; //= 1

			if($$.flags & ZLF_ARRAY)
			{
				unsigned long ptr_step;
				ptr_step = 4;
				
				$$.var_info = $$.var_info->next_node;

				if($$.var_info)
				{
					ptr_step = $$.var_info->size;
				}
				else
				{
					$$.flags &= ~ZLF_ARRAY;

					if(!($$.flags & ZLF_POINTER))
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

				}

				$$.size = ptr_step;

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				if(ptr_step > 1)
				{
					cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, ptr_step);
					cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
				}
				cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);

				$$.uvalue++;
			}
			else if($$.flags & ZLF_POINTER)
			{
				$$.flags -= 0x01000000;

				unsigned long var_size;
				var_size = 4;
				if(!($$.flags & ZLF_POINTER))
				{
					switch($$.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;					
							break;
						case ZLF_INT:
							if($$.flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							var_size = 8;
							break;
					}
				}

				$$.size = var_size;

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, 4);
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
	: T_CONSTANT_LONG											{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value) }
//	| T_CONSTANT_DOUBLE											{ cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $1.value) }
	| T_CONSTANT_STRING											{ cl_push(pp, OP_PUSH_OFFSET); cl_push_dw(pp, $1.value) }
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
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JNZ_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		}
		expr
		{
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
		}
	| expr T_AND
		{
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		}
		expr
		{
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
		}
	| expr '?'
		{
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, 0);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		}
		expr ':'
		{
			zl_names_map *lb_skip, *lb_else;
			lb_else = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_new(&pp->labels_table, 0);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			cl_stack_push(&pp->cl_stack, lb_skip);
		}
		expr
		{
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
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
	| T_LABEL '(' argument_expression_list ')'					{ cl_push(pp, OP_RCALL); $$.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, $1.string); free_str($1.string); if($$.uvalue == 1) ZL_ERROR("undeclared identifier"); cl_push_dw(pp, $$.uvalue); cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX); }
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
/*
	| T_SIZEOF '(' T_LABEL ')'
		{
			$$.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			unsigned long var_offset;
			$$.data = cl_var_find(pp->vars_table, $3.string, &var_offset);
			free_str($3.string);
			if(!$$.data)
			{
				ZL_ERROR("undeclared identifier");
			}

			$$.flags = ((zl_names_map *)$$.data)->flags;

			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, ((zl_names_map *)$$.data)->size);
		}
*/
	| T_SIZEOF '('
		{
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		}
		unary_expression ')'
		{
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
			$$.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, $4.size);
		}
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
/*
	| T_SIZEOF '(' T_LABEL ')'
		{
			unsigned long var_offset;
			$$.data = cl_var_find(pp->vars_table, $3.string, &var_offset);
			free_str($3.string);
			if(!$$.data)
			{
				ZL_ERROR("undeclared identifier");
			}

			$$.flags = ((zl_names_map *)$$.data)->flags;

			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, ((zl_names_map *)$$.data)->size);
		}
*/
	| T_SIZEOF '('
		{
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		}
		unary_expression ')'
		{
			$$.value = $4.size;
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
		}
;







%%









void yyerror(void *scanner, cl_parser_params *pp, char *err)
{
	pp->error_msg = cl_sprintf("zlc: error at line %d: %s", pp->lineno, err);
}

int zl_compile(unsigned char **hardcode, unsigned char **data_table,/* unsigned long *vars_count,*/ zl_funcs_list *funcs, char *code, zl_names_map **vars_map, char **error_msg)
{
	int ret;
	cl_parser_params pp;
	zl_names_map *temp_node;

	memset(&pp, 0, sizeof(cl_parser_params));

	pp.source_code = code;
	pp.sc_length = strlen(code);
	pp.funcs_list = funcs;
	pp.lineno = 1;

	void *scanner;

	yylex_init(&scanner);
	yyset_extra(&pp, scanner);
	ret = yyparse(scanner, &pp);
	yylex_destroy(scanner);

	if(error_msg)
	{
		*error_msg = pp.error_msg;
	}
	else if(ret)
	{
		free_str(pp.error_msg);
	}

	cl_stack_free(&pp.cl_stack);
	cl_stack_free(&pp.cl_loop_stack);

	cl_push(&pp, OP_EOF);

	if(!ret && cl_jump_fix(pp.hard_code, pp.jumps_table, pp.labels_table))
	{
		ret = 1;
		if(error_msg)
		{
			*error_msg = alloc_string("zlc: error, jump to undefined label");
		}
	}

	cl_labels_free(&pp.labels_table);
	cl_jumps_free(&pp.jumps_table);
	cl_names_free(&pp.structs_table);

	*hardcode = pp.hard_code;
	*data_table = pp.data_table;

	// new method without clear variables, but clear names
	*vars_map = pp.vars_table;

	while(pp.vars_table)
	{
		if(!(pp.vars_table->flags & ZLF_EXTERNAL))
		{
			free_str(pp.vars_table->name);
		}

		if(!(pp.vars_table->flags & ZLF_ARRAY))
		{
			while(pp.vars_table->elements)
			{
				temp_node = pp.vars_table->elements->next_node;
				free(pp.vars_table->elements);
				pp.vars_table->elements = temp_node;
			}
		}
		pp.vars_table = pp.vars_table->next_node;
	}

/*
	// old method with clear internal variables
	*vars_count = 0;
	while(pp.vars_table && !(pp.vars_table->flags & ZLF_EXTERNAL))
	{
		(*vars_count)++;
		zl_names_map *temp_node;
		temp_node = pp.vars_table;
		pp.vars_table = pp.vars_table->next_node;
		free_str(temp_node->name);
		zfree(temp_node);
	}

	*vars_map = pp.vars_table;

	if(pp.vars_table)
	{
		pp.vars_table->offset = *vars_count;
	}

	while(pp.vars_table)
	{
		(*vars_count)++;
		if(pp.vars_table->next_node && !(pp.vars_table->next_node->flags & ZLF_EXTERNAL))
		{
			zl_names_map *temp_node;
			temp_node = pp.vars_table->next_node;
			pp.vars_table->next_node = temp_node->next_node;
			free_str(temp_node->name);
			zfree(temp_node);
		}
		else
		{
			pp.vars_table = pp.vars_table->next_node;
			if(pp.vars_table)
			{
				pp.vars_table->offset = *vars_count;
			}
		}
	}
*/
	return ret;
}
