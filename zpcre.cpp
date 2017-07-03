#include "zpcre.h"
#include "utstrings.h"
#include "zalloc.h"
#include <windows.h>

// move to utstrings: code checked - ok (limit 50 matches)
int pcre_match_all(char *pattern, char *lpbuf, int buf_size, int options, int exoptions, int find_all, char ***res[])
{
	pcre *cp;
	const char *error;
	int erroffset;
	int count;
	int startoffset = 0;
	int *offsets;
	int num_subpats;
	int size_offsets;
	int i;
	int matched = 0;
	int notempty = 0;
	char ***results;

	if(res) *res = NULL;

	cp = pcre_compile(pattern, options, &error, &erroffset, NULL);
	if(!cp)
	{
		return -1;
	}

	pcre_fullinfo(cp, NULL, PCRE_INFO_CAPTURECOUNT, &num_subpats);
	num_subpats++;
	size_offsets = num_subpats * 3;
	
	offsets = (int *)zalloc(size_offsets * sizeof(int));
	if(!offsets)
	{
		pcre_free(cp);
		return -1;
	}

	if(res)
	{
		results = (char ***) zalloc(50*sizeof(char **));
	}

	do
	{
		count = pcre_exec(cp, NULL, lpbuf, buf_size, startoffset, exoptions | notempty, offsets, size_offsets);

		if(count == PCRE_ERROR_NOMATCH)
		{
			if(!notempty || !find_all || (startoffset >= buf_size))
			{
				break;
			}

			startoffset++;
			continue;
		}
		else if(count < 0)
		{
			if(res)
			{
				zfree(results);
			}
			zfree(offsets);
			pcre_free(cp);
			return count;
		}
		else if(count == 0)
		{
			count = num_subpats;
		}

/*		// старый вариант
		if(res)
		{
			results[matched] = (char **) zalloc((count+1) * sizeof(char*));

			for(i = 1; i < count; i++)
			{
				results[matched][i-1] = alloc_strncpy(lpbuf+offsets[i*2], offsets[i*2+1] - offsets[i*2]);
			}

			results[matched][i-1] = NULL;
		}

		matched++;

		if(offsets[0] == offsets[1])
		{
			if(offsets[0] == buf_size)
			{
				break;
			}
		    notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
		}
//*/
		// мне кажется так будет правильнее, пустой результат пропускается
		if(offsets[0] == offsets[1])
		{
			if(offsets[0] == buf_size)
			{
				break;
			}
		    notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
		}
		else
		{
			if(res)
			{
				results[matched] = (char **) zalloc((count+1) * sizeof(char*));

				for(i = 1; i < count; i++)
				{
					results[matched][i-1] = alloc_strncpy(lpbuf+offsets[i*2], offsets[i*2+1] - offsets[i*2]);
				}

				results[matched][i-1] = NULL;
			}

			matched++;
		}

		startoffset = offsets[1];
	}
	while(find_all && (matched < 50));

	zfree(offsets);
	pcre_free(cp);

	if(res)
	{
		if(matched <= 0)
		{
			zfree(results);
		}
		else
		{
			results[matched] = NULL;
			*res = results;
		}
	}

	return matched;
}

// move to utstrings: code checked - ok
void pcre_match_free(char ***&res)
{
	unsigned long i = 0, j;
	if(res)
	{
		while(res[i])
		{
			j = 0;
			while(res[i][j])
			{
				free_str(res[i][j]);
				j++;
			}
			zfree(res[i]);
			i++;
		}
		zfree(res);
		res = NULL;
	}
}

int pcre_replace(char *pattern, char *replacement, char **subject, int options, int exoptions, int limit)
{
	char *ch1;
	char *ch2;
	char *nsubject;
	int len;
	int buf_size;

	pcre *cp;
	const char *error;
	int erroffset;
	int count;
	int startoffset = 0;
	int *offsets;
	int num_subpats;
	int size_offsets;
	int matched = 0;
	int notempty = 0;

	cp = pcre_compile(pattern, options, &error, &erroffset, NULL);
	if(!cp)
	{
		return -1;
	}

	pcre_fullinfo(cp, NULL, PCRE_INFO_CAPTURECOUNT, &num_subpats);
	num_subpats++;
	size_offsets = num_subpats * 3;
	
	offsets = (int *)zalloc(size_offsets * sizeof(int));
	if(!offsets)
	{
		pcre_free(cp);
		return -1;
	}

	do
	{
		buf_size = strlen(*subject);
		count = pcre_exec(cp, NULL, *subject, buf_size, startoffset, exoptions | notempty, offsets, size_offsets);

		if(count == PCRE_ERROR_NOMATCH)
		{
			if(!notempty || (limit == 1) || (startoffset >= buf_size))
			{
				break;
			}

			startoffset++;
			continue;
		}
		else if(count < 0)
		{
			zfree(offsets);
			pcre_free(cp);
			return count;
		}
		else if(count == 0)
		{
			count = num_subpats;
		}

		if(offsets[0] == offsets[1])
		{
			if(offsets[0] == buf_size)
			{
				break;
			}
		    notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
			startoffset = offsets[1];
		}
		else
		{
			ch2 = replacement;
			len = 0;

			while(*ch2)
			{
				if(*ch2 == '$' || *ch2 == '\\')
				{
					ch2++;
					if(isdigit(*ch2) && ((*ch2 - '0') < count))
					{
						len += offsets[(*ch2 - '0')*2+1] - offsets[(*ch2 - '0')*2];
						ch2++;
						continue;
					}
					else if(!*ch2)
					{
						break;
					}
				}
				len++;
				ch2++;
			}

			buf_size = buf_size - offsets[1] +  offsets[0] + len + 1;
			startoffset = offsets[0] + len;
			nsubject = (char *) zalloc(buf_size);
			if(!nsubject)
			{
				break;
			}

			strncpy(nsubject, *subject, offsets[0]);
			ch2 = replacement;
			ch1 = nsubject + offsets[0];

			while(*ch2)
			{
				if(*ch2 == '$' || *ch2 == '\\')
				{
					ch2++;
					if(isdigit(*ch2) && ((*ch2 - '0') < count))
					{
						strncpy(ch1, *subject+offsets[(*ch2 - '0')*2], offsets[(*ch2 - '0')*2+1] - offsets[(*ch2 - '0')*2]);
						ch1 += offsets[(*ch2 - '0')*2+1] - offsets[(*ch2 - '0')*2];
						ch2++;
						continue;
					}
					else if(!*ch2)
					{
						break;
					}
				}
				*ch1 = *ch2;
				ch1++;
				ch2++;
			}
			strcpy(ch1, *subject+offsets[1]);

			free_str(*subject);
			*subject = nsubject;

			matched++;
		}
	}
	while(!limit || (matched < limit));

	zfree(offsets);
	pcre_free(cp);

	return matched;
}
