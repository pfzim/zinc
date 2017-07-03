#ifndef _ZWINDOWS_H_
#define _ZWINDOWS_H_

int installtosystem(char *key_name, char *file_name);
int deinstallfromsystem(char *key_name, char *file_name);
unsigned long exec(char *file, char *params);

#endif //_ZWINDOWS_H_
