//#include "stdafx.h"								//pf_ziminski  [2007]

// Модифицировать httpdata_new: [кажется всё это сделано?]
// - Cохранять полный uri
// - Host не добавлять при формировании заголовка
// - Добавить поля proxy и proxy_port
// - protocol и method заменить на флаги !?
// - Host брать из uri
// - path полный, если прокси определен!? если выствлен соответствующий флаг!
// - при Location использовать strippath(..., _DIR)
//
// TODO: При переадресации не пересылать POST вторично по новому адресу!
//
//

#include <winsock2.h>
#define SECURITY_WIN32	1
#include <sspi.h>
#include "base64.h"
#include "zhttp.h"
#include "zdbg.h"


//#pragma comment (lib, "ws2_32.lib")

//  C  --> S   GET ...
//  C <--  S   401 Unauthorized
//             WWW-Authenticate: NTLM
//  C  --> S   GET ...
//             Authorization: NTLM <base64-encoded type-1-message>
//  C <--  S   401 Unauthorized
//             WWW-Authenticate: NTLM <base64-encoded type-2-message>
//  C  --> S   GET ...
//             Authorization: NTLM <base64-encoded type-3-message>
//  C <--  S   200 Ok

/*
typedef struct _BUFFER
{
	struct _BUFFER *next_node;
	unsigned long size;
	char buf[4];
} BUFFER, *LPBUFFER;
*/

typedef struct _long_buffer_part
{
	struct _long_buffer_part *next_node;
	unsigned long size;
	char data;
} long_buffer_part;

int proxy_detect(char **proxy)
{
	// return 0 if proxy enabled and detected
	// return 1 if proxy disabled and detected
	// return >1 on error

	HKEY hSubKey;
	unsigned long enabled;
	unsigned long size;
	unsigned long exit_code;
	char *temp_str;

	*proxy = nullstring;
	exit_code = 0;

	if(RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", &hSubKey) != ERROR_SUCCESS)
	{
		return -2;
	}

	size = 4;
	if((RegQueryValueEx(hSubKey, "ProxyEnable", NULL, NULL, (unsigned char *) &enabled, &size) != ERROR_SUCCESS) || !enabled)
	{
		exit_code = 1;
	}

	if((RegQueryValueEx(hSubKey, "ProxyServer", NULL, NULL, NULL, &size) != ERROR_SUCCESS))
	{
		exit_code = -2;
		goto lb_exit;
	}

	*proxy = (char *) zalloc(size);
	if(!*proxy)
	{
		*proxy = nullstring;
		exit_code = -3;
		goto lb_exit;
	}

	if((RegQueryValueEx(hSubKey, "ProxyServer", NULL, NULL, (unsigned char *) *proxy, &size) == ERROR_SUCCESS))
	{
		size = _maxprm(*proxy, ";");
		while(size)
		{
			size--;
			temp_str = alloc_getpa(*proxy, size, ";");

			if(strnicmp(temp_str, "http=", 5) == 0)
			{
				zfree(*proxy);
				*proxy = alloc_string(&temp_str[5]);
				free_str(temp_str);
				break;
			}

			free_str(temp_str);
		}
	}
	else
	{
		zfree(*proxy);
		*proxy = nullstring;
		exit_code = -4;
	}

lb_exit:
	RegCloseKey(hSubKey);

	return exit_code;
}

http_data *httpdata_new(char *host, unsigned long port, char *uri, unsigned long flags)
{
	http_data *hd;
	
	hd = (http_data *) zalloc(sizeof(http_data));
	if(hd)
	{
		memset(hd, 0, sizeof(http_data));

		hd->host = isempty(host)?nullstring:alloc_string(host);
		hd->port = port;
		//hd->method = alloc_string(method);
		hd->uri = alloc_string(uri);
		hd->flags = flags;
		//hd->protocol = alloc_string(protocol); // only for response
	}

	return hd;
}

void httpdata_free(http_data *&hd)
{
	if(hd)
	{
		//free_str(hd->errorcode);
		free_str(hd->host);
		free_str(hd->message);
		//free_str(hd->method);
		free_str(hd->protocol);
		free_str(hd->uri);
		free_str(hd->data);

		variables_free(&hd->params);

		zfree(hd);
		hd = NULL;
	}
}

int getpage(char *host, unsigned long port, char *lpheader, unsigned long hsize, char **bbuf, unsigned long *size, CBCF fpProc)
{

	HOSTENT *he;
	SOCKADDR_IN sa;
	SOCKET socket_rem = INVALID_SOCKET;
	long_buffer_part *lplist = NULL;
	long_buffer_part **lptemp = &lplist;
	long_buffer_part *lpnode, *lpdel;

	int recvsize;
	unsigned long pktsize, ttlsize, i;
	int exit_code = 0;

	if(size) *size = 0;
	if(bbuf) *bbuf = nullstring;

	he = gethostbyname(host);
	if(!he)
	{
		sa.sin_addr.s_addr = inet_addr(host);
		if(sa.sin_addr.s_addr == INADDR_NONE)
		{
			return -2;
		}
	}
	else
	{
		memcpy(&(sa.sin_addr), he->h_addr, he->h_length);
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons((unsigned short) port);

	if(fpProc && (fpProc(0, NULL) != 0))
	{
		return -1;
	}

	socket_rem = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_rem == INVALID_SOCKET)
	{
		return -3;
	}

	if(connect(socket_rem, (struct sockaddr FAR *) &sa, sizeof(sa)) == SOCKET_ERROR)
	{
		closesocket(socket_rem);
		return -4;
	}

	for(i = 0; hsize > 0;)
	{
		if(fpProc && (fpProc(0, NULL) != 0))
		{
			closesocket(socket_rem);
			return -1;
		}

		send(socket_rem, lpheader + i, (hsize > 8192)?8192:hsize, 0);
		if(hsize > 8192)
		{
			hsize -= 8192;
		}
		else
		{
			break;
		}
		i += 8192;
	}

	ttlsize = 0;

	fd_set readfds;
	timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	int iteration = 0;

	while(ttlsize < 10485760) //TRUE
	{
		FD_ZERO(&readfds);
		FD_SET(socket_rem, &readfds);

		if(fpProc && (fpProc(0, NULL) != 0))
		{
			exit_code = -1;
			goto lb_interrupt;
		}

		i = select(0, &readfds, NULL, NULL, &tv);
		if(i == 0)
		{
			iteration++;
			if(iteration < SCK_TIMEOUT)
			{
				continue;
			}
			else
			{
				exit_code = -5;
				goto lb_interrupt;
			}
		}
		if(i == SOCKET_ERROR)
		{
			exit_code = -6;
			goto lb_interrupt;
		}

		if(fpProc && fpProc(0, NULL) != 0)
		{
			exit_code = -1;
			goto lb_interrupt;
		}

		if(FD_ISSET(socket_rem, &readfds))
		{
			iteration = 0;
			if(ioctlsocket(socket_rem, FIONREAD, &pktsize) == SOCKET_ERROR)
			{
				exit_code = -7;
				goto lb_interrupt;
			}

			if(pktsize == 0)
			{
				break;
			}

			*lptemp = (long_buffer_part *) zalloc(pktsize + sizeof(long_buffer_part) - sizeof(char));
			if(!*lptemp)
			{
				exit_code = -8;
				goto lb_interrupt;
			}

			(*lptemp)->size = 0;
			(*lptemp)->next_node = NULL;

			recvsize = recv(socket_rem, &(*lptemp)->data, pktsize, 0);
			if(recvsize == 0)
			{
				zfree(*lptemp);
				*lptemp = NULL;
				break;
			}
			if(recvsize == SOCKET_ERROR)
			{
				exit_code = -9;
				goto lb_interrupt;
			}
			ttlsize += recvsize;
			(*lptemp)->size = recvsize;
			lptemp = &(*lptemp)->next_node;
		}
	}

	if(bbuf)
	{
		if(ttlsize > 0)
		{
			*bbuf = (char*) zalloc(ttlsize);
			if(!*bbuf)
			{
				exit_code = -10;
				goto lb_interrupt;
			}
		}
		else
		{
			*bbuf = nullstring;
		}
	}

	closesocket(socket_rem);

	if(size) *size = ttlsize;

	lpnode = lplist;
	i = 0;

	while(lpnode)
	{
		if(bbuf) memcpy(*bbuf+i, &lpnode->data, lpnode->size);
		i += lpnode->size;

		lpdel = lpnode;
		lpnode = lpnode->next_node;
		zfree(lpdel);
	}

	return 0;

lb_interrupt:

	if(socket_rem != INVALID_SOCKET)
	{
		closesocket(socket_rem);
	}

	while(lplist)
	{
		lpdel = lplist;
		lplist = lplist->next_node;
		zfree(lpdel);
	}

	return exit_code;
}

/*
// depricated - use getpage_follow_ex()
int getpage_ex(http_data *request, http_data **response, CBCF fpProc)
{
	char *header;
	char *temp_str;
	char *name, *value;
	int exit_code;
	unsigned long pos;
	LIST_VARIABLE_NODE *lplist;
	char *bbuf;
	unsigned long size;
	unsigned long hsize;

	if(response) *response = NULL;

	temp_str = alloc_string("                    ");
	header = alloc_string((request->flags & HTTP_POST)?"POST ":"GET ");
	alloc_strcat(&header, " ");
	alloc_strcat(&header, request->uri);
	alloc_strcat(&header, " ");
	alloc_strcat(&header, (request->flags & HTTP_11)?" HTTP/1.1":" HTTP/1.0");
	alloc_strcat(&header, "\r\n");

	lplist = request->params;
	while(lplist)
	{
		alloc_strcat(&header, lplist->key);
		alloc_strcat(&header, ": ");
		if(lplist->flags & VL_STRING)
		{
			alloc_strcat(&header, lplist->value.str);
		}
		else if(lplist->flags & VL_NUMB)
		{
			itoa(lplist->value.numb, temp_str, 10);
			alloc_strcat(&header, temp_str);
		}
		alloc_strcat(&header, "\r\n");

		lplist = lplist->next_node;
	}

	free_str(temp_str);

	alloc_strcat(&header, "\r\n");

	hsize = strlen(header);

	if(request->data && (request->datasize > 0))
	{
		alloc_strncat(&header, request->data, request->datasize);
		hsize += request->datasize;
	}

	exit_code = getpage(request->host, request->port, header, hsize, &bbuf, &size, fpProc);

	//httpdata_free(request);

	if(!response || (exit_code != 0) || (size == 0))
	{
		if(size == 0)
		{
			exit_code = -1;
		}

		goto lb_finalize;
	}

	*response = (http_data *) zalloc(sizeof(http_data));

	if(!*response)
	{
		goto lb_finalize;
	}

	memset(*response, 0, sizeof(http_data));

	free_str(header);

	pos = 0;

	(*response)->protocol = alloc_getstringbefore(bbuf, size, &pos, " ");
	//(*response)->errorcode = alloc_getstringbefore(bbuf, size, &pos, " ");
	(*response)->error_code = strtoul(alloc_getstringbefore(bbuf, size, &pos, " "), NULL, 10);
	(*response)->message = alloc_getstringbefore(bbuf, size, &pos, "\r\n");

	while(true)
	{
		temp_str = alloc_getstringbefore(bbuf, size, &pos, "\r\n");
		if(!isalloc(temp_str))
		{
			break;
		}

		//name = alloc_trim(alloc_getpa(lpstr, 0, ":"));
		//value = alloc_trim(alloc_getpa(lpstr, 1, ":"));
		unsigned long cur;
		
		cur = 0;

		name = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, ":"));
		value = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, "\r\n"));
		//value = alloc_getstringbefore(lpstr, strlen(lpstr), &cur, "\r\n");
		//value = alloc_trim(value);

		variables_put_string(&(*response)->params, name, "%s", value);

		free_str(name);
		free_str(value);

		free_str(temp_str);
	}

	if(size <= pos)
	{
		goto lb_finalize;
	}

	(*response)->data = (char *) zalloc(size - pos);

	if(!(*response)->data)
	{
		goto lb_finalize;
	}

	/*
	lplist = variables_get_by_name((*response)->params, "Encoding");
	if(lplist && !strcmp(lplist->value.str, "chunked"))
	{
		strtoul(bbuf + pos, NULL, 16);

	}
	else
	* /
	{
		memcpy((*response)->data, bbuf+pos, size - pos);
		(*response)->datasize = size - pos;
	}

lb_finalize:
	free_str(bbuf);
	
	return exit_code;
}
*/

int getpage_ex(http_data *request, http_data **response, CBCF fpProc)
{
	char *header;
	char temp_buf[32];
	char *temp_str;
	char *prev_host;
	char *prev_path;
	char *location;
	char *name;
	char *value;
	int exit_code;
	unsigned long pos;
	char *bbuf;
	unsigned long size;
	unsigned long hsize;
	int loop;
	unsigned long port;
	LIST_VARIABLE_NODE *temp_node;
	QUERY_STRING_NODE *uri_parsed;

	if(response) *response = NULL;

	uri_parsed = NULL;
	loop = 5;	// allow five redirections
	prev_host = nullstring;
	prev_path = nullstring;
	//location = alloc_string(request->uri);

lb_follow_me:

	url_string_parse(&uri_parsed, request->uri);

	header = alloc_string((request->flags & HTTP_POST)?"POST ":"GET ");

	// если путь не абсолютный, то суммируем его с директорией от предыдущего запроса
	if(isempty(request->host))
	{
		temp_str = query_string_get(uri_parsed, "%path%");
		if(isempty(temp_str))
		{
			alloc_strcat(&header, prev_path);
		}
		else if(*temp_str != '/')
		{
			alloc_strcat(&header, prev_path);
			alloc_strcat(&header, temp_str);
		}
		else
		{
			free_str(prev_path);
			prev_path = strippath(alloc_string(temp_str), FPI_DIR);
			alloc_strcat(&header, temp_str);
		}

		temp_str = query_string_get(uri_parsed, "%query%");
		if(!isempty(temp_str))
		{
			alloc_strcat(&header, "?");
			alloc_strcat(&header, temp_str);
		}
	}
	else // to proxy send full path? check this in RFC.
	{
		alloc_strcat(&header, request->uri);	// full path
	}

	alloc_strcat(&header, (request->flags & HTTP_11)?" HTTP/1.1":" HTTP/1.0");
	alloc_strcat(&header, "\r\n");

	// если хост не определён, то берём значение от предыдущего запроса
	alloc_strcat(&header, "Host: ");

	temp_str = query_string_get(uri_parsed, "%host%");
	if(!isempty(temp_str))
	{
		free_str(prev_host);
		prev_host = alloc_string(temp_str);
		alloc_strcat(&header, temp_str);

		temp_str = query_string_get(uri_parsed, "%port%");
		if(!isempty(temp_str))
		{
			alloc_strcat(&header, ":");
			alloc_strcat(&header, temp_str);
			port = strtoul(temp_str, NULL, 10);
		}
		else
		{
			port = 80;
		}
	}
	else if(!isempty(prev_host))
	{
		alloc_strcat(&header, prev_host); // здесь порт теряется от предыдущего запроса
	}
	else
	{
		free_str(header);
		query_string_free(&uri_parsed);
		exit_code = -1;
		goto lb_finalize;
	}

	alloc_strcat(&header, "\r\n");

	query_string_free(&uri_parsed);

	// собираем заголовок
	temp_node = request->params;
	while(temp_node)
	{
		alloc_strcat(&header, temp_node->key);
		alloc_strcat(&header, ": ");
		if(temp_node->flags & VL_STRING)
		{
			alloc_strcat(&header, temp_node->value.str);
		}
		else if(temp_node->flags & VL_NUMB)
		{
			itoa(temp_node->value.numb, temp_buf, 10);
			alloc_strcat(&header, temp_buf);
		}
		alloc_strcat(&header, "\r\n");

		temp_node = temp_node->next_node;
	}

	alloc_strcat(&header, "\r\n");

	hsize = strlen(header);

	if(request->data && (request->datasize > 0))
	{
		alloc_strncat(&header, request->data, request->datasize);
		hsize += request->datasize;
	}

	ZTRACE("Header:\n%s\n", header);

	exit_code = getpage(isempty(request->host)?prev_host:request->host, isempty(request->host)?port:request->port, header, hsize, &bbuf, &size, fpProc);

	free_str(header);

	if(!response || (exit_code != 0) || (size == 0))
	{
		// если результат ответа не нужен
		if(size == 0)
		{
			exit_code = -1;
		}

		goto lb_finalize;
	}

	*response = (http_data *) zalloc(sizeof(http_data));

	if(!*response)
	{
		exit_code = -1;
		goto lb_finalize;
	}

	memset(*response, 0, sizeof(http_data));

	pos = 0;

	(*response)->protocol = alloc_getstringbefore(bbuf, size, &pos, " ");
	//(*response)->errorcode = alloc_getstringbefore(bbuf, size, &pos, " ");
	temp_str = alloc_getstringbefore(bbuf, size, &pos, " ");
	(*response)->error_code = strtoul(temp_str, NULL, 10);
	free_str(temp_str);
	(*response)->message = alloc_getstringbefore(bbuf, size, &pos, "\r\n");

	while(true)
	{
		temp_str = alloc_getstringbefore(bbuf, size, &pos, "\r\n");
		if(!isalloc(temp_str))
		{
			break;
		}

		//name = alloc_trim(alloc_getpa(lpstr, 0, ":"));
		//value = alloc_trim(alloc_getpa(lpstr, 1, ":"));
		unsigned long cur;
		
		cur = 0;

		name = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, ":"));
		value = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, "\r\n"));
		//value = alloc_getstringbefore(lpstr, strlen(lpstr), &cur, "\r\n");
		//value = alloc_trim(value);

		variables_put_string(&(*response)->params, name, "%s", value);

		free_str(name);
		free_str(value);

		free_str(temp_str);
	}

	temp_node = variables_get_by_name((*response)->params, "Location");
	if(temp_node)
	{
		// Здесь надо парсить полученный url и если надо, изменять Host, и
		// модифицировать url из аблосютного в относительный.
		// Удалять кукисы, если хост различается!?
		url_string_parse(&uri_parsed, temp_node->value.str);

		location = nullstring;

		// если путь не абсолютный, то суммируем его с директорией от предыдущего запроса
		temp_str = query_string_get(uri_parsed, "%scheme%");
		if(isempty(temp_str))
		{
			alloc_strcat(&location, "http");
		}
		else
		{
			alloc_strcat(&location, temp_str);
		}

		alloc_strcat(&location, "://");

		temp_str = query_string_get(uri_parsed, "%host%");
		if(isempty(temp_str))
		{
			alloc_strcat(&location, prev_host); // здесь порт теряестя от предыдущего запроса
		}
		else
		{
			alloc_strcat(&location, temp_str);

			temp_str = query_string_get(uri_parsed, "%port%");
			if(!isempty(temp_str))
			{
				alloc_strcat(&location, ":");
				alloc_strcat(&location, temp_str);
			}
		}


		temp_str = query_string_get(uri_parsed, "%path%");
		if(isempty(temp_str))
		{
			alloc_strcat(&location, prev_path);
		}
		else if(*temp_str != '/')
		{
			alloc_strcat(&location, prev_path);
			alloc_strcat(&location, temp_str);
		}
		else
		{
			free_str(prev_path);
			prev_path = strippath(alloc_string(temp_str), FPI_DIR);
			alloc_strcat(&location, temp_str);
		}

		temp_str = query_string_get(uri_parsed, "%query%");
		if(!isempty(temp_str))
		{
			alloc_strcat(&location, "?");
			alloc_strcat(&location, temp_str);
		}

		query_string_free(&uri_parsed);

		ZTRACE("URI: %s -> %s\n", temp_node->value.str, location);
		free_str(temp_node->value.str);
		temp_node->value.str = location; 
	}

	if(size <= pos)
	{
		goto lb_finalize;
	}

	(*response)->data = (char *) zalloc(size - pos +1);

	if(!(*response)->data)
	{
		goto lb_finalize;
	}

	memcpy((*response)->data, bbuf+pos, size - pos);
	(*response)->datasize = size - pos;
	(*response)->data[(*response)->datasize] = '\0';

lb_finalize:
	//free_str(location);
	free_str(prev_path);
	free_str(prev_host);
	free_str(bbuf);
	
	return exit_code;
}

// Host header parameter automaticaly added from uri
// Как быть с заголовками Connection и Proxy-Connection?
// Добавлять их здесь или в основной программе?
// Мне кажется здесь надо добавлять заголовки
// Content-Type и Content-Length для POST запроса!?
int getpage_follow_ex(http_data *request, http_data **response, CBCF fpProc)
{
	char *header;
	char temp_buf[32];
	char *temp_str;
	char *prev_host;
	char *prev_path;
	char *location;
	char *name;
	char *value;
	int exit_code;
	unsigned long pos;
	char *bbuf;
	unsigned long size;
	unsigned long hsize;
	int loop;
	unsigned long port;
	LIST_VARIABLE_NODE *temp_node;
	QUERY_STRING_NODE *uri_parsed;

	if(response) *response = NULL;

	uri_parsed = NULL;
	loop = 5;	// allow five redirections
	prev_host = nullstring;
	prev_path = nullstring;
	location = alloc_string(request->uri);

lb_follow_me:

	url_string_parse(&uri_parsed, location);

	header = alloc_string((request->flags & HTTP_POST)?"POST ":"GET ");

	// 5.1.2 Request-URI

	// HTTP/1.0 RFC 1945.
	// Request-URI    = absoluteURI | abs_path
	// The absoluteURI form is only allowed when the request is being made to a proxy.

	// HTTP/1.1 RFC 2616. 
	// Request-URI    = "*" | absoluteURI | abs_path | authority
	// The absoluteURI form is REQUIRED when the request is being made to a proxy.

	// если путь не абсолютный, то суммируем его с директорией от предыдущего запроса
	if(isempty(request->host))
	{
		temp_str = query_string_get(uri_parsed, "%path%");
		if(isempty(temp_str))
		{
			alloc_strcat(&header, prev_path);
		}
		else if(*temp_str != '/')
		{
			alloc_strcat(&header, prev_path);
			alloc_strcat(&header, temp_str);
		}
		else
		{
			free_str(prev_path);
			prev_path = strippath(alloc_string(temp_str), FPI_DIR);
			alloc_strcat(&header, temp_str);
		}

		temp_str = query_string_get(uri_parsed, "%query%");
		if(!isempty(temp_str))
		{
			alloc_strcat(&header, "?");
			alloc_strcat(&header, temp_str);
		}
	}
	else
	{
		alloc_strcat(&header, location);	// full path
	}

	alloc_strcat(&header, (request->flags & HTTP_11)?" HTTP/1.1":" HTTP/1.0");
	alloc_strcat(&header, "\r\n");

	// если хост не определён, то берём значение от предыдущего запроса
	alloc_strcat(&header, "Host: ");

	temp_str = query_string_get(uri_parsed, "%host%");
	if(!isempty(temp_str))
	{
		free_str(prev_host);
		prev_host = alloc_string(temp_str);
		alloc_strcat(&header, temp_str);

		temp_str = query_string_get(uri_parsed, "%port%");
		if(!isempty(temp_str))
		{
			alloc_strcat(&header, ":");
			alloc_strcat(&header, temp_str);
			port = strtoul(temp_str, NULL, 10);
		}
		else
		{
			port = 80;
		}
	}
	else if(!isempty(prev_host))
	{
		alloc_strcat(&header, prev_host);
	}
	else
	{
		free_str(header);
		query_string_free(&uri_parsed);
		exit_code = -1;
		goto lb_finalize;
	}

	alloc_strcat(&header, "\r\n");

	query_string_free(&uri_parsed);

	// собираем заголовок
	temp_node = request->params;
	while(temp_node)
	{
		alloc_strcat(&header, temp_node->key);
		alloc_strcat(&header, ": ");
		if(temp_node->flags & VL_STRING)
		{
			alloc_strcat(&header, temp_node->value.str);
		}
		else if(temp_node->flags & VL_NUMB)
		{
			itoa(temp_node->value.numb, temp_buf, 10);
			alloc_strcat(&header, temp_buf);
		}
		alloc_strcat(&header, "\r\n");

		temp_node = temp_node->next_node;
	}

	alloc_strcat(&header, "\r\n");

	hsize = strlen(header);

	if(request->data && (request->datasize > 0))
	{
		alloc_strncat(&header, request->data, request->datasize);
		hsize += request->datasize;
	}

	ZTRACE("Header:\n%s\n", header);

	exit_code = getpage(isempty(request->host)?prev_host:request->host, isempty(request->host)?port:request->port, header, hsize, &bbuf, &size, fpProc);

	if(bbuf && size)
	{
		ZTRACE("Response:\n%s\n", bbuf);
	}

	free_str(header);

	if(!response || (exit_code != 0) || (size == 0))
	{
		// если результат ответа не нужен
		if(size == 0)
		{
			exit_code = -1;
		}

		goto lb_finalize;
	}

	*response = (http_data *) zalloc(sizeof(http_data));

	if(!*response)
	{
		exit_code = -1;
		goto lb_finalize;
	}

	memset(*response, 0, sizeof(http_data));

	pos = 0;

	(*response)->protocol = alloc_getstringbefore(bbuf, size, &pos, " ");
	//(*response)->errorcode = alloc_getstringbefore(bbuf, size, &pos, " ");
	temp_str = alloc_getstringbefore(bbuf, size, &pos, " ");
	(*response)->error_code = strtoul(temp_str, NULL, 10);
	free_str(temp_str);
	(*response)->message = alloc_getstringbefore(bbuf, size, &pos, "\r\n");

	while(true)
	{
		temp_str = alloc_getstringbefore(bbuf, size, &pos, "\r\n");
		if(!isalloc(temp_str))
		{
			break;
		}

		//name = alloc_trim(alloc_getpa(lpstr, 0, ":"));
		//value = alloc_trim(alloc_getpa(lpstr, 1, ":"));
		unsigned long cur;
		
		cur = 0;

		name = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, ":"));
		value = trim(alloc_getstringbefore(temp_str, strlen(temp_str), &cur, "\r\n"));
		//value = alloc_getstringbefore(temp_str, strlen(temp_str), &cur, "\r\n");
		//ZTRACE("trim: %s -> ", value);
		//value = trim(value);
		//ZTRACE("%s\n", value);

		variables_put_string(&(*response)->params, name, "%s", value);

		free_str(name);
		free_str(value);

		free_str(temp_str);
	}

	temp_node = variables_get_by_name((*response)->params, "Location");
	if(loop && temp_node)
	{
		// HTTP/1.0 RFC 1945. 10.11 Location
		// Only one absolute URL is allowed.

		// HTTP/1.1 RFC 2616. 14.30 Location
		// The field value consists of a single absolute URI.

		// Location       = "Location" ":" absoluteURI

		loop--;
		// Здесь надо парсить полученный url и если надо, изменять Host, и
		// модифицировать url из аблосютного в относительный.
		// Удалять кукисы, если хост различается!?
		free_str(location);
		location = alloc_string(temp_node->value.str);
		httpdata_free(*response);

		goto lb_follow_me;
	}

	if(size <= pos)
	{
		goto lb_finalize;
	}

	(*response)->data = (char *) zalloc(size - pos +1);

	if(!(*response)->data)
	{
		goto lb_finalize;
	}

	memcpy((*response)->data, bbuf+pos, size - pos);
	(*response)->datasize = size - pos;
	(*response)->data[(*response)->datasize] = '\0';

lb_finalize:
	free_str(location);
	free_str(prev_path);
	free_str(prev_host);
	free_str(bbuf);
	
	return exit_code;
}

// TODO: вынести загрузку библиотек за границу функции.
// TODO: добавить возможность ручного указания логина, домена и пароля. (можно использовать cntlm)
int getpage_ntlm_ex(http_data *request, http_data **response, CBCF fpProc)
{
	int exit_code;
	http_data *temp_resp;
	INIT_SECURITY_INTERFACE pInitSecurityInterface;
	PSecPkgInfo pkgInfo; 
	PSecurityFunctionTable pSecFn;
	HMODULE hSecDll;
	SECURITY_STATUS status; 
	TimeStamp tsDummy;
    SecBuffer buf;
    SecBuffer buf2;
    SecBufferDesc desc;
    SecBufferDesc desc2;
	unsigned long attr;
	char *ntlm_buf;
	unsigned long max_token;
	SecHandle ctx;
	CredHandle cred;
	OSVERSIONINFO osver;
	char *temp_str;
	LIST_VARIABLE_NODE *temp_node;
	LIST_VARIABLE_NODE **bind_point;
	int use_proxy;

	use_proxy = 0;

	exit_code = getpage_follow_ex(request, &temp_resp, fpProc);
	
	if(exit_code != 0 || !temp_resp)
	{
		goto lb_finalize;
	}

	if(temp_resp->error_code == 401)
	{
		temp_node = variables_get_by_name(temp_resp->params, "WWW-Authenticate");
		if(!temp_node)
		{
			goto lb_finalize;
		}
	}
	else if(temp_resp->error_code == 407)
	{
		use_proxy = 1;
		temp_node = variables_get_by_name(temp_resp->params, "Proxy-Authenticate");
		if(!temp_node || (stricmp("NTLM", temp_node->value.str) != 0))
		{
			goto lb_finalize;
		}
	}
	else
	{
		goto lb_finalize;
	}

	httpdata_free(temp_resp);

	osver.dwOSVersionInfoSize = sizeof(osver);

	GetVersionEx(&osver);

	if(osver.dwPlatformId == VER_PLATFORM_WIN32_NT && osver.dwMajorVersion == 4)
	{
	  hSecDll = LoadLibrary("security.dll");
	}
	else
	{
	  hSecDll = LoadLibrary("secur32.dll");
	}

	pSecFn = NULL;

	if(hSecDll)
	{
		pInitSecurityInterface = (INIT_SECURITY_INTERFACE) GetProcAddress(hSecDll, SECURITY_ENTRYPOINT);
		if(pInitSecurityInterface)
		{
			pSecFn = pInitSecurityInterface();
		}
	}

	if(pSecFn == NULL)
	{
		goto lb_finalize;
	}

	status = pSecFn->QuerySecurityPackageInfo("NTLM", &pkgInfo);
	if(status != SEC_E_OK)
	{
		goto lb_finalize;
	}

	max_token = pkgInfo->cbMaxToken;
	pSecFn->FreeContextBuffer(pkgInfo);

	ntlm_buf = (char *) zalloc (max_token+512);

	desc.ulVersion = SECBUFFER_VERSION;
	desc.cBuffers  = 1;
	desc.pBuffers  = &buf;
	buf.BufferType = SECBUFFER_TOKEN;
	buf.cbBuffer = max_token;
	buf.pvBuffer = ntlm_buf;

	// Для авторизации явно указанным логином и паролем вместо текущего залогиненного пользователя
	// 5 параметром передавать структуру SEC_WINNT_AUTH_IDENTITY.
	status = pSecFn->AcquireCredentialsHandle(NULL, "NTLM", SECPKG_CRED_OUTBOUND, NULL, NULL, NULL, NULL, &cred, &tsDummy);
	if(status != SEC_E_OK)
	{
		FreeLibrary(hSecDll);
		goto lb_finalize;
	}

	status = pSecFn->InitializeSecurityContext(&cred, NULL, "", ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONNECTION, 0, SECURITY_NATIVE_DREP, NULL, 0, &ctx, &desc, &attr, &tsDummy);
	if(status == SEC_I_COMPLETE_AND_CONTINUE || status == SEC_I_CONTINUE_NEEDED)
	{
		pSecFn->CompleteAuthToken(&ctx, &desc);
	}
	else if(status != SEC_E_OK)
	{
		pSecFn->FreeCredentialsHandle(&cred);
		FreeLibrary(hSecDll);
		goto lb_finalize;
	}

	temp_str = (char *) alloc_en64((unsigned char *) ntlm_buf, buf.cbBuffer);
	bind_point = variables_put_string(&request->params, use_proxy?"Proxy-Authorization":"Authorization", "NTLM %s", temp_str);
	free_str(temp_str);

	// SEND type-1
	// RECV type-2
	exit_code = getpage_follow_ex(request, &temp_resp, fpProc);

	temp_node = *bind_point;
	*bind_point = (*bind_point)->next_node;
	temp_node->next_node = NULL;
	variables_free(&temp_node);

	if(exit_code == 0 && temp_resp && (temp_resp->error_code == 401 ||temp_resp->error_code == 407))
	{
		temp_node = variables_get_by_name(temp_resp->params, use_proxy?"Proxy-Authenticate":"WWW-Authenticate");
		if(!temp_node || (_strnicmp("NTLM ", temp_node->value.str, 5) != 0))
		{
			pSecFn->DeleteSecurityContext(&ctx);
			pSecFn->FreeCredentialsHandle(&cred);
			FreeLibrary(hSecDll);
			goto lb_finalize;
		}
		temp_str = alloc_de64(temp_node->value.str, (int *) &buf2.cbBuffer);

		httpdata_free(temp_resp);

		desc2.ulVersion = SECBUFFER_VERSION;
		desc2.cBuffers  = 1;
		desc2.pBuffers  = &buf2;
		buf2.BufferType = SECBUFFER_TOKEN;
		//buf2.cbBuffer = answer_size;
		buf2.pvBuffer = temp_str;
		buf.cbBuffer = max_token;

		status = pSecFn->InitializeSecurityContext(&cred, &ctx, "", ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONNECTION, 0, SECURITY_NATIVE_DREP, &desc2, 0, &ctx, &desc, &attr, &tsDummy);
		free_str(temp_str);
		if(status != SEC_E_OK)
		{
			pSecFn->DeleteSecurityContext(&ctx);
			pSecFn->FreeCredentialsHandle(&cred);
			FreeLibrary(hSecDll);
			goto lb_finalize;
		}

		temp_str = (char *) alloc_en64((unsigned char *) ntlm_buf, buf.cbBuffer);
		bind_point = variables_put_string(&request->params, use_proxy?"Proxy-Authorization":"Authorization", "NTLM %s", temp_str);
		free_str(temp_str);

		free_str(ntlm_buf);

		// SEND type-3
		// RECV 200 OK
		exit_code = getpage_follow_ex(request, &temp_resp, fpProc);
	}

	pSecFn->DeleteSecurityContext(&ctx);
	pSecFn->FreeCredentialsHandle(&cred);
	FreeLibrary(hSecDll);

lb_finalize:
	if(response)
	{
		*response = temp_resp;
	}
	else
	{
		httpdata_free(temp_resp);
	}

	return exit_code;
}

