#ifndef _ZODBC_H_
#define _ZODBC_H_

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

typedef struct _odbc_row
{
	unsigned long *lengths;
	unsigned long *offsets;
	char *data;
} odbc_row;

typedef struct _odbc_result
{
	short num_cols;
	unsigned long num_rows;
	struct _odbc_row **rows;
} odbc_result;

class zodbc
{
	protected:
		SQLHENV hEnv;
		SQLHDBC hDBC;
		SQLHSTMT hStmt;
		char *last_error_message;

		virtual int get_error(SQLSMALLINT HandleType, SQLHANDLE Handle);

	public:
		zodbc();
		~zodbc();
		virtual int connect(char *conn_str);
		virtual int query(char *query);
		virtual int store_result(odbc_result **res);
		virtual unsigned long affected_rows();
		virtual unsigned long last_id();
		virtual int disconnect();
		virtual int set_error(char *msg);
		virtual char *error_msg();
};

int free_result(odbc_result *res);

#endif //_ZODBC_H_
