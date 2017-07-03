#ifndef _ZPCRE_H_
#define _ZPCRE_H_

#include "pcre/pcre.h"

int pcre_match_all(char *pattern, char *lpbuf, int buf_size, int options, int exoptions, int find_all, char ***res[]);
void pcre_match_free(char ***&res);
int pcre_replace(char *pattern, char *replacement, char **subject, int options, int exoptions, int limit);


#endif // _ZPCRE_H_