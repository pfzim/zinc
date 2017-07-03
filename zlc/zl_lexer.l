%{

#include <malloc.h>
#include "zl_parser.h"

#define YY_EXTRA_TYPE cl_parser_params *
#define YYPP yyget_extra(yyscanner)
#define YY_DECL int yylex(zlval *yylval, yyscan_t yyscanner)

#define YY_INPUT(buf,result,max_size) \
	memcpy(buf, YYPP->source_code + YYPP->sc_offset, ((YYPP->sc_length - YYPP->sc_offset) > ((unsigned long) max_size))?max_size:(YYPP->sc_length - YYPP->sc_offset));\
	result = ((YYPP->sc_length - YYPP->sc_offset)> ((unsigned long) max_size))?max_size:(YYPP->sc_length - YYPP->sc_offset);\
	YYPP->sc_offset += result;

#define YY_NO_UNISTD_H			1

#define TRACK_LINES(s, l)						\
do {											\
	char *p = (s), *boundary = p+(l);			\
												\
	while (p<boundary)							\
	{											\
		if (*p == '\n') {						\
			YYPP->lineno++;						\
		}										\
		p++;									\
	}											\
} while (0)

#define TRACK_LINE(c) \
{ \
	if (c == '\n') \
	{ \
		YYPP->lineno++; \
	} \
}

%}

%x ST_IN_SCRIPTING
%x ST_IN_ASM
%x ST_DOUBLE_QUOTES
%x ST_SINGLE_QUOTE
%x ST_BACKQUOTE
%x ST_HEREDOC
%x ST_LOOKING_FOR_PROPERTY
%x ST_LOOKING_FOR_VARNAME
%x ST_COMMENT
%x ST_ONE_LINE_COMMENT

LNUM						[0-9]+
DNUM						([0-9]*[\.][0-9]+)|([0-9]+[\.][0-9]*)
EXPONENT_DNUM				(({LNUM}|{DNUM})[eE][+-]?{LNUM})
HNUM						"0x"[0-9a-fA-F]+
LABEL						[a-zA-Z_][a-zA-Z0-9_]*
WHITESPACE					[ \n\r\t]+
TABS_AND_SPACES				[ \t]*
TOKENS						[;:,.\[\]()|^&+-/*=%!~$<>?@]
ENCAPSED_TOKENS				[\[\]{}$]
ESCAPED_AND_WHITESPACE		[\n\t\r #'.:;,()|^&+-/*=%!~<>?@]+
ANY_CHAR					(.|[\n])
NEWLINE						("\r"|"\n"|"\r\n")

%option stack
%option noyywrap
%option nounput
%option	never-interactive
%option	reentrant
/* %option noyylineno */

%%

<ST_IN_ASM>"eip" {
	yylval->value = 0; // register offset at regs table
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"esp" {
	yylval->value = 1;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"eax" {
	yylval->value = 2;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"ecx" {
	yylval->value = 3;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"ebx" {
	yylval->value = 4;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"edx" {
	yylval->value = 5;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"ebp" {
	yylval->value = 6;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"esd" {
	yylval->value = 7;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"ebd" {
	yylval->value = 8;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"esi" {
	yylval->value = 9;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"edi" {
	yylval->value = 10;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"eflags" {
	yylval->value = 11;
	yylval->flags = ARG_REG;
	return T_REGISTER;
}

<ST_IN_ASM>"push" {
	yylval->value = INS_PUSH;
	return T_OPERATOR;
}

<ST_IN_ASM>"pop" {
	yylval->value = INS_POP;
	return T_OPERATOR;
}

<ST_IN_ASM>"rcall" {
	yylval->value = INS_RCALL;
	return T_OPERATOR;
}

<ST_IN_ASM>"call" {
	yylval->value = INS_CALL;
	return T_OPERATOR;
}

<ST_IN_ASM>"test" {
	yylval->value = INS_TEST;
	return T_OPERATOR;
}

<ST_IN_ASM>"add" {
	yylval->value = INS_ADD;
	return T_OPERATOR;
}

<ST_IN_ASM>"mov" {
	yylval->value = INS_MOV;
	return T_OPERATOR;
}

<ST_IN_ASM>"sub" {
	yylval->value = INS_SUB;
	return T_OPERATOR;
}

<ST_IN_ASM>"mul" {
	yylval->value = INS_MUL;
	return T_OPERATOR;
}

<ST_IN_ASM>"div" {
	yylval->value = INS_DIV;
	return T_OPERATOR;
}

<ST_IN_ASM>"mod" {
	yylval->value = INS_MOD;
	return T_OPERATOR;
}

<ST_IN_ASM>"inc" {
	yylval->value = INS_INC;
	return T_OPERATOR;
}

<ST_IN_ASM>"dec" {
	yylval->value = INS_DEC;
	return T_OPERATOR;
}

<ST_IN_ASM>"shl" {
	yylval->value = INS_SHL;
	return T_OPERATOR;
}

<ST_IN_ASM>"shr" {
	yylval->value = INS_SHR;
	return T_OPERATOR;
}

<ST_IN_ASM>"or" {
	yylval->value = INS_OR;
	return T_OPERATOR;
}

<ST_IN_ASM>"and" {
	yylval->value = INS_AND;
	return T_OPERATOR;
}

<ST_IN_ASM>"xor" {
	yylval->value = INS_XOR;
	return T_OPERATOR;
}

<ST_IN_ASM>"dbg_print_regs" {
	yylval->value = INS_DBG_PRINT;
	return T_OPERATOR;
}

<ST_IN_ASM>"nop" {
	yylval->value = INS_NOP;
	return T_OPERATOR;
}

<ST_IN_ASM>"jmp" {
	yylval->value = OP_JMP;
	return T_GOTO;
}

<ST_IN_ASM>"jz" {
	yylval->value = OP_JZ;
	return T_GOTO;
}

<ST_IN_ASM>"jnz" {
	yylval->value = OP_JNZ;
	return T_GOTO;
}

<INITIAL>"_asm"[ \n\r\t]*"{" {
	TRACK_LINES(yytext, yyleng);
	BEGIN(ST_IN_ASM);
	return T_ASM;
}

<ST_IN_ASM>"}" {
	BEGIN(INITIAL);
	return yytext[0];
}

<INITIAL>"if" {
	return T_IF;
}

<INITIAL>"else" {
	return T_ELSE;
}

<INITIAL>"do" {
	return T_DO;
}

<INITIAL>"while" {
	return T_WHILE;
}

<INITIAL>"for" {
	return T_FOR;
}

<INITIAL>"break" {
	return T_BREAK;
}

<INITIAL>"continue" {
	return T_CONTINUE;
}

<INITIAL>"goto" {
	yylval->value = OP_JMP;
	return T_GOTO;
}

<INITIAL>"nop" {
	return T_NOP;
}

<INITIAL>"___dbg_print_regs" {
	return T_DBG_PRINT_REGS;
}

<INITIAL>"sizeof" {
	return T_SIZEOF;
}

<INITIAL>"function" {
	return T_FUNCTION;
}

<INITIAL>"return" {
	return T_RETURN;
}

<INITIAL>"==" {
	return T_EQ;
}

<INITIAL>"!=" {
	return T_NE;
}

<INITIAL>"&&" {
	return T_AND;
}

<INITIAL>"||" {
	return T_OR;
}

<INITIAL>"->" {
	return T_PTR_OP;
}

<INITIAL>">>" {
	return T_SHR;
}

<INITIAL>"<<" {
	return T_SHL;
}

<INITIAL>"++" {
	return T_INC;
}

<INITIAL>"--" {
	return T_DEC;
}

<INITIAL>"import" {
	return T_IMPORT;
}

<INITIAL>"from" {
	return T_FROM;
}

<INITIAL>"extern" {
	return T_EXTERN;
}

<INITIAL>"var" {
	return T_VAR;
}

<INITIAL>"char" {
	return T_CHAR;
}

<INITIAL>"string" {
	return T_STRING;
}

<INITIAL>"int" {
	return T_INT;
}

<INITIAL>"long" {
	return T_LONG;
}

<INITIAL>"short" {
	return T_SHORT;
}

<INITIAL>"signed" {
	return T_SIGNED;
}

<INITIAL>"unsigned" {
	return T_UNSIGNED;
}

<INITIAL>"float" {
	return T_FLOAT;
}

<INITIAL>"double" {
	return T_DOUBLE;
}

<INITIAL>"void" {
	return T_VOID;
}

<INITIAL,ST_IN_ASM>"__COMPILER__" {
	//char *text;
	//unsigned long len;

	yylval->string = cl_strdup("zlc v0.01 compiled at "__DATE__" "__TIME__" (c)oded by dEmon");
	yylval->size = strlen(yylval->string)+1;
	//yylval->value = (unsigned long) cl_section_data_push(&YYPP->data_table, ZLF_SECT_CONST, (char *) text, strlen(text)+1);
	//free_str(text);
	yylval->flags = ZLF_UNSIGNED | ZLF_CHAR;
	yylval->flags += 0x01000000;

	return T_CONSTANT_STRING;
}

<INITIAL,ST_IN_ASM>"__DATE__" {
	//char *text;
	//unsigned long len;
	
	yylval->string = cl_strdup("Sep 25 2005");
	yylval->size = strlen(yylval->string)+1;
	//yylval->value = (unsigned long) cl_section_data_push(&YYPP->data_table, ZLF_SECT_CONST, (char *) text, strlen(text)+1);
	//free_str(text);
	yylval->flags = ZLF_UNSIGNED | ZLF_CHAR;
	yylval->flags += 0x01000000;

	return T_CONSTANT_STRING;
}

<INITIAL,ST_IN_ASM>"__TIME__" {
	//char *text;
	//unsigned long len;
	
	yylval->string = cl_strdup("11:05:16");
	yylval->size = strlen(yylval->string)+1;
	//yylval->value = (unsigned long) cl_section_data_push(&YYPP->data_table, ZLF_SECT_CONST, (char *) text, strlen(text)+1);
	//free_str(text);
	yylval->flags = ZLF_UNSIGNED | ZLF_CHAR;
	yylval->flags += 0x01000000;

	return T_CONSTANT_STRING;
}

<INITIAL>"..." {
	return T_ELLIPSIS;
}

<INITIAL>"struct" {
	return T_STRUCT;
}

<INITIAL>">=" {
	return T_GE;
}

<INITIAL>"<=" {
	return T_LE;
}

<INITIAL>"+=" {
	return T_ADD_ASSIGN;
}

<INITIAL>"-=" {
	return T_SUB_ASSIGN;
}

<INITIAL>"*=" {
	return T_MUL_ASSIGN;
}

<INITIAL>"/=" {
	return T_DIV_ASSIGN;
}

<INITIAL>"%=" {
	return T_MOD_ASSIGN;
}

<INITIAL>"&=" {
	return T_AND_ASSIGN;
}

<INITIAL>"|=" {
	return T_OR_ASSIGN;
}

<INITIAL>"^=" {
	return T_XOR_ASSIGN;
}

<INITIAL>"<<=" {
	return T_SHL_ASSIGN;
}

<INITIAL>">>=" {
	return T_SHR_ASSIGN;
}

<INITIAL>"false"|"FALSE"|"null"|"NULL"|"__stdcall" {
	yylval->value = 0;
	yylval->flags = ZLF_SIGNED | ZLF_LONG | ZLF_INT;
	return T_CONSTANT_LONG;
}

<INITIAL>"true"|"TRUE"|"__cdecl" {
	yylval->value = 1;
	yylval->flags = ZLF_SIGNED | ZLF_LONG | ZLF_INT;
	return T_CONSTANT_LONG;
}

<INITIAL,ST_IN_ASM>{TOKENS} {
	return yytext[0];
}

<INITIAL,ST_IN_ASM>(["]([^"\r\n]|(\\\"))*["]) {
	char *text;
	//char *unesc;
	//unsigned long len;
	
	text = cl_strndup(yytext+1, yyleng-2);
	yylval->string = alloc_unescapestring(text, &yylval->size);
	//yylval->value = (unsigned long) cl_section_data_push(&YYPP->data_table, ZLF_SECT_CONST, (char *) unesc, len+1);
	//free_str(unesc);
	free_str(text);
	yylval->flags = ZLF_UNSIGNED | ZLF_CHAR;
	yylval->flags += 0x01000000;
	return T_CONSTANT_STRING;
}

<INITIAL,ST_IN_ASM>([']([^'\r\n\\]|(\\r)|(\\n)|(\\t)|(\\a)|(\\b)|(\\f)|(\\v)|(\\')|(\\\")|(\\?)|(\\\\)|(\\[0-8][0-8]{0,2}|(\\x[0-9a-fA-F]+)))[']) {
	char *text, *unesc;
	unsigned long len;
	text = cl_strndup(yytext+1, yyleng-2);
	unesc = alloc_unescapestring(text, &len);
	yylval->value = *unesc;
	free_str(unesc);
	free_str(text);
	yylval->flags = ZLF_UNSIGNED | ZLF_CHAR;
	return T_CONSTANT_LONG;
}

<INITIAL,ST_IN_ASM>{LNUM} {
	yylval->value = cl_strtol(yytext, yyleng);
	yylval->flags = ZLF_SIGNED | ZLF_LONG | ZLF_INT;
	return T_CONSTANT_LONG;
}

<INITIAL,ST_IN_ASM>{HNUM} {
	yylval->value = cl_strtoul(yytext, yyleng);
	yylval->flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
	return T_CONSTANT_LONG;
}

<INITIAL,ST_IN_ASM>{DNUM}|{EXPONENT_DNUM} {
	yylval->dvalue = cl_strtod(yytext, yyleng);
	yylval->flags = ZLF_DOUBLE;
	return T_CONSTANT_DOUBLE;
}

<INITIAL,ST_IN_ASM>{LABEL} {
	yylval->string = cl_strndup(yytext, yyleng);
	return T_LABEL;
}

<INITIAL>{WHITESPACE} {
	TRACK_LINES(yytext, yyleng);
	//return T_WHITESPACE;
}

<ST_IN_ASM>[ \r\t]+ {
	//return T_WHITESPACE;
}

<INITIAL,ST_IN_ASM>"/*"[\n\r.]*"*/" {
	TRACK_LINES(yytext, yyleng);
	//return T_WHITESPACE;
}

<ST_IN_ASM>[\n] {
	YYPP->lineno++;
	return yytext[0];
}

<INITIAL,ST_IN_ASM>. {
	TRACK_LINE(yytext[0]);
	return yytext[0];
}

<INITIAL,ST_IN_ASM>"//".*$ {
	//return T_COMMENT;
}

<INITIAL>"/*" {
	BEGIN(ST_COMMENT);
	yymore();
}

<ST_COMMENT>[^*]+ {
	yymore();
}

<ST_COMMENT>"*" {
	yymore();
}

<ST_COMMENT>"*/" {
	TRACK_LINES(yytext, yyleng);
	BEGIN(INITIAL);
	//return T_COMMENT;
}

<<EOF>> {
	yyterminate();
}

%%
