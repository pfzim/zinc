#include "zl_vars.h"
#include <malloc.h>
#include <string.h>

zlval *zl_val_alloc(zlval **zl_tree)
{
	zlval *zl_val;
	zl_val = (zlval *) malloc(sizeof(zlval));
	if(zl_val)
	{
		memset(zl_val, 0, sizeof(zlval));

		if(zl_tree)
		{
			zl_val->next_node = *zl_tree;
			*zl_tree = zl_val;
		}
	}

	return zl_val;
}

zlval *zl_val_find(zlval *zl_tree, char *name)
{
	zlval *temp_node;
	temp_node = zl_tree;
	while(temp_node)
	{
		if(!strcmp(temp_node->name, name))
		{
			break;
		}

		temp_node = temp_node->next_node;
	}

	return temp_node;
}

unsigned long zl_val_free(zlval **zl_tree)
{
	zlval *temp_node;
	zlval *del_node;
	temp_node = *zl_tree;
	while(temp_node)
	{
		del_node = temp_node;
		temp_node = temp_node->next_node;

		free(del_node->name);
		free(del_node);
	}

	return 0;
}

