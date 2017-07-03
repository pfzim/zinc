#include "zscript.h"														// pf_ziminski (c) 2009
#include "utstrings.h"
#include <windows.h>

/*
#include <shellapi.h>
#include <shlobj.h>
#include <io.h>
#include "zibuffer.h"
#include "zhttp.h"
#include "zwindows.h"
#include "zfiles.h"

//#pragma comment (lib, "ws2_32.lib")
//#pragma comment (lib, "shell32.lib")

#ifndef __BUILD_STR__
#define __BUILD_STR__		"62"
#endif

#ifndef APP_NAME
#define APP_NAME			"simple script (build "__BUILD_STR__")"
#endif


// hInstance, in_file - optional parameters for detect SCRIPTDIR and MYDIR
int simple_script(char *data, unsigned long data_size, HINSTANCE hInstance, char *in_file)
{
	int exit_code;
	unsigned long pos;
	char *temp_str;

	char *stack[32];
	char **esp;

	memset(stack, 0, sizeof(stack));
	esp = stack;
	pos = 0;

	exit_code = -1;

	while(1)
	{
		temp_str = ltrim(alloc_getstringbefore(data, data_size, &pos, "\r\n"));
		if(pos >= data_size)
		{
			free_str(temp_str);
			break;
		}

		if(isempty(temp_str) || (*temp_str == ';'))
		{
			// skip empty string or comment
		}
		else if(!strncmp(temp_str, "PUSH", 4))
		{
			esp++;
			*esp = alloc_unescapestring(trim(&temp_str[4]), NULL);
		}
		else if(!strncmp(temp_str, "POP", 3))
		{
			free_str(*esp);
			esp--;
		}
		else if(!strncmp(temp_str, "CAT", 3))
		{
			alloc_strcat(esp, *(esp -1));
			free_str(*(esp -1));
			*(esp -1) = *esp;
			esp--;
		}
		else if(!strncmp(temp_str, "VERSION", 7))
		{
			if(strcmp(*esp, __BUILD_STR__))
			{
				if(MessageBox(NULL, "Версия приложения и скрипта не соответствуют друг другу!\nПродолжить выполнение?", APP_NAME, MB_YESNO) == IDNO)
				{
					break;
				}
			}
		}
		else if(!strncmp(temp_str, "MSG", 3))
		{
			MessageBox(NULL, *esp, APP_NAME, MB_OK);
		}
		else if(!strncmp(temp_str, "GET", 3))
		{
			http_data *req, *resp;
			unsigned long x;
			EXTBUFFER zb;
			char *proxy;
			unsigned long port;
			int use_proxy;
			
			free_str(temp_str);
			use_proxy = proxy_detect(&temp_str);

			if(!use_proxy)
			{
				proxy = alloc_getpa(temp_str, 0, ":");
				port = strtoul(_pa(temp_str, 1, ":"), NULL, 10);
			}

			req = httpdata_new(use_proxy?NULL:proxy, port, *esp, 0);

			if(!use_proxy)
			{
				free_str(proxy);
			}

			variables_put_string(&req->params, "User-Agent", APP_NAME);
			variables_put_string(&req->params, "Accept", "* /*);
			variables_put_string(&req->params, "Connection", "close");
			if(!use_proxy)
			{
				variables_put_string(&req->params, "Proxy-Connection", "close");
			}

			x = getpage_ntlm_ex(req, &resp, NULL);

			httpdata_free(req);

			if(x == 0)
			{
				if(__initbufferedwrite(*(esp-1), &zb, 32768))
				{
					__addblock(&zb, resp->data, resp->datasize);
					__donebufferedwrite(&zb);
					__destructbufferedreadwrite(&zb);
				}

				httpdata_free(resp);
			}
		}
		else if(!strncmp(temp_str, "RUN", 3))
		{
			WinExec(*esp, SW_SHOWDEFAULT);
		}
		else if(!strncmp(temp_str, "SHELLRUN", 8))
		{
			ShellExecute(NULL, "open", *esp, NULL, NULL, SW_SHOW);
		}
		else if(!strncmp(temp_str, "MYEXEC", 6))
		{
			exec(*esp, *(esp-1));
		}
		else if(!strncmp(temp_str, "QUIT", 4))
		{
			exit_code = 0;
			free_str(temp_str);
			break;
		}
		else if(!strncmp(temp_str, "CWD", 3))
		{
			SetCurrentDirectory(*esp);
		}
		else if(!strncmp(temp_str, "PWD", 3))
		{
			esp++;
			*esp = (char *) zalloc(4096);
			if(*esp)
			{
				GetCurrentDirectory(4096, *esp);
			}
		}
		else if(!strncmp(temp_str, "MYDIR", 5))
		{
			esp++;
			*esp = nullstring;
			free_str(temp_str);
			temp_str = (char *) zalloc(MAX_PATH);
			if(temp_str)
			{
				GetModuleFileName(hInstance, temp_str, MAX_PATH);
				*esp = alloc_filepath(temp_str, FPI_DIR);
			}
		}
		else if(!strncmp(temp_str, "SCRIPTDIR", 9))
		{
			esp++;
			if(!isempty(in_file))
			{
				*esp = alloc_filepath(in_file, FPI_DIR);
			}
			else
			{
				*esp = nullstring;
			}
		}
		else if(!strncmp(temp_str, "DUP", 3))
		{
			esp++;
			*esp = alloc_string(*(esp -1));
		}
		else if(!strncmp(temp_str, "COPY", 4))
		{
			fcopy(*esp, *(esp -1), 1);
		}
		else if(!strncmp(temp_str, "MKDIR", 5))
		{
			mkrdir(*esp);
		}
		else if(!strncmp(temp_str, "REMOVE", 6))
		{
			remove(*esp);
		}
		else if(!strncmp(temp_str, "SLEEP", 5))
		{
			Sleep(5000);
		}
		else if(!strncmp(temp_str, "XCHG", 4))
		{
			*(unsigned long *) (esp -1) ^= (unsigned long) *esp;
			*(unsigned long *) esp ^= (unsigned long) *(esp -1);
			*(unsigned long *) (esp -1) ^= (unsigned long) *esp;
		}
		else if(!strncmp(temp_str, "WAITMUTEX", 9))
		{
			HANDLE hMutex;
			hMutex = OpenMutex(SYNCHRONIZE, FALSE, *esp);
			if(hMutex)
			{
				if(WaitForSingleObject(hMutex, INFINITE) == WAIT_OBJECT_0)
				{
					ReleaseMutex(hMutex);
				}
				CloseHandle(hMutex);
			}
		}
		else
		{
			MessageBox(NULL, temp_str, APP_NAME": unknown command", MB_OK);
		}

		free_str(temp_str);
	}

	esp = stack;
	while(*esp)
	{
		free_str(*esp);
		esp++;
	}

	return exit_code;
}
*/

typedef struct _ss_labels
{
	unsigned long offset;
	char *name;
} ss_labels;

void cb_ss_label_free(void *data)
{
	zfree(data);
}

int cb_ss_label_cmp(void *data1, void *data2)
{
	return !strcmp(((ss_labels *)data1)->name, (char *) data2);
}

int ss_label_define(db_list_node **labels, char *name, unsigned long offset)
{
	ss_labels *temp_node;
	if(!isempty(name))
	{
		if(!list_find(*labels, cb_ss_label_cmp, name))
		{
			temp_node = (ss_labels *) zalloc(sizeof(ss_labels) + strlen(name) + 1);
			if(temp_node)
			{
				temp_node->offset = offset;
				temp_node->name = (char *) ((unsigned long) temp_node + sizeof(ss_labels));
				strcpy(temp_node->name, name);
				list_insert(labels, temp_node);
			}
		}
	}

	return 0;
}

unsigned long ss_label_find(db_list_node *labels, char *name)
{
	db_list_node *temp_node;

	temp_node = list_find(labels, cb_ss_label_cmp, name);
	if(temp_node)
	{
		return ((ss_labels *)temp_node->data)->offset;
	}
	return 0;
}

// simple slow script
int simple_script_ex(char *data, unsigned long data_size, ss_funcs_list *funcs)
{
	int exit_code;
	int error_code;
	char *find_label;
	unsigned long pos;
	unsigned long j;
	char *temp_str;

	db_list_node *labels;

	char *stack[32];
	char **esp;

	memset(stack, 0, sizeof(stack));
	esp = stack;
	*esp = nullstring;

	labels = NULL;

	pos = 0;
	find_label = nullstring;

	exit_code = -1;
	error_code = 0;

	while(1)
	{
		temp_str = ltrim(alloc_getstringbefore(data, data_size, &pos, "\r\n"));
		if(!isalloc(temp_str) && (pos >= data_size))
		{
			break;
		}

		if(isempty(temp_str) || (*temp_str == ';'))
		{
			// skip empty string or comment
		}
		else if(*temp_str == ':')
		{
			ss_label_define(&labels, trim(temp_str + 1), pos);
			if(!isempty(find_label) && !strcmp(find_label, temp_str + 1))
			{
				free_str(find_label);
			}
		}
		else if(isempty(find_label))
		{
			if(!strncmp(temp_str, "JMP", 3))
			{
				j = ss_label_find(labels, trim(&temp_str[3]));
				if(j)
				{
					pos = j;
				}
				else
				{
					find_label = alloc_string(&temp_str[3]);
				}
			}
			else if(!strncmp(temp_str, "JNZ", 3))
			{
				if(error_code)
				{
					j = ss_label_find(labels, trim(&temp_str[3]));
					if(j)
					{
						pos = j;
					}
					else
					{
						find_label = alloc_string(&temp_str[3]);
					}
				}
			}
			else if(!strncmp(temp_str, "JZ", 2))
			{
				if(!error_code)
				{
					j = ss_label_find(labels, trim(&temp_str[2]));
					if(j)
					{
						pos = j;
					}
					else
					{
						find_label = alloc_string(&temp_str[2]);
					}
				}
			}
			if(!strncmp(temp_str, "CALL", 4))
			{
				char temp_buf[20];
				if(esp >= stack + 31)
				{
					break;
				}
				esp++;
				ultoa(pos, temp_buf, 10);
				*esp = alloc_string(temp_buf);

				j = ss_label_find(labels, trim(&temp_str[4]));
				if(j)
				{
					pos = j;
				}
				else
				{
					find_label = alloc_string(&temp_str[4]);
				}
			}
			else if(!strncmp(temp_str, "RET", 3))
			{
				if(esp <= stack)
				{
					break;
				}

				pos = strtoul(*esp, NULL, 10);
				free_str(*esp);
				esp--;
			}
			else if(!strncmp(temp_str, "PUSH", 4))
			{
				if(esp >= stack + 31)
				{
					break;
				}
				esp++;
				*esp = alloc_unescapestring(trim(&temp_str[4]), NULL);
			}
			else if(!strncmp(temp_str, "POP", 3))
			{
				if(esp <= stack)
				{
					break;
				}
				free_str(*esp);
				esp--;
			}
			else if(!strncmp(temp_str, "CAT", 3))
			{
				if(esp <= stack + 1)
				{
					break;
				}

				alloc_strcat(esp, *(esp -1));
				free_str(*(esp -1));
				*(esp -1) = *esp;
				esp--;
			}
			else if(!strncmp(temp_str, "DUP", 3))
			{
				if(esp >= stack + 31)
				{
					break;
				}

				esp++;
				*esp = alloc_string(*(esp -1));
			}
			else if(!strncmp(temp_str, "XCHG", 4))
			{
				if(esp <= stack + 1)
				{
					break;
				}

				*(unsigned long *) (esp -1) ^= (unsigned long) *esp;
				*(unsigned long *) esp ^= (unsigned long) *(esp -1);
				*(unsigned long *) (esp -1) ^= (unsigned long) *esp;
			}
			else if(!strncmp(temp_str, "RESET", 5))
			{
				error_code = 0;
			}
			else if(!strncmp(temp_str, "ERROR", 5))
			{
				error_code = -1;
			}
			else if(!strncmp(temp_str, "QUIT", 4))
			{
				exit_code = 0;
				free_str(temp_str);
				break;
			}
			else if(!strncmp(temp_str, "BIE", 3)) // break if error
			{
				if(error_code)
				{
					exit_code = error_code;
					free_str(temp_str);
					break;
				}
			}
			else
			{
				for(j = 0; funcs[j].name; j++)
				{
					if(!strncmp(temp_str, funcs[j].name, strlen(funcs[j].name)))
					{
						break;
					}
				}

				if(funcs[j].proc)
				{
					error_code = ((int (__cdecl *)(char *, char ***))funcs[j].proc)(temp_str, &esp);
				}
				/*
				else
				{
					MessageBox(NULL, temp_str, APP_NAME": unknown command", MB_OK);
				}
				*/
			}
		}

		free_str(temp_str);
	}

	esp = stack;
	while(*esp)
	{
		free_str(*esp);
		esp++;
	}

	free_str(find_label);
	list_free(&labels, cb_ss_label_free);

	return exit_code;
}
