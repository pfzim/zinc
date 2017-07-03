#include "zsqlite.h"															// pf_ziminski (c) 2008
#include "utstrings.h"
#include <time.h>
#include "zdbg.h"

#ifdef ZSQLITE_UTF8
char *decode_utf8(char *param1, unsigned long param2)
{
    int len;
	unsigned short *temp_str;
	char *dec_str;

	dec_str = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, param1, param2, NULL, 0);
	if(!len)
	{
		return nullstring;
	}

    temp_str = (unsigned short *)zalloc(len * sizeof(unsigned short));
	if(!temp_str)
	{
		return nullstring;
	}

	if(MultiByteToWideChar(CP_UTF8, 0, param1, param2, temp_str, len))
	{
		param2 = WideCharToMultiByte(CP_ACP, 0, temp_str, len, NULL, 0, NULL, NULL);

		if(param2)
		{
			dec_str = (char *)zalloc(param2);
			if(dec_str)
			{
				if(!WideCharToMultiByte(CP_ACP, 0, temp_str, len, dec_str, param2, NULL, NULL))
				{
					zfree(dec_str);
					dec_str = nullstring;
				}
			}
		}
	}

	zfree(temp_str);

    return dec_str;
}

char *encode_utf8(char *param1, unsigned long param2)
{
    int len;
	unsigned short *temp_str;
	char *dec_str;

	dec_str = NULL;

	len = MultiByteToWideChar(CP_ACP, 0, param1, param2, NULL, 0);
	if(!len)
	{
		return nullstring;
	}

    temp_str = (unsigned short *)zalloc(len * sizeof(unsigned short));
	if(!temp_str)
	{
		return nullstring;
	}

	if(MultiByteToWideChar(CP_ACP, 0, param1, param2, temp_str, len))
	{
		param2 = WideCharToMultiByte(CP_UTF8, 0, temp_str, len, NULL, 0, NULL, NULL);

		if(param2)
		{
			dec_str = (char *)zalloc(param2);
			if(dec_str)
			{
				if(!WideCharToMultiByte(CP_UTF8, 0, temp_str, len, dec_str, param2, NULL, NULL))
				{
					zfree(dec_str);
					dec_str = nullstring;
				}
			}
		}
	}

	zfree(temp_str);

    return dec_str;
}
#endif

static void cb_sql_now(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char *temp_str;
	SYSTEMTIME st;

	GetLocalTime(&st);
	temp_str = alloc_string_ex("%.4d-%.2d-%.2d %.2d:%.2d:%.2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	if(temp_str)
	{
		sqlite3_result_text(context, temp_str, -1, zfree);
	}

	//sqlite3_result_int(context, time(NULL));
}

static void cb_sql_curdate(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char *temp_str;
	SYSTEMTIME st;

	GetLocalTime(&st);
	temp_str = alloc_string_ex("%.4d-%.2d-%.2d", st.wYear, st.wMonth, st.wDay);
	if(temp_str)
	{
		sqlite3_result_text(context, temp_str, -1, zfree);
	}
}

static void cb_sql_curtime(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char *temp_str;
	SYSTEMTIME st;

	GetLocalTime(&st);
	temp_str = alloc_string_ex("%.2d:%.2d:%.2d", st.wHour, st.wMinute, st.wSecond);
	if(temp_str)
	{
		sqlite3_result_text(context, temp_str, -1, zfree);
	}
}

static void cb_sql_password(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char *temp_str;
	char *ch;

	if(argc == 1)
	{
		temp_str = (char *) sqlite3_value_text(argv[0]);
		if(temp_str)
		{
			ZTRACE("password: %s -> ", temp_str);

			ch = temp_str;
			while(*ch)
			{
				if((*ch >= 'a' && *ch <= 'm') || (*ch >= 'A' && *ch <= 'M'))
				{
					*ch += 13;
				}
				else if((*ch >= 'n' && *ch <= 'z') || (*ch >= 'N' && *ch <= 'Z'))
				{
					*ch -= 13;
				}

				ch++;
			}

			ZTRACE("%s\n", temp_str);

			sqlite3_result_text(context, temp_str, -1, SQLITE_STATIC);	// SQLITE_TRANSIENT
		}

		//sqlite3_result_value(context, argv[0]);
	}
}

zsqlite::zsqlite()
{
	hStmt = NULL;
	hSQL = NULL;
	last_error_message = nullstring;
}

zsqlite::~zsqlite()
{
	if(hSQL)
	{
		disconnect();
	}
	free_str(last_error_message);
}

int zsqlite::connect(char *db_name)
{
	if(sqlite3_open(db_name, &hSQL) != SQLITE_OK)
	{
		set_error((char *) sqlite3_errmsg(hSQL));
		disconnect();
		return -1;
	}

	sqlite3_create_function(hSQL, "NOW", 0, SQLITE_ANY, 0, &cb_sql_now, NULL, NULL);
	sqlite3_create_function(hSQL, "CURDATE", 0, SQLITE_ANY, 0, &cb_sql_curdate, NULL, NULL);
	sqlite3_create_function(hSQL, "CURTIME", 0, SQLITE_ANY, 0, &cb_sql_curtime, NULL, NULL);
	sqlite3_create_function(hSQL, "PASSWORD", 1, SQLITE_UTF8, 0, &cb_sql_password, NULL, NULL);

	return 0;
}

int zsqlite::query(char *query)
{
	if(hStmt)
	{
		sqlite3_finalize(hStmt);
	}

#ifdef ZSQLITE_UTF8
	char *temp_str;

	temp_str = encode_utf8(query, strlen(query)+1); // beta utf8
	//ZTRACE("encoded utf8: %s\n", temp_str);
	if(sqlite3_prepare_v2(hSQL, temp_str, -1, &hStmt, NULL) != SQLITE_OK)
	{
		free_str(temp_str); // beta utf8
#else
	if(sqlite3_prepare_v2(hSQL, query, -1, &hStmt, NULL) != SQLITE_OK)
	{
#endif
		set_error((char *) sqlite3_errmsg(hSQL));
		return -1;
	}

#ifdef ZSQLITE_UTF8
	free_str(temp_str); // beta utf8
#endif

	// перенести сюда первый sqlite3_step
	switch(sqlite3_step(hStmt))
	{
		case SQLITE_OK:
		case SQLITE_DONE:
			sqlite3_finalize(hStmt);
			hStmt = NULL;
		case SQLITE_ROW:
			break;
		default:
			set_error((char *) sqlite3_errmsg(hSQL));
			sqlite3_finalize(hStmt);
			hStmt = NULL;
			return -1;
	}

	return 0;
}

int zsqlite::store_result(odbc_result **res)
{
	int i;
	int j;
	unsigned long temp_val;
	int max_rows;

	*res = NULL;

	if(!hStmt)
	{
		set_error("no data");
		return 0;				// по хорошему здесь надо возвращать ноль!
	}

	max_rows = 128;
	*res = (odbc_result *) zalloc(sizeof(odbc_result) + max_rows*sizeof(odbc_row *));
	if(!*res)
	{
		set_error("sqlite error: no memory for query result\n");
		return -1;
	}

	(*res)->num_rows = 0;
	(*res)->rows = (odbc_row **) ((unsigned long) (*res) + sizeof(odbc_result));

	(*res)->num_cols = sqlite3_column_count(hStmt);
	if((*res)->num_cols)
	{
		i = 0;
		do
		{
			(*res)->rows[i] = (odbc_row *) zalloc(sizeof(odbc_row) + ((*res)->num_cols*sizeof(unsigned long)*2));
			if(!(*res)->rows[i])
			{
				set_error("sqlite error: no memory for row info\n");
				return -1;
			}

			(*res)->rows[i]->lengths = (unsigned long *) ((unsigned long) (*res)->rows[i] + sizeof(odbc_row));
			(*res)->rows[i]->offsets = (unsigned long *) ((unsigned long) (*res)->rows[i] + sizeof(odbc_row) + (*res)->num_cols*sizeof(unsigned long));
			(*res)->rows[i]->data = NULL;

			temp_val = 0;
			for(j = 0; j < (*res)->num_cols; j++)
			{
				(*res)->rows[i]->lengths[j] = sqlite3_column_bytes(hStmt, j);
				(*res)->rows[i]->offsets[j] = temp_val;
				temp_val += (*res)->rows[i]->lengths[j] + 1;
			}

			(*res)->rows[i]->data = (char *) zalloc(temp_val);
			if(!(*res)->rows[i]->data)
			{
				set_error("sqlite error: no memory for row data\n");
				return -1;
			}

			for(j = 0; j < (*res)->num_cols; j++)
			{
#ifdef ZSQLITE_UTF8
				char *temp_str;
				temp_str = decode_utf8((char *) sqlite3_column_text(hStmt, j), (*res)->rows[i]->lengths[j]+1);
				//ZTRACE("decoded utf8: %s\n", temp_str);
				strncpy_tiny((*res)->rows[i]->data + (*res)->rows[i]->offsets[j], temp_str, (*res)->rows[i]->lengths[j]);
				free_str(temp_str);
#else
				strncpy_tiny((*res)->rows[i]->data + (*res)->rows[i]->offsets[j], (char *) sqlite3_column_text(hStmt, j), (*res)->rows[i]->lengths[j]);
#endif
			}

			i++;
			(*res)->num_rows++;

			if(i >= max_rows)
			{
				max_rows += 128;
				*res = (odbc_result *) zrealloc(*res, sizeof(odbc_result) + max_rows*sizeof(odbc_row *));
				if(!*res)
				{
					set_error("sqlite error: no memory\n");
					return -1;
				}

				(*res)->rows = (odbc_row **) ((unsigned long) (*res) + sizeof(odbc_result));
			}
		} while(sqlite3_step(hStmt) == SQLITE_ROW);
	}

	sqlite3_finalize(hStmt);
	hStmt = NULL;

	return 0;
}

unsigned long zsqlite::affected_rows()
{
	return (unsigned long) 0;
}

unsigned long zsqlite::last_id()
{
	return (unsigned long) sqlite3_last_insert_rowid(hSQL);
}

int zsqlite::set_error(char *msg)
{
	free_str(last_error_message);
	last_error_message = alloc_string(msg);

	return 0;
}

char *zsqlite::error_msg()
{
	return last_error_message;
}

int zsqlite::disconnect()
{
	if(hStmt)
	{
		sqlite3_finalize(hStmt);
	}

	sqlite3_close(hSQL);
	hSQL = NULL;

	return 0;
}

