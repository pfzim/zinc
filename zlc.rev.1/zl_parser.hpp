/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_AND_ASSIGN = 258,
     T_SUB_ASSIGN = 259,
     T_MUL_ASSIGN = 260,
     T_DIV_ASSIGN = 261,
     T_MOD_ASSIGN = 262,
     T_ADD_ASSIGN = 263,
     T_SHR_ASSIGN = 264,
     T_SHL_ASSIGN = 265,
     T_XOR_ASSIGN = 266,
     T_OR_ASSIGN = 267,
     T_OR = 268,
     T_AND = 269,
     T_NE = 270,
     T_EQ = 271,
     T_GE = 272,
     T_LE = 273,
     T_SHR = 274,
     T_SHL = 275,
     T_DEC = 276,
     T_INC = 277,
     T_POINTER = 278,
     T_MINUS = 279,
     T_PLUS = 280,
     T_IF = 281,
     T_ELSE = 282,
     T_PTR_OP = 283,
     T_WHILE = 284,
     T_DO = 285,
     T_BREAK = 286,
     T_CONTINUE = 287,
     T_FOR = 288,
     T_LABEL = 289,
     T_CONSTANT_DOUBLE = 290,
     T_CONSTANT_LONG = 291,
     T_CONSTANT_STRING = 292,
     T_VOID = 293,
     T_CHAR = 294,
     T_SHORT = 295,
     T_INT = 296,
     T_LONG = 297,
     T_FLOAT = 298,
     T_DOUBLE = 299,
     T_SIGNED = 300,
     T_UNSIGNED = 301,
     T_STRUCT = 302,
     T_STRUCT_DEFINED = 303,
     T_ELLIPSIS = 304,
     T_STRING = 305,
     T_EXTERN = 306,
     T_VAR = 307,
     T_GOTO = 308,
     T_SIZEOF = 309,
     T_REGISTER = 310,
     T_OPERATOR = 311,
     T_ASM = 312,
     T_PARAM_SEPARATOR = 313,
     T_NOP = 314
   };
#endif
#define T_AND_ASSIGN 258
#define T_SUB_ASSIGN 259
#define T_MUL_ASSIGN 260
#define T_DIV_ASSIGN 261
#define T_MOD_ASSIGN 262
#define T_ADD_ASSIGN 263
#define T_SHR_ASSIGN 264
#define T_SHL_ASSIGN 265
#define T_XOR_ASSIGN 266
#define T_OR_ASSIGN 267
#define T_OR 268
#define T_AND 269
#define T_NE 270
#define T_EQ 271
#define T_GE 272
#define T_LE 273
#define T_SHR 274
#define T_SHL 275
#define T_DEC 276
#define T_INC 277
#define T_POINTER 278
#define T_MINUS 279
#define T_PLUS 280
#define T_IF 281
#define T_ELSE 282
#define T_PTR_OP 283
#define T_WHILE 284
#define T_DO 285
#define T_BREAK 286
#define T_CONTINUE 287
#define T_FOR 288
#define T_LABEL 289
#define T_CONSTANT_DOUBLE 290
#define T_CONSTANT_LONG 291
#define T_CONSTANT_STRING 292
#define T_VOID 293
#define T_CHAR 294
#define T_SHORT 295
#define T_INT 296
#define T_LONG 297
#define T_FLOAT 298
#define T_DOUBLE 299
#define T_SIGNED 300
#define T_UNSIGNED 301
#define T_STRUCT 302
#define T_STRUCT_DEFINED 303
#define T_ELLIPSIS 304
#define T_STRING 305
#define T_EXTERN 306
#define T_VAR 307
#define T_GOTO 308
#define T_SIZEOF 309
#define T_REGISTER 310
#define T_OPERATOR 311
#define T_ASM 312
#define T_PARAM_SEPARATOR 313
#define T_NOP 314




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





