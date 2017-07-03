#ifndef _ZSQLITE_H_
#define _ZSQLITE_H_

#include "zodbc.h"
#include "sqlite/sqlite3.h"

#define ZSQLITE_UTF8	1

class zsqlite
{
	protected:
		sqlite3 *hSQL;
		sqlite3_stmt *hStmt;
		char *last_error_message;

	public:
		zsqlite();
		~zsqlite();
		virtual int connect(char *db_name);
		virtual int query(char *query);
		virtual int store_result(odbc_result **res);
		virtual unsigned long affected_rows();
		virtual unsigned long last_id();
		virtual int disconnect();
		virtual int set_error(char *msg);
		virtual char *error_msg();
};

#endif //_ZSQLITE_H_