#include "zl_helper.h"									// pfzim (c) 2009
#include "../utstrings.h"
#include "../zpcre.h"
#include "../base64.h"

unsigned long fn_isempty(char *text)
{
	return isempty(text);
}

unsigned long fn_isalloc(char *text)
{
	return isalloc(text);
}

unsigned long fn_msgbox(char *text1, char *text2)
{
	return MessageBox(NULL, text2, text1, MB_TOPMOST | MB_OK);
}

unsigned long fn_yes(char *text1, char *text2)
{
	return (MessageBox(NULL, text2, text1, MB_TOPMOST | MB_YESNO) == IDYES);
}

unsigned long fn_free_str(char *text)
{
	free_str(text);
	return 0L;
}

unsigned long fn_pcre_match_free(char ***res)
{
	pcre_match_free(res);
	return 0L;
}

char *fn_strstri(char *text1, char *text2)
{
	char *res;
	text1 = _strlwr(alloc_string(text1));
	text2 = _strlwr(alloc_string(text2));
	res = strstr(text1, text2);
	free_str(text1);
	free_str(text2);

	return res;
}

char *fn_dupn(unsigned long n)
{
	return alloc_string_ex("%d", n);
}

zl_funcs_list fn_list[] = {
	{"isalloc",				fn_isalloc},
	{"isempty",				fn_isempty},
	{"free_str",			fn_free_str},
	{"strstri",				fn_strstri},
	{"msg",					fn_msgbox},
	{"yes",					fn_yes},
	{"dupn",				fn_dupn},
	{"pcre_match_free",		fn_pcre_match_free},

	{"alloc_string",		alloc_string},
	{"alloc_strcat",		alloc_strcat},
	{"alloc_addslashes",	alloc_addslashes},
	{"alloc_urlencode",		alloc_urlencode},
	{"zalloc",				zalloc},
	{"zrealloc",			zrealloc},
	{"zfree",				zfree},
	{"pcre_match_all",		pcre_match_all},
	{"pcre_replace",		pcre_replace},
	{"alloc_en64",			alloc_en64},
	{"alloc_de64",			alloc_de64},

	{"strstr",				strstr},
	{"strcmp",				strcmp},
	{"stricmp",				stricmp},
	{"urldecode",			urldecode},
	{"atoi",				atoi},
	{"swapchar",			swapchar},
	{"trim",				trim},
	{"ltrim",				ltrim},
	{"rtrim",				rtrim},
	{"isfloat",				isfloat},
	{"isinteger",			isinteger},
	{"strlen",				strlen},
	{"hex2char",			hex2char},
	{"killspaces",			killspaces},
	{NULL,					NULL}
};

unsigned long zl_load_predefined_functions(unsigned char *import_section, zl_map_section *map_section, unsigned long count, unsigned long **modules)
{
	return zl_load_functions(import_section, map_section, count, fn_list, modules);
}
