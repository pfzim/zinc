#include "zmysql.h"															// pf_ziminski (c) 2008
#include "utstrings.h"

//#pragma comment (lib, "libmysql.lib")

zmysql::zmysql()
{
	hSQL = NULL;
	last_error_message = nullstring;

	if(mysql_thread_init())
	{
		set_error("cannot init mysql thread");
	}
}

zmysql::~zmysql()
{
	if(hSQL)
	{
		disconnect();
	}
	mysql_thread_end();
	free_str(last_error_message);
}

int zmysql::connect(char *db_host, char *db_user, char *db_pass, char *db_name)
{
	//int ret_code;
	my_bool b;

	hSQL = mysql_init(hSQL);
	if(!hSQL)
	{
		set_error("cannot init mysql");
		return -1;
	}

	//hSQL = mysql_real_connect(hSQL, db_host, db_user, db_pass, NULL, MYSQL_PORT, NULL, 0);
	if(!mysql_real_connect(hSQL, db_host, db_user, db_pass, NULL, MYSQL_PORT, NULL, 0))
	{
		set_error((char *) mysql_error(hSQL));
		return -1;
	}

	b = 1;
	mysql_options(hSQL, MYSQL_OPT_RECONNECT, (const char*) &b);

	if(mysql_select_db(hSQL, db_name) != 0)
	{
		set_error((char *) mysql_error(hSQL));
		disconnect();
		return -1;
	}

	return 0;
}

int zmysql::query(char *query)
{
	int ret_code;

	if(!hSQL)
	{
		set_error("You are not connected!");
		return -1;
	}

	ret_code = mysql_query(hSQL, query);

	if(ret_code)
	{
		set_error((char *) mysql_error(hSQL));
		return -1;
	}

	return 0;
}

int zmysql::store_result(odbc_result **res)
{
	MYSQL_RES *hRes;
	MYSQL_ROW row;
	unsigned long *lengths;
	unsigned long i;
	short j;
	unsigned long temp_val;
	unsigned long max_rows;

	*res = NULL;

	if(mysql_field_count(hSQL) > 0)
	{
		hRes = mysql_store_result(hSQL);
	}
	else if(mysql_errno(hSQL) != 0)
	{
		set_error((char *) mysql_error(hSQL));
		return -1;
	}
	else
	{
		return 1;
	}

	max_rows = 128;
	*res = (odbc_result *) zalloc(sizeof(odbc_result) + max_rows*sizeof(odbc_row *));
	if(!*res)
	{
		set_error("no memory");
		return -1;
	}
	(*res)->num_rows = 0;
	(*res)->rows = (odbc_row **) ((unsigned long) (*res) + sizeof(odbc_result));

	(*res)->num_cols = mysql_field_count(hSQL);

	i = 0;

	while(row = mysql_fetch_row(hRes))
	{
		(*res)->rows[i] = (odbc_row *) zalloc(sizeof(odbc_row) + ((*res)->num_cols*sizeof(unsigned long)*2));
		if(!(*res)->rows[i])
		{
			set_error("no memory");
			return -1;
		}

		(*res)->rows[i]->lengths = (unsigned long *) ((unsigned long) (*res)->rows[i] + sizeof(odbc_row));
		(*res)->rows[i]->offsets = (unsigned long *) ((unsigned long) (*res)->rows[i] + sizeof(odbc_row) + (*res)->num_cols*sizeof(unsigned long));
		(*res)->rows[i]->data = NULL;

		lengths = mysql_fetch_lengths(hRes);
		temp_val = 0;
		for(j = 0; j < (*res)->num_cols; j++)
		{
			(*res)->rows[i]->lengths[j] = lengths[j];
			(*res)->rows[i]->offsets[j] = temp_val;
			temp_val += (*res)->rows[i]->lengths[j] + 1;
		}

		(*res)->rows[i]->data = (char *) zalloc(temp_val);

		if(!(*res)->rows[i]->data)
		{
			set_error("no memory");
			return -1;
		}

		temp_val = (unsigned long) (*res)->rows[i]->data;
		for(j = 0; j < (*res)->num_cols; j++)
		{
			if(row[j])
			{
				memcpy((void *) temp_val, row[j], (*res)->rows[i]->lengths[j] + 1);
			}
			else
			{
				*((char *)temp_val) = '\0';
			}
			temp_val += (*res)->rows[i]->lengths[j] + 1;
		}
		i++;
		(*res)->num_rows++;

		if(i >= max_rows)
		{
			max_rows += 128;
			*res = (odbc_result *) zrealloc(*res, sizeof(odbc_result) + max_rows*sizeof(odbc_row *));
			if(!*res)
			{
				set_error("no memory");
				return -1;
			}

			(*res)->rows = (odbc_row **) ((unsigned long) (*res) + sizeof(odbc_result));
		}
	}

	mysql_free_result(hRes);

	return 0;
}

unsigned long zmysql::affected_rows()
{
	return (unsigned long) mysql_affected_rows(hSQL);
}

unsigned long zmysql::last_id()
{
	return (unsigned long) mysql_insert_id(hSQL);
}

int zmysql::set_error(char *msg)
{
	free_str(last_error_message);
	last_error_message = alloc_string(msg);

	return 0;
}

char *zmysql::error_msg()
{
	return last_error_message;
}

int zmysql::disconnect()
{
	mysql_close(hSQL);
	hSQL = NULL;

	return 0;
}

