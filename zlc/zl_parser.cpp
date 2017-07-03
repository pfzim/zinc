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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



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
     T_IMPORT = 308,
     T_FROM = 309,
     T_GOTO = 310,
     T_SIZEOF = 311,
     T_RETURN = 312,
     T_REGISTER = 313,
     T_OPERATOR = 314,
     T_ASM = 315,
     T_PARAM_SEPARATOR = 316,
     T_NOP = 317,
     T_DBG_PRINT_REGS = 318,
     T_FUNCTION = 319
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
#define T_IMPORT 308
#define T_FROM 309
#define T_GOTO 310
#define T_SIZEOF 311
#define T_RETURN 312
#define T_REGISTER 313
#define T_OPERATOR 314
#define T_ASM 315
#define T_PARAM_SEPARATOR 316
#define T_NOP 317
#define T_DBG_PRINT_REGS 318
#define T_FUNCTION 319




/* Copy the first part of user declarations.  */
#line 1 ".\\zl_parser.y"


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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 235 "zl_parser.cpp"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   804

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  59
/* YYNRULES -- Number of rules. */
#define YYNRULES  193
/* YYNRULES -- Number of states. */
#define YYNSTATES  352

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   319

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      86,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,     2,     2,    34,    21,     2,
      87,    88,    32,    30,     3,    31,    46,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,    90,
      24,     4,    25,    15,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    89,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,    19,    85,    36,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    17,    18,
      22,    23,    26,    27,    28,    29,    38,    39,    40,    41,
      42,    44,    45,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    14,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    32,    37,    41,
      45,    48,    50,    53,    58,    61,    63,    68,    69,    73,
      75,    76,    78,    82,    84,    86,    89,    92,    95,    98,
     101,   105,   109,   112,   116,   117,   125,   126,   127,   135,
     136,   137,   138,   149,   150,   151,   152,   165,   166,   167,
     171,   174,   178,   186,   196,   197,   198,   207,   209,   213,
     214,   215,   219,   222,   223,   228,   230,   233,   235,   239,
     241,   242,   247,   249,   252,   255,   257,   259,   264,   268,
     272,   274,   278,   280,   281,   286,   288,   290,   292,   294,
     296,   298,   300,   302,   304,   306,   308,   310,   314,   315,
     317,   321,   323,   327,   331,   335,   339,   343,   347,   351,
     355,   359,   363,   367,   369,   372,   377,   379,   381,   385,
     389,   393,   397,   401,   405,   409,   413,   417,   421,   425,
     429,   433,   437,   441,   445,   449,   450,   455,   456,   461,
     462,   463,   471,   474,   477,   480,   483,   486,   489,   492,
     495,   498,   503,   505,   510,   511,   517,   519,   523,   527,
     531,   535,   539,   543,   547,   551,   555,   559,   563,   567,
     571,   575,   579,   583,   587,   591,   595,   601,   604,   607,
     610,   613,   618,   619
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      92,     0,    -1,    93,    92,    -1,    -1,   125,    -1,   119,
      -1,    95,    94,    -1,    -1,    96,    -1,   103,    -1,   104,
      -1,   105,    -1,   117,    -1,   118,    -1,   125,    -1,    98,
      -1,    -1,    84,    97,    94,    85,    -1,    79,    99,    85,
      -1,   100,    86,    99,    -1,   117,    99,    -1,   100,    -1,
      74,    53,    -1,    78,   102,     3,   102,    -1,    78,   102,
      -1,    78,    -1,    53,    87,   101,    88,    -1,    -1,   102,
       3,   101,    -1,   102,    -1,    -1,    77,    -1,    43,    77,
      89,    -1,    55,    -1,    90,    -1,   138,    90,    -1,    81,
      90,    -1,    82,    90,    -1,    50,    90,    -1,    51,    90,
      -1,    50,    55,    90,    -1,    51,    55,    90,    -1,    76,
      90,    -1,    76,   140,    90,    -1,    -1,    44,    87,   140,
      88,   106,    95,   115,    -1,    -1,    -1,    48,   107,    87,
     140,    88,   108,    95,    -1,    -1,    -1,    -1,    49,   109,
      95,    48,   110,    87,   140,    88,   111,    90,    -1,    -1,
      -1,    -1,    52,    87,   138,    90,   112,   138,    90,   113,
     138,    88,   114,    95,    -1,    -1,    -1,    45,   116,    95,
      -1,    53,    16,    -1,    74,    53,    90,    -1,    72,    55,
      53,    87,    55,    88,    90,    -1,    72,    55,    53,    87,
      55,    88,    73,    56,    90,    -1,    -1,    -1,    83,    53,
      87,   120,   122,    88,   121,    96,    -1,   123,    -1,   123,
       3,   122,    -1,    -1,    -1,   127,   124,   132,    -1,   127,
      90,    -1,    -1,   127,   126,   128,    90,    -1,   137,    -1,
     137,   127,    -1,   129,    -1,   128,     3,   129,    -1,   132,
      -1,    -1,   132,     4,   130,   134,    -1,    32,    -1,    32,
     131,    -1,   131,   133,    -1,   133,    -1,    53,    -1,   133,
      43,   148,    89,    -1,   133,    43,    89,    -1,    87,   132,
      88,    -1,   140,    -1,    84,   135,    85,    -1,   134,    -1,
      -1,   134,     3,   136,   135,    -1,    57,    -1,    58,    -1,
      59,    -1,    60,    -1,    61,    -1,    62,    -1,    63,    -1,
      64,    -1,    65,    -1,    70,    -1,    71,    -1,   140,    -1,
     140,     3,   138,    -1,    -1,   140,    -1,   140,     3,   139,
      -1,   142,    -1,   141,     4,   140,    -1,   141,    10,   140,
      -1,   141,     6,   140,    -1,   141,     7,   140,    -1,   141,
       8,   140,    -1,   141,     9,   140,    -1,   141,    12,   140,
      -1,   141,    11,   140,    -1,   141,     5,   140,    -1,   141,
      13,   140,    -1,   141,    14,   140,    -1,    53,    -1,    32,
     141,    -1,   141,    43,   140,    89,    -1,    55,    -1,    56,
      -1,    87,   142,    88,    -1,   142,    30,   142,    -1,   142,
      31,   142,    -1,   142,    32,   142,    -1,   142,    33,   142,
      -1,   142,    19,   142,    -1,   142,    21,   142,    -1,   142,
      34,   142,    -1,   142,    20,   142,    -1,   142,    29,   142,
      -1,   142,    28,   142,    -1,   142,    25,   142,    -1,   142,
      24,   142,    -1,   142,    26,   142,    -1,   142,    27,   142,
      -1,   142,    23,   142,    -1,   142,    22,   142,    -1,    -1,
     142,    17,   143,   142,    -1,    -1,   142,    18,   144,   142,
      -1,    -1,    -1,   142,    15,   145,   142,    16,   146,   142,
      -1,    36,   142,    -1,    35,   142,    -1,    30,   142,    -1,
      31,   142,    -1,    21,   141,    -1,   141,    39,    -1,   141,
      38,    -1,    39,   141,    -1,    38,   141,    -1,    53,    87,
     139,    88,    -1,   141,    -1,    75,    87,   127,    88,    -1,
      -1,    75,    87,   147,   141,    88,    -1,    55,    -1,    87,
     148,    88,    -1,   148,    30,   148,    -1,   148,    31,   148,
      -1,   148,    32,   148,    -1,   148,    33,   148,    -1,   148,
      19,   148,    -1,   148,    21,   148,    -1,   148,    34,   148,
      -1,   148,    20,   148,    -1,   148,    29,   148,    -1,   148,
      28,   148,    -1,   148,    25,   148,    -1,   148,    24,   148,
      -1,   148,    26,   148,    -1,   148,    27,   148,    -1,   148,
      23,   148,    -1,   148,    22,   148,    -1,   148,    17,   148,
      -1,   148,    18,   148,    -1,   148,    15,   148,    16,   148,
      -1,    36,   148,    -1,    35,   148,    -1,    30,   148,    -1,
      31,   148,    -1,    75,    87,   127,    88,    -1,    -1,    75,
      87,   149,   141,    88,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    97,    97,    98,   102,   103,   107,   108,   111,   112,
     113,   114,   115,   116,   117,   118,   123,   122,   135,   139,
     140,   141,   145,   161,   162,   163,   164,   186,   207,   208,
     209,   213,   214,   215,   240,   241,   245,   246,   247,   255,
     263,   278,   293,   303,   318,   317,   336,   346,   335,   366,
     380,   387,   365,   399,   416,   437,   398,   467,   470,   469,
     486,   504,   523,   543,   581,   585,   580,   652,   658,   664,
     669,   668,   743,   745,   744,   765,   766,   802,   803,   807,
     867,   866,   983,   984,   988,   996,  1008,  1039,  1058,  1072,
    1079,  1111,  1118,  1126,  1125,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1200,  1201,  1205,  1206,
    1207,  1211,  1212,  1239,  1268,  1297,  1326,  1355,  1384,  1413,
    1442,  1471,  1500,  1532,  1586,  1599,  1685,  1687,  1694,  1695,
    1696,  1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,
    1706,  1707,  1708,  1709,  1710,  1712,  1711,  1732,  1731,  1752,
    1762,  1751,  1781,  1782,  1783,  1784,  1791,  1807,  1851,  1895,
    1939,  1983,  2022,  2054,  2081,  2080,  2097,  2100,  2101,  2102,
    2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2112,
    2113,  2114,  2115,  2116,  2117,  2118,  2119,  2120,  2121,  2122,
    2123,  2124,  2150,  2149
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "','", "'='", "T_AND_ASSIGN", 
  "T_SUB_ASSIGN", "T_MUL_ASSIGN", "T_DIV_ASSIGN", "T_MOD_ASSIGN", 
  "T_ADD_ASSIGN", "T_SHR_ASSIGN", "T_SHL_ASSIGN", "T_XOR_ASSIGN", 
  "T_OR_ASSIGN", "'?'", "':'", "T_OR", "T_AND", "'|'", "'^'", "'&'", 
  "T_NE", "T_EQ", "'<'", "'>'", "T_GE", "T_LE", "T_SHR", "T_SHL", "'+'", 
  "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'@'", "T_DEC", "T_INC", 
  "T_POINTER", "T_MINUS", "T_PLUS", "'['", "T_IF", "T_ELSE", "'.'", 
  "T_PTR_OP", "T_WHILE", "T_DO", "T_BREAK", "T_CONTINUE", "T_FOR", 
  "T_LABEL", "T_CONSTANT_DOUBLE", "T_CONSTANT_LONG", "T_CONSTANT_STRING", 
  "T_VOID", "T_CHAR", "T_SHORT", "T_INT", "T_LONG", "T_FLOAT", "T_DOUBLE", 
  "T_SIGNED", "T_UNSIGNED", "T_STRUCT", "T_STRUCT_DEFINED", "T_ELLIPSIS", 
  "T_STRING", "T_EXTERN", "T_VAR", "T_IMPORT", "T_FROM", "T_GOTO", 
  "T_SIZEOF", "T_RETURN", "T_REGISTER", "T_OPERATOR", "T_ASM", 
  "T_PARAM_SEPARATOR", "T_NOP", "T_DBG_PRINT_REGS", "T_FUNCTION", "'{'", 
  "'}'", "'\\n'", "'('", "')'", "']'", "';'", "$accept", "program_list", 
  "program", "statement_list", "statement", "compound_statement", "@1", 
  "assembler_statement", "assembler_command_list", "assembler_command", 
  "assembler_argument_list", "operator_parameter", "expression_statement", 
  "buildin_statement", "selection_statement", "@2", "@3", "@4", "@5", 
  "@6", "@7", "@8", "@9", "@10", "else_statement", "@11", 
  "labeled_statement", "jump_statement", "declaration_function", "@12", 
  "@13", "function_args", "function_arg", "@14", "declaration_statement", 
  "@15", "declaration_specifiers", "init_declarator_list", 
  "init_declarator", "@16", "pointer", "declarator", "direct_declarator", 
  "initializer", "initializer_list", "@17", "type_specifier", 
  "expression", "argument_expression_list", "assignment_expression", 
  "unary_expression", "expr", "@18", "@19", "@20", "@21", "@22", 
  "const_expr", "@23", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,    44,    61,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,    63,    58,   268,   269,   124,
      94,    38,   270,   271,    60,    62,   272,   273,   274,   275,
      43,    45,    42,    47,    37,    33,   126,    64,   276,   277,
     278,   279,   280,    91,   281,   282,    46,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   123,   125,    10,    40,    41,    93,
      59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      95,    95,    95,    95,    95,    95,    97,    96,    98,    99,
      99,    99,   100,   100,   100,   100,   100,   100,   101,   101,
     101,   102,   102,   102,   103,   103,   104,   104,   104,   104,
     104,   104,   104,   104,   106,   105,   107,   108,   105,   109,
     110,   111,   105,   112,   113,   114,   105,   115,   116,   115,
     117,   118,   119,   119,   120,   121,   119,   122,   122,   122,
     124,   123,   125,   126,   125,   127,   127,   128,   128,   129,
     130,   129,   131,   131,   132,   132,   133,   133,   133,   133,
     134,   134,   135,   136,   135,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   138,   138,   139,   139,
     139,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   141,   141,   141,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   143,   142,   144,   142,   145,
     146,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   147,   142,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   149,   148
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     0,     1,     1,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     3,     3,
       2,     1,     2,     4,     2,     1,     4,     0,     3,     1,
       0,     1,     3,     1,     1,     2,     2,     2,     2,     2,
       3,     3,     2,     3,     0,     7,     0,     0,     7,     0,
       0,     0,    10,     0,     0,     0,    12,     0,     0,     3,
       2,     3,     7,     9,     0,     0,     8,     1,     3,     0,
       0,     3,     2,     0,     4,     1,     2,     1,     3,     1,
       0,     4,     1,     2,     2,     1,     1,     4,     3,     3,
       1,     3,     1,     0,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     2,     4,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     0,     4,     0,     4,     0,
       0,     7,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     1,     4,     0,     5,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     2,     2,     2,
       2,     4,     0,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     0,     0,     0,     3,     5,     4,    73,    75,
       0,     0,     1,     2,    72,     0,    76,     0,    64,    82,
      86,     0,     0,    77,     0,    79,    85,     0,    69,    83,
       0,     0,    74,    84,    80,     0,     0,     0,    67,    70,
      89,    78,     0,     0,     0,     0,     0,   166,     0,     0,
      88,     0,     0,    65,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   123,   126,   127,     0,     0,     0,
      81,    90,   162,   111,   189,   190,   188,   187,   192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,    62,     0,    68,    71,   123,   156,   162,   154,   155,
     124,   153,   152,   160,   159,   108,   164,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   157,     0,   149,   145,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,   184,   185,   172,
     175,   173,   183,   182,   179,   178,   180,   181,   177,   176,
     168,   169,   170,   171,   174,     0,    16,    66,     0,   109,
       0,     0,    93,    91,   128,   112,   120,   114,   115,   116,
     117,   113,   119,   118,   121,   122,     0,     0,     0,     0,
     133,   136,   134,   144,   143,   140,   139,   141,   142,   138,
     137,   129,   130,   131,   132,   135,   191,     0,     0,    63,
       7,   161,   108,   163,     0,     0,   125,     0,   146,   148,
     193,   186,     0,    46,    49,     0,     0,     0,   123,     0,
       0,    27,     0,     0,    34,     0,     7,     8,    15,     9,
      10,    11,    12,    13,    14,     0,   106,   110,   165,    94,
     150,     0,     0,     0,     0,    38,     0,    39,     0,    60,
       0,    42,     0,     0,     0,    25,     0,    21,    27,    36,
      37,    17,     6,    35,     0,     0,     0,     0,     0,    40,
      41,     0,    61,    43,    30,    22,     0,    33,    31,    24,
      18,    27,    20,   107,   151,    44,     0,    50,    53,     0,
      29,     0,     0,    19,     0,    47,     0,     0,    26,    30,
      32,    23,    57,     0,     0,     0,    28,    58,    45,    48,
       0,    54,     0,    51,     0,    59,     0,     0,    52,    55,
       0,    56
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    14,    15,   255,   256,   257,   230,   258,   286,   287,
     319,   320,   259,   260,   261,   324,   272,   333,   273,   326,
     346,   327,   344,   350,   338,   342,   262,   263,    16,    38,
     112,    47,    48,    65,   264,    25,    18,    32,    33,    52,
      34,    35,    36,   127,   128,   235,    19,   265,   188,   266,
      82,    83,   208,   209,   207,   295,   191,    61,   164
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -274
static const short yypact[] =
{
     423,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,   -17,   -31,    48,   423,  -274,  -274,   -39,   642,
      11,   -22,  -274,  -274,  -274,   -25,  -274,   -20,  -274,    20,
    -274,   -25,    -1,  -274,   -28,    65,    33,    22,   642,  -274,
      43,   -25,  -274,    33,  -274,   102,    46,    47,   136,  -274,
    -274,  -274,   371,   386,   386,   386,   386,  -274,    53,   386,
    -274,   203,   -41,  -274,   642,   -25,     4,   404,   404,     4,
     404,   404,     4,     4,    64,  -274,  -274,    68,   371,   404,
    -274,  -274,   458,   532,  -274,  -274,  -274,  -274,   642,   242,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,  -274,
     100,  -274,    74,  -274,  -274,  -274,   117,     2,  -274,  -274,
     117,  -274,  -274,   117,   117,   404,   642,   158,    81,   276,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,  -274,  -274,   404,  -274,  -274,  -274,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,    88,     4,  -274,   492,   585,   618,   649,
     693,   720,   733,   733,   222,   222,   222,   222,    96,    96,
     115,   115,  -274,  -274,  -274,    79,  -274,  -274,    90,   176,
      93,     4,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,    95,   404,   404,   404,
     664,   707,   746,   759,   759,   770,   770,   770,   770,   112,
     112,   141,   141,  -274,  -274,  -274,  -274,   -35,   386,  -274,
     296,  -274,   404,  -274,   -33,   371,  -274,   512,   602,   634,
    -274,   550,    98,  -274,  -274,   -46,   -32,    99,   -13,   129,
     358,    -6,    97,   103,  -274,   105,   296,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,   104,   185,  -274,  -274,  -274,
    -274,   404,   108,   296,   106,  -274,   107,  -274,   404,  -274,
     148,  -274,   149,   -12,   139,   -16,   156,   157,    -6,  -274,
    -274,  -274,  -274,  -274,   404,   404,   154,   404,   196,  -274,
    -274,   187,  -274,  -274,   -16,  -274,   201,  -274,  -274,   277,
    -274,    -6,  -274,  -274,   568,  -274,   191,  -274,  -274,   193,
     280,   195,   -16,  -274,   296,  -274,   198,   404,  -274,   -16,
    -274,  -274,   241,   296,   404,   197,  -274,  -274,  -274,  -274,
     200,  -274,   296,  -274,   404,  -274,   199,   202,  -274,  -274,
     296,  -274
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -274,   297,  -274,    55,  -183,   204,  -274,  -274,  -251,  -274,
     -15,  -266,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -245,  -274,  -274,  -274,
    -274,   249,  -274,  -274,    35,  -274,   -18,  -274,   274,  -274,
     289,    -2,   285,   268,    86,  -274,  -274,  -273,    91,   -52,
     -55,   -37,  -274,  -274,  -274,  -274,  -274,   109,  -274
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short yytable[] =
{
      81,    26,    41,   279,   279,   301,   288,    29,   143,   274,
     143,   116,   117,   117,   120,   117,   117,   123,   124,   309,
      49,   313,    21,   276,   117,    30,    81,   306,    30,    40,
     118,   119,   110,   121,   122,    17,    69,   312,    20,   307,
     141,   142,   129,   288,   275,   143,    49,   283,    22,   111,
      17,    24,    29,   240,   335,   268,   331,   115,   277,    31,
     323,   308,    31,   114,    27,    28,   288,    37,   284,    44,
     163,   347,   285,   189,   125,   304,    45,    46,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,    42,
     298,   206,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   190,   227,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   104,   105,   106,   107,
     108,    50,    53,    54,    62,    63,   234,    55,    56,    64,
      88,   332,   158,   159,   160,   161,   162,   106,   107,   108,
     339,   125,   117,   117,   117,   126,   185,    57,   186,   345,
     143,   192,    84,    85,    86,    87,   193,   351,    89,   229,
     237,   238,   239,   160,   161,   162,   226,    58,   231,   232,
     189,   233,   280,    81,   236,   271,   278,   289,   294,    59,
     291,    60,   305,   290,   293,   297,   299,   300,   282,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,    90,   296,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   302,   303,
     117,   310,   315,   311,   317,   316,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,   107,   108,    90,   314,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   318,   321,   325,
     322,   328,   340,   329,   330,   334,   337,   341,   343,   348,
     349,   144,   109,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   292,    23,   113,   336,    51,   187,    66,    39,    43,
      80,   269,     0,   267,     0,     0,    67,    68,    69,     0,
     165,    70,    71,     0,    72,    73,     0,   241,     0,     0,
     242,     0,     0,     0,   243,   244,   245,   246,   247,   248,
       0,    75,    76,     1,     2,     3,     4,     5,     6,     7,
       8,     9,     0,     0,   194,     0,    10,    11,     0,     0,
     249,    77,   250,     0,     0,   251,     0,   252,   253,    66,
     186,     0,     0,    79,     0,     0,   254,     0,    67,    68,
      69,     0,    66,    70,    71,     0,    72,    73,     0,     0,
       0,    67,    68,    69,     0,     0,    70,    71,     0,    72,
      73,    74,     0,    75,    76,     0,    53,    54,     0,     0,
       0,    55,    56,     0,    74,    66,    75,    76,     0,     0,
       0,     0,     0,    77,    67,    68,    69,     0,     0,    70,
      71,    57,    72,    73,     0,    79,    77,     0,   281,     0,
       0,     0,     0,     0,     0,    78,     0,    74,    79,    75,
      76,    58,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,    59,     0,     0,     0,     0,     0,    77,
       1,     2,     3,     4,     5,     6,     7,     8,     9,     0,
       0,    79,     0,    10,    11,    12,   141,   142,     0,     0,
       0,   143,     0,     0,     0,     0,    13,    90,   228,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   144,   270,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   144,     0,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     1,
       2,     3,     4,     5,     6,     7,     8,     9,     0,     0,
       0,     0,    10,    11,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,    -1,    -1,   156,   157,
     158,   159,   160,   161,   162
};

static const short yycheck[] =
{
      52,    19,     3,    16,    16,   278,   251,    32,    43,    55,
      43,    66,    67,    68,    69,    70,    71,    72,    73,   285,
      38,   294,    53,    55,    79,    53,    78,    43,    53,    31,
      67,    68,    73,    70,    71,     0,    32,   288,    55,    55,
      38,    39,    79,   288,    90,    43,    64,    53,     0,    90,
      15,    90,    32,    88,   327,    88,   322,    53,    90,    87,
     311,    77,    87,    65,    53,    87,   311,    87,    74,     4,
      88,   344,    78,   125,    87,    87,    43,    55,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    90,
     273,   143,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   126,   164,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    30,    31,    32,    33,
      34,    88,    30,    31,    88,    88,   191,    35,    36,     3,
      87,   324,    30,    31,    32,    33,    34,    32,    33,    34,
     333,    87,   207,   208,   209,    87,    56,    55,    84,   342,
      43,     3,    53,    54,    55,    56,    85,   350,    59,    90,
     207,   208,   209,    32,    33,    34,    88,    75,    88,     3,
     232,    88,    53,   235,    89,    87,    87,    90,     3,    87,
      85,    89,    53,    90,    90,    87,    90,    90,   250,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    15,   271,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    90,    90,
     295,    85,    88,    86,    48,   297,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,   295,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    90,    77,    88,
       3,    88,   334,     3,    89,    87,    45,    90,    88,    90,
      88,    15,    89,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,   256,    15,    64,   329,    41,   112,    21,    29,    34,
      52,   235,    -1,   232,    -1,    -1,    30,    31,    32,    -1,
      88,    35,    36,    -1,    38,    39,    -1,   228,    -1,    -1,
      44,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    88,    -1,    70,    71,    -1,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    82,    21,
      84,    -1,    -1,    87,    -1,    -1,    90,    -1,    30,    31,
      32,    -1,    21,    35,    36,    -1,    38,    39,    -1,    -1,
      -1,    30,    31,    32,    -1,    -1,    35,    36,    -1,    38,
      39,    53,    -1,    55,    56,    -1,    30,    31,    -1,    -1,
      -1,    35,    36,    -1,    53,    21,    55,    56,    -1,    -1,
      -1,    -1,    -1,    75,    30,    31,    32,    -1,    -1,    35,
      36,    55,    38,    39,    -1,    87,    75,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    53,    87,    55,
      56,    75,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    87,    -1,    -1,    -1,    -1,    -1,    75,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    87,    -1,    70,    71,    72,    38,    39,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    83,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    70,    71,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      70,    71,    72,    83,    92,    93,   119,   125,   127,   137,
      55,    53,     0,    92,    90,   126,   127,    53,    87,    32,
      53,    87,   128,   129,   131,   132,   133,    87,   120,   131,
     132,     3,    90,   133,     4,    43,    55,   122,   123,   127,
      88,   129,   130,    30,    31,    35,    36,    55,    75,    87,
      89,   148,    88,    88,     3,   124,    21,    30,    31,    32,
      35,    36,    38,    39,    53,    55,    56,    75,    84,    87,
     134,   140,   141,   142,   148,   148,   148,   148,    87,   148,
      15,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    89,
      73,    90,   121,   122,   132,    53,   141,   141,   142,   142,
     141,   142,   142,   141,   141,    87,    87,   134,   135,   142,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    38,    39,    43,    15,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,   127,   149,    88,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,    56,    84,    96,   139,   140,
     127,   147,     3,    85,    88,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   145,   143,   144,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,    88,   141,    16,    90,
      97,    88,     3,    88,   141,   136,    89,   142,   142,   142,
      88,   148,    44,    48,    49,    50,    51,    52,    53,    74,
      76,    79,    81,    82,    90,    94,    95,    96,    98,   103,
     104,   105,   117,   118,   125,   138,   140,   139,    88,   135,
      16,    87,   107,   109,    55,    90,    55,    90,    87,    16,
      53,    90,   140,    53,    74,    78,    99,   100,   117,    90,
      90,    85,    94,    90,     3,   146,   140,    87,    95,    90,
      90,   138,    90,    90,    87,    53,    43,    55,    77,   102,
      85,    86,    99,   138,   142,    88,   140,    48,    90,   101,
     102,    77,     3,    99,   106,    88,   110,   112,    88,     3,
      89,   102,    95,   108,    87,   138,   101,    45,   115,    95,
     140,    90,   116,    88,   113,    95,   111,   138,    90,    88,
     114,    95
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror (scanner, pp, "syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *scanner, cl_parser_params *pp);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void *scanner, cl_parser_params *pp)
#else
int
yyparse (scanner, pp)
    void *scanner;
    cl_parser_params *pp;
#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 16:
#line 123 ".\\zl_parser.y"
    {
			pp->current_level++;
		;}
    break;

  case 17:
#line 127 ".\\zl_parser.y"
    {
			cl_var_free_level(&pp->vars_table, pp->current_level);
			pp->current_level--;
		;}
    break;

  case 22:
#line 146 ".\\zl_parser.y"
    {
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, yyvsp[0].string);
			free_str(yyvsp[0].string);

			if(!label)
			{
				ZL_ERROR("internal error");
			}

			cl_push(pp, (unsigned char) yyvsp[-1].value);
			cl_label_reference(label, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 23:
#line 161 ".\\zl_parser.y"
    { cl_push_op(pp, (unsigned char) yyvsp[-3].value, &yyvsp[-2], &yyvsp[0]); ;}
    break;

  case 24:
#line 162 ".\\zl_parser.y"
    { cl_push_op(pp, (unsigned char) yyvsp[-1].value, &yyvsp[0], NULL) ;}
    break;

  case 25:
#line 163 ".\\zl_parser.y"
    { cl_push_op(pp, (unsigned char) yyvsp[0].value, NULL, NULL) ;}
    break;

  case 26:
#line 165 ".\\zl_parser.y"
    {
			cl_label_node *func;

			func = cl_label_find(pp->funcs_table, yyvsp[-3].string);
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
		;}
    break;

  case 28:
#line 207 ".\\zl_parser.y"
    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value); ;}
    break;

  case 29:
#line 208 ".\\zl_parser.y"
    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value); ;}
    break;

  case 31:
#line 213 ".\\zl_parser.y"
    { yyval = yyvsp[0]; yyval.flags = ARG_REG ;}
    break;

  case 32:
#line 214 ".\\zl_parser.y"
    { yyval = yyvsp[-2]; yyval.flags = ARG_PREG ;}
    break;

  case 33:
#line 215 ".\\zl_parser.y"
    { yyval = yyvsp[0]; yyval.flags = ARG_IMM ;}
    break;

  case 35:
#line 241 ".\\zl_parser.y"
    { cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); ;}
    break;

  case 36:
#line 245 ".\\zl_parser.y"
    { cl_push(pp, OP_NOP); ;}
    break;

  case 37:
#line 246 ".\\zl_parser.y"
    { cl_push(pp, OP_DBG_PRINT); ;}
    break;

  case 38:
#line 248 ".\\zl_parser.y"
    {
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack) ZL_ERROR("break not in loop");

			cl_label_reference((cl_label_node *) pp->cl_loop_stack->next_node->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 39:
#line 256 ".\\zl_parser.y"
    {
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack || !pp->cl_loop_stack->next_node) ZL_ERROR("continue not in loop");

			cl_label_reference((cl_label_node *) pp->cl_loop_stack->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 40:
#line 264 ".\\zl_parser.y"
    {
			cl_stack_buffer *lsb;
			lsb = pp->cl_loop_stack;
			yyvsp[-1].value = yyvsp[-1].value*2-1;
			while(yyvsp[-1].value--)
			{
				lsb = lsb->next_node;
				if(!lsb) ZL_ERROR("break not in loop");
			}
			cl_push(pp, OP_JMP);

			cl_label_reference((cl_label_node *) lsb->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 41:
#line 279 ".\\zl_parser.y"
    {
			cl_stack_buffer *lsb;
			lsb = pp->cl_loop_stack;
			yyvsp[-1].value = (yyvsp[-1].value-1)*2;
			while(yyvsp[-1].value--)
			{
				lsb = lsb->next_node;
				if(!lsb) ZL_ERROR("continue not in loop");
			}
			cl_push(pp, OP_JMP);

			cl_label_reference((cl_label_node *) lsb->data, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 42:
#line 294 ".\\zl_parser.y"
    {
			cl_label_node *lb_label;

			lb_label = cl_label_find(pp->labels_table, "@exit_func");

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_label, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 43:
#line 304 ".\\zl_parser.y"
    {
			cl_label_node *lb_label;

			lb_label = cl_label_find(pp->labels_table, "@exit_func");

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_JMP);
			cl_label_reference(lb_label, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 44:
#line 318 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 45:
#line 329 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		;}
    break;

  case 46:
#line 336 ".\\zl_parser.y"
    {
			cl_label_node *lb_exit, *lb_condition;
			lb_exit = cl_label_define(&pp->labels_table, NULL);
			lb_condition = cl_label_define(&pp->labels_table, NULL);
			lb_condition->offset = pp->hc_fill;
			lb_condition->flags = ZLF_DEFINED;
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
		;}
    break;

  case 47:
#line 346 ".\\zl_parser.y"
    {
			cl_label_node *lb_exit;
			lb_exit= (cl_label_node *) pp->cl_loop_stack->next_node->data;
			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_exit, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 48:
#line 354 ".\\zl_parser.y"
    {
			cl_label_node *lb_exit, *lb_condition;
			lb_condition = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_condition, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		;}
    break;

  case 49:
#line 366 ".\\zl_parser.y"
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
		;}
    break;

  case 50:
#line 380 ".\\zl_parser.y"
    {
			cl_label_node *lb_condition;
			lb_condition = (cl_label_node *) pp->cl_loop_stack->data;
			lb_condition->offset = pp->hc_fill;
			lb_condition->flags = ZLF_DEFINED;
		;}
    break;

  case 51:
#line 387 ".\\zl_parser.y"
    {
			cl_label_node *lb_exit, *lb_condition;
			lb_condition = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_label_reference((cl_label_node *) cl_stack_pop(&pp->cl_stack), pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		;}
    break;

  case 53:
#line 399 ".\\zl_parser.y"
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
		;}
    break;

  case 54:
#line 416 ".\\zl_parser.y"
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
		;}
    break;

  case 55:
#line 437 ".\\zl_parser.y"
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
		;}
    break;

  case 56:
#line 453 ".\\zl_parser.y"
    {
			cl_label_node *lb_exit, *lb_loop;
			lb_loop = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (cl_label_node *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_label_reference(lb_loop, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_exit->offset = pp->hc_fill;
			lb_exit->flags = ZLF_DEFINED;
		;}
    break;

  case 58:
#line 470 ".\\zl_parser.y"
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
		;}
    break;

  case 60:
#line 487 ".\\zl_parser.y"
    {
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, yyvsp[-1].string);
			free_str(yyvsp[-1].string);

			if(label->flags & ZLF_DEFINED)
			{
				ZL_ERROR("label redefined");
			}

			label->offset = pp->hc_fill;
			label->flags = ZLF_DEFINED;
		;}
    break;

  case 61:
#line 505 ".\\zl_parser.y"
    {
			cl_label_node *label;

			label = cl_label_define(&pp->labels_table, yyvsp[-1].string);
			free_str(yyvsp[-1].string);

			if(!label)
			{
				ZL_ERROR("internal error");
			}

			cl_push(pp, (unsigned char) yyvsp[-2].value);
			cl_label_reference(label, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 62:
#line 524 ".\\zl_parser.y"
    {
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, yyvsp[-4].string);
			free_str(yyvsp[-4].string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_DEFINED;
			if(!yyvsp[-5].uvalue)
			{
				func->flags |= ZLF_STDCALL;
			}
			func->library = NULL;
			func->params = yyvsp[-2].uvalue;
		;}
    break;

  case 63:
#line 544 ".\\zl_parser.y"
    {
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, yyvsp[-6].string);
			free_str(yyvsp[-6].string);

			if(func->flags & ZLF_DEFINED)
			{
				ZL_ERROR("function redefined");
			}

			func->flags = ZLF_DEFINED;
			if(!yyvsp[-7].uvalue)
			{
				func->flags |= ZLF_STDCALL;
			}
			func->library = yyvsp[-1].string;
			func->params = yyvsp[-4].uvalue;
		;}
    break;

  case 64:
#line 581 ".\\zl_parser.y"
    {
			pp->current_level++;
		;}
    break;

  case 65:
#line 585 ".\\zl_parser.y"
    {
			cl_label_node *func;

			cl_label_define(&pp->labels_table, "@exit_func");

			func = cl_label_define(&pp->funcs_table, yyvsp[-4].string);
			free_str(yyvsp[-4].string);

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
		;}
    break;

  case 66:
#line 614 ".\\zl_parser.y"
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
		;}
    break;

  case 67:
#line 653 ".\\zl_parser.y"
    {
			yyval.value = 8;
			yyvsp[0].var->offset = yyval.value;
			yyvsp[0].var->flags |= ZLF_PARAM;
		;}
    break;

  case 68:
#line 659 ".\\zl_parser.y"
    {
			yyval.value = yyvsp[0].value + 4;
			yyvsp[-2].var->offset = yyval.value;
			yyvsp[-2].var->flags |= ZLF_PARAM;
		;}
    break;

  case 70:
#line 669 ".\\zl_parser.y"
    {
			if((yyvsp[0].flags & ZLF_EXTERNAL) && (pp->current_level > 0))
			{
				ZL_ERROR("external variable must be defined global");
			}

			if(!(yyvsp[0].flags & ZLF_TYPE))
			{
				yyvsp[0].flags |= ZLF_INT;	// use default type
			}

			cl_stack_push(&pp->cl_stack, (void *) yyvsp[0].flags);
		;}
    break;

  case 71:
#line 683 ".\\zl_parser.y"
    {
			if(yyvsp[0].var->flags & ZLF_AUTOARRAY)
			{
				//ZL_ERROR("unknown size");

				yyvsp[0].var->size[1] = 1;

				/*
				$3.var->flags += 0x01000000;
				$3.var->rows--;
				if(!$3.var->rows)
				{
					$3.var->flags &= ~ZLF_ARRAY;
				}
				*/
			}

			if(yyvsp[0].var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				//$3.var->flags += 0x01000000;
				//$3.var->size[0] = 4;

				level = yyvsp[0].var->rows;

				yyvsp[0].var->size[level] *= yyvsp[0].var->size[0];
				level--;

				while(level)
				{
					yyvsp[0].var->size[level] *= yyvsp[0].var->size[level+1];
					level--;
				}
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = yyvsp[0].var->rows;

			while(dc < uc)
			{
				x = yyvsp[0].var->size[dc];
				yyvsp[0].var->size[dc] = yyvsp[0].var->size[uc];
				yyvsp[0].var->size[uc] = x;

				dc++;
				uc--;
			}

			yyvsp[0].var->references = 0;

			yyval = yyvsp[0];

			cl_stack_pop(&pp->cl_stack);
		;}
    break;

  case 72:
#line 743 ".\\zl_parser.y"
    { /* nothing to do */ ;}
    break;

  case 73:
#line 745 ".\\zl_parser.y"
    {
			if((yyvsp[0].flags & ZLF_EXTERNAL) && (pp->current_level > 0))
			{
				ZL_ERROR("external variable must be defined global");
			}

			if(!(yyvsp[0].flags & ZLF_TYPE))
			{
				yyvsp[0].flags |= ZLF_INT;	// use default type
			}

			cl_stack_push(&pp->cl_stack, (void *) yyvsp[0].flags);
		;}
    break;

  case 74:
#line 759 ".\\zl_parser.y"
    {
			cl_stack_pop(&pp->cl_stack);
		;}
    break;

  case 75:
#line 765 ".\\zl_parser.y"
    { yyval.flags = yyvsp[0].flags ;}
    break;

  case 76:
#line 767 ".\\zl_parser.y"
    {
			yyval.flags = yyvsp[-1].flags | yyvsp[0].flags;
			if((yyval.flags & (ZLF_SIGNED | ZLF_UNSIGNED)) == (ZLF_SIGNED | ZLF_UNSIGNED))
			{
				ZL_ERROR("signed/unsigned keywords mutually exclusive");
			}

			if((yyval.flags & (ZLF_EXTERNAL | ZLF_INTERNAL)) == (ZLF_EXTERNAL | ZLF_INTERNAL))
			{
				ZL_ERROR("internal/external keywords mutually exclusive");
			}

			if((yyval.flags & (ZLF_SHORT | ZLF_LONG)) == (ZLF_SHORT | ZLF_LONG))
			{
				ZL_ERROR("short/long keywords mutually exclusive");
			}

			if((yyval.flags & (ZLF_SHORT | ZLF_LONG)) && (yyval.flags & ZLF_TYPE) && !(yyval.flags & ZLF_INT))
			{
				ZL_ERROR("short/long keywords can not be used with none 'int' type");
			}

			if((yyval.flags & (ZLF_SIGNED | ZLF_UNSIGNED)) && (yyval.flags & ZLF_TYPE) && !(yyval.flags & (ZLF_INT|ZLF_CHAR)))
			{
				ZL_ERROR("signed/unsigned keywords can not be used with none 'int/char' type ");
			}

			if((yyvsp[-1].flags & ZLF_TYPE) && (yyvsp[0].flags & ZLF_TYPE))
			{
				ZL_ERROR("type specifier followed by type specifier is illegal");
			}
		;}
    break;

  case 77:
#line 802 ".\\zl_parser.y"
    { /* nothing to do */ ;}
    break;

  case 78:
#line 803 ".\\zl_parser.y"
    { /* nothing to do */ ;}
    break;

  case 79:
#line 808 ".\\zl_parser.y"
    {
			if(yyvsp[0].var->flags & ZLF_AUTOARRAY)
			{
				ZL_ERROR("unknown size");
			}

			if(yyvsp[0].var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				level = yyvsp[0].var->rows;

				yyvsp[0].var->size[level] *= yyvsp[0].var->size[0];
				level--;

				while(level)
				{
					yyvsp[0].var->size[level] *= yyvsp[0].var->size[level+1];
					level--;
				}

				//$1.var->size[0] = $1.var->size[1];
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = yyvsp[0].var->rows;

			while(dc < uc)
			{
				x = yyvsp[0].var->size[dc];
				yyvsp[0].var->size[dc] = yyvsp[0].var->size[uc];
				yyvsp[0].var->size[uc] = x;

				dc++;
				uc--;
			}

			if((~yyvsp[0].var->flags & ZLF_EXTERNAL) && (yyvsp[0].var->level != 0))
			{
				yyvsp[0].var->offset = pp->stack_size;
				pp->stack_size += yyvsp[0].var->size[yyvsp[0].var->rows];
				if(yyvsp[0].var->size[yyvsp[0].var->rows] % 4)
				{
					pp->stack_size += 4 - (yyvsp[0].var->size[yyvsp[0].var->rows] % 4);
				}
			}

			yyvsp[0].var->references = 0;

			/*
			if(!cl_var_define(&pp->vars_table, &$1.var))
			{
				ZL_ERROR("redefinition");
			}
			*/
		;}
    break;

  case 80:
#line 867 ".\\zl_parser.y"
    {
			cl_var_node *var;

			if(yyvsp[-1].var->flags & ZLF_ARRAY)
			{
				unsigned long level;

				level = yyvsp[-1].var->rows;

				yyvsp[-1].var->size[level] *= yyvsp[-1].var->size[0];
				level--;

				while(level)
				{
					yyvsp[-1].var->size[level] *= yyvsp[-1].var->size[level+1];
					level--;
				}

				//$1.var->size[0] = $1.var->size[1];
			}

			unsigned long dc, uc, x;

			dc = 1;
			uc = yyvsp[-1].var->rows;

			while(dc < uc)
			{
				x = yyvsp[-1].var->size[dc];
				yyvsp[-1].var->size[dc] = yyvsp[-1].var->size[uc];
				yyvsp[-1].var->size[uc] = x;

				dc++;
				uc--;
			}

			yyvsp[-1].var->references = 0;

			var = yyvsp[-1].var;
			/*
			var = cl_var_define(&pp->vars_table, &$1.var);
			if(!var)
			{
				ZL_ERROR("redefinition");
			}
			*/

			cl_stack_push(&pp->cl_stack, (void *) var);

			if(yyvsp[-1].var->flags & ZLF_EXTERNAL)
			{
				cl_push(pp, OP_PUSH_PMEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else if(yyvsp[-1].var->level == 0) // global var
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
		;}
    break;

  case 81:
#line 938 ".\\zl_parser.y"
    {
			cl_var_node *var;

			var = (cl_var_node *) pp->cl_stack->data;
			if(var->flags & ZLF_AUTOARRAY)
			{
				if((yyvsp[0].size % var->size[var->rows-1]) > 0)
				{
					var->size[var->rows] = var->size[var->rows-1] - (yyvsp[0].size % var->size[var->rows-1]) + yyvsp[0].size;
				}
				else
				{
					var->size[var->rows] = yyvsp[0].size;
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
				if(var->size[var->rows] < yyvsp[0].size)
				{
					ZL_ERROR("too many initializers");
				}
			}

			if((~var->flags & ZLF_EXTERNAL) && (var->level != 0))
			{
				pp->stack_size += var->size[var->rows];
				if(yyvsp[-3].var->size[yyvsp[-3].var->rows] % 4)
				{
					pp->stack_size += 4 - (yyvsp[-3].var->size[yyvsp[-3].var->rows] % 4);
				}
			}

			cl_stack_pop(&pp->cl_stack);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		;}
    break;

  case 82:
#line 983 ".\\zl_parser.y"
    { yyval.flags = 0x01000000 ;}
    break;

  case 83:
#line 984 ".\\zl_parser.y"
    { yyval.flags = 0x01000000 + yyvsp[0].flags ;}
    break;

  case 84:
#line 989 ".\\zl_parser.y"
    {
			yyval = yyvsp[0];
			//memcpy(&$$, &$2, sizeof($$));

			yyval.var->flags += yyvsp[-1].flags;
			yyval.var->size[0] = 4;
		;}
    break;

  case 85:
#line 997 ".\\zl_parser.y"
    {
			yyval = yyvsp[0];

			if(yyval.var->flags & ZLF_VOID)
			{
				ZL_ERROR("illegal use of type 'void'");
			}
		;}
    break;

  case 86:
#line 1009 ".\\zl_parser.y"
    {
			yyval.var = cl_var_define(&pp->vars_table, yyvsp[0].string, pp->current_level);

			if(!yyval.var)
			{
				ZL_ERROR("redefinition");
			}

			yyval.var->name = yyvsp[0].string;
			yyval.var->flags = (unsigned long) pp->cl_stack->data;
			yyval.var->level = pp->current_level;
			yyval.var->rows = 0;
			yyval.var->size[0] = 4; // default var size

			switch(yyval.var->flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					yyval.var->size[0] = 1;
					break;
				case ZLF_INT:
					if(yyval.var->flags & ZLF_SHORT)
					{
						yyval.var->size[0] = 2;
					}
					break;
				case ZLF_DOUBLE:
					yyval.var->size[0] = 8;
					break;
			}
		;}
    break;

  case 87:
#line 1040 ".\\zl_parser.y"
    {
			yyval = yyvsp[-3];

			if(yyvsp[-3].var->flags & ZLF_ARRAY)
			{
				//ZL_ERROR("sorry, multi-level arrays not yet supported!");
			}
			
			if(yyvsp[-1].uvalue == 0)
			{
				ZL_ERROR("cannot allocate an array of constant size 0.");
			}

			yyval.var->flags |= ZLF_ARRAY;
			yyval.var->rows++;
			yyval.var->size[yyval.var->rows] = yyvsp[-1].uvalue; // кол-во элементов в строке массива
			// уровнем выше делать перерасчёт кол-ва элементов в размер строки массива
		;}
    break;

  case 88:
#line 1059 ".\\zl_parser.y"
    {
			// last array level with auto size
			yyval = yyvsp[-2];

			if(yyvsp[-2].var->flags & ZLF_ARRAY)
			{
				ZL_ERROR("unknown size, missing subscript!");
			}

			yyval.var->flags |= ZLF_ARRAY | ZLF_AUTOARRAY;
			yyval.var->rows++;
			yyval.var->size[yyval.var->rows] = 0; // set after initialize
		;}
    break;

  case 89:
#line 1073 ".\\zl_parser.y"
    {
			yyval = yyvsp[-1];
		;}
    break;

  case 90:
#line 1080 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			yyval.size = 4;

			if(!(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_POINTER))
			{
				switch(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						yyval.size = 1;
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(((cl_var_node *) pp->cl_stack->data)->flags & ZLF_SHORT)
						{
							yyval.size = 2;
							cl_push(pp, OP_SIZE_OVERRIDE_2);
						}
						break;
					case ZLF_DOUBLE:
						yyval.size = 8;
						ZL_ERROR("sorry, double not yet supported");
						break;
				}
			}

			cl_push(pp, OP_MOV_PREG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);
		;}
    break;

  case 91:
#line 1112 ".\\zl_parser.y"
    {
			yyval = yyvsp[-1];
		;}
    break;

  case 92:
#line 1119 ".\\zl_parser.y"
    {
			//if(((zl_names_map *) pp->cl_stack->data)->flags & ZLF_AUTOARRAY)
			//{
				//((zl_names_map *) pp->cl_stack->data)->elements->size += var_size;
			//}
		;}
    break;

  case 93:
#line 1126 ".\\zl_parser.y"
    {
			if(~((cl_var_node *) pp->cl_stack->data)->flags & ZLF_ARRAY)
			{
				ZL_ERROR("too many initializers");
			}

			// check array for overflow here or error!

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);

			if(yyvsp[-1].size > 1)
			{
				cl_push(pp, OP_ADD_REG_IMM); cl_push(pp, REG_EAX); cl_push_dw(pp, yyvsp[-1].size);
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
		;}
    break;

  case 94:
#line 1153 ".\\zl_parser.y"
    {
			yyval.size = yyvsp[-3].size + yyvsp[0].size;
		;}
    break;

  case 95:
#line 1184 ".\\zl_parser.y"
    { yyval.flags = ZLF_VOID ;}
    break;

  case 96:
#line 1185 ".\\zl_parser.y"
    { yyval.flags = ZLF_CHAR ;}
    break;

  case 97:
#line 1186 ".\\zl_parser.y"
    { yyval.flags = ZLF_SHORT ;}
    break;

  case 98:
#line 1187 ".\\zl_parser.y"
    { yyval.flags = ZLF_INT ;}
    break;

  case 99:
#line 1188 ".\\zl_parser.y"
    { yyval.flags = ZLF_LONG ;}
    break;

  case 100:
#line 1189 ".\\zl_parser.y"
    { yyval.flags = ZLF_FLOAT ;}
    break;

  case 101:
#line 1190 ".\\zl_parser.y"
    { yyval.flags = ZLF_DOUBLE ;}
    break;

  case 102:
#line 1191 ".\\zl_parser.y"
    { yyval.flags = ZLF_SIGNED ;}
    break;

  case 103:
#line 1192 ".\\zl_parser.y"
    { yyval.flags = ZLF_UNSIGNED ;}
    break;

  case 104:
#line 1193 ".\\zl_parser.y"
    { yyval.flags = ZLF_EXTERNAL ;}
    break;

  case 105:
#line 1194 ".\\zl_parser.y"
    { yyval.flags = ZLF_INTERNAL ;}
    break;

  case 107:
#line 1201 ".\\zl_parser.y"
    { cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); ;}
    break;

  case 108:
#line 1205 ".\\zl_parser.y"
    { yyval.size = 0; ;}
    break;

  case 109:
#line 1206 ".\\zl_parser.y"
    { yyval.size = 4; ;}
    break;

  case 110:
#line 1207 ".\\zl_parser.y"
    { yyval.size = 4 + yyvsp[0].size; ;}
    break;

  case 112:
#line 1213 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 113:
#line 1240 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 114:
#line 1269 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 115:
#line 1298 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 116:
#line 1327 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 117:
#line 1356 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 118:
#line 1385 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 119:
#line 1414 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 120:
#line 1443 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 121:
#line 1472 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 122:
#line 1501 ".\\zl_parser.y"
    {
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);

			if(!(yyvsp[-2].flags & ZLF_POINTER))
			{
				switch(yyvsp[-2].flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						cl_push(pp, OP_SIZE_OVERRIDE_1);
						break;
					case ZLF_INT:
						if(yyvsp[-2].flags & ZLF_SHORT)
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
		;}
    break;

  case 123:
#line 1533 ".\\zl_parser.y"
    {
			// push address of value to stack

			cl_var_node *var;

			yyval = yyvsp[0];

			var = cl_var_find(pp->vars_table, yyvsp[0].string);
			free_str(yyvsp[0].string);
			if(!var)
			{
				ZL_ERROR("undeclared identifier");
			}

			//memcpy(&$$.var, var, sizeof(cl_var_node));
			yyval.var = var;
			yyval.flags = var->flags;
			yyval.rows = var->rows;

			if(yyval.flags & ZLF_EXTERNAL)
			{
				cl_push(pp, OP_PUSH_PMEM);
				cl_var_reference(var, pp->hc_fill);
				cl_push_dw(pp, 0);
			}
			else if(yyval.var->level == 0) // global var
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

				if((yyval.flags & (ZLF_PARAM | ZLF_ARRAY)) == (ZLF_PARAM | ZLF_ARRAY))
				{
					// ссылка на массив переданная через параметр функции
					cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EBX); cl_push(pp, REG_EBX);
				}

				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EBX);
			}
		;}
    break;

  case 124:
#line 1587 ".\\zl_parser.y"
    {
			yyval = yyvsp[0];
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			if(!(yyvsp[0].flags & ZLF_POINTER))
			{
				ZL_ERROR("illegal indirection");
			}

			yyval.flags -= 0x01000000;
		;}
    break;

  case 125:
#line 1600 ".\\zl_parser.y"
    {
			// pop address
			// add to address offset
			// push address this address to stack

			// ul *v = {0,1,2};
			// v[1]; //= 1

			yyval.flags = yyvsp[-3].flags;

			if(yyval.flags & ZLF_ARRAY)
			{
				unsigned long ptr_step;
				ptr_step = 4;
				
				yyval.rows--;

				if(yyval.rows)
				{
					ptr_step = yyval.var->size[yyval.rows];
				}
				else
				{
					yyval.flags &= ~ZLF_ARRAY;

					if(!(yyval.flags & ZLF_POINTER))
					{
						ptr_step = yyval.var->size[0];
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
			else if(yyval.flags & ZLF_POINTER)
			{
				yyval.flags -= 0x01000000;

				if(!(yyval.flags & ZLF_POINTER))
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							yyval.var->size[0] = 1;
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
							{
								yyval.var->size[0] = 2;
							}
							break;
						case ZLF_DOUBLE:
							yyval.var->size[0] = 8;
							break;
					}
				}

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, yyval.var->size[0]);
				cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
				cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
			}
			else
			{
				ZL_ERROR("subscript requires array or pointer type");
			}
		;}
    break;

  case 126:
#line 1685 ".\\zl_parser.y"
    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value) ;}
    break;

  case 127:
#line 1688 ".\\zl_parser.y"
    {
			cl_push(pp, OP_PUSH_OFFSET);
			cl_const_define(&pp->data_table, yyvsp[0].string, yyvsp[0].size, pp->hc_fill); // добавляем ссылку на эти данные
			free_str(yyvsp[0].string);
			cl_push_dw(pp, 0);						// ставим заглушку
		;}
    break;

  case 128:
#line 1694 ".\\zl_parser.y"
    { yyval = yyvsp[-1]; ;}
    break;

  case 129:
#line 1695 ".\\zl_parser.y"
    { cl_do_op(pp, OP_ADD_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 130:
#line 1696 ".\\zl_parser.y"
    { cl_do_op(pp, OP_SUB_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 131:
#line 1697 ".\\zl_parser.y"
    { cl_do_op(pp, OP_MUL_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 132:
#line 1698 ".\\zl_parser.y"
    { cl_do_op(pp, OP_DIV_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 133:
#line 1699 ".\\zl_parser.y"
    { cl_do_op(pp, OP_OR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 134:
#line 1700 ".\\zl_parser.y"
    { cl_do_op(pp, OP_AND_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 135:
#line 1701 ".\\zl_parser.y"
    { cl_do_op(pp, OP_MOD_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 136:
#line 1702 ".\\zl_parser.y"
    { cl_do_op(pp, OP_XOR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 137:
#line 1703 ".\\zl_parser.y"
    { cl_do_op(pp, OP_SHL_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 138:
#line 1704 ".\\zl_parser.y"
    { cl_do_op(pp, OP_SHR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 139:
#line 1705 ".\\zl_parser.y"
    { cl_do_op(pp, OP_G_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 140:
#line 1706 ".\\zl_parser.y"
    { cl_do_op(pp, OP_L_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 141:
#line 1707 ".\\zl_parser.y"
    { cl_do_op(pp, OP_GE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 142:
#line 1708 ".\\zl_parser.y"
    { cl_do_op(pp, OP_LE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 143:
#line 1709 ".\\zl_parser.y"
    { cl_do_op(pp, OP_E_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 144:
#line 1710 ".\\zl_parser.y"
    { cl_do_op(pp, OP_NE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 145:
#line 1712 ".\\zl_parser.y"
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
		;}
    break;

  case 146:
#line 1725 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		;}
    break;

  case 147:
#line 1732 ".\\zl_parser.y"
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
		;}
    break;

  case 148:
#line 1745 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		;}
    break;

  case 149:
#line 1752 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = cl_label_define(&pp->labels_table, NULL);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_label_reference(lb_skip, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 150:
#line 1762 ".\\zl_parser.y"
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
		;}
    break;

  case 151:
#line 1775 ".\\zl_parser.y"
    {
			cl_label_node *lb_skip;
			lb_skip = (cl_label_node *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			lb_skip->flags = ZLF_DEFINED;
		;}
    break;

  case 152:
#line 1781 ".\\zl_parser.y"
    { yyval = yyvsp[0]; cl_push(pp, OP_BNOT_STK); ;}
    break;

  case 153:
#line 1782 ".\\zl_parser.y"
    { yyval = yyvsp[0]; cl_push(pp, OP_LNOT_STK); ;}
    break;

  case 154:
#line 1783 ".\\zl_parser.y"
    { yyval = yyvsp[0]; ;}
    break;

  case 155:
#line 1785 ".\\zl_parser.y"
    {
			yyval = yyvsp[0];
			yyval.flags &= ~ZLF_UNSIGNED;
			yyval.flags |= ZLF_SIGNED;
			cl_push(pp, OP_MINUS_STK);
		;}
    break;

  case 156:
#line 1791 ".\\zl_parser.y"
    { /* nothing to do */ ;}
    break;

  case 157:
#line 1808 ".\\zl_parser.y"
    {
			if(yyvsp[-1].flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);

			if(yyvsp[-1].flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if((yyvsp[-1].flags & ZLF_POINTER) == 0x01000000)
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
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
		;}
    break;

  case 158:
#line 1852 ".\\zl_parser.y"
    {
			if(yyvsp[-1].flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_ECX);

			if(yyvsp[-1].flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if((yyvsp[-1].flags & ZLF_POINTER) == 0x01000000)
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
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
		;}
    break;

  case 159:
#line 1896 ".\\zl_parser.y"
    {
			if(yyvsp[0].flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);

			if(yyvsp[0].flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if((yyvsp[-1].flags & ZLF_POINTER) == 0x01000000)
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
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
		;}
    break;

  case 160:
#line 1940 ".\\zl_parser.y"
    {
			if(yyvsp[0].flags & (ZLF_STRUCT | ZLF_ARRAY))
			{
				ZL_ERROR("needs l-value");
			}

			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_ECX); cl_push(pp, REG_EAX);

			if(yyvsp[0].flags & ZLF_POINTER)
			{
				unsigned long ptr_step;
				ptr_step = 4;

				if((yyvsp[-1].flags & ZLF_POINTER) == 0x01000000)
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							ptr_step = 1;
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
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
		;}
    break;

  case 161:
#line 1984 ".\\zl_parser.y"
    {
			cl_label_node *func;

			func = cl_label_define(&pp->funcs_table, yyvsp[-3].string);
			free_str(yyvsp[-3].string);

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
			if(yyvsp[-1].size)
			{
				cl_push(pp, OP_SUB_REG_IMM); cl_push(pp, REG_ESP); cl_push_dw(pp, yyvsp[-1].size);
			}
			cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);
		;}
    break;

  case 162:
#line 2023 ".\\zl_parser.y"
    {
			// get value from address and push
			if(!(yyvsp[0].flags & (ZLF_STRUCT | ZLF_ARRAY)))
			{
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);

				if(!(yyvsp[0].flags & ZLF_POINTER))
				{
					switch(yyvsp[0].flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_ECX); cl_push_dw(pp, 0x000000FF);
							cl_push(pp, OP_AND_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
							break;
						case ZLF_INT:
							if(yyvsp[0].flags & ZLF_SHORT)
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
		;}
    break;

  case 163:
#line 2055 ".\\zl_parser.y"
    {
			yyval.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			unsigned long var_size;
			switch(yyvsp[-1].flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if(yyvsp[-1].flags & ZLF_SHORT)
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
		;}
    break;

  case 164:
#line 2081 ".\\zl_parser.y"
    {
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		;}
    break;

  case 165:
#line 2085 ".\\zl_parser.y"
    {
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
			yyval.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[-1].var->size[yyvsp[-1].rows]);
		;}
    break;

  case 166:
#line 2097 ".\\zl_parser.y"
    { /* nop */ ;}
    break;

  case 167:
#line 2100 ".\\zl_parser.y"
    { /* nop */ ;}
    break;

  case 168:
#line 2101 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value + yyvsp[0].value; ;}
    break;

  case 169:
#line 2102 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value - yyvsp[0].value; ;}
    break;

  case 170:
#line 2103 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value * yyvsp[0].value; ;}
    break;

  case 171:
#line 2104 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value / yyvsp[0].value; ;}
    break;

  case 172:
#line 2105 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value | yyvsp[0].value; ;}
    break;

  case 173:
#line 2106 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value & yyvsp[0].value; ;}
    break;

  case 174:
#line 2107 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value % yyvsp[0].value; ;}
    break;

  case 175:
#line 2108 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value ^ yyvsp[0].value; ;}
    break;

  case 176:
#line 2109 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value << yyvsp[0].value; ;}
    break;

  case 177:
#line 2110 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value >> yyvsp[0].value; ;}
    break;

  case 178:
#line 2111 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value < yyvsp[0].value; ;}
    break;

  case 179:
#line 2112 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value > yyvsp[0].value; ;}
    break;

  case 180:
#line 2113 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value >= yyvsp[0].value; ;}
    break;

  case 181:
#line 2114 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value <= yyvsp[0].value; ;}
    break;

  case 182:
#line 2115 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value == yyvsp[0].value; ;}
    break;

  case 183:
#line 2116 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value != yyvsp[0].value; ;}
    break;

  case 184:
#line 2117 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value || yyvsp[0].value; ;}
    break;

  case 185:
#line 2118 ".\\zl_parser.y"
    { yyval.value = yyvsp[-2].value && yyvsp[0].value; ;}
    break;

  case 186:
#line 2119 ".\\zl_parser.y"
    { yyval.value = yyvsp[-4].value ? yyvsp[-2].value : yyvsp[0].value; ;}
    break;

  case 187:
#line 2120 ".\\zl_parser.y"
    { yyval.value = ~yyvsp[0].value; ;}
    break;

  case 188:
#line 2121 ".\\zl_parser.y"
    { yyval.value = !yyvsp[0].value; ;}
    break;

  case 189:
#line 2122 ".\\zl_parser.y"
    { /* nothing to do */ ;}
    break;

  case 190:
#line 2123 ".\\zl_parser.y"
    { yyval.value = -yyvsp[0].value; ;}
    break;

  case 191:
#line 2125 ".\\zl_parser.y"
    {
			unsigned long var_size;
			switch(yyvsp[-1].flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if(yyvsp[-1].flags & ZLF_SHORT)
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

			yyval.value = var_size;
		;}
    break;

  case 192:
#line 2150 ".\\zl_parser.y"
    {
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		;}
    break;

  case 193:
#line 2154 ".\\zl_parser.y"
    {
			yyval.value = yyvsp[-1].var->size[yyvsp[-1].rows];
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
		;}
    break;


    }

/* Line 991 of yacc.c.  */
#line 3947 "zl_parser.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (scanner, pp, yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror (scanner, pp, "syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (scanner, pp, "syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror (scanner, pp, "parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 2166 ".\\zl_parser.y"










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


