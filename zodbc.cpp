#include "zodbc.h"															// pf_ziminski (c) 2008
#include "utstrings.h"
#include "zdbg.h"

/*
#define ODBC_CONNSTR			"DRIVER={SQL Server};SERVER=application\\sql1c;UID=sa;PWD=celestoderm;DATABASE=directum2;"

char **res;
zodbc db;
ret_code = db.connect(ODBC_CONNSTR);
ret_code = db.query("SELECT UserName FROM dbo.MBUser");
while(!db.fetch_row(&res))
{
	MessageBox(NULL, res[0], "test", MB_OK);
	ret_code = db.free_row(res);
}
ret_code = db.disconnect();
*/

/*
query() - запрос, получение всех данных?
если SELECT - возвращает массив данных,
если UPDATE - возвращает 0 если успешно,
если ќЎ»Ѕ ј - возвращает -1.
num_cols()
num_rows()
fetch_row()
free_res() - очистка данных
*/

zodbc::zodbc()
{
	SQLRETURN ret_code;

	last_error_message = nullstring;
	hEnv = SQL_NULL_HENV;
	hDBC = SQL_NULL_HDBC;
	hStmt = SQL_NULL_HSTMT;

	ret_code = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if((ret_code == SQL_SUCCESS) || (ret_code == SQL_SUCCESS_WITH_INFO))
	{
		ret_code = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		if((ret_code == SQL_SUCCESS) || (ret_code == SQL_SUCCESS_WITH_INFO))
		{
			ret_code = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);
		}

		if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
		{
			get_error(SQL_HANDLE_ENV, hEnv);
			SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
			hEnv = SQL_NULL_HENV;
		}
		else
		{
			SQLSetConnectAttr(hDBC, SQL_LOGIN_TIMEOUT, (void *) 10, 0);
		}
	}
}

zodbc::~zodbc()
{
	if(hDBC != SQL_NULL_HDBC) SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
	if(hEnv != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	free_str(last_error_message);
}

int zodbc::get_error(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
	SQLRETURN ret_code;
	SQLCHAR state[6];
	SQLCHAR msg[1024];
	SQLINTEGER native;
	SQLSMALLINT msg_len;

	ret_code = SQLGetDiagRec(HandleType, Handle, 1, state, &native, msg, 1024, &msg_len);
	
	free_str(last_error_message);
	last_error_message = alloc_string_ex("Error %d performing: SQLState = %s; SQL message = %s", native, state, msg);

	//CharToOem(last_error_message, last_error_message); // for console debug

	return (ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO);
}

int zodbc::set_error(char *msg)
{
	free_str(last_error_message);
	last_error_message = alloc_string(msg);

	return 0;
}

char *zodbc::error_msg()
{
	return last_error_message;
}

int zodbc::connect(char *conn_str)
{
	SQLRETURN ret_code;
	SQLSMALLINT conn_len;
	unsigned char real_conn[1024];

	ret_code = SQLDriverConnect(hDBC, NULL, (unsigned char *) conn_str, strlen(conn_str), real_conn, sizeof(real_conn), &conn_len, SQL_DRIVER_NOPROMPT);
	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_DBC, hDBC);
		return -1;
	}

	/*
	if(hStmt != SQL_NULL_HSTMT)
	{
		//SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = SQL_NULL_HSTMT;
	}
	*/

	// SQLAllocHandle(SQL_HANDLE_STMT) надо делать каждый раз перед SQLExecDirect()
	// или достаточно сделать один раз после SQLDriverConnect()
	ret_code = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);
	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_DBC, hDBC);
		SQLDisconnect(hDBC);
		hStmt = SQL_NULL_HSTMT;
		return -1;
	}

	return 0;
}

int zodbc::query(char *query)
{
	SQLRETURN ret_code;

	ret_code = SQLExecDirect(hStmt, (unsigned char *) query, strlen(query));

	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_STMT, hStmt);
		return -1;
	}

	return 0;
}

int zodbc::store_result(odbc_result **res)
{
	SQLRETURN ret_code;
	SQLINTEGER buf_size;
	SQLINTEGER real_size;
	unsigned long i;
	short j;
	unsigned long temp_val;
	unsigned long max_rows;

	*res = NULL;

	if(hStmt == SQL_NULL_HSTMT)
	{
		set_error("make query before");
		return -1;
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

	ret_code = SQLNumResultCols(hStmt, &(*res)->num_cols);
	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_STMT, hStmt);
		return -1;
	}

	i = 0;

	while(SQLFetch(hStmt) == SQL_SUCCESS)
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

		temp_val = 0;
		for(j = 0; j < (*res)->num_cols; j++)
		{
			ret_code = SQLGetData(hStmt, j+1, SQL_C_CHAR, &real_size, 0, &buf_size);

			if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
			{
				(*res)->rows[i]->lengths[j] = 0;
				get_error(SQL_HANDLE_STMT, hStmt);
				return -1;
			}

			// почитать повнимательнее в MSDN про SQL_NO_TOTAL
			if((buf_size == SQL_NULL_DATA) || (buf_size == SQL_NO_TOTAL))
			{
				buf_size = 0;
			}

			(*res)->rows[i]->lengths[j] = buf_size;
			(*res)->rows[i]->offsets[j] = temp_val;
			temp_val += buf_size + 1;
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
			// мне кажетс€ здесь можно оптимизировать
			// использу€ (*res)->rows[i]->offsets[j] вместо temp_val
			ret_code = SQLGetData(hStmt, j+1, SQL_C_CHAR, (void *) temp_val, (*res)->rows[i]->lengths[j] + 1, &real_size);

			if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
			{
				//ZTRACE("SQLError: j = %d\n", j);
				get_error(SQL_HANDLE_STMT, hStmt);
				return -1;
			}
			//temp_val += (*res)->rows[i]->lengths[j] + 1;
			temp_val += (*res)->rows[i]->lengths[j];
			*((char *) temp_val) = 0;
			temp_val++;
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
	
	ret_code = SQLCloseCursor(hStmt);
	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_STMT, hStmt);
		return -1;
	}

	//SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	//hStmt = SQL_NULL_HSTMT;

	return 0;
}

unsigned long zodbc::affected_rows()
{
	SQLRETURN ret_code;
	SQLINTEGER count;

	if(hStmt == SQL_NULL_HSTMT)
	{
		set_error("no query");
		return 0;
	}

	ret_code = SQLRowCount(hStmt, &count);
	if((ret_code != SQL_SUCCESS) && (ret_code != SQL_SUCCESS_WITH_INFO))
	{
		get_error(SQL_HANDLE_STMT, hStmt);
		return 0;
	}

	return (unsigned long) count;
}

unsigned long zodbc::last_id()
{
	unsigned long id;
	odbc_result *res;

	id = 0;

	if(query("SELECT LAST_INSERT_ID()"))
	{
		return 0;
	}

	if(!store_result(&res) && (res->num_rows == 1) && (res->num_cols == 1))
	{
		id = strtoul(res->rows[0]->data, NULL, 10);
	}

	free_result(res);

	return id;
}

int zodbc::disconnect()
{
	if(hStmt != SQL_NULL_HSTMT)
	{
		//SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = SQL_NULL_HSTMT;
	}
	if(hDBC != SQL_NULL_HDBC)
	{
		SQLDisconnect(hDBC);
		//hDBC = SQL_NULL_HDBC;
	}

	return 0;
}

// отдельные функции дл€ работы с массивом данных
int free_result(odbc_result *res)
{
	unsigned long i;

	if(res)
	{
		for(i = 0; i < res->num_rows; i++)
		{
			zfree(res->rows[i]->data);
			zfree(res->rows[i]);
		}
		zfree(res);
	}

	return 0;
}
