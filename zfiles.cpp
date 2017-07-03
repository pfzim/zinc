//#include "stdafx.h"								//pf_ziminski  [2003]
#include "zfiles.h"
#include "zalloc.h"
#include "utstrings.h"
#include "snprintf.h"
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#include "zlist.h"
#include "zdbg.h"

// move to zfiles: unchecked
int mkrdir(char *dir)
{
	char *temp_str;
	char *ch;
	char *ch2;

	if(!dir || !*dir)
	{
		return -1;
	}

	//printf("\n%s\n", dir);

	temp_str = (char *) zalloc(strlen(dir)+1);
	if(!temp_str)
	{
		return -1;
	}

	ch = dir;
	ch2 = temp_str;

	// dos: skip drive letter
	if(isalpha(*ch))
	{
		*ch2 = *ch;
		ch++;
		ch2++;

		if(*ch == ':')
		{
			*ch2 = *ch;
			ch++;
			ch2++;
		}
	}

	// skip first slash
	while(*(ch) == '\\' || *(ch) == '/')
	{
		*ch2 = *ch;
		ch++;
		ch2++;
	}

	if(*ch)
	{
		while(1)
		{
			if(*ch == '\\' || *ch == '/' || !*ch)
			{
				*ch2 = '\0';
				if(_access(temp_str, 0))
				{
					//printf("mkdir %s\n", temp_str);
					if(_mkdir(temp_str))
					{
						//printf("error!\n");
						zfree(temp_str);
						return -1;
					}
				}

				// skip double slashes
				while(*ch == '\\' || *ch == '/')
				{
					*ch2 = *ch;
					ch++;
					ch2++;
				}

				if(!*ch)
				{
					break;
				}
			}

			*ch2 = *ch;
			ch++;
			ch2++;
		}
	}

	zfree(temp_str);
	return 0;
}

// make path to file: c:/some/new/dir/new.file.dst
int mkfdir(char *dir)
{
	char *temp_str;
	char *ch;
	char *ch2;

	if(!dir || !*dir)
	{
		return -1;
	}

	//printf("\n%s\n", dir);

	temp_str = (char *) zalloc(strlen(dir)+1);
	if(!temp_str)
	{
		return -1;
	}

	ch = dir;
	ch2 = temp_str;

	// dos: skip drive letter
	if(isalpha(*ch))
	{
		*ch2 = *ch;
		ch++;
		ch2++;

		if(*ch == ':')
		{
			*ch2 = *ch;
			ch++;
			ch2++;
		}
	}

	// skip first slash
	while(*(ch) == '\\' || *(ch) == '/')
	{
		*ch2 = *ch;
		ch++;
		ch2++;
	}

	while(*ch)
	{
		if(*ch == '\\' || *ch == '/')
		{
			*ch2 = '\0';
			if(_access(temp_str, 0))
			{
				//printf("mkdir %s\n", temp_str);
				if(_mkdir(temp_str))
				{
					//printf("error!\n");
					zfree(temp_str);
					return -1;
				}
			}

			// skip double slashes
			while(*ch == '\\' || *ch == '/')
			{
				*ch2 = *ch;
				ch++;
				ch2++;
			}

			if(!*ch)
			{
				break;
			}
		}

		*ch2 = *ch;
		ch++;
		ch2++;
	}

	zfree(temp_str);
	return 0;
}

// temporary function - remove and make inline
inline unsigned long _myread(int fp, void *buffer, unsigned long size)
{
	register void *b;
	register int rcount;
	register unsigned long total;

	b = buffer;
	total = size;

	while(!_eof(fp) && (total > 0))
	{
		rcount = (total > 32767)?32767:total;

		rcount = _read(fp, b, rcount);
		if(rcount <= 0)
		{
			break;
		}

		total -= rcount;
		b = (void *) ((unsigned long) b + rcount);
	}

	return size - total;
}

// temporary function - remove and make inline
inline unsigned long _mywrite(int fp, void *buffer, unsigned long size)
{
	register void *b;
	register int wcount;
	register unsigned long total;

	b = buffer;
	total = size;

	while(total > 0)
	{
		wcount = (total > 32767)?32767:total;

		wcount = _write(fp, b, wcount);
		if(wcount <= 0)
		{
			break;
		}

		total -= wcount;
		b = (void *) ((unsigned long) b + wcount);
	}

	return size - total;
}

// move to zfiles: unchecked
// prevent copy to it self:
// strip double slashes and compare strings without case sensitive
int fcopy(char *src, char *dst, int flags)
{
	//FILE *sf;
	//FILE *df;
	int sf;
	int df;
	unsigned long rcount;
	unsigned long wcount;
	unsigned long buf_size;
	void *temp_buf;

	if(!src || !dst || !*src || !*dst)
	{
		return -1;
	}

	if(!stricmp(killdoubleslashes(src), killdoubleslashes(dst)))
	{
		// copy to it self
		return 0;
	}

	if(!flags && !_access(dst, 0))
	{
		return -1;
	}

	if(_access(src, 0))
	{
		return -1;
	}

	//sf = fopen(src, "r");
	sf = _open(src, _O_RDONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
	if(sf == -1)
	{
		return -1;
	}

	//df = fopen(dst, "w");
	df = _open(dst, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
	if(df == -1)
	{
		//fclose(df);
		_close(df);
		return -1;
	}

	buf_size = _filelength(sf);
	if(buf_size > 31457280)
	{
		buf_size = 31457280;
	}

lb_alloc:	
	temp_buf = zalloc(buf_size);
	if(!temp_buf)
	{
		ZTRACE("Buffer size %d allocation error!\r\n", buf_size);
		if(buf_size > 1048576)
		{
			buf_size -= 1048576;
			goto lb_alloc;
		}
	
		_close(sf);
		_close(df);
		return -1;
	}

	while(!_eof(sf))
	{
		// возможно в функции _read ограничено количество считываемых байт до 32767,
		// поэтому нужно сделать считывание в цикле до полного заполнения буфера, 
		// а потом запись в цикле по 32767
		rcount = _myread(sf, temp_buf, buf_size);
		if(rcount != buf_size)
		{
			_close(sf);
			_close(df);
			zfree(temp_buf);
			return -1;
		}

		wcount = _mywrite(df, temp_buf, rcount);
		if(wcount != rcount)
		{
			_close(sf);
			_close(df);
			zfree(temp_buf);
			return -1;
		}
	}

	//fclose(sf);
	//fclose(df);
	_close(sf);
	_close(df);
	zfree(temp_buf);

	return 0;
}

int clear_dir(char *path)
{
	struct _finddata_t fd;
	long hFind;
	char *temp_str;
	size_t i;

	//db_list_node *stack;

	//stack = NULL;
	temp_str = alloc_string(path);
	//i = strlen(path);
	//if(i && ((path[i-1] == '\\') || (path[i-1] == '/')))
	{
		alloc_strcat(&temp_str, "*");
	}
	//else
	{
	//	alloc_strcat(&temp_str, "\\*");
	}

lb_scan:
	ZTRACE("clear directory: %s\n", temp_str);
	hFind = _findfirst(temp_str, &fd);
	
	free_str(temp_str);

	if(hFind != -1L)
	{
		do
		{
			temp_str = alloc_string(path);
			alloc_strcat(&temp_str, fd.name);

			//ZTRACE("delete found: %s\n", temp_str);

			if(fd.attrib & _A_RDONLY)
			{
				_chmod(temp_str, _S_IREAD | _S_IWRITE);
			}

			if(fd.attrib & _A_SUBDIR)
			{
				if(strcmp(fd.name, ".") && strcmp(fd.name, ".."))
				{
					//stack_push(&stack, (void *) hFind);
					//alloc_strcat(&temp_str, "\\*");

					//hFind = _findfirst(temp_str, &fd);
					//goto lb_scan;
					alloc_strcat(&temp_str, "\\");
					clear_dir(temp_str);
					_rmdir(temp_str);
				}
			}
			else
			{
				remove(temp_str);
			}

			free_str(temp_str);
		}
		while(_findnext(hFind, &fd) == 0);

		_findclose(hFind);
	}

	/*
	if(stack)
	{
		hFind = stack_pop(&stack);
		goto lb_resume;
	}
	*/

	return 0;
}

int rmdir(char *path)
{
	int result;
	
	result = clear_dir(path);
	_rmdir(path);

	return result;
}

int zfile_open(zfile_node **node, char *filename, unsigned long buffer_size, unsigned long mode)
{
	return zfile_open_ex(node, filename, buffer_size, mode, 1);
}

// mode can be: _O_CREAT | _O_TRUNC - create new file or truncate exist
// w can be: 1 - write mode
int zfile_open_ex(zfile_node **node, char *filename, unsigned long buffer_size, unsigned long mode, unsigned long w)
{
	register int fp;
	__int64 fs;

	if(w)
	{
		mode |= _O_RDWR;
	}
	else
	{
		mode |= _O_RDONLY;
	}

	fp = _open(filename, mode | _O_BINARY, _S_IREAD | _S_IWRITE);
	if(fp == -1)
	{
		return -1;
	}

	fs = _filelengthi64(fp);

	if(!buffer_size)
	{
		buffer_size = (unsigned long) fs;
	}

	if(buffer_size < 512)
	{
		buffer_size = 32767;
	}
	else if(buffer_size > 10485760)
	{
		buffer_size = 10485760;
	}

	*node = (zfile_node *) zalloc(buffer_size + sizeof(zfile_node));
	if(!*node)
	{
		_close(fp);
		return -2;
	}

	(*node)->fp = fp;
	(*node)->file_pos = 0;
	(*node)->file_size = fs;
	(*node)->buffer = (char *) (((unsigned long) (*node)) + sizeof(zfile_node));
	(*node)->buffer_size = buffer_size;
	(*node)->buffer_fill = 0;

	return 0;
}

void zfile_close(zfile_node **node)
{
	if(*node)
	{
		_close((*node)->fp);
		zfree(*node);
		*node = NULL;
	}
}

// backward seek so bad! --> add
// ***BRW_FORWARD|BRW_BACKWARD|BRW_CENTERED***
unsigned long zfile_getch(zfile_node *node, __int64 pos)
{
	if((pos >= node->file_pos) && (pos - node->file_pos < node->buffer_fill))
	{
		return node->buffer[pos - node->file_pos];
	}
	else if(pos < node->file_size)
	{
		if(_lseeki64(node->fp, pos, SEEK_SET) != pos)
		{
			return 0xFFFFFFFF;
		}

		node->file_pos = pos;
		// здесь надо в цикле читать по 32767 байт
		node->buffer_fill = _myread(node->fp, node->buffer, node->buffer_size);
		if(node->buffer_fill <= 0)
		{
			node->buffer_fill = 0;
			return 0xFFFFFFFF;
		}

		return (*(node->buffer)) & 0x000000FF;
	}

	return 0xFFFFFFFF; //EOF
}

unsigned long zfile_flush(zfile_node *node)
{
	//flush buffer
	register unsigned long wcount;

	// возможно сначала надо делать 
	if(_lseeki64(node->fp, node->file_pos, SEEK_SET) == node->file_pos)
	{
		// здесь надо в цикле писать по 32767 байт
		wcount = _mywrite(node->fp, node->buffer, node->buffer_fill);
		if(wcount != node->buffer_fill)
		{
			memdel(node->buffer, node->buffer_fill, 0, wcount);
			node->file_pos += wcount;
			node->buffer_fill -= wcount;
		}
		else
		{
			node->file_pos += node->buffer_fill;
			node->buffer_fill = 0;
		}
	}

	return node->buffer_fill;
}

unsigned long zfile_push(zfile_node *node, char ch)
{
	if(node->buffer_fill >= node->buffer_size)
	{
		zfile_flush(node);

		if(node->buffer_fill >= node->buffer_size)
		{
			return 0xFFFFFFFF;
		}
	}

	node->buffer[node->buffer_fill++] = ch;

	return ch & 0xFF;
}

int zfile_append(zfile_node *node, char *buf, unsigned long size)
{
	unsigned long temp_pos = 0, tmp;
	do
	{
		if(node->buffer_fill >= node->buffer_size)
		{
			zfile_flush(node);

			if(node->buffer_fill >= node->buffer_size)
			{
				return -1;
			}
		}
		tmp = (node->buffer_size-node->buffer_fill<size-temp_pos)?node->buffer_size-node->buffer_fill:size-temp_pos;
		memcpy(&node->buffer[node->buffer_fill], &buf[temp_pos], tmp);
		temp_pos += tmp;
		node->buffer_fill += tmp;
	}
	while(temp_pos < size);

	return 0;
}

int zfile_print(zfile_node *node, char *str)
{
	if(!str)
	{
		return -1;
	}

	return zfile_append(node, str, strlen(str));
}

int zfile_printf(zfile_node *node, char *str, ...)
{
	register int exit_code;
	register char *out;
	register va_list list;

	va_start(list, str);
	vasprintf(&out, str, list);
	va_end(list);

	exit_code = zfile_append(node, out, strlen(out));
	zfree(out);

	return exit_code;
}

// |01234|5678|901234|
//     |****|
//     |********|
//   |**|
//        |**|
//         |*****|
//               |**|
int zfile_read(zfile_node *node, char *out, __int64 offset, unsigned long size)
{
	//buffer size must be (size + 1) для null terminator [removed!]
	unsigned long rcount=0;

	if(offset < node->file_pos)												//1 ok
	{
		_lseeki64(node->fp, offset, SEEK_SET);
		rcount = (unsigned long) ((offset + size <= node->file_pos)?(size):(node->file_pos - offset));
		if(_myread(node->fp, out, rcount) != rcount)
		{
			return 2;
		}

		if(offset + size > node->file_pos)									//1.2 ok
		{
			rcount = (unsigned long) ((offset + size > node->file_pos + node->buffer_fill)?(node->buffer_fill):(offset + size - node->file_pos));
			memcpy(&out[node->file_pos - offset], node->buffer, rcount);
		}
	}

	if(offset + size > node->file_pos + node->buffer_fill)					//2
	{
		_lseeki64(node->fp, (node->file_pos + node->buffer_fill < offset)?(offset):(node->file_pos + node->buffer_fill), SEEK_SET);
		rcount = (unsigned long) ((node->file_pos + node->buffer_fill < offset)?(size):(offset + size - node->file_pos - node->buffer_fill));
		if(_myread(node->fp, &out[(node->file_pos + node->buffer_fill < offset)?(0):(node->file_pos + node->buffer_fill - offset)], rcount) != rcount)
		{
			return 2;
		}
	}

	if((offset >= node->file_pos) && (offset < node->file_pos + node->buffer_fill))	//3
	{
		rcount = (unsigned long) ((offset + size > node->file_pos + node->buffer_fill)?(node->file_pos + node->buffer_fill - offset):(size));
		memcpy(out, &node->buffer[offset - node->file_pos], rcount);
	}

	//out[size] = '\0'; [removed!]
	return 0;
}

// return allocated buffer with null terminator (buffer length = size + 1)
char *alloc_zfile_read(zfile_node *node, __int64 offset, unsigned long size)
{
	char *out;

	out = (char *) zalloc(size +1);
	if(out)
	{
		zfile_read(node, out, offset, size);
		out[size] = '\0';

		return out;
	}

	return nullstring;
}

// unsupported large files >= 2GB (look func _chsize)
int zfile_truncatetop(zfile_node *node, unsigned long offset)
{
	void *temp_buf;
	__int64 j;
	unsigned long buf_size;
	unsigned long rcount;
	int error_code;

	buf_size = (node->file_size - offset > 31457280) ? 31457280 : (unsigned long) (node->file_size - offset);

lb_alloc:	
	temp_buf = zalloc(buf_size);
	if(!temp_buf)
	{
		ZTRACE("Buffer size %d allocation error!\r\n", buf_size);
		if(buf_size > 1048576)
		{
			buf_size -= 1048576;
			goto lb_alloc;
		}
	
		return -1;
	}

	error_code = 0;

	j = offset;

	while(j < node->file_size)
	{
		if(_lseeki64(node->fp, j, SEEK_SET) == -1L)
		{
			break;
		}

		rcount = _myread(node->fp, temp_buf, buf_size);
		if(rcount == 0)
		{
			break;
		}

		if(_lseeki64(node->fp, j - offset, SEEK_SET) == -1L)
		{
			error_code = -2;
			goto lb_finalize;
		}

		if(_mywrite(node->fp, temp_buf, rcount) != rcount)
		{
			error_code = -3;
			goto lb_finalize;
		}

		j += rcount;
	}

	if(_chsize(node->fp, (unsigned long) (node->file_size - offset)) == -1)
	{
		error_code = -4;
	}

lb_finalize:
	zfree(temp_buf);

	_lseeki64(node->fp, 0, SEEK_END);

	return 0;
}
