#include "Shlwapi.h"
#include "utstrings.h"
#include "zalloc.h"
#include "zquery.h"
#include "zlist.h"
#include "zdbg.h"
#include "zdbdlg.h"

// *** callback functions ****************************************************************************************

int cb_fnd_substr(void *data1, void *data2)
{
	return StrStrI((char *) data1, (char *) data2)?0:-1;
}

int cb_cmp_str(void *data1, void *data2)
{
	return StrCmpI((char *) data1, (char *) data2);
}

int cb_cmp_num(void *data1, void *data2)
{
	return ((unsigned long) data1) - ((unsigned long) data2);
}

int cb_cmp_currency(void *data1, void *data2)
{
	__int64 res;

	res = ((*(__int64 *) data1) - (*(__int64 *) data2));
	return (int) (res == 0)?0:((res > 0)?1:-1);
}

int cb_cmp_datetime(void *data1, void *data2)
{
	if((*(unsigned __int64 *) data1) > (*(unsigned __int64 *) data2))
	{
		return 1;
	}
	else if((*(unsigned __int64 *) data1) < (*(unsigned __int64 *) data2))
	{
		return 0;
	}
	return 0;
}

// *** callback functions ****************************************************************************************

void parsed_table_free(db_parsed_table **db_data)
{
	unsigned long i;

	if(*db_data)
	{
		for(i = 0; i < (*db_data)->rows; i++)
		{
			zfree((*db_data)->data[i]);
		}

		zfree((*db_data)->data);
		zfree(*db_data);

		*db_data = NULL;
	}
}

int parsed_table_bsort(db_list_node **db_list, db_parsed_table *db_table, unsigned char id)
{
	int done;
	//unsigned long i;
	unsigned long k;
	db_list_node **temp_node;
	db_list_node *c;
	db_list_node *b;
	int (* cbfunc)(void *, void *);

	// define data offset
	k = 0;
	while(db_table->data_map[k] && (db_table->data_map[k] != id))
	{
		k++;
	}

	if(!db_table->data_map[k])
	{
		k = 0;
	}

	// detect data type
	switch(db_table->flags[k] & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc = cb_cmp_str;
			break;
		case DT_CURRENCY:
			cbfunc = cb_cmp_currency;
			break;
		case DT_DATETIME:
			cbfunc = cb_cmp_datetime;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc = cb_cmp_num;
	}

	// sorting (bubble sort)
	done = 0;
	while(!done)
	{
		done = 1;
		for(temp_node = db_list; *temp_node && (*temp_node)->next_node; temp_node = &(*temp_node)->next_node)
		{
			if(cbfunc(((char **) (*temp_node)->data)[k], ((char **) (*temp_node)->next_node->data)[k]) > 0)
			{
				// a->c->b->
				// exchange to
				// a->b->c->
				c = *temp_node;
				b = (*temp_node)->next_node;

				*temp_node = b;
				c->next_node = b->next_node;
				b->next_node = c;

				done = 0;
			}
		}
	}

	return 0;
}

int parsed_table_bsort(db_parsed_table *db_table, unsigned char id)
{
	//lv_column_node *temp_node;
	char **a;
	int done;
	unsigned long i;
	unsigned long k;
	int (* cbfunc)(void *, void *);

	// define data offset
	k = 0;
	while(db_table->data_map[k] && (db_table->data_map[k] != id))
	{
		k++;
	}

	if(!db_table->data_map[k])
	{
		k = 0;
	}

	// detect data type
	switch(db_table->flags[k] & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc = cb_cmp_str;
			break;
		case DT_CURRENCY:
			cbfunc = cb_cmp_currency;
			break;
		case DT_DATETIME:
			cbfunc = cb_cmp_datetime;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc = cb_cmp_num;
	}

	// sorting (bubble sort)
	done = 0;
	while(!done)
	{
		done = 1;
		for(i = 0; i < db_table->rows -1; i++)
		{
			if(cbfunc(db_table->data[i][k], db_table->data[i+1][k]) > 0)
			{
				a = db_table->data[i];
				db_table->data[i] = db_table->data[i+1];
				db_table->data[i+1] = a;

				done = 0;
			}
		}
	}

	return 0;
}

int parsed_table_qsort(db_parsed_table *db_table, unsigned char id, unsigned char direction)
{
	char **a;
	unsigned long i;
	unsigned long j;
	unsigned long k;
	unsigned long l;
	unsigned long r;
	unsigned long n;
	int z;
	char **pivot;
	db_list_node *stack;
	int (* cbfunc)(void *, void *);
	int (* cbfunc_id)(void *, void *);

	if((db_table->rows < 2) || !db_table->data)
	{
		return 0;
	}

	stack = NULL;

	// define data offset
	k = 0;
	while(db_table->data_map[k] && (db_table->data_map[k] != id))
	{
		k++;
	}

	if(!db_table->data_map[k])
	{
		k = 0;
	}

	// detect data type
	switch(db_table->flags[k] & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc = cb_cmp_str;
			break;
		case DT_CURRENCY:
			cbfunc = cb_cmp_currency;
			break;
		case DT_DATETIME:
			cbfunc = cb_cmp_datetime;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc = cb_cmp_num;
	}

	// detect data type of 'id' column
	n = 0;
	switch(db_table->flags[n] & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc_id = cb_cmp_str;
			break;
		case DT_CURRENCY:
			cbfunc_id = cb_cmp_currency;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc_id = cb_cmp_num;
	}


	// sorting (quick sort)
	l = 0;
	r = db_table->rows-1;

	if(direction)
	{
		goto lb_direction_desc;
	}

	do
	{
		i = l;
		j = r;
		pivot = db_table->data[(i+j+1)/2];

		// partition
		while(i <= j)
		{
lb_stage_a1:
			z = cbfunc(db_table->data[i][k], pivot[k]);
			if((z < 0) || ((z == 0) && (cbfunc_id(db_table->data[i][n], pivot[n]) < 0)))
			{
				i++;
				goto lb_stage_a1;
			}

lb_stage_a2:
			z = cbfunc(db_table->data[j][k], pivot[k]);
			if((z > 0) || ((z == 0) && (cbfunc_id(db_table->data[j][n], pivot[n]) > 0)))
			{
				j--;
				goto lb_stage_a2;
			}

			if(i <= j)
			{
				ZASSERT(j != 0);
				a = db_table->data[i];
				db_table->data[i] = db_table->data[j];
				db_table->data[j] = a;
				i++;
				j--;
			}
		}

		if(i < r)
		{
			stack_push(&stack, (void *) i);
			stack_push(&stack, (void *) r);
		}

		// optimized
		if(j > l)
		{
			r = j;
		}
		else
		{
			r = (unsigned long) stack_pop(&stack);
			l = (unsigned long) stack_pop(&stack);
		}
	}
	while(r | l);

	goto lb_finish;


lb_direction_desc:
	do
	{
		i = l;
		j = r;
		pivot = db_table->data[(i+j+1)/2];

		// partition
		while(i <= j)
		{
lb_stage_d1:
			z = cbfunc(db_table->data[i][k], pivot[k]);
			if((z > 0) || ((z == 0) && (cbfunc_id(db_table->data[i][n], pivot[n]) > 0)))
			{
				i++;
				goto lb_stage_d1;
			}

lb_stage_d2:
			z = cbfunc(db_table->data[j][k], pivot[k]);
			if((z < 0) || ((z == 0) && (cbfunc_id(db_table->data[j][n], pivot[n]) < 0)))
			{
				j--;
				goto lb_stage_d2;
			}

			if(i <= j)
			{
				ZASSERT(j != 0);
				a = db_table->data[i];
				db_table->data[i] = db_table->data[j];
				db_table->data[j] = a;
				i++;
				j--;
			}
		}

		if(i < r)
		{
			stack_push(&stack, (void *) i);
			stack_push(&stack, (void *) r);
		}

		// optimized
		if(j > l)
		{
			r = j;
		}
		else
		{
			r = (unsigned long) stack_pop(&stack);
			l = (unsigned long) stack_pop(&stack);
		}
	}
	while(r | l);


lb_finish:

	stack_free(&stack);

	return 0;
}

int parsed_table_qsort(db_list_node **disp_list, db_parsed_table *db_table, unsigned char id, unsigned char direction)
{
	int exit_code;
	unsigned long i;
	
	exit_code = parsed_table_qsort(db_table, id, direction);
	// regenerate visible list

	db_list_node *display_list;
	db_list_node *del_list;

	display_list = NULL;

	for(i = 0; i < db_table->rows; i++)
	{
		list_add(&display_list, db_table->data[i]);
	}

	del_list = *disp_list;
	*disp_list = display_list;

	list_free(&del_list, NULL);

	return exit_code;
}

// фильтрация ранее отфильтрованного списка
int parsed_table_filter(db_list_node **db_list, db_parsed_table *db_table, unsigned char id, unsigned long flags, void *data)
{
	unsigned long k;
	db_list_node **temp_node;
	int (* cbfunc)(void *, void *);

	// define data offset
	k = 0;
	while(db_table->data_map[k] && (db_table->data_map[k] != id))
	{
		k++;
	}

	if(!db_table->data_map[k])
	{
		return -1;
	}

	if((db_table->flags[k] & DT_TYPEMASK) != (flags & DT_TYPEMASK))
	{
		return -2;
	}

	// detect data type
	switch(db_table->flags[k] & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc = cb_fnd_substr;
			break;
		case DT_CURRENCY:
			cbfunc = cb_cmp_currency;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc = cb_cmp_num;
	}

	// filtering
	temp_node = db_list;
	while(*temp_node)
	{
		if(cbfunc(((char **) (*temp_node)->data)[k], data) != 0)
		{
			list_del_node(temp_node, NULL);
		}
		else
		{
			temp_node = &(*temp_node)->next_node;
		}
	}

	return 0;
}

// too slow!?
int parsed_table_data_filter(db_list_node **db_list, db_parsed_table *db_table, unsigned long flags, void *data)
{
	unsigned long k;
	unsigned long i;
	//db_list_node **temp_node;
	int (* cbfunc)(void *, void *);
	db_list_node *display_list;
	db_list_node *del_list;

	// detect data type
	switch(flags & DT_TYPEMASK)
	{
		case DT_STRING:
			cbfunc = cb_fnd_substr;
			break;
		case DT_CURRENCY:
			cbfunc = cb_cmp_currency;
			break;
		case DT_ULONG:
		case DT_DATE:
		default:
			cbfunc = cb_cmp_num;
	}

	// filtering
	display_list = NULL;

	for(i = 0; i < db_table->rows; i++)
	{
		k = 0;
		while(db_table->data_map[k])
		{
			if((db_table->flags[k] & DT_TYPEMASK) == (flags & DT_TYPEMASK))
			{
				if(cbfunc(db_table->data[i][k], data) == 0)
				{
					list_add(&display_list, db_table->data[i]);
					break;
				}
			}
			k++;
		}
	}

	del_list = *db_list;
	*db_list = display_list;

	list_free(&del_list, NULL);

	return 0;
}

/*
int filter_table(db_lv_data *db_table, char *text)
{
	db_list_node *display_list;
	db_list_node *del_list;
	unsigned long k;
	unsigned long i;

	k = 0;
	display_list = NULL;

	for(i = 0; i < db_table->pd->rows; i++)
	{
		list_add(&display_list, db_table->pd->data[i]);
		k++;
	}

	_lock(db_table->crit_sect);

	del_list = db_table->display_list;
	db_table->display_list = display_list;

	_unlock(db_table->crit_sect);

	ListView_SetItemCount(GetDlgItem(db_table->hwnd, IDC_MLIST), k);

	list_free(&del_list, NULL);

	return 0;
}

*/

void parse_db_result(odbc_result *res, db_parsed_table *db_data)
{
	unsigned long h, i, j, k;
	char **db_node;

	if(res)
	{
		if(res->num_rows > 0)
		{
			db_data->data = (char ***) zalloc((res->num_rows +1) * sizeof(char **));
			if(db_data->data)
			{
				//db_data->cols = res->num_cols;

				i = 0;

				for(i = 0; i < res->num_rows; i++)
				{
					k = res->num_cols * sizeof(char *);
					for(j = 0; (signed) j < res->num_cols; j++)
					{
						if(db_data->flags[j] & (DT_CURRENCY | DT_DATETIME | DT_DOUBLE))
						{
							k += 8;
						}
						else if(!(db_data->flags[j] & (DT_ULONG | DT_LONG | DT_DATE)))
						{
							k += res->rows[i]->lengths[j]+1;
						}
					}

					db_node = (char **) zalloc(k);
					if(db_node)
					{
						j = ((unsigned long) db_node) + (res->num_cols * sizeof(char *));

						k = 0;
						h = 0;

						while((signed) k < db_data->cols)
						{
							if(db_data->flags[k] & DT_STRING)
							{
								db_node[k] = (char *) j;
								strncpy_tiny((char *) j, res->rows[i]->data + res->rows[i]->offsets[h], res->rows[i]->lengths[h]);
								j += res->rows[i]->lengths[k]+1;
							}
							else if(db_data->flags[k] & DT_LONG)
							{
								db_node[k] = (char *) strtol(res->rows[i]->data + res->rows[i]->offsets[h], NULL, 10);
							}
							else if(db_data->flags[k] & DT_DATE)
							{
								db_node[k] = (char *) strtodate(res->rows[i]->data + res->rows[i]->offsets[h]);
							}
							else if(db_data->flags[k] & DT_DATETIME)
							{
								db_node[k] = (char *) j;
								*(unsigned long *) j = strtodate(res->rows[i]->data + res->rows[i]->offsets[h]);
								j += 4;
								*(unsigned long *) j = strtotime2(res->rows[i]->data + res->rows[i]->offsets[h]);
								j += 4;
							}
							else if(db_data->flags[k] & DT_DOUBLE)
							{
								db_node[k] = (char *) j;
								*(double *) db_node[k] = strtod(res->rows[i]->data + res->rows[i]->offsets[h], NULL);
								j += 8;
							}
							else if(db_data->flags[k] & DT_CURRENCY)
							{
								db_node[k] = (char *) j;
								*(__int64 *) db_node[k] = _atoi64(res->rows[i]->data + res->rows[i]->offsets[h]);
								j += 8;
							}
							else if(db_data->flags[k] & DT_USERDATA)
							{
								db_node[k] = NULL;
								k++;

								continue;
							}
							else // DT_ULONG
							{
								db_node[k] = (char *) strtoul(res->rows[i]->data + res->rows[i]->offsets[h], NULL, 10);
							}
							
							k++;
							h++;
						}

						db_data->data[db_data->rows] = db_node;
						db_data->rows++;
					}
				}
			}
		}
	}
}


void cb_table_free(void *data, void *data2)
{
	list_free((db_list_node **) &data, cb_vl_free, NULL);
}

