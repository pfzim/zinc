#ifndef _ZINI_H_
#define _ZINI_H_

//#include "zibuffer.h"

#ifndef NEW_INI_STYLE
#define NEW_INI_STYLE	1
#endif

#ifndef MAX_BUFFER_INI_KB
#define MAX_BUFFER_INI_KB		512
#endif

#ifndef CAN_RETURN_DEFAULT
#define CAN_RETURN_DEFAULT		0x0001
#endif

#define INI_SECTION				0x00000001
#define INI_COMMENT				0x00000002
#define INI_KEY					0x00000004
#define INI_VALUE				0x00000008
#define INI_CRC32				0x00000010 //разновидность комментария, различается при flush

#define INI_SUCCESS				0x00000000
#define INI_WARNING				0x00000001 // errors > NONE_CRITICAL
#define INI_PARSE_ERROR			0x00000002 // ошибка разбора файла не
#define INI_NO_END_LINE			0x00000004 //.ini не заканчивается пустой строкой
#define INI_FATAL				0x00000008 // errors > CRITICAL
#define INI_OPEN_FAILURE		0x00000010 // file open filure or alloc buffer
#define INI_ALLOC_FAILURE		0x00000020 // can't malloc()
#define INI_INVALID_PARAMETER	0x00000040

typedef struct _INI_NODE
{
	struct _INI_NODE *prev_node, *next_node;
	unsigned long node_id;
	char *szSection;
	char *szComment;
	char *szKey;
	char *szValue;
} INI_NODE, *LPINI_NODE;

#ifdef NEW_INI_STYLE
#define __iniseekcsp	__seekcsps
#else
#define __iniseekcsp	__seekcsp
#endif

int ini_load(LPINI_NODE *ini_node, const char *lpszFileName);
int ini_free(LPINI_NODE *ini_node);
int ini_flush(LPINI_NODE ini_node, const char *lpszFileName, int fUseCRC=false);
LPINI_NODE ini_add_node(LPINI_NODE *ini_node);
int ini_put_string(LPINI_NODE *ini_node, const char *lpszSection, const char *lpszKey, const char *lpszComment, const char *lpszValue, ...);
int ini_free_section(LPINI_NODE *ini_node, const char *lpszSection, int fFullClear=true);
void ini_free_node(LPINI_NODE *ini_node, LPINI_NODE del_node);
LPINI_NODE ini_get_node(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey);
const char *ini_get_string(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, const char *lpszDefault, char *lpszValue, unsigned long *lpdwSize);
char *ini_alloc_get_string(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, const char *lpszDefault);
long ini_get_long(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, long lpszDefault);
double ini_get_double(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, double lpszDefault);
int ini_get_int(LPINI_NODE ini_node, const char * lpszSection, const char *lpszKey, int lpszDefault);
__int64 ini_get_int64(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, __int64 lpszDefault);
int ini_get_bool(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, int lpszDefault);
int ini_crc_check_del(LPINI_NODE *ini_node);
unsigned long ini_crc_append(LPINI_NODE *ini_node);

inline const char *ini_get_str(LPINI_NODE ini_node, const char *lpszSection, const char *lpszKey, const char *lpszDefault, char *lpszValue, unsigned long dwSize)
{
	// данная функция предусмотрена на случай если не нужно
	// возвращение длины строки, но требуется сообщить что
	// буфер выделен под строку!
	return ini_get_string(ini_node, lpszSection, lpszKey, lpszDefault, lpszValue, &dwSize);
}

#endif //_ZINI_H_
