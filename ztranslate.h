#ifndef _ZTRANSLATE_H_
#define _ZTRANSLATE_H_

typedef struct _langpack_info
{
	unsigned long flags;
	char *file_name;
	unsigned long version;
	char *mod_name;
	char *name;
	char *description;
	char *copyright;
	char *mail;
	char *url;
	char *update;
} langpack_info;

unsigned long NameHashFunction(const char *szStr);

int LoadLanguagePack();
int FreeLanguagePack();
char *Translate(char *text, unsigned long line);
langpack_info *LangPackInfo();
//unsigned long LangPackVersion();
//char *LangPackFile();

#endif //_ZTRANSLATE_H_
