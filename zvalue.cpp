//#include "stdafx.h"								//pf_ziminski  [2010]
#include "zvalue.h"
#include "utstrings.h"
#include "zdbg.h"
#include <stdio.h>


int cb_vl_by_id(void *data1, void *data2)
{
	return ((value_node *) data1)->id == (unsigned long) data2;
}

inline void vl_free_data(value_node *data)
{
	if(data->flags & DF_BYNAME)
	{
		free_str(data->name);
	}

	switch(data->flags & DT_TYPEMASK)
	{
		case DT_STRING:
		case DT_RAWSTRING:
			free_str(data->str_val);
			break;
	}
}

void cb_vl_free(void *data, void *data2)
{
	vl_free_data((value_node *) data);

	zfree(data);
}

int cb_vl_dup(void *data1, void *data2)
{
	value_node *temp_node;

	temp_node = (value_node *) zalloc(sizeof(value_node));
	if(temp_node)
	{
		memcpy(temp_node, data1, sizeof(value_node));
		if(((value_node *) data1)->flags & DF_BYNAME)
		{
			temp_node->name = alloc_string(((value_node *) data1)->name);
		}

		switch(((value_node *) data1)->flags & DT_TYPEMASK)
		{
			case DT_STRING:
			case DT_RAWSTRING:
				temp_node->str_val = alloc_string(((value_node *) data1)->str_val);
				break;
		}

		list_add((db_list_node **) data2, temp_node);

		return 0;
	}

	return 1;
}

db_list_node *value_list_duplicate(db_list_node *data_list)
{
	db_list_node *dup_data;

	dup_data = NULL;

	list_walk(data_list, cb_vl_dup, &dup_data);

	return dup_data;
}

inline void value_node_set(value_node *node, unsigned long flags, unsigned long id, void *data)
{
	memset(node, 0, sizeof(value_node));

	node->flags = flags;
	node->id = id;

	if(data)
	{
		switch(flags & DT_TYPEMASK)
		{
			case DT_DATETIME:
				node->datetime_val.date = ((unsigned long *) data)[0];
				node->datetime_val.time = ((unsigned long *) data)[1];
				break;
			case DT_CURRENCY:
				node->i64_val = *(__int64 *) data;
				break;
			case DT_DOUBLE:
				node->dbl_val = *(double *) data;
				break;
			case DT_STRING:
			case DT_RAWSTRING:	// specific sql type
				node->str_val = alloc_string((char *) data);
				break;
			default:
				node->p_val = data;
		}
	}
}

inline value_node *value_node_new(unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = (value_node *) zalloc(sizeof(value_node));
	if(temp_node)
	{
		value_node_set(temp_node, flags, id, data);
	}

	return temp_node;
}

value_node *value_list_add(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = value_node_new(flags, id, data);
	if(temp_node)
	{
		list_add(data_list, temp_node);
	}

	return temp_node;
}

// overwrite existing node without reallocate
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;

	temp_node = (value_node *) list_extract(list_walk(*data_list, cb_vl_by_id, (void *) id));
	if(temp_node)
	{
		vl_free_data(temp_node);
		value_node_set(temp_node, flags, id, data);
		//ZTRACE("value_list_set: %d!\n", id);
	}
	else
	{
		temp_node = value_node_new(flags, id, data);
		if(temp_node)
		{
			list_add(data_list, temp_node);
		}
	}

	return temp_node;
}

/*
// always create (alloc) new node (not overwrite if exist)
value_node *value_list_set(db_list_node **data_list, unsigned long flags, unsigned long id, void *data)
{
	value_node *temp_node;
	db_list_node *exist_node;

	temp_node = value_node_new(flags, id, data);
	if(temp_node)
	{
		exist_node = list_walk(*data_list, cb_vl_by_id, (void *) id);
		if(exist_node)
		{
			cb_vl_free(exist_node->data, NULL);
			exist_node->data = temp_node;
		}
		else
		{
			list_add(data_list, temp_node);
		}
	}

	return temp_node;
}
*/

int cb_vl_print_sql(void *data, char **result)
{
	//char *result;

	*result = nullstring;

	if(!data || (~((value_node *) data)->flags & DF_CHANGED))
	{
		return 0;
	}

	if(((value_node *) data)->flags & DT_ULONG)
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_ultoa(((value_node *) data)->ul_val, *result, 10);
		}
	}
	else if(((value_node *) data)->flags & DT_LONG)
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_ltoa(((value_node *) data)->l_val, *result, 10);
		}
	}
	else if(((value_node *) data)->flags & DT_DATE)
	{
		if(((value_node *) data)->datetime_val.date)
		{
			*result = alloc_string_ex("'%.4d-%.2d-%.2d'", zyear(((value_node *) data)->datetime_val.date), zmonth(((value_node *) data)->datetime_val.date), zday(((value_node *) data)->datetime_val.date));
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else if(((value_node *) data)->flags & DT_DOUBLE)	// 8 byte
	{
		*result = (char *) zalloc(256);
		if(*result)
		{
			_snprintf(*result, 255, "%.4f", ((value_node *) data)->dbl_val);
		}
	}
	else if(((value_node *) data)->flags & DT_CURRENCY)	// 8 byte
	{
		*result = (char *) zalloc(34);
		if(*result)
		{
			_i64toa(((value_node *) data)->i64_val, *result, 10);
		}
	}
	else if(((value_node *) data)->flags & DT_DATETIME)	// 8 byte
	{
		if(((value_node *) data)->datetime_val.date)
		{
			*result = alloc_string_ex("'%.4d-%.2d-%.2d %.2d:%.2d:%.2d'", zyear(((value_node *) data)->datetime_val.date), zmonth(((value_node *) data)->datetime_val.date), zday(((value_node *) data)->datetime_val.date), zhour(((value_node *) data)->datetime_val.time), zminute(((value_node *) data)->datetime_val.time), zsecond(((value_node *) data)->datetime_val.time));
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else if(((value_node *) data)->flags & DT_RAWSTRING)	// строка без кавычек, для функций типа NOW()
	{
		if(((value_node *) data)->str_val)
		{
			*result = alloc_string(((value_node *) data)->str_val);
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}
	else // DT_STRING
	{
		if(((value_node *) data)->str_val)
		{
			*result = alloc_string("'");
			alloc_strcat(result, ((value_node *) data)->str_val);
			alloc_strcat(result, "'");
		}
		else
		{
			*result = alloc_string("NULL");
		}
	}

	return 1;
}
