#ifndef _ZVALUE_H_
#define _ZVALUE_H_

#include "zlist.h"
//#include "zcontrols.h"
//#include "zodbc.h"

#define DT_TYPEMASK				0x0000FFFF
#define DT_STRING				0x00000001	// default
#define DT_LONG					0x00000002
#define DT_ULONG				0x00000004
#define DT_DOUBLE				0x00000008
#define DT_CURRENCY				0x00000010
#define DT_DATE					0x00000020
#define DT_DATETIME				0x00000040
#define DT_DATA					0x00000080
#define DT_RAWSTRING			0x00000100
#define DT_USERMASK				0xFF000000	// used for this->p_val to determine how to free() data
#define DT_USERTYPE1			0x01000000
#define DT_USERMASK2			0x02000000
#define DT_USERMASK3			0x03000000

//#define DT_NEWTYPE				0x00000000


typedef struct _value_node
{
	union
	{
		unsigned long id;
		char *name;
	};
	unsigned long flags;		// data type like (DT_NUMBER, DT_FLOAT)
	union
	{
		void *p_val;			// pointer to any data (how to free? by type in flags?)
		char *str_val;			// pointer to string
		unsigned long ul_val;	// unsigned long
		long l_val;				// long
		double dbl_val;			// double
		__int64 i64_val;		// int64
		struct _data_node
		{
			void *ptr;			// pointer to any data
			unsigned long size;	// size of data
		} data_val;				// pointer to any data with size
		struct _date_time_value
		{
			unsigned long date;	// date zdate()
			unsigned long time;	// time ztime()
		} datetime_val;			// datatime
		struct _64bit_value
		{
			unsigned long hi32;
			unsigned long lo32;
		} val64;				// 64bit value as two unsigned long values
	};
} value_node;


int cb_vl_by_id(void *data1, void *data2);
void cb_vl_free(void *data, void *data2);
int cb_vl_print_sql(void *data, char **result);
int cb_vl_dup(void *data1, void *data2);

db_list_node *value_list_duplicate(db_list_node *data_list);
value_node *value_list_add(db_list_node **data_list, unsigned long flags, unsigned long id, void *data);
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data);

#endif //_ZVALUE_H_
