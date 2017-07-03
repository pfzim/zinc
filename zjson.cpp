#include "zjson.h"								// pf_ziminski (c) 2010
#include "zalloc.h"
#include "utstrings.h"
//#include "zlc/zl_compiler.h"
#include <windows.h>
#include "zdbg.h"
#include <stdio.h>

void cb_json_free(void *data)
{
	if(data)
	{
		if(((json_value *) data)->flags & JS_STRING)
		{
			free_str(((json_value *) data)->str_val);
		}
		else if(((json_value *) data)->flags & JS_ARRAY)
		{
			list_free(&((json_value *) data)->arr_val, cb_json_free);
		}
		else if(((json_value *) data)->flags & JS_OBJECT)
		{
			json_free(&((json_value *) data)->obj_val);
		}

		zfree(data);
	}
}

json_value *json_val(unsigned long flags, void *data)
{
	json_value *temp_value;

	temp_value = (json_value *) zalloc(sizeof(json_value));
	if(temp_value)
	{
		memset(temp_value, 0, sizeof(json_value));

		temp_value->flags = flags;
		if(flags & JS_STRING)
		{
			temp_value->str_val = alloc_string((char *) data);
		}
		else if(flags & JS_ULONG)
		{
			temp_value->ul_val = (unsigned long) data;
		}
		else if(flags & JS_LONG)
		{
			temp_value->l_val = (long) data;
		}
		else if(flags & JS_DOUBLE)
		{
			temp_value->dbl_val = *(double *) data;
		}
		else if(flags & JS_ARRAY)
		{
			temp_value->arr_val = (db_list_node *) data;
		}
		else if(flags & JS_OBJECT)
		{
			temp_value->obj_val = (db_tree_node *) data;
		}
	}

	return temp_value;
}

db_tree_node *json_set(db_tree_node **root_node, char *path, unsigned long flags, void *data)
{
	json_value *temp_value;
	
	temp_value = json_val(flags, data);
	if(temp_value)
	{
		tree_path_update(root_node, path, temp_value, cb_json_free);
	}

	return NULL;
}

db_tree_node *json_assign(db_tree_node **root_node, char *path, json_value *value)
{
	return tree_path_update(root_node, path, value, cb_json_free);
}

json_value *json_find(db_tree_node *root_node, char *path)
{
	db_tree_node *temp_node;

	temp_node = tree_path_find(root_node, path);
	if(temp_node && temp_node->data)
	{
		return (json_value *) ((db_path_data *) temp_node->data)->data;
	}

	return NULL;
}

int json_free(db_tree_node **root_node)
{
	tree_path_free(root_node, cb_json_free);

	return 0;
}

json_value *json_array_push(json_value *node, json_value *value)
{
	if(node->flags & JS_ARRAY)
	{
		list_add(&node->arr_val, value);
	}
	
	return value;
}

json_value *json_dup0(json_value *value)
{
	json_value *temp_node;

	temp_node = (json_value *) zalloc(sizeof(json_value));
	if(temp_node)
	{
		memcpy(temp_node, value, sizeof(json_value));
	}
	
	return temp_node;
}


char *json_print(db_tree_node *root_node)
{
	db_list_node *stack_buffer;
	db_list_node *list_node;
	char temp_buf[33];
	char *result;
	char *temp_str;
	int first;

	stack_buffer = NULL;
	list_node = NULL;

	result = alloc_string("{");

lb_obj:
	first = 1;
	while(root_node)
	{
		if(!first)
		{
			alloc_strcat(&result, ", ");
		}
		else
		{
			first = 0;
		}

		alloc_strcat(&result, "\"");
		// escape string here
		alloc_strcat(&result, ((db_path_data *) root_node->data)->name);
		alloc_strcat(&result, "\": ");

		if(((db_path_data *) root_node->data)->data)
		{
			if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_STRING)
			{
				//alloc_strcat(&result, "\"");
				// escape string here
				temp_str = alloc_escapestring(((json_value *) ((db_path_data *) root_node->data)->data)->str_val, strlen(((json_value *) ((db_path_data *) root_node->data)->data)->str_val));
				alloc_strcat(&result, temp_str);
				free_str(temp_str);
				//alloc_strcat(&result, "\"");
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_LONG)
			{
				ltoa(((json_value *) ((db_path_data *) root_node->data)->data)->l_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ULONG)
			{
				ultoa(((json_value *) ((db_path_data *) root_node->data)->data)->ul_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_DOUBLE)
			{
				_snprintf(temp_buf, 32, "%f", ((json_value *) ((db_path_data *) root_node->data)->data)->dbl_val);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ARRAY)
			{
				alloc_strcat(&result, "[");

				stack_push(&stack_buffer, root_node);
				stack_push(&stack_buffer, (void *) 1);

				list_node = ((json_value *) ((db_path_data *) root_node->data)->data)->arr_val;

				goto lb_arr;
			}
			/* never fired
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_OBJECT)
			{
				alloc_strcat(&result, "{");

				stack_push(&stack_buffer, root_node);
				stack_push(&stack_buffer, (void *) 1);

				root_node = ((json_value *) ((db_path_data *) root_node->data)->data)->obj_val;

				goto lb_obj;
			}
			*/
		}

		if(root_node->child_node)
		{
			alloc_strcat(&result, "{");

			stack_push(&stack_buffer, root_node);
			stack_push(&stack_buffer, (void *) 1);

			root_node = root_node->child_node;

			goto lb_obj;
		}

		root_node = root_node->next_node;
	}

	alloc_strcat(&result, "}");

	goto lb_stack;

lb_arr:
	first = 1;
	while(list_node)
	{
		if(!first)
		{
			alloc_strcat(&result, ", ");
		}
		else
		{
			first = 0;
		}

		if(list_node->data)
		{
			if(((json_value *) list_node->data)->flags & JS_STRING)
			{
				//alloc_strcat(&result, "\"");
				// escape string here
				temp_str = alloc_escapestring(((json_value *) list_node->data)->str_val, strlen(((json_value *) list_node->data)->str_val));
				alloc_strcat(&result, temp_str);
				free_str(temp_str);
				//alloc_strcat(&result, "\"");
			}
			else if(((json_value *) list_node->data)->flags & JS_LONG)
			{
				ltoa(((json_value *) list_node->data)->l_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_ULONG)
			{
				ultoa(((json_value *) list_node->data)->ul_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_DOUBLE)
			{
				_snprintf(temp_buf, 32, "%f", ((json_value *) list_node->data)->dbl_val);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_ARRAY)
			{
				alloc_strcat(&result, "[");

				stack_push(&stack_buffer, list_node);
				stack_push(&stack_buffer, 0);

				list_node = ((json_value *) list_node->data)->arr_val;

				goto lb_arr;
			}
			else if(((json_value *) list_node->data)->flags & JS_OBJECT)
			{
				alloc_strcat(&result, "{");

				stack_push(&stack_buffer, list_node);
				stack_push(&stack_buffer, 0);

				root_node = ((json_value *) list_node->data)->obj_val;

				goto lb_obj;
			}
		}

		list_node = list_node->next_node;
	}

	alloc_strcat(&result, "]");

lb_stack:
	
	while(stack_buffer)
	{
		if(stack_pop(&stack_buffer))
		{
			root_node = (db_tree_node *) stack_pop(&stack_buffer);
			if(!root_node)
			{
				break;
			}

			root_node = root_node->next_node;

			if(root_node)
			{
				alloc_strcat(&result, ", ");

				goto lb_obj;
			}
	
			alloc_strcat(&result, "}");
		}
		else
		{
			list_node = (db_list_node *) stack_pop(&stack_buffer);
			if(!list_node)
			{
				break;
			}

			list_node = list_node->next_node;

			if(list_node)
			{
				alloc_strcat(&result, ", ");

				goto lb_arr;
			}
	
			alloc_strcat(&result, "]");
		}
	}

	return result;
}

char *json_print_fmt(db_tree_node *root_node)
{
	db_list_node *stack_buffer;
	db_list_node *list_node;
	char temp_buf[33];
	char *result;
	char *temp_str;
	int first;
	unsigned long deep, i;

	stack_buffer = NULL;
	list_node = NULL;

	result = alloc_string("{\n");
	deep = 1;
	for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

lb_obj:
	first = 1;
	while(root_node)
	{
		if(!first)
		{
			alloc_strcat(&result, ",\n");
			for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
		}
		else
		{
			first = 0;
		}

		alloc_strcat(&result, "\"");
		// escape string here
		alloc_strcat(&result, ((db_path_data *) root_node->data)->name);
		alloc_strcat(&result, "\": ");

		if(((db_path_data *) root_node->data)->data)
		{
			if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_STRING)
			{
				//alloc_strcat(&result, "\"");
				// escape string here
				temp_str = alloc_escapestring(((json_value *) ((db_path_data *) root_node->data)->data)->str_val, strlen(((json_value *) ((db_path_data *) root_node->data)->data)->str_val));
				alloc_strcat(&result, temp_str);
				free_str(temp_str);
				//alloc_strcat(&result, "\"");
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_LONG)
			{
				ltoa(((json_value *) ((db_path_data *) root_node->data)->data)->l_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ULONG)
			{
				ultoa(((json_value *) ((db_path_data *) root_node->data)->data)->ul_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_DOUBLE)
			{
				_snprintf(temp_buf, 32, "%f", ((json_value *) ((db_path_data *) root_node->data)->data)->dbl_val);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ARRAY)
			{
				alloc_strcat(&result, "[\n");
				deep++;
				for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

				stack_push(&stack_buffer, root_node);
				stack_push(&stack_buffer, (void *) 1);

				list_node = ((json_value *) ((db_path_data *) root_node->data)->data)->arr_val;

				goto lb_arr;
			}
			/* never fired
			else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_OBJECT)
			{
				alloc_strcat(&result, "{");

				stack_push(&stack_buffer, root_node);
				stack_push(&stack_buffer, (void *) 1);

				root_node = ((json_value *) ((db_path_data *) root_node->data)->data)->obj_val;

				goto lb_obj;
			}
			*/
		}

		if(root_node->child_node)
		{
			alloc_strcat(&result, "{\n");
			deep++;
			for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

			stack_push(&stack_buffer, root_node);
			stack_push(&stack_buffer, (void *) 1);

			root_node = root_node->child_node;

			goto lb_obj;
		}

		root_node = root_node->next_node;
	}

	deep--;
	alloc_strcat(&result, "\n");
	for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
	alloc_strcat(&result, "}");

	goto lb_stack;

lb_arr:
	first = 1;
	while(list_node)
	{
		if(!first)
		{
			alloc_strcat(&result, ",\n");
			for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
		}
		else
		{
			first = 0;
		}

		if(list_node->data)
		{
			if(((json_value *) list_node->data)->flags & JS_STRING)
			{
				//alloc_strcat(&result, "\"");
				// escape string here
				temp_str = alloc_escapestring(((json_value *) list_node->data)->str_val, strlen(((json_value *) list_node->data)->str_val));
				alloc_strcat(&result, temp_str);
				free_str(temp_str);
				//alloc_strcat(&result, "\"");
			}
			else if(((json_value *) list_node->data)->flags & JS_LONG)
			{
				ltoa(((json_value *) list_node->data)->l_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_ULONG)
			{
				ultoa(((json_value *) list_node->data)->ul_val, temp_buf, 10);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_DOUBLE)
			{
				_snprintf(temp_buf, 32, "%f", ((json_value *) list_node->data)->dbl_val);
				alloc_strcat(&result, temp_buf);
			}
			else if(((json_value *) list_node->data)->flags & JS_ARRAY)
			{
				alloc_strcat(&result, "[\n");
				deep++;
				for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

				stack_push(&stack_buffer, list_node);
				stack_push(&stack_buffer, 0);

				list_node = ((json_value *) list_node->data)->arr_val;

				goto lb_arr;
			}
			else if(((json_value *) list_node->data)->flags & JS_OBJECT)
			{
				alloc_strcat(&result, "{\n");
				deep++;
				for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

				stack_push(&stack_buffer, list_node);
				stack_push(&stack_buffer, 0);

				root_node = ((json_value *) list_node->data)->obj_val;

				goto lb_obj;
			}
		}

		list_node = list_node->next_node;
	}

	deep--;
	alloc_strcat(&result, "\n");
	for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
	alloc_strcat(&result, "]");

lb_stack:
	
	while(stack_buffer)
	{
		if(stack_pop(&stack_buffer))
		{
			root_node = (db_tree_node *) stack_pop(&stack_buffer);
			if(!root_node)
			{
				break;
			}

			root_node = root_node->next_node;

			if(root_node)
			{
				alloc_strcat(&result, ",\n");
				for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

				goto lb_obj;
			}
	
			deep--;
			alloc_strcat(&result, "\n");
			for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
			alloc_strcat(&result, "}");
		}
		else
		{
			list_node = (db_list_node *) stack_pop(&stack_buffer);
			if(!list_node)
			{
				break;
			}

			list_node = list_node->next_node;

			if(list_node)
			{
				alloc_strcat(&result, ",\n");
				for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");

				goto lb_arr;
			}
	
			deep--;
			alloc_strcat(&result, "\n");
			for(i = 0; i < deep; i++) alloc_strcat(&result, "    ");
			alloc_strcat(&result, "]");
		}
	}

	return result;
}

// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************
// ************************************************************************************************************************

// Тестовая переделка под плоскую модель db_list_node
// Думаю от неё стоит отказаться, так как появляются накладные расходы
// и лишние проверки на дочернии объекты (JS_OBJECT)
// Использование child_node более удобно!?

/*
int cb_path_cmp(void *data1, void *data2)
{
	return (((db_path_data *) data1)->hash == (unsigned long) ((void **) data2)[0]) && !strncmp(((db_path_data *) data1)->name, (char *) ((void **) data2)[1], (unsigned long) ((void **) data2)[2]) && (((db_path_data *) data1)->name[(unsigned long) ((void **) data2)[2]] == 0);
}

json_value *json_find(db_list_node *root_node, char *path)
{
	char *name;
	unsigned long len;
	db_list_node *temp_node;
	void *params[3];

	if(!*path)
	{
		return NULL;
	}

	len = 0;
	name = path;
	temp_node = root_node;

	while(1)
	{
		if(!*path || (*path == '/'))
		{
			if(len)
			{
				params[0] = (void *) 0; //hash(name);
				params[1] = (void *) name;
				params[2] = (void *) len;

				// tree_walk функция здесь не подходит, потому что она ищет и в дочерних ветках тоже
				temp_node = list_walk(temp_node, cb_path_cmp, params);
				if(!*path)
				{
					break;
				}

				if(!temp_node)
				{
					break;
				}

				if((temp_node->data)
					&& (((db_path_data *) temp_node->data)->data)
					&& (((json_value *) ((db_path_data *) temp_node->data)->data)->flags & JS_OBJECT)
				)
				{
					temp_node = ((json_value *) ((db_path_data *) temp_node->data)->data)->arr_val;
				}
				else
				{
					temp_node = NULL;
					break;
				}
			}
			else if(!*path)
			{
				break;
			}

			len = 0;
			name = ++path;
		}
		else
		{
			len++;
			path++;
		}
	}

	if(temp_node && temp_node->data)
	{
		return (json_value *) ((db_path_data *) temp_node->data)->data;
	}

	return NULL;
}

db_list_node *json_path_update(db_list_node **root_node, char *path, void *data, DBLFF cbfunc)
{
	char *name;
	unsigned long len;
	db_list_node *temp_node;
	db_list_node **bind_point;
	void *params[3];

	if(!*path)
	{
		return NULL;
	}

	len = 0;
	name = path;
	temp_node = NULL;
	bind_point = root_node;

	while(*path)
	{
		if(*path == '/')
		{
			if(len)
			{
				params[0] = (void *) 0; // hash(name);
				params[1] = (void *) name;
				params[2] = (void *) len;

				temp_node = list_walk(*bind_point, cb_path_cmp, params);
				if(temp_node)
				{
					if(((json_value *) ((db_path_data *) temp_node->data)->data)->flags & JS_OBJECT)
					{
						bind_point = &((json_value *) ((db_path_data *) temp_node->data)->data)->arr_val;
					}

					// здесь какая-то херня получается, а вдруг он не есть JS_OBJECT
				}
				else
				{
					temp_node = (db_list_node *) zalloc(sizeof(db_list_node) + sizeof(db_path_data) + len + 1);
					if(temp_node)
					{
						temp_node->data = (char *) (((unsigned long) temp_node) + sizeof(db_list_node));
						temp_node->next_node = NULL;
						//temp_node->next_node = *bind_point;
						//temp_node->child_node = NULL;

						// add to end of list
						while(*bind_point)
						{
							bind_point = &(*bind_point)->next_node;
						}

						*bind_point = temp_node;

						((db_path_data *) temp_node->data)->hash = (unsigned long) 0;
						((db_path_data *) temp_node->data)->name = (char *) (((unsigned long) temp_node->data) + sizeof(db_path_data));
						((db_path_data *) temp_node->data)->data = data;
						strncpy(((db_path_data *) temp_node->data)->name, name, len);
						((db_path_data *) temp_node->data)->name[len] = '\0';

						//bind_point = &temp_node->child_node;
						bind_point = (db_list_node **) &((db_path_data *) temp_node->data)->data;
					}
				}

				if(!temp_node)
				{
					break;
				}
			}

			len = 0;
			name = ++path;
		}
		else
		{
			len++;
			path++;
		}
	}

	params[0] = (void *) 0; // hash(name);
	params[1] = (void *) name;
	params[2] = (void *) len;

	temp_node = list_walk(*bind_point, cb_path_cmp, params);
	if(temp_node)
	{
		if(cbfunc)
		{
			cbfunc(((db_path_data *) temp_node->data)->data);
		}

		((db_path_data *) temp_node->data)->data = data;
	}
	else
	{
		temp_node = (db_list_node *) zalloc(sizeof(db_list_node) + sizeof(db_path_data) + len + 1);
		if(temp_node)
		{
			temp_node->data = (char *) (((unsigned long) temp_node) + sizeof(db_list_node));
			temp_node->next_node = NULL;
			//temp_node->next_node = *bind_point;
			//temp_node->child_node = NULL;

			// add to end of list
			while(*bind_point)
			{
				bind_point = &(*bind_point)->next_node;
			}

			*bind_point = temp_node;

			((db_path_data *) temp_node->data)->hash = (unsigned long) 0;
			((db_path_data *) temp_node->data)->name = (char *) (((unsigned long) temp_node->data) + sizeof(db_path_data));
			((db_path_data *) temp_node->data)->data = data;
			strncpy(((db_path_data *) temp_node->data)->name, name, len);
			((db_path_data *) temp_node->data)->name[len] = '\0';
		}
	}

	return temp_node;
}

db_list_node *json_assign(db_list_node **root_node, char *path, json_value *value)
{
	return json_path_update(root_node, path, value, cb_json_free);
}

/* end of new JSON */

/*
int cb_json_print_value(void *data1, void *data2);
int cb_json_print(void *pdata, void *cdata, void *data1, void *data2);
char *json_print0(db_tree_node *root_node, char **result);

int cb_json_print_value(void *data1, void *data2)
{
	char temp_buf[33];

	if(data1)
	{
		if(((json_value *) data1)->flags & JS_STRING)
		{
			alloc_strcat((char **) data2, "\"");
			alloc_strcat((char **) data2, ((json_value *) data1)->str_val);
			alloc_strcat((char **) data2, "\"");
		}
		else if(((json_value *) data1)->flags & JS_LONG)
		{
			ltoa(((json_value *) data1)->l_val, temp_buf, 10);
			alloc_strcat((char **) data2, temp_buf);
		}
		else if(((json_value *) data1)->flags & JS_ULONG)
		{
			ultoa(((json_value *) data1)->ul_val, temp_buf, 10);
			alloc_strcat((char **) data2, temp_buf);
		}
		else if(((json_value *) data1)->flags & JS_DOUBLE)
		{
			_snprintf(temp_buf, 32, "%f", ((json_value *) data1)->dbl_val);
			alloc_strcat((char **) data2, temp_buf);
		}
		else if(((json_value *) data1)->flags & JS_ARRAY)
		{
			alloc_strcat((char **) data2, "[");
			list_walk(((json_value *) data1)->arr_val, cb_json_print_value, data2);
			alloc_strcat((char **) data2, "]");
		}
		else if(((json_value *) data1)->flags & JS_OBJECT)
		{
			json_print0(((json_value *) data1)->obj_val, (char **) data2);
		}

		alloc_strcat((char **) data2, ", ");
	}

	return 0;
}

int cb_json_print(void *data1, void *data2)
{
	alloc_strcat((char **) data2, "\"");
	alloc_strcat((char **) data2, ((db_path_data *) data1)->name);
	alloc_strcat((char **) data2, "\" : ");

	cb_json_print_value(((db_path_data *) data1)->data, data2);

	return 0;
}

int cb_json_print1(void *pdata, void *cdata, void *data1, void *data2)
{
	alloc_strcat((char **) data2, "\"");
	alloc_strcat((char **) data2, ((db_path_data *) data1)->name);
	alloc_strcat((char **) data2, "\" : ");

	cb_json_print_value(((db_path_data *) data1)->data, data2);

	/*
	if(((db_path_data *) data1)->data)
	{
		if(((json_value *) ((db_path_data *) data1)->data)->flags & JS_STRING)
		{
			alloc_strcat((char **) data2, "\"");
			alloc_strcat((char **) data2, ((json_value *) ((db_path_data *) data1)->data)->str_val);
			alloc_strcat((char **) data2, "\", ");
		}
		else if(((json_value *) ((db_path_data *) data1)->data)->flags & JS_ARRAY)
		{
			alloc_strcat((char **) data2, "[");
			list_walk(((json_value *) ((db_path_data *) data1)->data)->arr_val, cb_json_print_value, data2);
			alloc_strcat((char **) data2, "]");
		}
		else if(((json_value *) ((db_path_data *) data1)->data)->flags & JS_OBJECT)
		{
			alloc_strcat((char **) data2, "{");
			tree_walk(((json_value *) ((db_path_data *) data1)->data)->obj_val, cb_json_print, data2);
			alloc_strcat((char **) data2, "}");
		}
	}
	* /

	return 0;
}

char *json_print1(db_tree_node *root_node)
{
	char *result;

	result = alloc_string("{");

	tree_walk(root_node, cb_json_print1, &result);

	alloc_strcat(&result, "}");

	return result;
}

char *json_print0(db_tree_node *root_node, char **result)
{
	db_list_node *stack_buffer;
	char temp_buf[33];

	stack_buffer = NULL;

	alloc_strcat(result, "{");

	do
	{
		while(root_node)
		{
			alloc_strcat(result, "\"");
			// escape string here
			alloc_strcat(result, ((db_path_data *) root_node->data)->name);
			alloc_strcat(result, "\": ");

			if(((db_path_data *) root_node->data)->data)
			{
				if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_STRING)
				{
					alloc_strcat(result, "\"");
					// escape string here
					alloc_strcat(result, ((json_value *) ((db_path_data *) root_node->data)->data)->str_val);
					alloc_strcat(result, "\"");
				}
				else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_LONG)
				{
					ltoa(((json_value *) ((db_path_data *) root_node->data)->data)->l_val, temp_buf, 10);
					alloc_strcat(result, temp_buf);
				}
				else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ULONG)
				{
					ultoa(((json_value *) ((db_path_data *) root_node->data)->data)->ul_val, temp_buf, 10);
					alloc_strcat(result, temp_buf);
				}
				else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_DOUBLE)
				{
					_snprintf(temp_buf, 32, "%f", ((json_value *) ((db_path_data *) root_node->data)->data)->dbl_val);
					alloc_strcat(result, temp_buf);
				}
				else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_ARRAY)
				{
					alloc_strcat(result, "[");
					list_walk(((json_value *) ((db_path_data *) root_node->data)->data)->arr_val, cb_json_print_value, result);
					alloc_strcat(result, "]");
				}
				else if(((json_value *) ((db_path_data *) root_node->data)->data)->flags & JS_OBJECT)
				{
					stack_push(&stack_buffer, root_node);
					root_node = ((json_value *) ((db_path_data *) root_node->data)->data)->obj_val;

					alloc_strcat(result, "{");

					continue;
				}
			}

			if(root_node->child_node)
			{
				//if(root_node->next_node)
				{
					stack_push(&stack_buffer, root_node);
				}
				root_node = root_node->child_node;

				alloc_strcat(result, "{");
			}
			else
			{
				root_node = root_node->next_node;
				if(root_node)
				{
					alloc_strcat(result, ", ");
				}
			}
		}

		do
		{
			root_node = (db_tree_node *) stack_pop(&stack_buffer);
			alloc_strcat(result, "}");
		}
		while(root_node && !root_node->next_node);

		if(root_node)
		{
			root_node = root_node->next_node;
		}
	}
	while(root_node);

	//alloc_strcat(&result, "}");

	return *result;
}

char *json_print_old(db_tree_node *root_node)
{
	char *result;

	result = NULL;

	return json_print0(root_node, &result);
}

*/