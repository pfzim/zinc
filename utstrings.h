#ifndef _UTSTRINGS_H_
#define _UTSTRINGS_H_

#include "zalloc.h"
#include "zlist.h"

#define FPI_DIR				0x01
#define FPI_FNAME			0x02
#define FPI_FEXT			0x04

#define LVF_ALLOC			0x00000001
#define LVF_STRING			0x00000002

#define RV_KEYSYM			'%'
#define RV_KEYSTR			"%"
#define emptystring			nullstring
//#define emptystring			""
//#define nullstring			NULL	//emptystring


// sql generator
// query types
#define SG_INSERT			0x00000001
#define SG_UPDATE			0x00000002
#define SG_SELECT			0x00000004
#define SG_DELETE			0x00000008
// query flags
#define SG_ORDER			0x00000010		// ORDER BY
#define SG_DESC				0x00000020		// ORDER BY col DESC
#define SG_KEY				0x00000040		// always select
#define SG_GROUP			0x00000080		// GROUP BY
#define SG_VISIBLE			0x00000100		// deprecated by SG_SELECT
#define SG_RESERVED1		0x00000200		// reserved
#define SG_RESERVED2		0x00000400
#define SG_RESERVED3		0x00000800

// value types
#define SG_TYPE_MASK		0x0001F000
#define SG_RAWVAL			0x00001000
#define SG_STRING			0x00002000
#define SG_NUMBER			0x00004000
#define SG_NULL				0x00008000
#define SG_DEFAULT			0x00010000

#define SG_RESERVED4		0x00010000
#define SG_RESERVED5		0x00020000
#define SG_RESERVED6		0x00040000
#define SG_RESERVED7		0x00080000
#define SG_RESERVED8		0x00100000

// modifiers
#define SG_AND				0x00020000
#define SG_LIKE				0x00040000
#define SG_OR				0x00080000
#define SG_BEGINS			0x00100000
#define SG_ENDS				0x00200000
#define SG_IS				0x00400000
#define SG_GROUP_OPEN		0x00800000
#define SG_GROUP_CLOSE		0x01000000
#define SG_PASSWORD			0x02000000
#define SG_LSS				0x04000000
#define SG_GTR				0x08000000
#define SG_LEQ				0x10000000
#define SG_GEQ				0x20000000
#define SG_EQU				0x40000000
#define SG_NEQ				0x80000000
#define SG_CONTAINE			(SG_BEGINS | SG_ENDS)

typedef struct _KEYWORD_NODE
{
	char *szKey;
	char *szValue;
} KEYWORD_NODE, *LPKEYWORD_NODE;

typedef struct _QUERY_STRING_NODE
{
	char *key;
	char *value;
	struct _QUERY_STRING_NODE *next_node;
} QUERY_STRING_NODE, *LP_QUERY_STRING_NODE;

typedef struct _DB_SQL_QUERY_NODE
{
	char *key;
	char *value;
	unsigned long flags;
} DB_SQL_QUERY_NODE;

typedef struct _LIST_VARIABLE_NODE
{
	struct _LIST_VARIABLE_NODE *next_node;
	unsigned long flags;
	char *key;
	union
	{
		unsigned long numb;
		void *ptr;
		char *str;
	} value;
	unsigned long size;
} LIST_VARIABLE_NODE, *LP_LIST_VARIABLE_NODE;

char *alloc_filepath(const char *lpszFullPath, unsigned long dwOption);
// путь + имя файла без расширения
char *strippath(char *path, unsigned long flags);

unsigned long _maxprm(const char *lpszCommand, const char *lpszSeparators);
// сколько параметров в строке разделёных сепараторами
// не считая сепараторы в скобках и кавычках
//extern inline char *__instrstr(const char *lpszChars, char chChar);
// ищит в строке символ
// find szChar char in lpszChars string
// return position of szChar in lpszChars
unsigned long _cprs(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators);
// возвращает конечную позицию параметра номер... [no-zero-based]
char *_pa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators);
unsigned long _ulpa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators);
long _ipa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators);
// берёт параметр номер dwParamNum [zero-based-parameter]
// учитывает то что нужно пропускать сепараторы в скобках и кавычках
char *alloc_getpa(const char *lpszCommand, unsigned long dwParamNum, const char *lpszSeparators);
//тоже что и _pa() только выделяет память вместо static
unsigned long _skipstring(const char *lpszString);
// возвращает конец стороки "text \n который нужно пропустить"
// возвращает значение равное позиции заканчивающейся кавычки!
// позиция первого символа равна 0
unsigned long _skipblock(const char *lpszString);
// пропустить блок из скобок (стороки "тоже пропускаем" ?)
char *alloc_getblock(const char *lpszString, unsigned long *lpdwPos);
//берём блок заключенный в скобки [скобки исключаем]
//char *alloc_cutspaces(char *lpszString);
//
//char *alloc_killspaces(char *lpszString);
//удаляет пробелы и табы в начале и конце StrTrim()
char *ltrim(char *in);
char *rtrim(char *in);
char *trim(char *in);
char *ltrim_any(char *in, char *any);
char *rtrim_any(char *in, char *any);
char *trim_any(char *in, char *any);

char *killspaces(char *in);
char *killdoubleslashes(char *in);

char *strncpy_tiny(char *out, const char *in, size_t len);
//аналогично strncpy только ещё добаваляет null-terminator
char *alloc_string(const char *in);
char *alloc_strcat(char **out, const char *in);
char *alloc_strncpy(const char *lpin, size_t len);
char *alloc_strncat(char **lpout, const char *lpin, size_t len);

char *alloc_dbescape(char **string);
char *alloc_addslashes(char **string);

int isfloat(char *in, int fix);
int isinteger(char *in);

char *alloc_getstring(const char *buffer, unsigned long len, /*unsigned long *line, */unsigned long *pos);
// извлекает строку из буфера от pos до '\n'

char *alloc_str_delete(char **lpstr, unsigned long pos, unsigned long len);
char *alloc_str_replace(char **lpstr, unsigned long pos, unsigned long len, char *value);
char *alloc_str_replace(char **str, char *key, char *val);
char *replace_variable(char **lpstr, LPKEYWORD_NODE lpkey);

unsigned long url_string_parse(LP_QUERY_STRING_NODE *main_node, char *text);
unsigned long query_string_parse(LP_QUERY_STRING_NODE *main_node, char *text);
void query_string_free(LP_QUERY_STRING_NODE *main_node);
LP_QUERY_STRING_NODE query_string_put(LP_QUERY_STRING_NODE *main_node, const char *key, char *value, unsigned long size);
char *query_string_get(LP_QUERY_STRING_NODE main_node, const char* key);

char *alloc_unescapestring(const char *lpszInput, unsigned long *lpdwLen);
char *alloc_escapestring(const char *lpszInput, unsigned long dwLen);

char hex2char(char *hex);
char *alloc_urlencode(char *text);
char *urldecode(char *url);
char *strdel_word(char *in, char *word);
char *strdel(char *in, unsigned long count);
char *strmov_bytes(char *in, unsigned long count);
void *memdel(void *in, unsigned long size, unsigned long offset, unsigned long count);
void *memmov(void *in, unsigned long size, unsigned long offset, unsigned long count);
char *swapchar(char *in, char och, char nch);
int extcmp(char *str, char *ext);

LIST_VARIABLE_NODE **variables_put_string(LIST_VARIABLE_NODE **lplist, char *name, const char *strings, ...);
LIST_VARIABLE_NODE *variables_get_by_name(LIST_VARIABLE_NODE *lplist, char *name);
LIST_VARIABLE_NODE **variables_node_to_bindpoint(LIST_VARIABLE_NODE **lplist, LIST_VARIABLE_NODE *node);
void variables_free(LIST_VARIABLE_NODE **list);

char *alloc_getstringbefore(const char *buffer, unsigned long len, unsigned long *pos, const char *word);
char alloc_getstringbefore(char **out, const char *buffer, unsigned long len, unsigned long *pos, char *delimeters/*, unsigned long delimeters_count*/);

char *alloc_string_ex(char *strings, ...);

char *alloc_text2binary(char *data, unsigned long data_size, char zero, char one);
char *alloc_binary2text(char *data, unsigned long *data_size, char zero);

LP_QUERY_STRING_NODE query_node_get(LP_QUERY_STRING_NODE main_node, const char *key);
char *query_build(LP_QUERY_STRING_NODE main_node);

char *sql_query_build(unsigned long qtype, db_list_node *db_list, char *table_name, char *additional, char *condition, ...);
db_list_node *sql_query_add(db_list_node **db_list, char *key, char *value, unsigned long vtype);
void cb_sql_free(void *data);

char *format_number(char *str);
char *crypt_xor(char *buf, unsigned long len, char *hash, unsigned long hash_len);

// input format: yyyy-mm-dd
unsigned long strtodate(char *date_str);
// input format: dd.mm.yyyy
unsigned long strtodate2(char *date_str);
// input format: hh:mm:ss.mss
unsigned long strtotime(char *date_str);
// input format: xx.xx.xx hh:mm:ss.mss
unsigned long strtotime2(char *date_str);

__int64 roundcurrency(__int64 num);
__int64 strtocurrency(char *str);
char *currencytostr(__int64 num, char *str);
char *currencytostr2(__int64 num, char *str);	// without spaces between thousands

char *alloc_dig2str(__int64 p_summa, int p_sex, char *p_one, char *p_four, char *p_many);
char *alloc_dig2str(double p_summa, int p_sex, char *p_one, char *p_four, char *p_many, char *k_one, char *k_four, char *k_many);
char *alloc_currency2str(__int64 p_summa0, int p_sex, char *p_one, char *p_four, char *p_many, char *k_one, char *k_four, char *k_many);


#define zdate(dd, mm, yy)			(((dd) & 0x1F) | (((mm) & 0x0F) << 5) | (((yy) & 0x7FFFFF) << 9))
#define zday(x)						((x) & 0x0000001F)
#define zmonth(x)					(((x) & 0x000001E0) >> 5)
#define zyear(x)					((x) >> 9)

#define ztime(hh, mm, ss, ms)		((ms & 0x3FF) | ((ss & 0x3F) << 10) | ((mm & 0x3F) << 16) | ((hh & 0x1F) << 22))
#define zmicrosecond(x)				((x) & 0x000003FF)
#define zsecond(x)					(((x) & 0x0000FC00) >> 10)
#define zminute(x)					(((x) & 0x003F0000) >> 16)
#define zhour(x)					((x) >> 22)

#define __instrstr(lpstr, ch)		strchr(lpstr, ch)
#define isempty(lpstr)				((!(lpstr)) || (*(lpstr) == '\0'))
#define isalloc(lpstr)				((lpstr) && ((char *) (lpstr) != nullstring))

// инвертируем скобки из открытых в закрытые
inline char charInvert(char chChar)
{
	switch(chChar)
	{
		case '(': return ')';
		case '[': return ']';
		case '{': return '}';
	}
	return '\0';
}


//хорошая идея из zend_language_engine
//
//здесь:

extern char *nullstring;

inline void free_str(char *&lpstr)
{
	if(isalloc(lpstr))
	{
		zfree(lpstr);
	}
	lpstr = nullstring;
}

inline void free_str(char **str)
{
	if(isalloc(*str))
	{
		zfree(*str);
	}
	*str = nullstring;
}

//
//в .cpp:
//char *nullstring = "";


#endif //_UTSTRINGS_H_
