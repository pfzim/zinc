#ifndef _ZFILES_H_
#define _ZFILES_H_

#include <fcntl.h>

typedef struct _zfile_node
{
	int fp;
	char *buffer;
	unsigned long buffer_size;
	unsigned long buffer_fill;
	__int64 file_size;
	__int64 file_pos;
} zfile_node;

int mkrdir(char *dir);
int mkfdir(char *dir);
int fcopy(char *src, char *dst, int flags); // flags = 1 - overwrite
int clear_dir(char *path);

// mode can be: _O_CREAT | _O_TRUNC - create new file or truncate exist
int zfile_open(zfile_node **node, char *filename, unsigned long buffer_size, unsigned long mode);
int zfile_open_ex(zfile_node **node, char *filename, unsigned long buffer_size, unsigned long mode, unsigned long access);
void zfile_close(zfile_node **node);
unsigned long zfile_getch(zfile_node *node, __int64 pos);
unsigned long zfile_flush(zfile_node *node);
unsigned long zfile_push(zfile_node *node, char ch);
int zfile_append(zfile_node *node, char *buf, unsigned long size);
int zfile_print(zfile_node *node, char *str);
int zfile_printf(zfile_node *node, char *str, ...);
int zfile_read(zfile_node *node, char *out, __int64 offset, unsigned long size);
char *alloc_zfile_read(zfile_node *node, __int64 offset, unsigned long size);

#endif //_ZFILES_H_
