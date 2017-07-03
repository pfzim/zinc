#ifndef _ZLIST_H_
#define _ZLIST_H_

typedef struct _db_list_node
{
	union
	{
		void *data;
		char *str_val;
		long l_val;
		unsigned long ul_val;
	};
	struct _db_list_node *next_node;
} db_list_node;

typedef struct _db_tree_node
{
	union
	{
		void *data;
		char *str_val;
		long l_val;
		unsigned long ul_val;
	};
	struct _db_tree_node *child_node;
	struct _db_tree_node *next_node;
} db_tree_node;

typedef struct _db_path_data
{
	unsigned long hash;
	char *name;
	void *data; // other data
} db_path_data;

//typedef int (*DBLCF)(void *, void *);
//typedef int (*DBTCF)(void *, void *, void *, void *);
//typedef void (*DBLFF)(void *);

#define list_find				list_walk
#define list_bind_find			list_bind_walk
#define tree_find				tree_walk

int cb_by_count(void *data1, void *data2);
int cb_by_zero_count(void *data1, void *data2);
int cb_items_count(void *data1, void *data2);

db_list_node *list_insert(db_list_node **db_list, void *data);	// append to root
db_list_node *list_add(db_list_node **db_list, void *data);		// append to end (slowly)
db_list_node *list_walk(db_list_node *db_list, int (*cbcmp)(void *, void *), void *data);
int list_free(db_list_node **db_list, void (*cbfunc)(void *));
int list_free(db_list_node **db_list, void (* cbfunc)(void *, void *), void *data);

db_list_node **list_bind_find(db_list_node **db_list, int (*cbcmp)(void *, void *), void *data);
db_list_node *list_get(db_list_node *db_list, unsigned long n);
int list_del(db_list_node **db_list, void (*cbfree)(void *), int (*cbcmp)(void *, void *), void *data);
int list_del2(db_list_node **db_list, db_list_node *db_node, void (*cbfree)(void *));
int list_del_node(db_list_node **db_list, void (*cbfree)(void *));
db_list_node *list_sort(db_list_node **db_node, int (* cbfunc)(void *, void *));

void *stack_push(db_list_node **cl_stack, void *data);			// like list_insert
void *stack_pop(db_list_node **cl_stack);
void stack_free(db_list_node **cl_stack);						// like list_free(..., NULL)

db_tree_node *tree_insert(db_tree_node **bind_point, void *data);
db_tree_node *tree_add(db_tree_node **bind_point, void *data);
db_tree_node *tree_walk(db_tree_node *root_node, int (*cbcmp)(void *, void *, void *, void *), void *data);
db_tree_node *tree_list(db_tree_node *root_node, int (*cbcmp)(void *, void *, void *, void *), void *data);
db_tree_node **tree_bind_walk(db_tree_node **root_node, int (*cbcmp)(void *, void *, void *, void *), void *data);
int tree_free(db_tree_node **root_node, void (*cbfunc)(void *));
int tree_free(db_tree_node **root_node, void (* cbfunc)(void *, void *), void *data);

db_tree_node *tree_path_update(db_tree_node **root_node, char *path, void *data, void (*cbfunc)(void *));
db_tree_node *tree_path_find(db_tree_node *root_node, char *path);
int tree_path_free(db_tree_node **root_node, void (*cbfunc)(void *));

inline void *list_extract(db_list_node *db_list)
{
	return db_list?db_list->data:0L;
}

inline void *tree_extract(db_tree_node *db_list)
{
	return db_list?db_list->data:0L;
}

#endif //_ZLIST_H_
