#include "zquery.h"															// pf_ziminski (c) 2009
#include <string.h>
#include "utstrings.h"
#include "zqparser.h"
#include "zdbg.h"

#if 0
void demo()
{
	lv_column_add(&lv_info, 1, 0, 99, $("ID"), NULL);
	lv_column_add(&lv_info, 2, 0, 99, $("User name"), NULL);
	lv_column_add(&lv_info, 3, 0, 99, $("Departament"), NULL);

	sql_query_column_add(&sql_query, 1, 0, "id", NULL, NULL);
	sql_query_column_add(&sql_query, 2, 0, "uid->dc_users.id:name", NULL, NULL);
	sql_query_column_add(&sql_query, 3, 0, "sid->dc_orgs.id:name", NULL, NULL);

	sql_query_column_add(&sql_query, 4, 0, "sid", NULL, NULL);

	sql_query_reset_conds(sql_query);

	temp_node = list_walk(sql_query, cb_sql_query_by_key, "sid");
	sql_query_cond_add(temp_node, SG_NUMBER | SG_AND, (char *) 1234);
	sql_query_cond_add(temp_node, SG_NUMBER | SG_AND, (char *) 1234);

	SelectDialog(sql_query, lv_info, &temp_node);
}

// other ****************************************************************************************************************

unsigned long sql_column_add(db_lv_data *db_tbl, unsigned long flags, char *db_col, char *db_cond, /*char *db_join_table, char *db_join_cond1, char *db_join_cond2,*/ unsigned long cx, char *xml_name, char *hu_name)
{
	if(db_tbl->cols_count < 256)
	{
		db_tbl->cols[db_tbl->cols_count] = (db_column_node *) zalloc(sizeof(db_column_node));
		if(db_tbl->cols[db_tbl->cols_count])
		{
			memset(db_tbl->cols[db_tbl->cols_count], 0, sizeof(db_column_node));
			db_tbl->cols[db_tbl->cols_count]->flags = flags;
			db_tbl->cols[db_tbl->cols_count]->db_col = alloc_string(db_col);
			if(flags & SG_NUMBER)
			{
				db_tbl->cols[db_tbl->cols_count]->db_cond = alloc_string_ex("%d", db_cond);
			}
			else
			{
				db_tbl->cols[db_tbl->cols_count]->db_cond = alloc_string(db_cond);
			}
			//db_tbl->cols[db_tbl->cols_count]->db_join_table = alloc_string(db_join_table);
			//db_tbl->cols[db_tbl->cols_count]->db_join_cond1 = alloc_string(db_join_cond1);
			//db_tbl->cols[db_tbl->cols_count]->db_join_cond2 = alloc_string(db_join_cond2);
			db_tbl->cols[db_tbl->cols_count]->cx = cx;
			db_tbl->cols[db_tbl->cols_count]->xml_name = alloc_string(xml_name);
			db_tbl->cols[db_tbl->cols_count]->hu_name = alloc_string(hu_name);
			db_tbl->cols_count++;

			return db_tbl->cols_count;
		}
	}

	return 0;
}

void sql_filter_reset(db_lv_data *db_tbl)
{
	unsigned long j;
	unsigned long k;

	j = 0;
	k = 0;
	while(k < db_tbl->cols_count)
	{
		db_tbl->cols[k] = db_tbl->cols[j];

		if(!isempty(db_tbl->cols[j]->db_cond))
		{
			free_str(db_tbl->cols[j]->db_cond);
		}

		if(isempty(db_tbl->cols[j]->hu_name))
		{
			free_str(db_tbl->cols[j]->db_col);
			//free_str(db_tbl->cols[j]->db_join_cond1);
			//free_str(db_tbl->cols[j]->db_join_cond2);
			//free_str(db_tbl->cols[j]->db_join_table);
			free_str(db_tbl->cols[j]->hu_name);
			zfree(db_tbl->cols[j]);
			db_tbl->cols_count--;
		}
		else
		{
			k++;
		}

		j++;
	}
}

char *sql_query_from_table(unsigned long flags, db_lv_data *db_tbl, char *table_name, unsigned long *data_offsets, unsigned long *cols_offsets)
{
	unsigned long j;
	unsigned long i;
	unsigned long offset;
	char *temp_str;
	char *temp_slct;
	char *temp_from;
	char *temp_whre;
	char *temp_ordr;
	char *temp_str2;

	j = 0;
	offset = 0;
	temp_str = nullstring;
	temp_slct = alloc_string("SELECT ");
	temp_from = alloc_string(" FROM ");
	temp_whre = nullstring;
	temp_ordr = nullstring;
	alloc_strcat(&temp_from, table_name);
	alloc_strcat(&temp_from, " AS m");

	for(i = 0; i < db_tbl->cols_count; i++)
	{
		data_offsets[i] = offset;
		cols_offsets[offset] = i;

		my_query_parse(db_tbl->cols[i]->db_col, &temp_str, &temp_str2, &j);

		alloc_strcat(&temp_from, temp_str2);
		free_str(temp_str2);

		// add to select
		if((db_tbl->cols[i]->flags & (CL_VISIBLE | CL_KEY))|| (flags && !isempty(db_tbl->cols[i]->xml_name)))
		{
			if(offset)
			{
				alloc_strcat(&temp_slct, ", ");
			}
			alloc_strcat(&temp_slct, temp_str);

			offset++;
		}
		j++;

		// add to order
		if(db_tbl->cols[i]->flags & CL_ORDER)
		{
			if(!isempty(temp_ordr))
			{
				alloc_strcat(&temp_ordr, ", ");
			}

			alloc_strcat(&temp_ordr, temp_str);

			if(db_tbl->cols[i]->flags & CL_DESC)
			{
				alloc_strcat(&temp_ordr, " DESC");
			}
		}

		// add to where
		if(!isempty(db_tbl->cols[i]->db_cond))
		{
			if(!isempty(temp_whre))
			{
				if(db_tbl->cols[i]->flags & SG_AND)
				{
					alloc_strcat(&temp_whre, " AND ");
				}
				else if(db_tbl->cols[i]->flags & SG_OR)
				{
					alloc_strcat(&temp_whre, " OR ");
				}
			}

			if(db_tbl->cols[i]->flags & SG_GROUP_OPEN)
			{
				alloc_strcat(&temp_whre, " (");
			}

			alloc_strcat(&temp_whre, temp_str);

			if(db_tbl->cols[i]->flags & (SG_LIKE | SG_BEGINS | SG_CONTAINE | SG_ENDS))
			{
				alloc_strcat(&temp_whre, " LIKE ");
			}
			else if(db_tbl->cols[i]->flags & SG_LSS)
			{
				alloc_strcat(&temp_whre, "<");
			}
			else if(db_tbl->cols[i]->flags & SG_GTR)
			{
				alloc_strcat(&temp_whre, ">");
			}
			else if(db_tbl->cols[i]->flags & SG_LEQ)
			{
				alloc_strcat(&temp_whre, "<=");
			}
			else if(db_tbl->cols[i]->flags & SG_GEQ)
			{
				alloc_strcat(&temp_whre, ">=");
			}
			else if(db_tbl->cols[i]->flags & SG_NEQ)
			{
				alloc_strcat(&temp_whre, "<>");
			}
			else if(db_tbl->cols[i]->flags & SG_IS)
			{
				alloc_strcat(&temp_whre, " IS ");
			}
			else
			{
				alloc_strcat(&temp_whre, "=");
			}

			alloc_strcat(&temp_whre, db_tbl->cols[i]->db_cond);

			if(db_tbl->cols[i]->flags & SG_GROUP_CLOSE)
			{
				alloc_strcat(&temp_whre, ") ");
			}
		}

		free_str(temp_str);
	}

	alloc_strcat(&temp_slct, temp_from);
	free_str(temp_from);

	if(!isempty(temp_whre))
	{
		alloc_strcat(&temp_slct, " WHERE ");
		alloc_strcat(&temp_slct, temp_whre);
	}
	free_str(temp_whre);

	if(!isempty(temp_ordr))
	{
		alloc_strcat(&temp_slct, " ORDER BY ");
		alloc_strcat(&temp_slct, temp_ordr);
	}
	free_str(temp_ordr);

	return temp_slct;
}

int cb_lv_build_cols(void *data1, void *data2)
{
	LVCOLUMN lvcol;

	lvcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvcol.cx = ((lv_column_node *) data1)->cx;
	if(lvcol.cx == 0)
	{
		lvcol.cx = 100;
	}

	lvcol.pszText = ((lv_column_node *) data1)->hu_name;

	if(((lv_column_node *) data1)->flags & CL_THOUSANDS)
	{
		lvcol.fmt = LVCFMT_RIGHT;
	}
	else
	{
		lvcol.fmt = LVCFMT_LEFT;
	}

	ListView_InsertColumn((HWND) data2, 0, &lvcol);

	return 0;
}

#endif	// **************************************************************************************************************

int cb_lv_by_ud(void *data1, void *data2)
{
	return ((lv_column_node *) data1)->user_data == data2;
}

int cb_lv_by_id(void *data1, void *data2)
{
	return ((lv_column_node *) data1)->id == (unsigned long) data2;
}

void cb_free_lv(void *data, void *data2)
{
	free_str(((lv_column_node *) data)->hu_name);
	if(data2)
	{
		((void (_cdecl *)(void *))data2)(((lv_column_node *) data)->user_data);
	}
	zfree((lv_column_node *) data);
}

int lv_free(db_list_node **query, void (*cb_free_user_data)(void *))
{
	return list_free(query, cb_free_lv, cb_free_user_data);
}

db_list_node *lv_column_add(db_list_node **db_node, unsigned char id, unsigned long flags, unsigned long cx, char *hu_name, void *user_data)
{
	lv_column_node *temp_node;

	temp_node = (lv_column_node *) zalloc(sizeof(lv_column_node));
	if(temp_node)
	{
		memset(temp_node, 0, sizeof(lv_column_node));

		temp_node->id = id;
		temp_node->flags = flags;
		temp_node->cx = cx;
		temp_node->hu_name = alloc_string(hu_name);
		temp_node->user_data = user_data;

		return list_add(db_node, temp_node);
	}

	return NULL;
}

/*
int lv_build(db_list_node *db_node, HWND hwnd, unsigned long *order_ids, unsigned long count)
{
	LVCOLUMN lvcol;
	unsigned long i;
	void *data1;

	for(i = 0; i < count; i++)
	{
		data1 = list_extract(list_walk(db_node, cb_lv_by_id, pv(order_ids[i])));
		if(data1)
		{
			lvcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
			lvcol.cx = ((lv_column_node *) data1)->cx;
			if(lvcol.cx == 0)
			{
				lvcol.cx = 100;
			}

			lvcol.pszText = ((lv_column_node *) data1)->hu_name;

			if(((lv_column_node *) data1)->flags & CL_THOUSANDS)
			{
				lvcol.fmt = LVCFMT_RIGHT;
			}
			else
			{
				lvcol.fmt = LVCFMT_LEFT;
			}

			ListView_InsertColumn(hwnd, i, &lvcol);
		}
	}

	return 0;
}

int lv_build(db_list_node *db_node, HWND hwnd)
{
	LVCOLUMN lvcol;
	unsigned long i;
	unsigned long j;
	void *data1;

	j = 1;
	i = j;
	while(data1 = list_extract(list_walk(db_node, cb_by_count, pv(&i))))
	{
		lvcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvcol.cx = ((lv_column_node *) data1)->cx;
		if(lvcol.cx == 0)
		{
			lvcol.cx = 100;
		}

		lvcol.pszText = ((lv_column_node *) data1)->hu_name;

		if(((lv_column_node *) data1)->flags & CL_THOUSANDS)
		{
			lvcol.fmt = LVCFMT_RIGHT;
		}
		else
		{
			lvcol.fmt = LVCFMT_LEFT;
		}

		ListView_InsertColumn(hwnd, j-1, &lvcol);

		j++;
		i = j;
	}

	return 0;
}


int lv_read_order(HWND hwnd, unsigned long *cols_cx, unsigned long *order_ids, unsigned long count)
{
	unsigned long i;
	int *cols_order;
	unsigned long *new_order_ids;
	unsigned long lv_cols_show;

	// сохряняем текущий порядок и размер колонок
	lv_cols_show = Header_GetItemCount(ListView_GetHeader(hwnd));
	new_order_ids = (unsigned long *) zalloc(sizeof(unsigned long) * count);
	if(new_order_ids)
	{
		cols_order = (int *) zalloc(sizeof(int) * lv_cols_show);
		if(cols_order)
		{
			ListView_GetColumnOrderArray(hwnd, lv_cols_show, cols_order);

			if(lv_cols_show > count)
			{
				lv_cols_show = count;
			}

			for(i = 0; i < lv_cols_show; i++)
			{
				new_order_ids[i] = order_ids[cols_order[i]];

				//db_table->cols[db_table->lv_cols_order[i]]->cx = ListView_GetColumnWidth(hw, i);
			}

			memcpy(order_ids, new_order_ids, sizeof(unsigned long) * count);

			zfree(cols_order);
		}

		zfree(new_order_ids);
	}

	return 0;
}

*/
// ************************************************************************************************

int cb_sql_query_by_key(void *data1, void *data2)
{
	return !strcmp(((sql_query_node *) data1)->db_col, (char *) data2);
}

int cb_sql_query_by_ud(void *data1, void *data2)
{
	return ((sql_query_node *) data1)->user_data == data2;
}

int cb_sql_query_by_id(void *data1, void *data2)
{
	return ((sql_query_node *) data1)->id == (unsigned long) data2;
}

void cb_free_sql_values(void *data)
{
	free_str(((sql_value_node *) data)->value);
	zfree((sql_query_node *) data);
}

void cb_free_sql_query(void *data, void *data2)
{
	free_str(((sql_query_node *) data)->db_col);
	list_free(&((sql_query_node *) data)->db_cond, cb_free_sql_values);
	if(data2)
	{
		((void (_cdecl *)(void *))data2)(((sql_query_node *) data)->user_data);
	}
	zfree((sql_query_node *) data);
}

int sql_query_free(db_list_node **db_node, void (*cb_free_user_data)(void *))
{
	return list_free(db_node, cb_free_sql_query, cb_free_user_data);
}

db_list_node *sql_query_column_add(db_list_node **db_node, unsigned char id, unsigned long flags, char *db_col, db_list_node *db_cond, void *user_data)
{
	sql_query_node *temp_node;

	temp_node = (sql_query_node *) zalloc(sizeof(sql_query_node));
	if(temp_node)
	{
		memset(temp_node, 0, sizeof(sql_query_node));

		temp_node->id = id;
		temp_node->flags = flags;
		temp_node->db_col = alloc_string(db_col);
		temp_node->db_cond = db_cond;
		temp_node->user_data = user_data;

		return list_add(db_node, temp_node);
	}

	return NULL;
}

char *sql_query_value(unsigned long flags, char *value)
{
	char *temp_str;
	char *temp_str2;

	switch(flags & SG_TYPE_MASK)
	{
		case SG_STRING:
			temp_str2 = alloc_string(value);
			//alloc_addslashes(&temp_str);
			alloc_dbescape(&temp_str2);
			if(flags & SG_PASSWORD)
			{
				temp_str = alloc_string("PASSWORD('");
			}
			else
			{
				temp_str = alloc_string("'");
			}
			if(flags & SG_ENDS)
			{
				alloc_strcat(&temp_str, "%");
			}
			alloc_strcat(&temp_str, temp_str2);
			if(flags & SG_BEGINS)
			{
				alloc_strcat(&temp_str, "%");
			}
			if(flags & SG_PASSWORD)
			{
				alloc_strcat(&temp_str, "')");
			}
			else
			{
				alloc_strcat(&temp_str, "'");
			}
			free_str(temp_str2);
			break;
		case SG_RAWVAL:
			temp_str = alloc_string(value);
			break;
		case SG_NUMBER:
			temp_str = alloc_string_ex("%d", value);
			break;
		case SG_DEFAULT:
			temp_str = alloc_string("DEFAULT");
			break;
		default:
			temp_str = alloc_string("NULL");
	}

	return temp_str;
}

db_list_node *sql_query_cond_add(db_list_node *col_node, unsigned long flags, char *value)
{
	sql_value_node *temp_node;

	if(col_node)
	{
		temp_node = (sql_value_node *) zalloc(sizeof(sql_value_node));
		if(temp_node)
		{
			memset(temp_node, 0, sizeof(sql_value_node));

			temp_node->flags = flags;
			temp_node->value = sql_query_value(flags, value);

			return list_add(&((sql_query_node *) col_node->data)->db_cond, temp_node);
		}
	}

	return NULL;
}

db_list_node *sql_query_cond_add(db_list_node *db_node, unsigned char id, unsigned long flags, char *value)
{
	db_list_node *temp_node;

	temp_node = list_walk(db_node, cb_sql_query_by_id, (void *) id);
	if(!temp_node)
	{
		return NULL;
	}

	return sql_query_cond_add(temp_node, flags, value);
}

int cb_query_build_cond(void *data1, void *data2)
{
	// add to where
	if(!isempty(((sql_query_parts *) data2)->where))
	{
		if(((sql_value_node *) data1)->flags & SG_OR)
		{
			alloc_strcat(&((sql_query_parts *) data2)->where, " OR ");
		}
		else
		{
			alloc_strcat(&((sql_query_parts *) data2)->where, " AND ");
		}
	}

	if(((sql_value_node *) data1)->flags & SG_GROUP_OPEN)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, " (");
	}

	alloc_strcat(&((sql_query_parts *) data2)->where, ((sql_query_parts *) data2)->temp_name);

	if(((sql_value_node *) data1)->flags & (SG_LIKE | SG_BEGINS | SG_CONTAINE | SG_ENDS))
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, " LIKE ");
	}
	else if(((sql_value_node *) data1)->flags & SG_LSS)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, "<");
	}
	else if(((sql_value_node *) data1)->flags & SG_GTR)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, ">");
	}
	else if(((sql_value_node *) data1)->flags & SG_LEQ)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, "<=");
	}
	else if(((sql_value_node *) data1)->flags & SG_GEQ)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, ">=");
	}
	else if(((sql_value_node *) data1)->flags & SG_NEQ)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, "<>");
	}
	else if(((sql_value_node *) data1)->flags & SG_IS)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, " IS ");
	}
	else
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, "=");
	}

	alloc_strcat(&((sql_query_parts *) data2)->where, ((sql_value_node *) data1)->value);

	if(((sql_value_node *) data1)->flags & SG_GROUP_CLOSE)
	{
		alloc_strcat(&((sql_query_parts *) data2)->where, ") ");
	}

	return 0;
}

int cb_query_build(void *data1, void *data2)
{
	char *temp_name;
	char *temp_join;
	char *temp_str;
	int used;

	used = 0;

	if((((sql_query_parts *) data2)->count_names < ((sql_query_parts *) data2)->offsets_count) && ((sql_query_parts *) data2)->offsets)
	{
		((sql_query_parts *) data2)->offsets[((sql_query_parts *) data2)->count_names] = ((sql_query_node *) data1)->id;
	}

	if(((sql_query_parts *) data2)->flags & SG_SELECT)
	{
		my_query_parse(((sql_query_node *)data1)->db_col, &temp_name, &temp_join, &((sql_query_parts *) data2)->count_joins);
	}
	else
	{
		temp_name = alloc_string("`");
		alloc_strcat(&temp_name, ((sql_query_node *)data1)->db_col);
		alloc_strcat(&temp_name, "`");
	}

	if(((sql_query_parts *) data2)->flags & SG_SELECT)
	{
		// add to SELECT
		if(((sql_query_node *) data1)->flags & (SG_SELECT | SG_VISIBLE | SG_KEY))
		{
			if(((sql_query_parts *) data2)->count_names)
			{
				alloc_strcat(&((sql_query_parts *) data2)->names, ", ");
			}
			alloc_strcat(&((sql_query_parts *) data2)->names, temp_name);

			((sql_query_parts *) data2)->count_names++;
			used++;
		}
		((sql_query_parts *) data2)->count_joins++;
	}
	else if(((sql_query_parts *) data2)->flags & SG_INSERT)
	{
		// add to INSERT
		if(	(((sql_query_node *) data1)->flags & SG_INSERT)
			&& (((sql_query_parts *) data2)->cb_print_data(list_extract(list_walk(((sql_query_parts *) data2)->values_list, ((sql_query_parts *) data2)->cb_find_data, (void *) ((sql_query_node *) data1)->id)), &temp_str)))
		{
			if(((sql_query_parts *) data2)->count_names)
			{
				alloc_strcat(&((sql_query_parts *) data2)->names, ", ");
				alloc_strcat(&((sql_query_parts *) data2)->values, ", ");
			}
			alloc_strcat(&((sql_query_parts *) data2)->names, temp_name);

			alloc_strcat(&((sql_query_parts *) data2)->values, temp_str);
			free_str(temp_str);

			((sql_query_parts *) data2)->count_names++;
		}
	}
	else if(((sql_query_parts *) data2)->flags & SG_UPDATE)
	{
		// add to UPDATE
		if(	(((sql_query_node *) data1)->flags & SG_UPDATE)
			&& (((sql_query_parts *) data2)->cb_print_data(list_extract(list_walk(((sql_query_parts *) data2)->values_list, ((sql_query_parts *) data2)->cb_find_data, (void *) ((sql_query_node *) data1)->id)), &temp_str)))
		{
			if(((sql_query_parts *) data2)->count_names)
			{
				alloc_strcat(&((sql_query_parts *) data2)->names, ", ");
			}

			alloc_strcat(&((sql_query_parts *) data2)->names, temp_name);

			alloc_strcat(&((sql_query_parts *) data2)->names, "=");

			alloc_strcat(&((sql_query_parts *) data2)->names, temp_str);
			free_str(temp_str);

			((sql_query_parts *) data2)->count_names++;
		}
	}

	if(((sql_query_parts *) data2)->flags & SG_SELECT)
	{
		// add to GROUP BY
		if(((sql_query_node *) data1)->flags & SG_GROUP)
		{
			if(!isempty(((sql_query_parts *) data2)->group))
			{
				alloc_strcat(&((sql_query_parts *) data2)->group, ", ");
			}

			alloc_strcat(&((sql_query_parts *) data2)->group, temp_name);

			used++;
		}
	}

	if(((sql_query_parts *) data2)->flags & (SG_SELECT | SG_UPDATE | SG_DELETE))
	{
		// add to ORDER BY
		if(((sql_query_node *) data1)->flags & SG_ORDER)
		{
			if(!isempty(((sql_query_parts *) data2)->order))
			{
				alloc_strcat(&((sql_query_parts *) data2)->order, ", ");
			}

			alloc_strcat(&((sql_query_parts *) data2)->order, temp_name);

			if(((sql_query_node *) data1)->flags & SG_DESC)
			{
				alloc_strcat(&((sql_query_parts *) data2)->order, " DESC");
			}

			used++;
		}

		((sql_query_parts *) data2)->temp_name = temp_name;

		if(((sql_query_node *) data1)->db_cond)
		{
			list_walk(((sql_query_node *) data1)->db_cond, cb_query_build_cond, data2);
			used++;
		}
	}

	if(((sql_query_parts *) data2)->flags & SG_SELECT)
	{
		if(used)
		{
			alloc_strcat(&((sql_query_parts *) data2)->from, temp_join);
		}

		free_str(temp_join);
	}

	free_str(temp_name);

	return 0;
}

char *sql_query_build2(unsigned long flags, db_list_node *db_node, char *table_name, unsigned char *data_ids, unsigned char count)
{
	char *temp_str;
	sql_query_parts sqp;

	memset(&sqp, 0, sizeof(sql_query_parts));

	sqp.flags = flags;
	sqp.offsets = data_ids;
	sqp.offsets_count = count;
	sqp.from = nullstring;
	sqp.where = nullstring;
	sqp.order = nullstring;
	sqp.group = nullstring;
	sqp.names = nullstring;
	sqp.values = nullstring;
	sqp.values_list = NULL;
	sqp.cb_find_data = NULL;
	sqp.cb_print_data = NULL;

	alloc_strcat(&sqp.from, table_name);
	alloc_strcat(&sqp.from, " AS m");

	list_walk(db_node, cb_query_build, &sqp);

	if((sqp.count_names < sqp.offsets_count) && (sqp.offsets))
	{
		sqp.offsets[sqp.count_names] = 0;
	}

	temp_str = alloc_string("SELECT ");
	alloc_strcat(&temp_str, sqp.names);

	free_str(sqp.names);

	alloc_strcat(&temp_str, " FROM ");
	alloc_strcat(&temp_str, sqp.from);

	free_str(sqp.from);

	if(!isempty(sqp.where))
	{
		alloc_strcat(&temp_str, " WHERE ");
		alloc_strcat(&temp_str, sqp.where);
	}

	free_str(sqp.where);

	if(!isempty(sqp.order))
	{
		alloc_strcat(&temp_str, " ORDER BY ");
		alloc_strcat(&temp_str, sqp.order);
	}

	free_str(sqp.order);

	if(!isempty(sqp.group))
	{
		alloc_strcat(&temp_str, " GROUP BY ");
		alloc_strcat(&temp_str, sqp.group);
	}

	free_str(sqp.group);

	return temp_str;
}

char *sql_query_build3(unsigned long flags, db_list_node *db_node, char *table_name, unsigned char *data_ids, unsigned char count, db_list_node *values_list, int (*cb_print_data)(void *, char **), int (*cb_find_data)(void *, void *), char *additional)
{
	char *temp_str;
	sql_query_parts sqp;

	memset(&sqp, 0, sizeof(sql_query_parts));

	sqp.flags = flags;
	sqp.offsets = data_ids;
	sqp.offsets_count = count;
	sqp.from = nullstring;
	sqp.where = nullstring;
	sqp.order = nullstring;
	sqp.group = nullstring;
	sqp.names = nullstring;
	sqp.values = nullstring;
	sqp.values_list = values_list;
	sqp.cb_find_data = cb_find_data;
	sqp.cb_print_data = cb_print_data;

	temp_str = nullstring;

	alloc_strcat(&sqp.from, table_name);

	if(flags & SG_SELECT)
	{
		alloc_strcat(&sqp.from, " AS m");
	}

	list_walk(db_node, cb_query_build, &sqp);

	if((sqp.count_names < sqp.offsets_count) && (sqp.offsets))
	{
		sqp.offsets[sqp.count_names] = 0;
	}

	if(flags & SG_DELETE)
	{
		temp_str = alloc_string("DELETE FROM ");

		alloc_strcat(&temp_str, sqp.from);

		if(!isempty(sqp.where))
		{
			alloc_strcat(&temp_str, " WHERE ");
			alloc_strcat(&temp_str, sqp.where);
		}

		if(!isempty(additional))
		{
			alloc_strcat(&temp_str, " ");
			alloc_strcat(&temp_str, additional);
		}
	}	
	else if(sqp.count_names)
	{
		if(flags & SG_SELECT)
		{
			temp_str = alloc_string("SELECT ");

			alloc_strcat(&temp_str, sqp.names);

			alloc_strcat(&temp_str, " FROM ");
			alloc_strcat(&temp_str, sqp.from);

			if(!isempty(sqp.where))
			{
				alloc_strcat(&temp_str, " WHERE ");
				alloc_strcat(&temp_str, sqp.where);
			}

			if(!isempty(sqp.order))
			{
				alloc_strcat(&temp_str, " ORDER BY ");
				alloc_strcat(&temp_str, sqp.order);
			}

			if(!isempty(sqp.group))
			{
				alloc_strcat(&temp_str, " GROUP BY ");
				alloc_strcat(&temp_str, sqp.group);
			}

			if(!isempty(additional))
			{
				alloc_strcat(&temp_str, " ");
				alloc_strcat(&temp_str, additional);
			}
		}
		else if(flags & SG_INSERT)
		{

			temp_str = alloc_string("INSERT INTO ");

			alloc_strcat(&temp_str, sqp.from);

			alloc_strcat(&temp_str, " (");
			alloc_strcat(&temp_str, sqp.names);
			alloc_strcat(&temp_str, ") VALUES (");
			alloc_strcat(&temp_str, sqp.values);
			alloc_strcat(&temp_str, ")");
		}	
		else if(flags & SG_UPDATE)
		{

			temp_str = alloc_string("UPDATE ");

			alloc_strcat(&temp_str, sqp.from);

			alloc_strcat(&temp_str, " SET ");
			alloc_strcat(&temp_str, sqp.names);

			if(!isempty(sqp.where))
			{
				alloc_strcat(&temp_str, " WHERE ");
				alloc_strcat(&temp_str, sqp.where);
			}

			if(!isempty(additional))
			{
				alloc_strcat(&temp_str, " ");
				alloc_strcat(&temp_str, additional);
			}
		}	
	}

	free_str(sqp.names);
	free_str(sqp.values);
	free_str(sqp.from);
	free_str(sqp.where);
	free_str(sqp.order);
	free_str(sqp.group);

	return temp_str;
}

int cb_query_reset_cond(void *data1, void *data2)
{
	list_free(&((sql_query_node *) data1)->db_cond, cb_free_sql_values);

	return 0;
}

db_list_node *sql_query_reset_conds(db_list_node *db_node)
{
	return list_walk(db_node, cb_query_reset_cond, NULL);
}

