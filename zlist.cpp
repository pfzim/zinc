#include "zlist.h"								// pf_ziminski (c) 2008
#include "zalloc.h"
//#include "zlc/zl_compiler.h"
#include <windows.h>

// universal for any data (one based count)
int cb_by_count(void *data1, void *data2)
{
	(* (unsigned long *) data2)--;
	return (* (unsigned long *) data2) == 0;
}

// universal for any data (zero based count)
int cb_by_zero_count(void *data1, void *data2)
{
	return (* (unsigned long *) data2)-- == 0;
}

int cb_items_count(void *data1, void *data2)
{
	(* (unsigned long *) data2)++;

	return 0;
}

db_list_node *list_insert(db_list_node **db_list, void *data)
{
	db_list_node *temp_node;

	temp_node = (db_list_node *) zalloc(sizeof(db_list_node));
	if(temp_node)
	{
		temp_node->data = data;
		temp_node->next_node = *db_list;
		*db_list = temp_node;
	}

	return *db_list;
}

db_list_node *list_add(db_list_node **db_list, void *data)
{
	db_list_node **temp_node;

	temp_node = db_list;

	while(*temp_node)
	{
		temp_node = &(*temp_node)->next_node;
	}

	*temp_node = (db_list_node *) zalloc(sizeof(db_list_node));
	if(*temp_node)
	{
		(*temp_node)->data = data;
		(*temp_node)->next_node = NULL;
	}

	return *temp_node;
}

int list_free(db_list_node **db_list, void (*cbfunc)(void *))
{
	db_list_node *del_node;

	del_node = *db_list;
	while(del_node)
	{
		*db_list = del_node->next_node;

		if(cbfunc)
		{
			cbfunc(del_node->data);
		}

		zfree(del_node);

		del_node = *db_list;
	}

	return 0;
}

int list_free(db_list_node **db_list, void (* cbfunc)(void *, void *), void *data)
{
	db_list_node *del_node;

	del_node = *db_list;
	while(del_node)
	{
		*db_list = del_node->next_node;

		if(cbfunc)
		{
			cbfunc(del_node->data, data);
		}

		zfree(del_node);

		del_node = *db_list;
	}

	return 0;
}

int list_del(db_list_node **db_list, void (*cbfree)(void *), int (*cbcmp)(void *, void *), void *data)
{
	db_list_node **last_node;
	db_list_node *del_node;

	last_node = db_list;

	while(*last_node && !cbcmp((*last_node)->data, data))
	{
		last_node = &(*last_node)->next_node;
	}

	if(*last_node)
	{
		del_node = *last_node;
		*last_node = (*last_node)->next_node;
		del_node->next_node = NULL;

		if(cbfree)
		{
			cbfree(del_node->data);
		}

		zfree(del_node);
	}

	return 0;
}

int list_del_node(db_list_node **db_list, void (*cbfree)(void *))
{
	db_list_node *del_node;

	if(*db_list)
	{
		del_node = *db_list;
		*db_list = (*db_list)->next_node;
		del_node->next_node = NULL;

		if(cbfree)
		{
			cbfree(del_node->data);
		}

		zfree(del_node);
	}

	return 0;
}

int list_del2(db_list_node **db_list, db_list_node *db_node, void (*cbfree)(void *))
{
	db_list_node **last_node;
	db_list_node *del_node;

	last_node = db_list;

	while(*last_node && ((*last_node) != db_node))
	{
		last_node = &(*last_node)->next_node;
	}

	if(*last_node)
	{
		del_node = *last_node;
		*last_node = (*last_node)->next_node;
		del_node->next_node = NULL;

		if(cbfree)
		{
			cbfree(del_node->data);
		}

		zfree(del_node);
	}

	return 0;
}

db_list_node *list_get(db_list_node *db_list, unsigned long n)
{
	while(db_list && n)
	{
		n--;
		db_list = db_list->next_node;
	}

	return db_list;
}

db_list_node *list_walk(db_list_node *db_list, int (*cbcmp)(void *, void *), void *data)
{
	while(db_list && !cbcmp(db_list->data, data))
	{
		db_list = db_list->next_node;
	}

	return db_list;
}

db_list_node **list_bind_walk(db_list_node **db_list, int (*cbcmp)(void *, void *), void *data)
{
	while(*db_list && !cbcmp((*db_list)->data, data))
	{
		db_list = &(*db_list)->next_node;
	}

	return (*db_list)?db_list:NULL;
}

/*
void qsort(int arr[], int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	// partition
	while(i <= j)
	{
		while(arr[i] < pivot)
		{
			i++;
		}

		while(arr[j] > pivot)
		{
			j--;
		}

		if(i <= j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	// recursion
	if(left < j)
	{
		qsort(arr, left, j);
	}

	if(i < right)
	{
		qsort(arr, i, right);
	}
}
*/

db_list_node *list_sort(db_list_node **db_node, int (* cbfunc)(void *, void *))
{
	db_list_node **temp_node;
	db_list_node *c;
	db_list_node *b;
	int done;

	done = 0;
	while(!done)
	{
		done = 1;
		for(temp_node = db_node; *temp_node && (*temp_node)->next_node; temp_node = &(*temp_node)->next_node)
		{
			if(cbfunc((*temp_node)->data, (*temp_node)->next_node->data) > 0)
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

	return *db_node;
}


void *stack_push(db_list_node **cl_stack, void *data)	// like list_insert
{
	db_list_node *temp_node;

	temp_node = (db_list_node *) zalloc(sizeof(db_list_node));
	if(temp_node)
	{
		temp_node->data = data;
		temp_node->next_node = *cl_stack;
		*cl_stack = temp_node;
	}

	return data;
}

void *stack_pop(db_list_node **cl_stack)
{
	void *data;
	db_list_node *temp_node;

	if(!*cl_stack)
	{
		return NULL;
	}

	temp_node = *cl_stack;
	data = temp_node->data;
	*cl_stack = temp_node->next_node;

	zfree(temp_node);

	return data;
}

void stack_free(db_list_node **cl_stack)	// like list_free(..., NULL)
{
	db_list_node *temp_node;

	while(*cl_stack)
	{
		temp_node = *cl_stack;
		*cl_stack = temp_node->next_node;

		zfree(temp_node);
	}
}


db_tree_node *tree_insert(db_tree_node **bind_point, void *data)
{
	db_tree_node *temp_node;

	temp_node = (db_tree_node *) zalloc(sizeof(db_tree_node));
	if(temp_node)
	{
		temp_node->data = data;
		temp_node->next_node = *bind_point;
		temp_node->child_node = NULL;
		*bind_point = temp_node;
	}

	return temp_node;
}

db_tree_node *tree_add(db_tree_node **bind_point, void *data)
{
	while(*bind_point)
	{
		bind_point = &(*bind_point)->next_node;
	}

	*bind_point = (db_tree_node *) zalloc(sizeof(db_tree_node));
	if(*bind_point)
	{
		(*bind_point)->data = data;
		(*bind_point)->next_node = NULL;
		(*bind_point)->child_node = NULL;
	}

	return *bind_point;
}

db_tree_node *tree_list(db_tree_node *root_node, int (*cbcmp)(void *, void *, void *, void *), void *data)
{
	while(root_node && !cbcmp(NULL /* not used here */, root_node->child_node?root_node->child_node->data:NULL, (void *) root_node->data, (void *) data))
	{
		root_node = root_node->next_node;
	}

	return root_node;
}

db_tree_node *tree_walk(db_tree_node *root_node, int (*cbcmp)(void *, void *, void *, void *), void *data)
{
	db_list_node *stack_buffer;
	stack_buffer = NULL;

	do
	{
		while(root_node)
		{
			if(cbcmp(stack_buffer?((db_tree_node *) stack_buffer->data)->data:NULL, root_node->child_node?root_node->child_node->data:NULL, (void *) root_node->data, (void *) data))
			{
				stack_free(&stack_buffer);
				return root_node;
			}

			if(root_node->child_node)
			{
				//if(root_node->next_node)
				{
					stack_push(&stack_buffer, root_node);
				}
				root_node = root_node->child_node;
			}
			else
			{
				root_node = root_node->next_node;
			}
		}

		do
		{
			root_node = (db_tree_node *) stack_pop(&stack_buffer);
		}
		while(root_node && !root_node->next_node);

		if(root_node)
		{
			root_node = root_node->next_node;
		}
	}
	while(root_node);

	return NULL;
}

db_tree_node **tree_bind_find(db_tree_node **root_node, int (*cbcmp)(void *, void *, void *, void *), void *data)
{
	db_list_node *stack_buffer;
	stack_buffer = NULL;

	do
	{
		while(*root_node)
		{
			if(cbcmp(stack_buffer?((db_tree_node *) stack_buffer->data)->data:NULL, (*root_node)->child_node?(*root_node)->child_node->data:NULL, (void *) (*root_node)->data, (void *) data))
			{
				stack_free(&stack_buffer);
				return root_node;
			}

			if((*root_node)->child_node)
			{
				stack_push(&stack_buffer, *root_node);
				root_node = &(*root_node)->child_node;
			}
			else
			{
				root_node = &(*root_node)->next_node;
			}
		}

		do
		{
			root_node = (db_tree_node **) stack_pop(&stack_buffer);
		}
		while(root_node && !(*root_node)->next_node);

		if(root_node)
		{
			root_node = &((db_tree_node *) root_node)->next_node;
		}
	}
	while(*root_node);

	return NULL;
}

int tree_free(db_tree_node **root_node, void (*cbfunc)(void *))
{
	db_tree_node *del_node;
	db_tree_node *temp_node;
	db_list_node *stack_buffer;

	stack_buffer = NULL;
	temp_node = *root_node;

	do
	{
		while(temp_node)
		{
			del_node = temp_node;

			if(temp_node->child_node)
			{
				if(temp_node->next_node)
				{
					stack_push(&stack_buffer, temp_node->next_node);
				}
				temp_node = temp_node->child_node;
			}
			else
			{
				temp_node = temp_node->next_node;
			}

			if(cbfunc)
			{
				cbfunc(del_node->data);
			}

			zfree(del_node);
		}

		temp_node = (db_tree_node *) stack_pop(&stack_buffer);
	}
	while(temp_node);

	*root_node = NULL;

	return NULL;
}

int tree_free(db_tree_node **root_node, int (* cbfunc)(void *, void *), void *data)
{
	db_tree_node *del_node;
	db_tree_node *temp_node;
	db_list_node *stack_buffer;

	stack_buffer = NULL;
	temp_node = *root_node;

	do
	{
		while(temp_node)
		{
			del_node = temp_node;

			if(temp_node->child_node)
			{
				if(temp_node->next_node)
				{
					stack_push(&stack_buffer, temp_node->next_node);
				}
				temp_node = temp_node->child_node;
			}
			else
			{
				temp_node = temp_node->next_node;
			}

			if(cbfunc)
			{
				cbfunc(del_node->data, data);
			}

			zfree(del_node);
		}

		temp_node = (db_tree_node *) stack_pop(&stack_buffer);
	}
	while(temp_node);

	*root_node = NULL;

	return NULL;
}

// эти функции выносим в клиентскую часть для совместимости с tree_* функциями
int cb_path_cmp(void *pdata, void *cdata, void *data1, void *data2)
{
	return (((db_path_data *) data1)->hash == (unsigned long) ((void **) data2)[0]) && !strncmp(((db_path_data *) data1)->name, (char *) ((void **) data2)[1], (unsigned long) ((void **) data2)[2]) && (((db_path_data *) data1)->name[(unsigned long) ((void **) data2)[2]] == 0);
}

//db_tree_node *tree_path_update(db_tree_node **root_node, char *path, int (*cb_cmp_func)(void *, void *, void *, void *), void *data)
db_tree_node *tree_path_update(db_tree_node **root_node, char *path, void *data, void (*cbfunc)(void *))
{
	char *name;
	unsigned long len;
	db_tree_node *temp_node;
	db_tree_node **bind_point;
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

				temp_node = tree_list(*bind_point, cb_path_cmp, params);
				if(temp_node)
				{
					bind_point = &temp_node->child_node;
				}
				else
				{
					temp_node = (db_tree_node *) zalloc(sizeof(db_tree_node) + sizeof(db_path_data) + len + 1);
					if(temp_node)
					{
						temp_node->data = (char *) (((unsigned long) temp_node) + sizeof(db_tree_node));
						temp_node->next_node = NULL;
						//temp_node->next_node = *bind_point;
						temp_node->child_node = NULL;

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

						bind_point = &temp_node->child_node;
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

	temp_node = tree_list(*bind_point, cb_path_cmp, params);
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
		temp_node = (db_tree_node *) zalloc(sizeof(db_tree_node) + sizeof(db_path_data) + len + 1);
		if(temp_node)
		{
			temp_node->data = (char *) (((unsigned long) temp_node) + sizeof(db_tree_node));
			temp_node->next_node = NULL;
			//temp_node->next_node = *bind_point;
			temp_node->child_node = NULL;

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

// path_find передаёт в функцию cb_cmp_func массив: [0] хэш имени, [1] имя, [2] длина имени
//db_tree_node *tree_path_find(db_tree_node *root_node, char *path, int (*cb_cmp_func)(void *, void *, void *, void *))
db_tree_node *tree_path_find(db_tree_node *root_node, char *path)
{
	char *name;
	unsigned long len;
	db_tree_node *temp_node;
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
				temp_node = tree_list(temp_node, cb_path_cmp, params);
				if(!*path)
				{
					break;
				}

				if(temp_node)
				{
					temp_node = temp_node->child_node;
				}

				if(!temp_node)
				{
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

	return temp_node;
}

int tree_path_free(db_tree_node **root_node, void (*cbfunc)(void *))
{
	db_tree_node *del_node;
	db_tree_node *temp_node;
	db_list_node *stack_buffer;

	stack_buffer = NULL;
	temp_node = *root_node;

	do
	{
		while(temp_node)
		{
			del_node = temp_node;

			if(temp_node->child_node)
			{
				if(temp_node->next_node)
				{
					stack_push(&stack_buffer, temp_node->next_node);
				}
				temp_node = temp_node->child_node;
			}
			else
			{
				temp_node = temp_node->next_node;
			}

			//if(del_node->data)
			{
				if(cbfunc)
				{
					cbfunc(((db_path_data *) del_node->data)->data);
				}

				//zfree(del_node->data)
			}

			zfree(del_node);
		}

		temp_node = (db_tree_node *) stack_pop(&stack_buffer);
	}
	while(temp_node);

	*root_node = NULL;

	return NULL;
}

