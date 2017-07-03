#ifndef _ZHTTP_H_
#define _ZHTTP_H_

#include "utstrings.h"


#define SCK_TIMEOUT				60			// 90

#define VL_STRING				0x00000001
#define VL_NUMB					0x00000002

#define HTTP_10						0x00000000	// default
#define HTTP_11						0x00000001
#define HTTP_FULL_URI				0x00000002
#define HTTP_GET					0x00000000	// default
#define HTTP_POST					0x00000004
#define HTTP_HEAD					0x00000008


typedef int (*CBCF)(unsigned long, unsigned long *);

typedef struct _http_data
{
	unsigned long flags;
	char *host;				// where to sent request - proxy, can be NULL
	unsigned long port;		// proxy port
	//char *method;
	char *uri;
	char *protocol;
	//char *errorcode;
	unsigned long error_code;
	char *message;
	LIST_VARIABLE_NODE *params;
	unsigned long datasize;
	char *data;
} http_data;

int proxy_detect(char **proxy);
http_data *httpdata_new(char *host, unsigned long port, char *uri, unsigned long flags);
void httpdata_free(http_data *&hd);
int getpage(char *host, unsigned long port, char *header, unsigned long hsize, char **bbuf, unsigned long *size, CBCF fpProc);
int getpage_ex(http_data *request, http_data **response, CBCF fpProc);
int getpage_follow_ex(http_data *request, http_data **response, CBCF fpProc);
int getpage_ntlm_ex(http_data *request, http_data **response, CBCF fpProc);

#endif //_ZHTTP_H_
