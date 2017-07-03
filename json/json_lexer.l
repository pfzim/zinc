%option stack
%option noyywrap
%option nounput
%option	never-interactive
%option	reentrant
/* %option noyylineno */
/* %option c++ */
/* %option yyclass="json_lex" */

%{

#include <malloc.h>
#include "json_parser.h"
//#include "json_parser.hpp"

#define YY_EXTRA_TYPE json_parser_params *
#define YYPP yyget_extra(yyscanner)
#define YY_DECL int yylex(json_value *yylval, yyscan_t yyscanner)

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

LNUM						[-][0-9]+
UNUM						[+]?[0-9]+
DNUM						([0-9]*[\.][0-9]+)|([0-9]+[\.][0-9]*)
EXPONENT_DNUM				(({LNUM}|{DNUM})[eE][+-]?{LNUM})
HNUM						"0x"[0-9a-fA-F]+
LABEL						[a-zA-Z_][a-zA-Z0-9_-]*
WHITESPACE					[ \n\r\t]+
TABS_AND_SPACES				[ \t]*
TOKENS						[;:,.\[\]()|^&+-/*=%!~$<>?@]
ENCAPSED_TOKENS				[\[\]{}$]
ESCAPED_AND_WHITESPACE		[\n\t\r #'.:;,()|^&+-/*=%!~<>?@]+
ANY_CHAR					(.|[\n])
NEWLINE						("\r"|"\n"|"\r\n")


%%

<INITIAL>"false"|"FALSE" {
	yylval->ul_val = 0;
	yylval->flags = JS_ULONG | JS_LONG;
	return T_FALSE;
}

<INITIAL>"true"|"TRUE" {
	yylval->ul_val = 1;
	yylval->flags = JS_ULONG | JS_LONG;
	return T_TRUE;
}

<INITIAL>"null"|"NULL" {
	yylval->str_val = nullstring;
	yylval->flags = JS_STRING;
	return T_NULL;
}

<INITIAL>{TOKENS} {
	return yytext[0];
}

<INITIAL>"\"" {
	BEGIN(ST_DOUBLE_QUOTES);
	yymore();
}

<ST_DOUBLE_QUOTES>[^\\\"\r\n]+ {
	yymore();
}

<ST_DOUBLE_QUOTES>(\\.) {
	yymore();
}

<ST_DOUBLE_QUOTES>"\"" {
	char *text;
	//char *unesc;
	//unsigned long len;
	
	text = cl_strndup(yytext+1, yyleng-2);
	yylval->str_val = alloc_unescapestring(text, NULL);
	//yylval->value = (unsigned long) cl_section_data_push(&YYPP->data_table, ZLF_SECT_CONST, (char *) unesc, len+1);
	//free_str(unesc);
	free_str(text);
	yylval->flags = JS_STRING;
	BEGIN(INITIAL);
	return T_CONST_STRING;
}

<INITIAL>{LNUM} {
	yylval->l_val = cl_strtol(yytext, yyleng);
	yylval->flags = JS_LONG | JS_ULONG;
	return T_CONST_LONG;
}

<INITIAL>{HNUM} {
	yylval->ul_val = cl_strtoul(yytext, yyleng);
	yylval->flags = JS_ULONG | JS_LONG;
	return T_CONST_ULONG;
}

<INITIAL>{UNUM} {
	yylval->ul_val = strtoul(yytext, NULL, 10);
	yylval->flags = JS_ULONG | JS_LONG;
	return T_CONST_ULONG;
}

<INITIAL>{DNUM}|{EXPONENT_DNUM} {
	yylval->dbl_val = cl_strtod(yytext, yyleng);
	yylval->flags = JS_DOUBLE;
	return T_CONST_DOUBLE;
}

<INITIAL>{LABEL} {
	yylval->str_val = cl_strndup(yytext, yyleng);
	return T_LABEL;
}

<INITIAL>{WHITESPACE} {
	TRACK_LINES(yytext, yyleng);
	//return T_WHITESPACE;
}

<INITIAL>"/*"[\n\r.]*"*/" {
	TRACK_LINES(yytext, yyleng);
	//return T_WHITESPACE;
}

<ST_IN_ASM>[\n] {
	YYPP->lineno++;
	return yytext[0];
}

<INITIAL>. {
	TRACK_LINE(yytext[0]);
	return yytext[0];
}

<INITIAL>"//".*$ {
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
