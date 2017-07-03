#ifndef _ZMYSQL_H_
#define _ZMYSQL_H_

#include <winsock2.h>
#include "libmysql/mysql.h"
#include "zodbc.h"

class zmysql
{
	protected:
		MYSQL *hSQL;
		//MYSQL_RES *hRes;
		char *last_error_message;

	public:
		zmysql();
		~zmysql();
		virtual int connect(char *db_host, char *db_user, char *db_pass, char *db_name);
		virtual int query(char *query);
		virtual int store_result(odbc_result **res);
		virtual unsigned long affected_rows();
		virtual unsigned long last_id();
		virtual int disconnect();
		virtual int set_error(char *msg);
		virtual char *error_msg();
};

#endif //_ZMYSQL_H_