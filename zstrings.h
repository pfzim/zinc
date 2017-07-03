#ifndef _ZSTRINGS_H_
#define _ZSTRINGS_H_

//#include "stdafx.h"
#include <stdlib.h>
#include "utstrings.h"
//#include "zdbg.h"

#define ZSTRING_DEFAULT_SIZE		256

class mystring
{
	char *data;
	unsigned long allocated_size;
	unsigned long length;

public:
	mystring()
	{
		length = 0;
		data = (char *) zalloc(ZSTRING_DEFAULT_SIZE);
		if(data)
		{
			data[0] = '\0';
			allocated_size = ZSTRING_DEFAULT_SIZE;
		}
		else
		{
			data = NULL;
			allocated_size = 0;
		}
	}

	mystring(char *text)
	{
		length = strlen(text);
		allocated_size = (length > 0)?(length + 1):ZSTRING_DEFAULT_SIZE;
		data = (char*) zalloc(allocated_size);
		if(data)
		{
			strcpy(data, text);
		}
		else
		{
			data = NULL;
			allocated_size = 0;
			length = 0;
		}
	}

	mystring(unsigned long size)
	{
		allocated_size = size;
		length = 0;
		if(allocated_size > 0)
		{
			data = (char*) zalloc(allocated_size);
			if(data)
			{
				data[0] = '\0';
			}
			else
			{
				data = NULL;
				allocated_size = 0;
			}
		}
		else
		{
			data = NULL;
		}
	}

	~mystring()
	{
		//printf("destroy...\r\n");
		free();
	}

	void free()
	{
		if(allocated_size > 0) //isalloc(data)
		{
			zfree(data);
		}
		
		allocated_size = 0;
		length = 0;
		data = NULL;
	}

	char *detach()
	{
		char *temp_str;
		
		if(allocated_size > 0)
		{
			temp_str = data;

			allocated_size = 0;
			length = 0;
			data = NULL;

			return temp_str;
		}

		return nullstring;
	}

	unsigned long lenght() const
	{
		return length;
	}

	unsigned long buffersize() const
	{
		return allocated_size;
	}

	char *c_str() const
	{
		return (length > 0)?data:nullstring;
	}

	char *buffer() const
	{
		return data;
	}

	mystring& operator = (char chr)
	{
		if(allocated_size >= 2 || resize(2))
		{
			data[0] = chr;
			data[1] = '\0';
			length = 1;
		}

		return *this;
	}

	mystring& operator = (char *text)
	{
		unsigned long sl;
		sl = strlen(text);
		if((allocated_size > sl) || resize(sl + 1))
		{
			strcpy(data, text);
			length = sl;
		}

		return *this;
	}

	mystring& operator = (long value)
	{
		if((allocated_size > 34) || resize(34))
		{
			ltoa(value, data, 10);
			length = strlen(data);
		}

		return *this;
	}

	mystring& operator = (unsigned long value)
	{
		if((allocated_size > 64) || resize(64))
		{
			ultoa(value, data, 10);
			length = strlen(data);
		}

		return *this;
	}

	mystring& append(char *text)
	{
		unsigned long sl;
		sl = length + strlen(text);
		if((allocated_size > sl) || resize(sl + 1))
		{
			strcat(data, text);
			length = sl;
		}

		return *this;
	}

	mystring& append(long value)
	{
		if((allocated_size > length + 64) || resize(length + 64))
		{
			ltoa(value, data + length, 34);
			length += strlen(data + length);
		}

		return *this;
	}

	mystring& append(unsigned long value)
	{
		if((allocated_size > length + 64) || resize(length + 64))
		{
			//ZTRACE("\\u%u\n", value);
			ultoa(value, data + length, 10);
			length += strlen(data + length);
		}

		return *this;
	}

	mystring& append(char ch)
	{
		if((allocated_size > (length + 1)) || resize(allocated_size + 1))
		{
			data[length] = ch;
			length++;
			data[length] = '\0';
		}

		return *this;
	}

	mystring& operator += (char *text)
	{
		return append(text);
	}

	mystring& operator += (long value)
	{
		return append(value);
	}

	mystring& operator += (unsigned long value)
	{
		return append(value);
	}

	mystring& operator += (char ch)
	{
		return append(ch);
	}

	char operator[] (unsigned long i) const
	{
		if(i <= length)
		{
			return data[i];
		}

		return 0;
	}

	int resize(unsigned long size)
	{
		void *temp_str;

		if((size > allocated_size) && (size - allocated_size) < ZSTRING_DEFAULT_SIZE)
		{
			size = allocated_size + ZSTRING_DEFAULT_SIZE;
		}

		if(allocated_size > 0)
		{
			temp_str = zrealloc(data, size);
			if(temp_str)
			{
				data = (char *) temp_str;
				allocated_size = size;
				return -1;
			}
			else
			{
				temp_str = zalloc(size);
				if(temp_str)
				{
					if(length > 0)
					{
						memcpy(temp_str, data, length);
					}
					zfree(data);
					data = (char *) temp_str;
					allocated_size = size;
					return -1;
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			data = (char *) zalloc(size);
			if(!data)
			{
				return 0;
			}
			allocated_size = size;
			return -1;
		}
	}

	unsigned long recalclen()
	{
		if(allocated_size > 0) // isalloc(data))
		{
			length = strlen(data);
		}
		else
		{
			length = 0;
			allocated_size = 0;
		}

		return length;
	}

    void set_data(char *data, unsigned long data_length, unsigned long data_size)
    {
		free();

   		length = data_length;
   		allocated_size = data_size;
   		data = data;
    }

	void set_data(char *data)
    {
		free();

    	length = strlen(data);
    	allocated_size = length + 1;
    	data = data;
    }

	void clear()
	{
		length = 0;
	}

	mystring& append_rtf(char *text)
	{
		static char hex_table[] = "0123456789abcdef";
		char *ch;

		ch = text;
		while(*ch)
		{
			if((*ch == '\r') && (*(ch+1) == '\n'))
			{
				append("\\line ");
				ch++;
			}
			else if((*ch == '\r') || (*ch == '\n'))
			{
				append("\\line ");
			}
			else if(*ch == '\t')
			{
				append("\\tab ");
			}
			else if((*ch == '\\') || (*ch == '{') || (*ch == '}'))
			{
				append('\\');
				append(*ch);
			}
			else if(((unsigned char) *ch) < 128)
			{
				//ZTRACE("ch%d\n", (unsigned char) *ch);
				append(*ch);
			}
			else
			{
				//ZTRACE("\\u%d\n", (unsigned char) *ch);
				//append("\\u");
				//append((unsigned long) ((unsigned char) (*ch)));
				//append('?');
				append("\\'");
				append(hex_table[((unsigned char) *ch) >> 4]);
				append(hex_table[((unsigned char) *ch) & 0x0F]);
			}

			ch++;
		}

		return *this;
	}
};

/*
int _tmain(int argc, _TCHAR* argv[])
{
	mystring ms;
	ms = "hello";
	ms = 'h';
	ms += " world!";
	printf("--APP: [%d] %s\r\n", ms.lenght(), ms.c_str());
	ms = "new lo";
	for(int i = 0; i < 10; i++)
	{
		ms += 'o';
	}
	ms += "ng world!";
	printf("--APP: [%d] %s\r\n", ms.lenght(), ms.c_str());
	ms += " hack";
	return 0;
}
/**/

#endif //_ZSTRINGS_H_
