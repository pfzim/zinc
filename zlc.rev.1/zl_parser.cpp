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




/* Copy the first part of user declarations.  */



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
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   765

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  50
/* YYNRULES -- Number of rules. */
#define YYNRULES  177
/* YYNRULES -- Number of states. */
#define YYNSTATES  318

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   314

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      81,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,     2,     2,    34,    21,     2,
      82,    83,    32,    30,     3,    31,    46,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,    85,
      24,     4,    25,    15,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    84,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    79,    19,    80,    36,     2,     2,     2,
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
      74,    75,    76,    77,    78
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    27,    31,    35,    38,    40,    43,    48,
      51,    53,    58,    59,    63,    65,    66,    68,    70,    72,
      74,    78,    82,    84,    87,    90,    93,    96,   100,   104,
     105,   113,   114,   115,   123,   124,   125,   126,   137,   138,
     139,   140,   153,   154,   155,   159,   162,   166,   169,   170,
     175,   177,   180,   182,   186,   188,   189,   194,   196,   199,
     202,   204,   206,   211,   215,   217,   221,   223,   224,   229,
     231,   233,   235,   237,   239,   241,   243,   245,   247,   249,
     251,   253,   257,   258,   260,   264,   266,   270,   274,   278,
     282,   286,   290,   294,   298,   302,   306,   310,   312,   317,
     319,   321,   325,   329,   333,   337,   341,   345,   349,   353,
     357,   361,   365,   369,   373,   377,   381,   385,   389,   390,
     395,   396,   401,   402,   403,   411,   414,   417,   420,   423,
     426,   429,   432,   435,   438,   441,   446,   448,   453,   454,
     460,   462,   466,   470,   474,   478,   482,   486,   490,   494,
     498,   502,   506,   510,   514,   518,   522,   526,   530,   534,
     538,   544,   547,   550,   553,   556,   561,   562
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      87,     0,    -1,    88,    87,    -1,    -1,    89,    -1,    95,
      -1,    96,    -1,    97,    -1,   109,    -1,   110,    -1,   111,
      -1,    90,    -1,    79,    87,    80,    -1,    76,    91,    80,
      -1,    92,    81,    91,    -1,   109,    91,    -1,    92,    -1,
      72,    53,    -1,    75,    94,     3,    94,    -1,    75,    94,
      -1,    75,    -1,    53,    82,    93,    83,    -1,    -1,    94,
       3,    93,    -1,    94,    -1,    -1,    74,    -1,    55,    -1,
      56,    -1,    53,    -1,    43,    53,    84,    -1,    43,    74,
      84,    -1,    85,    -1,   124,    85,    -1,    78,    85,    -1,
      50,    85,    -1,    51,    85,    -1,    50,    55,    85,    -1,
      51,    55,    85,    -1,    -1,    44,    82,   126,    83,    98,
      88,   107,    -1,    -1,    -1,    48,    99,    82,   126,    83,
     100,    88,    -1,    -1,    -1,    -1,    49,   101,    88,    48,
     102,    82,   126,    83,   103,    85,    -1,    -1,    -1,    -1,
      52,    82,   124,    85,   104,   124,    85,   105,   124,    83,
     106,    88,    -1,    -1,    -1,    45,   108,    88,    -1,    53,
      16,    -1,    72,    53,    85,    -1,   113,    85,    -1,    -1,
     113,   112,   114,    85,    -1,   123,    -1,   123,   113,    -1,
     115,    -1,   114,     3,   115,    -1,   118,    -1,    -1,   118,
       4,   116,   120,    -1,    32,    -1,    32,   117,    -1,   117,
     119,    -1,   119,    -1,    53,    -1,   119,    43,   134,    84,
      -1,   119,    43,    84,    -1,   126,    -1,    79,   121,    80,
      -1,   120,    -1,    -1,   120,     3,   122,   121,    -1,    57,
      -1,    58,    -1,    59,    -1,    60,    -1,    61,    -1,    62,
      -1,    63,    -1,    64,    -1,    65,    -1,    70,    -1,    71,
      -1,   126,    -1,   126,     3,   124,    -1,    -1,   126,    -1,
     126,     3,   125,    -1,   128,    -1,   127,     4,   126,    -1,
     127,    10,   126,    -1,   127,     6,   126,    -1,   127,     7,
     126,    -1,   127,     8,   126,    -1,   127,     9,   126,    -1,
     127,    12,   126,    -1,   127,    11,   126,    -1,   127,     5,
     126,    -1,   127,    13,   126,    -1,   127,    14,   126,    -1,
      53,    -1,   127,    43,   126,    84,    -1,    55,    -1,    56,
      -1,    82,   128,    83,    -1,   128,    30,   128,    -1,   128,
      31,   128,    -1,   128,    32,   128,    -1,   128,    33,   128,
      -1,   128,    19,   128,    -1,   128,    21,   128,    -1,   128,
      34,   128,    -1,   128,    20,   128,    -1,   128,    29,   128,
      -1,   128,    28,   128,    -1,   128,    25,   128,    -1,   128,
      24,   128,    -1,   128,    26,   128,    -1,   128,    27,   128,
      -1,   128,    23,   128,    -1,   128,    22,   128,    -1,    -1,
     128,    17,   129,   128,    -1,    -1,   128,    18,   130,   128,
      -1,    -1,    -1,   128,    15,   131,   128,    16,   132,   128,
      -1,    36,   128,    -1,    35,   128,    -1,    30,   128,    -1,
      31,   128,    -1,    21,   127,    -1,    32,   128,    -1,   127,
      39,    -1,   127,    38,    -1,    39,   127,    -1,    38,   127,
      -1,    53,    82,   125,    83,    -1,   127,    -1,    73,    82,
     113,    83,    -1,    -1,    73,    82,   133,   127,    83,    -1,
      55,    -1,    82,   134,    83,    -1,   134,    30,   134,    -1,
     134,    31,   134,    -1,   134,    32,   134,    -1,   134,    33,
     134,    -1,   134,    19,   134,    -1,   134,    21,   134,    -1,
     134,    34,   134,    -1,   134,    20,   134,    -1,   134,    29,
     134,    -1,   134,    28,   134,    -1,   134,    25,   134,    -1,
     134,    24,   134,    -1,   134,    26,   134,    -1,   134,    27,
     134,    -1,   134,    23,   134,    -1,   134,    22,   134,    -1,
     134,    17,   134,    -1,   134,    18,   134,    -1,   134,    15,
     134,    16,   134,    -1,    36,   134,    -1,    35,   134,    -1,
      30,   134,    -1,    31,   134,    -1,    73,    82,   113,    83,
      -1,    -1,    73,    82,   135,   127,    83,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,    91,    91,    92,    96,    97,    98,    99,   100,   101,
     102,   103,   107,   111,   115,   116,   117,   121,   122,   123,
     124,   125,   126,   147,   148,   149,   153,   154,   156,   157,
     158,   159,   163,   164,   168,   169,   176,   183,   197,   215,
     214,   235,   243,   234,   262,   273,   279,   261,   290,   305,
     325,   289,   353,   356,   355,   371,   384,   388,   390,   389,
     404,   405,   441,   442,   446,   524,   523,   707,   708,   712,
     750,   762,   791,   814,   836,   863,   867,   899,   898,   978,
     979,   980,   981,   982,   983,   984,   985,   986,   987,   988,
     994,   995,   998,  1000,  1001,  1005,  1006,  1033,  1062,  1091,
    1120,  1149,  1178,  1207,  1236,  1265,  1294,  1326,  1348,  1454,
    1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1464,  1465,
    1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1475,  1474,
    1492,  1491,  1509,  1519,  1508,  1536,  1537,  1538,  1539,  1546,
    1547,  1560,  1604,  1648,  1692,  1736,  1737,  1769,  1813,  1812,
    1828,  1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,  1839,
    1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,  1849,
    1850,  1851,  1852,  1853,  1854,  1855,  1897,  1896
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
  "T_STRING", "T_EXTERN", "T_VAR", "T_GOTO", "T_SIZEOF", "T_REGISTER",
  "T_OPERATOR", "T_ASM", "T_PARAM_SEPARATOR", "T_NOP", "'{'", "'}'",
  "'\\n'", "'('", "')'", "']'", "';'", "$accept", "statement_list",
  "statement", "compound_statement", "assembler_statement",
  "assembler_command_list", "assembler_command",
  "assembler_argument_list", "operator_parameter", "expression_statement",
  "buildin_statement", "selection_statement", "@1", "@2", "@3", "@4",
  "@5", "@6", "@7", "@8", "@9", "else_statement", "@10",
  "labeled_statement", "jump_statement", "declaration_statement", "@11",
  "declaration_specifiers", "init_declarator_list", "init_declarator",
  "@12", "pointer", "declarator", "direct_declarator", "initializer",
  "initializer_list", "@13", "type_specifier", "expression",
  "argument_expression_list", "assignment_expression", "unary_expression",
  "expr", "@14", "@15", "@16", "@17", "@18", "const_expr", "@19", 0
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
     306,   307,   308,   309,   310,   311,   312,   313,   314,   123,
     125,    10,    40,    41,    93,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    86,    87,    87,    88,    88,    88,    88,    88,    88,
      88,    88,    89,    90,    91,    91,    91,    92,    92,    92,
      92,    92,    92,    93,    93,    93,    94,    94,    94,    94,
      94,    94,    95,    95,    96,    96,    96,    96,    96,    98,
      97,    99,   100,    97,   101,   102,   103,    97,   104,   105,
     106,    97,   107,   108,   107,   109,   110,   111,   112,   111,
     113,   113,   114,   114,   115,   116,   115,   117,   117,   118,
     118,   119,   119,   119,   120,   120,   121,   122,   121,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     124,   124,   125,   125,   125,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   127,   127,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   129,   128,
     130,   128,   131,   132,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   133,   128,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   135,   134
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     1,     2,     4,     2,
       1,     4,     0,     3,     1,     0,     1,     1,     1,     1,
       3,     3,     1,     2,     2,     2,     2,     3,     3,     0,
       7,     0,     0,     7,     0,     0,     0,    10,     0,     0,
       0,    12,     0,     0,     3,     2,     3,     2,     0,     4,
       1,     2,     1,     3,     1,     0,     4,     1,     2,     2,
       1,     1,     4,     3,     1,     3,     1,     0,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     4,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     0,     4,
       0,     4,     0,     0,     7,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     4,     1,     4,     0,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       5,     2,     2,     2,     2,     4,     0,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,    44,     0,     0,     0,   107,   109,   110,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,     0,
       0,    22,     0,     3,     0,    32,     0,     3,     4,    11,
       5,     6,     7,     8,     9,    10,    58,    60,     0,    90,
     146,    95,   107,   139,   107,   146,   137,   138,   140,   136,
     135,   144,   143,     0,     0,     0,     0,    35,     0,    36,
       0,    55,    92,     0,   148,     0,     0,    20,     0,    16,
      22,    34,     0,     0,     1,     2,    57,     0,    61,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   141,     0,   132,   128,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,    38,     0,
       0,    93,    56,     0,     0,    25,    17,     0,    29,    27,
      28,    26,    19,    13,    22,    15,    12,   111,    67,    71,
       0,    62,     0,    64,    70,    91,    96,   104,    98,    99,
     100,   101,    97,   103,   102,   105,   106,     0,     0,     0,
       0,   116,   119,   117,   127,   126,   123,   122,   124,   125,
     121,   120,   112,   113,   114,   115,   118,    39,     0,    45,
      48,   145,    92,   147,     0,     0,    24,     0,     0,     0,
      14,    68,     0,    59,    69,    65,     0,   108,     0,   129,
     131,     0,    42,     0,     0,    94,   149,    21,    25,    30,
      31,    18,    63,     0,     0,     0,     0,     0,   150,     0,
       0,    73,     0,   133,    52,     0,     0,     0,    23,     0,
      66,    74,   173,   174,   172,   171,   176,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,     0,    53,
      40,    43,     0,    49,    76,     0,     0,     0,   151,     0,
     168,   169,   156,   159,   157,   167,   166,   163,   162,   164,
     165,   161,   160,   152,   153,   154,   155,   158,   134,     0,
      46,     0,    77,    75,   175,     0,     0,    54,     0,     0,
       0,   177,   170,    47,    50,    78,     0,    51
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    36,    37,    38,    39,    78,    79,   195,   196,    40,
      41,    42,   211,    64,   235,    65,   213,   308,   214,   301,
     316,   270,   299,    43,    44,    45,    87,    46,   150,   151,
     223,   152,   153,   154,   274,   275,   310,    47,    48,   130,
      49,    50,    51,   169,   170,   168,   268,   134,   232,   277
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -68
static const short yypact[] =
{
     334,   -18,   402,   402,   402,   402,   402,   -18,   -18,   -55,
     -68,   -68,   -37,   -35,   -44,    -3,   -68,   -68,   -68,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,   -14,
     -41,   -32,   -60,   334,   402,   -68,    42,   334,   -68,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -36,    27,    -8,    44,
     177,   510,   -68,    37,     1,    15,   -68,   -68,   -68,   -68,
     -68,    37,    37,   402,    13,   334,    11,   -68,    19,   -68,
     402,   -68,   402,    38,    27,     0,    53,   119,    51,    43,
     -32,   -68,    70,   298,   -68,   -68,   -68,    -9,   -68,   -68,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   -68,   -68,   402,   -68,   -68,   -68,   402,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   402,   402,    69,   402,   103,   -68,   -68,    72,
      76,   150,   -68,    80,   -18,   119,   -68,   -29,   -68,   -68,
     -68,   -68,   161,   -68,   -32,   -68,   -68,   -68,   133,   -68,
       8,   -68,   117,   167,   130,   -68,   -68,   -68,   -68,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -68,    92,   402,   402,
     402,   627,   656,   224,   683,   683,   720,   720,   720,   720,
      96,    96,    30,    30,   -68,   -68,   -68,   -68,    94,   -68,
     -68,   -68,   402,   -68,   -28,   109,   176,   120,   122,   119,
     -68,   -68,    -9,   -68,   130,   -68,    21,   -68,   470,   563,
     596,   334,   -68,   121,   402,   -68,   -68,   -68,   119,   -68,
     -68,   -68,   -68,   390,   125,   125,   125,   125,   -68,   127,
     125,   -68,   278,   -68,   166,   334,   402,   129,   -68,   390,
     -68,   -68,   -68,   -68,   -68,   -68,    27,   318,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   -68,   402,   -68,
     -68,   -68,   134,   -68,   209,   138,   136,   -18,   -68,   490,
     580,   612,   642,   670,   696,   709,   709,   731,   731,   731,
     731,   230,   230,    68,    68,   -68,   -68,   -68,   528,   334,
     -68,   402,   -68,   -68,   -68,   -24,   125,   -68,   137,   160,
     390,   -68,   546,   -68,   -68,   -68,   334,   -68
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -68,   -11,   -57,   -68,   -68,   -63,   -68,     3,   -67,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,   -68,
     -68,   -68,   -68,   -19,   -68,   -68,   -68,   -38,   -68,    63,
     -68,   118,   -68,   116,    46,   -66,   -68,   -68,   -56,    78,
     -26,    -1,    26,   -68,   -68,   -68,   -68,   -68,   -25,   -68
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short yytable[] =
{
      53,    55,    55,    55,    55,    55,    61,    62,   126,    88,
     142,   202,    80,    71,   129,   104,    71,   145,    66,   104,
      68,    75,    82,   148,   197,    81,    85,    63,    56,    57,
      58,    59,    60,    55,   155,    52,   133,   124,    70,    73,
      76,    74,    84,    77,   149,   198,   131,    90,    67,    86,
      69,   224,   225,   102,   103,   216,   226,   227,   104,   311,
      83,    80,   121,   122,   123,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   228,    89,   167,    72,
     104,   200,   135,    72,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   203,   229,   125,   127,    27,    28,   188,
     264,   265,   266,   230,   128,   231,   136,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,   132,   144,    80,   119,   120,   121,   122,
     123,   143,   221,   194,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     146,   189,   187,   192,   234,   224,   225,   190,   237,   191,
     226,   227,   137,   193,   199,   148,   131,    55,    55,    55,
     149,   205,   138,   206,   139,   140,   207,   212,   271,   218,
     228,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   217,   141,   208,   209,   210,   241,   229,   242,
     243,   244,   245,   236,   219,   247,   220,   230,   276,   246,
     272,   269,   302,   241,   273,   102,   103,   300,   303,   304,
     104,   238,   313,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   307,   314,   315,   309,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   317,
     262,   263,   264,   265,   266,   222,   201,    55,   204,   240,
     215,     0,     0,     0,     0,     0,   305,     0,     0,     0,
       0,   312,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,   248,   298,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   105,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   248,     0,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,     0,     1,     0,     0,     0,     0,
       0,     0,   267,     0,     2,     3,     4,     0,     0,     5,
       6,     0,     7,     8,     0,     0,     0,     0,     9,     0,
       0,   147,    10,    11,    12,    13,    14,    15,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,   278,     0,     0,    27,    28,    29,    30,     0,     0,
      31,     1,    32,    33,     0,     0,    34,     0,     0,    35,
       2,     3,     4,     1,     0,     5,     6,     0,     7,     8,
       0,     0,     2,     3,     4,     0,     0,     5,     6,     0,
       7,     8,     0,    54,     0,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,    16,    17,     0,
       0,     0,     0,    30,     0,     0,     0,     0,     0,   239,
       0,     0,    34,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,    34,   105,   233,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   248,   306,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   105,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,    -1,    -1,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,    -1,    -1,    -1,    -1,   117,   118,
     119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,   260,
     261,   262,   263,   264,   265,   266
};

static const short yycheck[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,    65,    47,
      77,     3,    31,    16,    70,    43,    16,    80,    55,    43,
      55,    53,    33,    32,    53,    85,    37,    82,     2,     3,
       4,     5,     6,    34,    90,    53,    74,    63,    82,    53,
      72,    82,     0,    75,    53,    74,    72,     3,    85,    85,
      85,    30,    31,    38,    39,    83,    35,    36,    43,    83,
      34,    80,    32,    33,    34,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    55,    85,   104,    82,
      43,   144,    82,    82,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    85,    73,    82,    85,    70,    71,   125,
      32,    33,    34,    82,    85,    84,    53,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    85,    81,   144,    30,    31,    32,    33,
      34,    80,   199,   134,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      80,    48,    83,     3,   211,    30,    31,    85,   214,    83,
      35,    36,    43,    83,     3,    32,   192,   168,   169,   170,
      53,     4,    53,    43,    55,    56,    84,    83,   235,     3,
      55,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    83,    74,   168,   169,   170,   223,    73,   224,
     225,   226,   227,    82,    84,   230,    84,    82,   246,    82,
     236,    45,     3,   239,    85,    38,    39,    83,    80,    83,
      43,   218,    85,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   299,    83,   310,   301,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,   316,
      30,    31,    32,    33,    34,   202,   148,   268,   152,   223,
     192,    -1,    -1,    -1,    -1,    -1,   277,    -1,    -1,    -1,
      -1,   306,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,   268,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    15,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    30,    31,    32,    -1,    -1,    35,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    83,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    83,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    21,    78,    79,    -1,    -1,    82,    -1,    -1,    85,
      30,    31,    32,    21,    -1,    35,    36,    -1,    38,    39,
      -1,    -1,    30,    31,    32,    -1,    -1,    35,    36,    -1,
      38,    39,    -1,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    82,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    21,    30,    31,    32,    35,    36,    38,    39,    44,
      48,    49,    50,    51,    52,    53,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    70,    71,    72,
      73,    76,    78,    79,    82,    85,    87,    88,    89,    90,
      95,    96,    97,   109,   110,   111,   113,   123,   124,   126,
     127,   128,    53,   127,    53,   127,   128,   128,   128,   128,
     128,   127,   127,    82,    99,   101,    55,    85,    55,    85,
      82,    16,    82,    53,    82,    53,    72,    75,    91,    92,
     109,    85,    87,   128,     0,    87,    85,   112,   113,    85,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    38,    39,    43,    15,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   126,    82,    88,    85,    85,   124,
     125,   126,    85,   113,   133,    82,    53,    43,    53,    55,
      56,    74,    94,    80,    81,    91,    80,    83,    32,    53,
     114,   115,   117,   118,   119,   124,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   131,   129,
     130,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,    83,   126,    48,
      85,    83,     3,    83,   127,    93,    94,    53,    74,     3,
      91,   117,     3,    85,   119,     4,    43,    84,   128,   128,
     128,    98,    83,   102,   104,   125,    83,    83,     3,    84,
      84,    94,   115,   116,    30,    31,    35,    36,    55,    73,
      82,    84,   134,    16,    88,   100,    82,   124,    93,    79,
     120,   126,   134,   134,   134,   134,    82,   134,    15,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    84,   132,    45,
     107,    88,   126,    85,   120,   121,   113,   135,    83,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   128,   108,
      83,   105,     3,    80,    83,   127,    16,    88,   103,   124,
     122,    83,   134,    85,    83,   121,   106,    88
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
        case 17:

    { cl_push(pp, (unsigned char) yyvsp[-1].value); cl_jump_define(&pp->jumps_table, yyvsp[0].string, pp->hc_fill); free_str(yyvsp[0].string); cl_push_dw(pp, 0) ;}
    break;

  case 18:

    { cl_push_op(pp, (unsigned char) yyvsp[-3].value, &yyvsp[-2], &yyvsp[0]); ;}
    break;

  case 19:

    { cl_push_op(pp, (unsigned char) yyvsp[-1].value, &yyvsp[0], NULL) ;}
    break;

  case 20:

    { cl_push_op(pp, (unsigned char) yyvsp[0].value, NULL, NULL) ;}
    break;

  case 21:

    { cl_push_op(pp, OP_CALL, &yyvsp[-3], NULL); free_str(yyvsp[-3].string); ;}
    break;

  case 23:

    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value); ;}
    break;

  case 24:

    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value); ;}
    break;

  case 26:

    { yyval = yyvsp[0]; yyval.flags = ARG_REG ;}
    break;

  case 27:

    { yyval = yyvsp[0]; yyval.flags = ARG_IMM ;}
    break;

  case 28:

    { yyval = yyvsp[0]; yyval.flags = ARG_DATA; ;}
    break;

  case 29:

    { yyval = yyvsp[0]; yyval.flags = ARG_MEM; yyval.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, yyvsp[0].string); free_str(yyvsp[0].string); if(yyval.uvalue == 1) ZL_ERROR("undeclared identifier");;}
    break;

  case 30:

    { yyval = yyvsp[-2]; yyval.flags = ARG_PMEM; yyval.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, yyvsp[-1].string); free_str(yyvsp[-1].string); if(yyval.uvalue == 1) ZL_ERROR("undeclared identifier"); ;}
    break;

  case 31:

    { yyval = yyvsp[-2]; yyval.flags = ARG_PREG ;}
    break;

  case 33:

    { cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); ;}
    break;

  case 34:

    { cl_push(pp, OP_NOP); ;}
    break;

  case 35:

    {
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack) ZL_ERROR("break not in loop");
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) pp->cl_loop_stack->next_node->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 36:

    {
			cl_push(pp, OP_JMP);
			if(!pp->cl_loop_stack || !pp->cl_loop_stack->next_node) ZL_ERROR("continue not in loop");
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) pp->cl_loop_stack->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 37:

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
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) lsb->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 38:

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
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) lsb->data)->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 39:

    {
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, 0);
			cl_stack_push(&pp->cl_stack, lb_skip);

			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			//cl_push(pp, OP_TEST_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 40:

    {
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
			//cl_label_define(&pp->labels_table, ((zl_names_map *)cl_stack_pop(&pp->cl_stack))->name, pp->hc_fill);
		;}
    break;

  case 41:

    {
			zl_names_map *lb_exit, *lb_condition;
			lb_exit = cl_label_new(&pp->labels_table, 0);
			lb_condition = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
		;}
    break;

  case 42:

    {
			zl_names_map *lb_exit;
			lb_exit= (zl_names_map *) pp->cl_loop_stack->next_node->data;
			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_exit->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 43:

    {
			zl_names_map *lb_exit, *lb_condition;
			lb_condition = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_condition->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
		;}
    break;

  case 44:

    {
			zl_names_map *lb_exit, *lb_condition, *lb_start;
			lb_exit = cl_label_new(&pp->labels_table, 0);
			lb_condition = cl_label_new(&pp->labels_table, 0);
			lb_start = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_loop_stack, lb_exit);			// exit loop
			cl_stack_push(&pp->cl_loop_stack, lb_condition);	// condition
			cl_stack_push(&pp->cl_stack, lb_start);				// start loop
		;}
    break;

  case 45:

    {
			zl_names_map *lb_condition;
			lb_condition = (zl_names_map *) pp->cl_loop_stack->data;
			lb_condition->offset = pp->hc_fill;
		;}
    break;

  case 46:

    {
			zl_names_map *lb_exit, *lb_condition;
			lb_condition = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JNZ_POP_STK);
			cl_jump_define(&pp->jumps_table, ((zl_names_map *) cl_stack_pop(&pp->cl_stack))->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_exit->offset = pp->hc_fill;
		;}
    break;

  case 48:

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
		;}
    break;

  case 49:

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
		;}
    break;

  case 50:

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
		;}
    break;

  case 51:

    {
			zl_names_map *lb_exit, *lb_loop;
			lb_loop = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);
			lb_exit = (zl_names_map *) cl_stack_pop(&pp->cl_loop_stack);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_loop->name, pp->hc_fill);
			cl_push_dw(pp, 0);

			lb_exit->offset = pp->hc_fill;
		;}
    break;

  case 53:

    {
			zl_names_map *lb_skip, *lb_else;
			lb_else = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_new(&pp->labels_table, 0);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			cl_stack_push(&pp->cl_stack, lb_skip);
		;}
    break;

  case 55:

    {
			zl_names_map *label;
			label = cl_label_define(&pp->labels_table, yyvsp[-1].string, pp->hc_fill);
			free_str(yyvsp[-1].string);
			if(!label)
			{
				ZL_ERROR("label redefined");
			}
		;}
    break;

  case 56:

    { cl_push(pp, (unsigned char) yyvsp[-2].value); cl_jump_define(&pp->jumps_table, yyvsp[-1].string, pp->hc_fill); free_str(yyvsp[-1].string); cl_push_dw(pp, 0) ;}
    break;

  case 57:

    { /* nothing to do */ ;}
    break;

  case 58:

    {
			if(!(yyvsp[0].flags & ZLF_TYPE))
			{
				yyvsp[0].flags |= ZLF_INT;
			}
			cl_stack_push(&pp->cl_stack, (void *) yyvsp[0].flags);
		;}
    break;

  case 59:

    {
			cl_stack_pop(&pp->cl_stack);
		;}
    break;

  case 60:

    { yyval.flags = yyvsp[0].flags ;}
    break;

  case 61:

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

  case 62:

    { /* nothing to do */ ;}
    break;

  case 63:

    { /* nothing to do */ ;}
    break;

  case 64:

    {
			if((yyvsp[0].flags & ZLF_AUTOARRAY))
			{
				ZL_ERROR("unknown size");
			}

			if(yyvsp[0].flags & ZLF_ARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long var_size;
				var_size = 4;

				if(!(yyvsp[0].flags & ZLF_POINTER))
				{
					switch(yyvsp[0].flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if(yyvsp[0].flags & ZLF_SHORT)
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
				el = yyvsp[0].var_info->elements;
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
						el = yyvsp[0].var_info->elements;
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
		;}
    break;

  case 65:

    {
			cl_stack_push(&pp->cl_stack, (void *) yyvsp[-1].var_info);
			cl_push(pp, OP_PUSH_PMEM); cl_push_dw(pp, yyvsp[-1].value*4);
		;}
    break;

  case 66:

    {
			yyvsp[-3].flags = ((zl_names_map *) pp->cl_stack->data)->flags;
			if(yyvsp[-3].flags & ZLF_AUTOARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long level_size;
				unsigned long var_size;
				var_size = 4;

				if(!(yyvsp[-3].flags & ZLF_POINTER))
				{
					switch(yyvsp[-3].flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if(yyvsp[-3].flags & ZLF_SHORT)
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
				el = yyvsp[-3].var_info->elements;
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
						el = yyvsp[-3].var_info->elements;
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
					level_size = yyvsp[-3].var_info->elements->size;
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
				yyvsp[-3].var_info->size = (level_size)?(level_size * (yyvsp[-3].var_info->elements->size / level_size) + ((yyvsp[-3].var_info->elements->size % level_size)?(level_size):(0))):(yyvsp[-3].var_info->elements->size);
				yyvsp[-3].var_info->elements->size = yyvsp[-3].var_info->size;
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
			else if(yyvsp[-3].flags & ZLF_ARRAY)
			{
				zl_names_map *el;
				unsigned long back_loop, i;
				unsigned long level_size;
				unsigned long var_size;
				var_size = 4;

				if(!(yyvsp[-3].flags & ZLF_POINTER))
				{
					switch(yyvsp[-3].flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;
							break;
						case ZLF_INT:
							if(yyvsp[-3].flags & ZLF_SHORT)
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
				el = yyvsp[-3].var_info->elements;
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
						el = yyvsp[-3].var_info->elements;
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
					level_size = yyvsp[-3].var_info->elements->size;
				}

				//$$.var_info->elements->size
			}

			cl_stack_pop(&pp->cl_stack);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		;}
    break;

  case 67:

    { yyval.flags = 0x01000000 ;}
    break;

  case 68:

    { yyval.flags = 0x01000000 + yyvsp[0].flags ;}
    break;

  case 69:

    {
			yyval = yyvsp[0];
			yyval.flags += yyvsp[-1].flags;
			yyval.var_info->flags = yyval.flags;
			
			if(yyval.flags & ZLF_ARRAY)
			{
				unsigned long var_size;
				var_size = 4;

				switch(yyval.flags & ZLF_TYPE)
				{
					case ZLF_CHAR:
						var_size = 1;					
						break;
					case ZLF_INT:
						if(yyval.flags & ZLF_SHORT)
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

				yyval.var_info->size = (yyval.var_info->size/var_size)*4;
			}
			else
			{
				yyval.var_info->size = 4;
			}
		;}
    break;

  case 70:

    {
			yyval = yyvsp[0];

			if(yyval.flags & ZLF_VOID)
			{
				ZL_ERROR("illegal use of type 'void'");
			}
		;}
    break;

  case 71:

    {
			yyval.flags = (unsigned long) pp->cl_stack->data;
			yyval.value = cl_var_define(&pp->vars_table, yyvsp[0].string, yyval.flags, &yyval.var_info);
			free_str(yyvsp[0].string);
			if(!yyval.value) ZL_ERROR("redefinition");
			yyval.value--;

			unsigned long var_size;
			var_size = 4;

			switch(yyval.flags & ZLF_TYPE)
			{
				case ZLF_CHAR:
					var_size = 1;					
					break;
				case ZLF_INT:
					if(yyval.flags & ZLF_SHORT)
					{
						var_size = 2;
					}
					break;
				case ZLF_DOUBLE:
					var_size = 8;
					break;
			}

			yyval.var_info->size = var_size;
		;}
    break;

  case 72:

    {
			yyval = yyvsp[-3];

			if(yyvsp[-3].flags & ZLF_ARRAY)
			{
				//ZL_ERROR("sorry, multi-level arrays not yet supported!");
			}
			
			if(yyvsp[-1].uvalue == 0)
			{
				ZL_ERROR("cannot allocate an array of constant size 0.");
			}

			yyval.flags |= ZLF_ARRAY;
			yyval.var_info->flags = yyval.flags;
			yyval.var_info->size *= yyvsp[-1].uvalue;
			
			zl_names_map *level;
			cl_var_define(&yyval.var_info->elements, NULL, 0, &level);

			level->size = yyvsp[-1].uvalue;
		;}
    break;

  case 73:

    {
			// last array level with auto size
			yyval = yyvsp[-2];

			if(yyvsp[-2].flags & ZLF_ARRAY)
			{
				ZL_ERROR("unknown size, missing subscript!");
			}

			yyval.flags |= ZLF_ARRAY | ZLF_AUTOARRAY;
			yyval.var_info->flags = yyval.flags;

			cl_var_define(&yyval.var_info->elements, NULL, 0, NULL);

			//$$.var_info->size = 0;
			yyval.var_info->elements->size = 0;
		;}
    break;

  case 74:

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
		;}
    break;

  case 76:

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
		;}
    break;

  case 77:

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

		;}
    break;

  case 79:

    { yyval.flags = ZLF_VOID ;}
    break;

  case 80:

    { yyval.flags = ZLF_CHAR ;}
    break;

  case 81:

    { yyval.flags = ZLF_SHORT ;}
    break;

  case 82:

    { yyval.flags = ZLF_INT ;}
    break;

  case 83:

    { yyval.flags = ZLF_LONG ;}
    break;

  case 84:

    { yyval.flags = ZLF_FLOAT ;}
    break;

  case 85:

    { yyval.flags = ZLF_DOUBLE ;}
    break;

  case 86:

    { yyval.flags = ZLF_SIGNED ;}
    break;

  case 87:

    { yyval.flags = ZLF_UNSIGNED ;}
    break;

  case 88:

    { yyval.flags = ZLF_EXTERNAL ;}
    break;

  case 89:

    { yyval.flags = ZLF_INTERNAL ;}
    break;

  case 91:

    { cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX); ;}
    break;

  case 96:

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

  case 97:

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

  case 98:

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

  case 99:

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

  case 100:

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

  case 101:

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

  case 102:

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

  case 103:

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

  case 104:

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

  case 105:

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

  case 106:

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

  case 107:

    {
			// push address of value to stack
			yyval = yyvsp[0];

			yyval.var_info = cl_var_find(pp->vars_table, yyvsp[0].string, &yyval.uvalue);
			free_str(yyvsp[0].string);
			if(!yyval.var_info)
			{
				ZL_ERROR("undeclared identifier");
			}

			yyval.flags = yyval.var_info->flags;
			yyval.size = yyval.var_info->size;
			
			if(yyval.flags & ZLF_ARRAY)
			{
				yyval.var_info = yyval.var_info->elements;
			}

			cl_push(pp, OP_PUSH_PMEM); cl_push_dw(pp, yyval.uvalue);
		;}
    break;

  case 108:

    {
			// pop address
			// add to address offset
			// push address this address to stack

			// ul *v = {0,1,2};
			// v[1]; //= 1

			if(yyval.flags & ZLF_ARRAY)
			{
				unsigned long ptr_step;
				ptr_step = 4;
				
				yyval.var_info = yyval.var_info->next_node;

				if(yyval.var_info)
				{
					ptr_step = yyval.var_info->size;
				}
				else
				{
					yyval.flags &= ~ZLF_ARRAY;

					if(!(yyval.flags & ZLF_POINTER))
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

				}

				yyval.size = ptr_step;

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				if(ptr_step > 1)
				{
					cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, ptr_step);
					cl_push(pp, OP_MUL_REG_REG); cl_push(pp, REG_ECX); cl_push(pp, REG_EBX);
				}
				cl_push(pp, OP_ADD_REG_REG); cl_push(pp, REG_EAX); cl_push(pp, REG_ECX);
				cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX);

				yyval.uvalue++;
			}
			else if(yyval.flags & ZLF_POINTER)
			{
				yyval.flags -= 0x01000000;

				unsigned long var_size;
				var_size = 4;
				if(!(yyval.flags & ZLF_POINTER))
				{
					switch(yyval.flags & ZLF_TYPE)
					{
						case ZLF_CHAR:
							var_size = 1;					
							break;
						case ZLF_INT:
							if(yyval.flags & ZLF_SHORT)
							{
								var_size = 2;
							}
							break;
						case ZLF_DOUBLE:
							var_size = 8;
							break;
					}
				}

				yyval.size = var_size;

				cl_push(pp, OP_POP_REG); cl_push(pp, REG_ECX);
				cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_PREG); cl_push(pp, REG_EAX); cl_push(pp, REG_EAX);
				cl_push(pp, OP_MOV_REG_IMM); cl_push(pp, REG_EBX); cl_push_dw(pp, var_size);
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

  case 109:

    { cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[0].value) ;}
    break;

  case 110:

    { cl_push(pp, OP_PUSH_OFFSET); cl_push_dw(pp, yyvsp[0].value) ;}
    break;

  case 111:

    { yyval = yyvsp[-1]; ;}
    break;

  case 112:

    { cl_do_op(pp, OP_ADD_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 113:

    { cl_do_op(pp, OP_SUB_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 114:

    { cl_do_op(pp, OP_MUL_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 115:

    { cl_do_op(pp, OP_DIV_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 116:

    { cl_do_op(pp, OP_OR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 117:

    { cl_do_op(pp, OP_AND_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 118:

    { cl_do_op(pp, OP_MOD_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 119:

    { cl_do_op(pp, OP_XOR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 120:

    { cl_do_op(pp, OP_SHL_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 121:

    { cl_do_op(pp, OP_SHR_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 122:

    { cl_do_op(pp, OP_G_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 123:

    { cl_do_op(pp, OP_L_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 124:

    { cl_do_op(pp, OP_GE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 125:

    { cl_do_op(pp, OP_LE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 126:

    { cl_do_op(pp, OP_E_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 127:

    { cl_do_op(pp, OP_NE_STK_STK, &yyval, &yyvsp[-2], &yyvsp[0]) ;}
    break;

  case 128:

    {
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JNZ_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		;}
    break;

  case 129:

    {
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
		;}
    break;

  case 130:

    {
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, pp->hc_fill);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
		;}
    break;

  case 131:

    {
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
		;}
    break;

  case 132:

    {
			zl_names_map *lb_skip;
			lb_skip = cl_label_new(&pp->labels_table, 0);
			cl_stack_push(&pp->cl_stack, lb_skip);

			cl_push(pp, OP_JZ_POP_STK);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
		;}
    break;

  case 133:

    {
			zl_names_map *lb_skip, *lb_else;
			lb_else = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip = cl_label_new(&pp->labels_table, 0);

			cl_push(pp, OP_JMP);
			cl_jump_define(&pp->jumps_table, lb_skip->name, pp->hc_fill);
			cl_push_dw(pp, 0);
			lb_else->offset = pp->hc_fill;
			cl_stack_push(&pp->cl_stack, lb_skip);
		;}
    break;

  case 134:

    {
			zl_names_map *lb_skip;
			lb_skip = (zl_names_map *) cl_stack_pop(&pp->cl_stack);
			lb_skip->offset = pp->hc_fill;
		;}
    break;

  case 135:

    { yyval = yyvsp[0]; cl_push(pp, OP_BNOT_STK); ;}
    break;

  case 136:

    { yyval = yyvsp[0]; cl_push(pp, OP_LNOT_STK); ;}
    break;

  case 137:

    { yyval = yyvsp[0]; ;}
    break;

  case 138:

    {
			yyval = yyvsp[0];
			yyval.flags &= ~ZLF_UNSIGNED;
			yyval.flags |= ZLF_SIGNED;
			cl_push(pp, OP_MINUS_STK);
		;}
    break;

  case 139:

    { /* nothing to do */ ;}
    break;

  case 140:

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

  case 141:

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

  case 142:

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

  case 143:

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

  case 144:

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

  case 145:

    { cl_push(pp, OP_RCALL); yyval.uvalue = cl_find_var_or_func(pp->vars_table, pp->funcs_list, yyvsp[-3].string); free_str(yyvsp[-3].string); if(yyval.uvalue == 1) ZL_ERROR("undeclared identifier"); cl_push_dw(pp, yyval.uvalue); cl_push(pp, OP_PUSH_REG); cl_push(pp, REG_EAX); ;}
    break;

  case 146:

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

  case 147:

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

  case 148:

    {
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		;}
    break;

  case 149:

    {
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
			yyval.flags = ZLF_UNSIGNED | ZLF_LONG | ZLF_INT;
			//cl_push(pp, OP_POP_REG); cl_push(pp, REG_EAX);
			cl_push(pp, OP_PUSH_IMM); cl_push_dw(pp, yyvsp[-1].size);
		;}
    break;

  case 150:

    { /* nop */ ;}
    break;

  case 151:

    { /* nop */ ;}
    break;

  case 152:

    { yyval.value = yyvsp[-2].value + yyvsp[0].value; ;}
    break;

  case 153:

    { yyval.value = yyvsp[-2].value - yyvsp[0].value; ;}
    break;

  case 154:

    { yyval.value = yyvsp[-2].value * yyvsp[0].value; ;}
    break;

  case 155:

    { yyval.value = yyvsp[-2].value / yyvsp[0].value; ;}
    break;

  case 156:

    { yyval.value = yyvsp[-2].value | yyvsp[0].value; ;}
    break;

  case 157:

    { yyval.value = yyvsp[-2].value & yyvsp[0].value; ;}
    break;

  case 158:

    { yyval.value = yyvsp[-2].value % yyvsp[0].value; ;}
    break;

  case 159:

    { yyval.value = yyvsp[-2].value ^ yyvsp[0].value; ;}
    break;

  case 160:

    { yyval.value = yyvsp[-2].value << yyvsp[0].value; ;}
    break;

  case 161:

    { yyval.value = yyvsp[-2].value >> yyvsp[0].value; ;}
    break;

  case 162:

    { yyval.value = yyvsp[-2].value < yyvsp[0].value; ;}
    break;

  case 163:

    { yyval.value = yyvsp[-2].value > yyvsp[0].value; ;}
    break;

  case 164:

    { yyval.value = yyvsp[-2].value >= yyvsp[0].value; ;}
    break;

  case 165:

    { yyval.value = yyvsp[-2].value <= yyvsp[0].value; ;}
    break;

  case 166:

    { yyval.value = yyvsp[-2].value == yyvsp[0].value; ;}
    break;

  case 167:

    { yyval.value = yyvsp[-2].value != yyvsp[0].value; ;}
    break;

  case 168:

    { yyval.value = yyvsp[-2].value || yyvsp[0].value; ;}
    break;

  case 169:

    { yyval.value = yyvsp[-2].value && yyvsp[0].value; ;}
    break;

  case 170:

    { yyval.value = yyvsp[-4].value ? yyvsp[-2].value : yyvsp[0].value; ;}
    break;

  case 171:

    { yyval.value = ~yyvsp[0].value; ;}
    break;

  case 172:

    { yyval.value = !yyvsp[0].value; ;}
    break;

  case 173:

    { /* nothing to do */ ;}
    break;

  case 174:

    { yyval.value = -yyvsp[0].value; ;}
    break;

  case 175:

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

  case 176:

    {
			cl_stack_push(&pp->cl_stack, (void *) pp->hc_fill);
		;}
    break;

  case 177:

    {
			yyval.value = yyvsp[-1].size;
			pp->hc_fill = (unsigned long) cl_stack_pop(&pp->cl_stack);
		;}
    break;


    }

/* Line 991 of yacc.c.  */


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

