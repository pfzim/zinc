#ifndef _ZSCRIPT_H_
#define _ZSCRIPT_H_

//#include <windows.h>

typedef struct _ss_funcs_list
{
	char *name;
	void *proc;
} ss_funcs_list;

//int simple_script(char *data, unsigned long data_size, HINSTANCE hInstance, char *in_file);
int simple_script_ex(char *data, unsigned long data_size, ss_funcs_list *funcs);

#endif //_ZSCRIPT_H_
