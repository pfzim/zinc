#include "zl_compiler.h"
#include "zl_opcodes.h"
#include <windows.h>
#include <stdio.h>
#include "../utstrings.h"
#include "strtod.h"

/*
cl_var_node *cl_var_define(cl_var_node **vars_table, char *name, unsigned long level)
{
	// find for already defined variable at this level
	// add new if not found

	cl_var_node *temp_node;

	temp_node = *vars_table;
	
	while(temp_node && ((temp_node->level != level) || strcmp(temp_node->name, name)))
	{
		temp_node = temp_node->next_node;
	}

	if(temp_node)
	{
		return 0;
	}

	temp_node = (cl_var_node *) zalloc(sizeof(cl_var_node));
	if(temp_node)
	{
		memset(temp_node, 0, sizeof(cl_var_node));

		temp_node->name = alloc_string(name);
		temp_node->level = level;

		temp_node->next_node = *vars_table;
		*vars_table = temp_node;

		//defined outside???
		//temp_node->flags = 0;
		//temp_node->elements = 0;
		//temp_node->size = 0;
		//temp_node->offset = 0x00000000;
		
	}

	return temp_node;
}
*/

cl_var_node *cl_var_define(cl_var_node **vars_table, char *name, unsigned long level)
{
	// find for already defined variable at this level
	// add new if not found

	cl_var_node *temp_node;

	temp_node = *vars_table;
	
	while(temp_node && ((temp_node->level != level) || strcmp(temp_node->name, name)))
	{
		temp_node = temp_node->next_node;
	}

	if(temp_node)
	{
		return NULL;
	}

	temp_node = (cl_var_node *) zalloc(sizeof(cl_var_node));
	if(temp_node)
	{
		/*
		memset(temp_node, 0, sizeof(cl_var_node));

		temp_node->name = alloc_string(name);
		temp_node->level = level;
		*/

		//memcpy(temp_node, var_node, sizeof(cl_var_node));
		memset(temp_node, 0, sizeof(cl_var_node));

		temp_node->next_node = *vars_table;
		*vars_table = temp_node;

		//temp_node->level = level;

		//defined outside???
		//temp_node->flags = 0;
		//temp_node->elements = 0;
		//temp_node->size = 0;
		//temp_node->offset = 0x00000000;
	}

	return temp_node;
}

/*
cl_var_node *cl_var_define(cl_var_node **vars_table, cl_var_node *var_node)
{
	// find for already defined variable at this level
	// add new if not found

	cl_var_node *temp_node;

	temp_node = *vars_table;
	
	while(temp_node && ((temp_node->level != var_node->level) || strcmp(temp_node->name, var_node->name)))
	{
		temp_node = temp_node->next_node;
	}

	if(temp_node)
	{
		return 0;
	}

	temp_node = (cl_var_node *) zalloc(sizeof(cl_var_node));
	if(temp_node)
	{
		/ *
		memset(temp_node, 0, sizeof(cl_var_node));

		temp_node->name = alloc_string(name);
		temp_node->level = level;
		* /

		memcpy(temp_node, var_node, sizeof(cl_var_node));

		temp_node->next_node = *vars_table;
		*vars_table = temp_node;

		//defined outside???
		//temp_node->flags = 0;
		//temp_node->elements = 0;
		//temp_node->size = 0;
		//temp_node->offset = 0x00000000;
	}

	return temp_node;
}
*/

cl_var_node *cl_var_find(cl_var_node *vars_table, char *name)
{
	cl_var_node *temp_node;

	temp_node = NULL;

	while(vars_table)
	{
		if(!strcmp(vars_table->name, name) && (!temp_node || (temp_node->level < vars_table->level)))
		{
			temp_node = vars_table;
		}

		vars_table = vars_table->next_node;
	}

	return temp_node;
}

unsigned long cl_var_free_level(cl_var_node **vars_table, unsigned long level)
{
	cl_var_node **temp_node;
	cl_var_node *del_node;
	
	temp_node = vars_table;

	while(*temp_node)
	{
		if((*temp_node)->level == level)
		{
			del_node = *temp_node;
			*temp_node = (*temp_node)->next_node;

			// clear var_node here!!!
			free_str(del_node->name);

			zfree(del_node);
		}
		else
		{
			temp_node = &(*temp_node)->next_node;
		}
	}

	return 0L;
}

unsigned long cl_var_reference(cl_var_node *var_node, unsigned long offset)
{
	if(var_node->references >= 1024)
	{
		return 1L;
	}

	var_node->reference_offsets[var_node->references] = offset;
	var_node->references++;

	return 0L;
}

cl_label_node *cl_label_define(cl_label_node **labels_table, char *name)
{
	cl_label_node *temp_node;

	if(!isempty(name))
	{
		temp_node = *labels_table;

		while(temp_node && strcmp(temp_node->name, name))
		{
			temp_node = temp_node->next_node;
		}

		if(temp_node)
		{
			return temp_node;
		}
	}

	temp_node = (cl_label_node *) zalloc(sizeof(cl_label_node));
	if(temp_node)
	{
		memset(temp_node, 0, sizeof(cl_label_node));

		temp_node->name = alloc_string(name);
		temp_node->references = 0;

		temp_node->next_node = *labels_table;
		*labels_table = temp_node;
	}

	return temp_node;
}

cl_label_node *cl_label_find(cl_label_node *labels_table, char *name)
{
	while(labels_table && strcmp(labels_table->name, name))
	{
		labels_table = labels_table->next_node;
	}

	return labels_table;
}

unsigned long cl_label_reference(cl_label_node *label_node, unsigned long offset)
{
	if(label_node->references >= 1024)
	{
		return 1L;
	}

	label_node->reference_offsets[label_node->references] = offset;
	label_node->references++;

	return 0L;
}

unsigned long cl_label_fix(cl_label_node *labels_table, unsigned char *hard_code)
{
	unsigned long references;

	while(labels_table)
	{
		references = labels_table->references;

		if(references && (~labels_table->flags & ZLF_DEFINED))
		{
			return 1L;
		}

		while(references)
		{
			references--;
			hard_code[labels_table->reference_offsets[references]] = (char)((unsigned long) ((labels_table->offset - labels_table->reference_offsets[references]) & 0xFF));
			hard_code[labels_table->reference_offsets[references]+1] = (char)((unsigned long) (((labels_table->offset - labels_table->reference_offsets[references]) >> 8) & 0xFF));
			hard_code[labels_table->reference_offsets[references]+2] = (char)((unsigned long) (((labels_table->offset - labels_table->reference_offsets[references]) >> 16) & 0xFF));
			hard_code[labels_table->reference_offsets[references]+3] = (char)((unsigned long) ((labels_table->offset - labels_table->reference_offsets[references]) >> 24));
		}

		labels_table = labels_table->next_node;
	}

	return 0L;
}

unsigned long cl_label_free(cl_label_node **labels_table)
{
	cl_label_node **temp_node;
	cl_label_node *del_node;
	
	temp_node = labels_table;

	while(*temp_node)
	{
		del_node = *temp_node;
		*temp_node = (*temp_node)->next_node;

		free_str(del_node->name);
		free_str(del_node->library);

		zfree(del_node);
	}

	return 0L;
}

cl_data_node *cl_const_define(cl_data_node **data_table, char *data, unsigned long size, unsigned long reference)
{
	cl_data_node *temp_node;

	temp_node = *data_table;

	while(temp_node && ((temp_node->size != (size+1)) || memcmp(temp_node->data, data, size)))
	{
		temp_node = temp_node->next_node;
	}

	if(temp_node)
	{
		temp_node->reference_offsets[temp_node->references] = reference;
		temp_node->references++;

		return temp_node;
	}

	temp_node = (cl_data_node *) zalloc(sizeof(cl_label_node));
	if(temp_node)
	{
		memset(temp_node, 0, sizeof(cl_label_node));

		temp_node->data = (char *) zalloc(size+1);
		memcpy(temp_node->data, data, size);
		temp_node->data[size] = 0;
		temp_node->size = size+1;
		temp_node->references = 1;
		temp_node->reference_offsets[0] = reference;

		temp_node->next_node = *data_table;
		*data_table = temp_node;
	}

	return temp_node;
}

cl_data_node *cl_const_free(cl_data_node **data_table)
{
	cl_data_node *data_node;

	while(*data_table)
	{
		data_node = *data_table;
		*data_table = data_node->next_node;

		zfree(data_node->data);
		zfree(data_node);
	}

	return *data_table;
}

/*
unsigned long cl_const_build_fix_free(cl_data_node *const_table, unsigned char *hard_code, unsigned char **const_section, unsigned long *size)
{
	unsigned long references;
	unsigned long offset;
	cl_data_node *temp_node;

	temp_node = const_table;
	offset = 0;
	*const_section = NULL;

	while(temp_node)
	{
		references = temp_node->references;
		while(references)
		{
			references--;
			hard_code[temp_node->reference_offsets[references]] = (char)((unsigned long) (offset & 0xFF));
			hard_code[temp_node->reference_offsets[references]+1] = (char)((unsigned long) ((offset >> 8) & 0xFF));
			hard_code[temp_node->reference_offsets[references]+2] = (char)((unsigned long) ((offset >> 16) & 0xFF));
			hard_code[temp_node->reference_offsets[references]+3] = (char)((unsigned long) (offset >> 24));
		}

		offset += temp_node->size;

		temp_node = temp_node->next_node;
	}

	if(offset)
	{
		*const_section = (unsigned char *) zalloc(offset);
		offset = 0;

		if(*const_section)
		{
			while(const_table)
			{
				memcpy(*const_section + offset, const_table->data, const_table->size);
				offset += const_table->size;

				temp_node = const_table;
				const_table = const_table->next_node;

				zfree(temp_node->data);
				zfree(temp_node);
			}
		}
	}

	*size = offset;

	return offset;
}
*/

unsigned long cl_link_sections(
	unsigned long flags,				// global offset?
	cl_data_node *const_table,
	cl_var_node *vars_table,
	cl_label_node *funcs_table,
	unsigned char *hard_code,
	unsigned long hard_code_size,
	unsigned char **const_section,
	unsigned long *const_size,
	unsigned char **data_section,
	unsigned long *data_size,
	unsigned char **reloc_section,
	unsigned long *reloc_size,
	unsigned char **import_section,
	unsigned long *import_size,
	unsigned char **export_section,
	unsigned long *export_size,
	unsigned char **map_section,
	unsigned long *map_size
)
{
	cl_data_node *data_node;
	cl_var_node *vars_node;
	cl_label_node *funcs_node;
	unsigned long reference;
	unsigned long offset;
	unsigned long cursor;
	unsigned long msize;

	*const_section = NULL;
	*data_section = NULL;
	*reloc_section = NULL;
	*import_section = NULL;
	*export_section = NULL;
	*map_section = NULL;

	if(flags)
	{
		offset = hard_code_size;
	}
	else
	{
		offset = 0;
	}

	msize = 0;

	// .const section

	data_node = const_table;
	cursor = 0;

	while(data_node)
	{
		reference = data_node->references;
		while(reference)
		{
			reference--;
			hard_code[data_node->reference_offsets[reference]] = (char)((unsigned long) ((offset + cursor) & 0xFF));
			hard_code[data_node->reference_offsets[reference]+1] = (char)((unsigned long) (((offset + cursor) >> 8) & 0xFF));
			hard_code[data_node->reference_offsets[reference]+2] = (char)((unsigned long) (((offset + cursor) >> 16) & 0xFF));
			hard_code[data_node->reference_offsets[reference]+3] = (char)((unsigned long) ((offset + cursor) >> 24));
		}

		cursor += data_node->size;

		data_node = data_node->next_node;
	}

	if(cursor)
	{
		*const_section = (unsigned char *) zalloc(cursor);

		data_node = const_table;
		cursor = 0;

		if(*const_section)
		{
			while(data_node)
			{
				memcpy(*const_section + cursor, data_node->data, data_node->size);
				cursor += data_node->size;

				data_node = data_node->next_node;
			}
		}
	}

	*const_size = cursor;

	if(flags)
	{
		offset += cursor;
	}

	// .data section

	cursor = 0;
	vars_node = vars_table;

	while(vars_node)
	{
		if(~vars_node->flags & ZLF_EXTERNAL)
		{
			reference = vars_node->references;
			while(reference)
			{
				reference--;
				hard_code[vars_node->reference_offsets[reference]] = (char)((unsigned long) ((offset + cursor) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+1] = (char)((unsigned long) (((offset + cursor) >> 8) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+2] = (char)((unsigned long) (((offset + cursor) >> 16) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+3] = (char)((unsigned long) ((offset + cursor) >> 24));
			}

			cursor += vars_node->size[vars_node->rows];
		}

		vars_node = vars_node->next_node;
	}

	if(cursor)
	{
		*data_section = (unsigned char *) zalloc(cursor);
	}

	*data_size = cursor;

	if(flags)
	{
		offset += cursor;
	}

	// .reloc section

	cursor = 0;
	vars_node = vars_table;

	while(vars_node)
	{
		if(vars_node->flags & ZLF_EXTERNAL)
		{
			reference = vars_node->references;
			while(reference)
			{
				reference--;
				hard_code[vars_node->reference_offsets[reference]] = (char)((unsigned long) ((offset + cursor) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+1] = (char)((unsigned long) (((offset + cursor) >> 8) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+2] = (char)((unsigned long) (((offset + cursor) >> 16) & 0xFF));
				hard_code[vars_node->reference_offsets[reference]+3] = (char)((unsigned long) ((offset + cursor) >> 24));
			}

			cursor += 4;
			msize += 360;
		}

		vars_node = vars_node->next_node;
	}

	if(cursor)
	{
		*reloc_section = (unsigned char *) zalloc(cursor);
	}

	*reloc_size = cursor;

	if(flags)
	{
		offset += cursor;
	}

	// .import section
	//		4 bytes - address
	//		4 bytes - params count
	//		4 bytes - call type [stdcall | cdecl]

	cursor = 0;
	funcs_node = funcs_table;

	while(funcs_node)
	{
		if(~funcs_node->flags & ZLF_FUNC_INTERNAL)
		{
			reference = funcs_node->references;
			while(reference)
			{
				reference--;
				hard_code[funcs_node->reference_offsets[reference]] = (char)((unsigned long) ((offset + cursor) & 0xFF));
				hard_code[funcs_node->reference_offsets[reference]+1] = (char)((unsigned long) (((offset + cursor) >> 8) & 0xFF));
				hard_code[funcs_node->reference_offsets[reference]+2] = (char)((unsigned long) (((offset + cursor) >> 16) & 0xFF));
				hard_code[funcs_node->reference_offsets[reference]+3] = (char)((unsigned long) ((offset + cursor) >> 24));
			}

			cursor += 12;
			msize += 360;
		}

		funcs_node = funcs_node->next_node;
	}

	if(cursor)
	{
		*import_section = (unsigned char *) zalloc(cursor);
		if(*import_section)
		{
			cursor = 0;
			funcs_node = funcs_table;

			while(funcs_node)
			{
				if(~funcs_node->flags & ZLF_FUNC_INTERNAL)
				{
					*((unsigned long *) (*import_section + cursor + 4)) = funcs_node->params;
					*((unsigned long *) (*import_section + cursor + 8)) = (funcs_node->flags & ZLF_STDCALL)?0:1;
					cursor += 12;
				}

				funcs_node = funcs_node->next_node;
			}
		}
	}

	*import_size = cursor;

	if(flags)
	{
		offset += cursor;
	}

	// .export section
	//		4 bytes - offset
	//		4 bytes - params count
	//		100 bytes - name

	cursor = 0;
	funcs_node = funcs_table;

	while(funcs_node)
	{
		if(funcs_node->flags & ZLF_FUNC_INTERNAL)
		{
			cursor += 108;
		}

		funcs_node = funcs_node->next_node;
	}

	if(cursor)
	{
		*export_section = (unsigned char *) zalloc(cursor);

		if(*export_section)
		{
			memset(*export_section, 0, cursor);

			cursor = 0;
			funcs_node = funcs_table;

			while(funcs_node)
			{
				if(funcs_node->flags & ZLF_FUNC_INTERNAL)
				{
					*((unsigned long *) (*export_section + cursor)) = funcs_node->offset;
					*((unsigned long *) (*export_section + cursor + 4)) = funcs_node->params;
					strncpy_tiny((char *) (*export_section + cursor + 8), funcs_node->name, 99);
					cursor += 108;
				}

				funcs_node = funcs_node->next_node;
			}
		}
	}

	*export_size = cursor;

	// .map section
	//		4 bytes - flags [.reloc | .import]
	//		100 bytes - name
	//		256 bytes - library

	if(cursor)
	{
		*map_section = (unsigned char *) zalloc(msize);
		*map_size = msize;

		if(*map_section)
		{
			memset(*map_section, 0, msize);

			cursor = 0;
			vars_node = vars_table;

			while(vars_node)
			{
				if(vars_node->flags & ZLF_EXTERNAL)
				{
					*((unsigned long *) (*map_section + cursor)) = 0;
					strncpy_tiny((char *) (*map_section + cursor + 4), vars_node->name, 99);
					cursor += 360;
				}

				vars_node = vars_node->next_node;
			}

			funcs_node = funcs_table;

			while(funcs_node)
			{
				if(~funcs_node->flags & ZLF_FUNC_INTERNAL)
				{
					*((unsigned long *) (*map_section + cursor)) = 1;
					strncpy_tiny((char *) (*map_section + cursor + 4), funcs_node->name, 99);
					if(!isempty(funcs_node->library))
					{
						strncpy_tiny((char *) (*map_section + cursor + 104), funcs_node->library, 255);
					}

					cursor += 360;
				}

				funcs_node = funcs_node->next_node;
			}
		}
	}

	return 0;
}

/*
unsigned long cl_export_build_free(cl_data_node *funcs_table, unsigned char *hard_code, unsigned char **export_section, unsigned long *size)
{
	unsigned long references;
	unsigned long offset;
	cl_data_node *temp_node;

	temp_node = const_table;
	offset = 0;
	*const_section = NULL;

	while(temp_node)
	{
		references = const_table->references;
		while(references)
		{
			references--;
			hard_code[temp_node->reference_offsets[references]] = (char)((unsigned long) (offset & 0xFF));
			hard_code[temp_node->reference_offsets[references]+1] = (char)((unsigned long) ((offset >> 8) & 0xFF));
			hard_code[temp_node->reference_offsets[references]+2] = (char)((unsigned long) ((offset >> 16) & 0xFF));
			hard_code[temp_node->reference_offsets[references]+3] = (char)((unsigned long) (offset >> 24));
		}

		offset += temp_node->size;

		temp_node = temp_node->next_node;
	}

	if(offset)
	{
		*const_section = (unsigned char *) zalloc(offset);
		offset = 0;

		if(*const_section)
		{
			while(const_table)
			{
				memcpy(*const_section + offset, const_table->data, const_table->size);
				offset += const_table->size;

				temp_node = const_table;
				const_table = const_table->next_node;

				zfree(temp_node->data);
				zfree(temp_node);
			}
		}
	}

	*size = offset;

	return offset;
}

/*
db_list_node *cl_section_data_push(db_list_node **section, unsigned long flags, char *data, unsigned long size)
{
	zl_section_node *temp_node;

	temp_node = (zl_section_node *) zalloc(sizeof(zl_section_node));
	if(temp_node)
	{
		//temp_node->name = alloc_string(name);
		if(data && !size)
		{
			temp_node->size = strlen(data) + 1;
		}
		else
		{
			temp_node->size = size;
		}

		temp_node->flags = flags;

		temp_node->data = zalloc(temp_node->size);
		if(temp_node->data)
		{
			memcpy(temp_node->data, data, temp_node->size);
		}

		temp_node->reference_offsets = NULL;

		return list_add(section, temp_node);
	}

	return NULL;
}

  db_list_node *cl_reference_add(db_list_node **refs, unsigned long offset)
{
	return list_add(refs, (void *) offset);
}

//*/

// *** revision 1 ************************

/*
unsigned long cl_data_add(cl_parser_params *pp, unsigned char *data, unsigned long size)
{
	unsigned long offset;

	if((pp->data_fill + size) > pp->data_buffer_size)
	{
		unsigned char *new_buf;
		pp->data_buffer_size = pp->data_fill + size + size%65536;
		new_buf = (unsigned char *) zalloc(pp->data_buffer_size);
		if(pp->data_table)
		{
			memcpy(new_buf, pp->data_table, pp->data_fill);
			zfree(pp->data_table);
		}
		pp->data_table = new_buf;
	}

	memcpy((void *)(((unsigned long)pp->data_table) + pp->data_fill), data, size);

	offset = pp->data_fill;
	pp->data_fill += size;

	return offset;
}
//*/

unsigned long cl_code_add(cl_parser_params *pp, unsigned char *data, unsigned long size)
{
	unsigned long offset;

	if((pp->hc_fill + size) > pp->hc_buffer_size)
	{
		unsigned char *new_buf;
		pp->hc_buffer_size = pp->hc_fill + size + size%65536;
		new_buf = (unsigned char *) zalloc(pp->hc_buffer_size);
		if(pp->hard_code)
		{
			memcpy(new_buf, pp->hard_code, pp->hc_fill);
			zfree(pp->hard_code);
		}
		pp->hard_code = new_buf;
	}

	memcpy((void *)(((unsigned long)pp->hard_code) + pp->hc_fill), data, size);

	offset = pp->hc_fill;
	pp->hc_fill += size;

	return offset;
}

/*
zl_names_map *cl_label_define(zl_names_map **labels_table, char *name, unsigned long offset)
{
	zl_names_map *temp_node, *last_node;

	temp_node = (zl_names_map *) zalloc(sizeof(zl_names_map));
	if(!temp_node)
	{
		return 0L;
	}

	temp_node->name = cl_strdup(name);
	temp_node->offset = offset;
	temp_node->next_node = NULL;

	if(*labels_table)
	{
		last_node = *labels_table;
		while(last_node->next_node)
		{
			if(!strcmp(last_node->name, name))
			{
				free_str(temp_node->name);
				zfree(temp_node);
				return 0L;
			}
			last_node = last_node->next_node;
		}

		if(!strcmp(last_node->name, name))
		{
			free_str(temp_node->name);
			zfree(temp_node);
			return 0L;
		}
		last_node->next_node = temp_node;
	}
	else
	{
		*labels_table = temp_node;
	}

	return temp_node;
}

zl_names_map *cl_jump_define(zl_names_map **labels_table, char *name, unsigned long offset)
{
	zl_names_map *temp_node, *last_node;

	temp_node = (zl_names_map *) zalloc(sizeof(zl_names_map));
	if(!temp_node)
	{
		return 0L;
	}

	temp_node->name = cl_strdup(name);
	temp_node->offset = offset;
	temp_node->next_node = NULL;

	if(*labels_table)
	{
		last_node = *labels_table;
		while(last_node->next_node)
		{
			last_node = last_node->next_node;
		}
		last_node->next_node = temp_node;
	}
	else
	{
		*labels_table = temp_node;
	}

	return temp_node;
}

void cl_names_free(zl_names_map **labels_table)
{
	zl_names_map *temp_node;

	while(*labels_table)
	{
		temp_node = *labels_table;
		*labels_table = temp_node->next_node;

		// Наполнить. Не всё очищается.
		zfree(temp_node->name);
		zfree(temp_node);
	}
}

unsigned long cl_jump_fix(unsigned char *hardcode, zl_names_map *jumps_table, zl_names_map *labels_table)
{
	zl_names_map *temp_node;
	while(jumps_table)
	{
		temp_node = labels_table;
		while(temp_node)
		{
			if(!strcmp(jumps_table->name, temp_node->name))
			{
				hardcode[jumps_table->offset] = (char)((unsigned long) (temp_node->offset & 0xFF));
				hardcode[(jumps_table->offset)+1] = (char)((unsigned long) ((temp_node->offset >> 8) & 0xFF));
				hardcode[(jumps_table->offset)+2] = (char)((unsigned long) ((temp_node->offset >> 16) & 0xFF));
				hardcode[(jumps_table->offset)+3] = (char)((unsigned long) (temp_node->offset >> 24));
				break;
			}
			temp_node = temp_node->next_node;
		}

		if(!temp_node)
		{
			return 1L;
		}

		jumps_table = jumps_table->next_node;
	}

	return 0L;
}

zl_names_map *cl_label_new(zl_names_map **jumps_table, unsigned long offset)
{
	zl_names_map *temp_node;
	unsigned long last_label;

	temp_node = *jumps_table;
	last_label = 0;
	while(temp_node)
	{
		temp_node = temp_node->next_node;
		last_label++;
	}

	temp_node = (zl_names_map *) zalloc(sizeof(zl_names_map));
	if(temp_node)
	{
		temp_node->name = cl_sprintf("@%u", last_label);
		temp_node->offset = offset;
		temp_node->next_node = *jumps_table;
		*jumps_table = temp_node;
	}

	return temp_node;
}
*/

void *cl_stack_push(cl_stack_buffer **cl_stack, void *data)
{
	cl_stack_buffer *temp_node;

	temp_node = (cl_stack_buffer *) zalloc(sizeof(cl_stack_buffer));
	if(temp_node)
	{
		temp_node->data = data;
		temp_node->next_node = *cl_stack;
		*cl_stack = temp_node;
	}

	return data;
}

void *cl_stack_pop(cl_stack_buffer **cl_stack)
{
	void *data;
	cl_stack_buffer *temp_node;

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

void cl_stack_free(cl_stack_buffer **cl_stack)
{
	cl_stack_buffer *temp_node;

	while(*cl_stack)
	{
		temp_node = *cl_stack;
		*cl_stack = temp_node->next_node;

		zfree(temp_node);
	}
}

/*
unsigned long cl_var_define(zl_names_map **vars_table, char *name, unsigned long flags, zl_names_map **out)
{
	zl_names_map *temp_node, *last_node;
	unsigned long offset;

	offset = 1;
	if(*vars_table)
	{
		last_node = *vars_table;
		while(last_node->next_node)
		{
			if(name && (strcmp(name, last_node->name) == 0))
			{
				return 0;
			}
			offset++;
			last_node = last_node->next_node;
		}

		if(name && (strcmp(name, last_node->name) == 0))
		{
			return 0;
		}

		offset++;
	}

	temp_node = (zl_names_map *) zalloc(sizeof(zl_names_map));
	if(!temp_node)
	{
		return 0;
	}

	memset(temp_node, 0, sizeof(zl_names_map));

	if(!isempty(name))
	{
		temp_node->name = cl_strdup(name);
	}
	else
	{
		temp_node->name = nullstring;
	}

	temp_node->flags = flags;
	//temp_node->next_node = NULL;

	if(*vars_table)
	{
		last_node->next_node = temp_node;
	}
	else
	{
		*vars_table = temp_node;
	}

	if(out)
	{
		*out = temp_node;
	}

	return offset;
}

zl_names_map *cl_var_find(zl_names_map *vars_table, char *name, unsigned long *val)
{
	unsigned long offset;

	offset = 0;
	while(vars_table && strcmp(vars_table->name, name))
	{
		offset++;
		vars_table = vars_table->next_node;
	}

	if(val)
	{
		*val = offset*4;
	}

	return vars_table;
}

zl_funcs_list *cl_func_find(zl_funcs_list *funcs, char *name, unsigned long *val)
{
	unsigned long offset;

	offset = 0;
	while(funcs[offset].name && strcmp(funcs[offset].name, name))
	{
		offset++;
	}

	if(val)
	{
		*val = offset*16;
	}

	if(funcs[offset].name)
	{
		return &funcs[offset];
	}

	return 0L;
}

unsigned long cl_find_var_or_func(zl_names_map *vars_table, zl_funcs_list *funcs, char *name)
{
	unsigned long offset;

	if(cl_func_find(funcs, name, &offset) || cl_var_find(vars_table, name, &offset))
	{
		return offset;
	}

	return 1;
}
*/

unsigned long cl_push(cl_parser_params *pp, unsigned char code)
{
	return cl_code_add(pp, &code, 1);
}

unsigned long cl_push_dw(cl_parser_params *pp, unsigned long data)
{
	//cl_push(hardcode, (char)(data & 0xFF));
	//cl_push(hardcode, (char)(data >> 8 & 0xFF));
	//cl_push(hardcode, (char)(data >> 16 & 0xFF));
	//cl_push(hardcode, (char)(data >> 24));

	return cl_code_add(pp, (unsigned char *) &data, 4);
}

//*
zl_opcodes_list zl_opcodes[] = {
	{OP_PUSH_IMM,		INS_PUSH,		ARG_IMM,	ARG_NONE},
	{OP_PUSH_REG,		INS_PUSH,		ARG_REG,	ARG_NONE},
	{OP_PUSH_MEM,		INS_PUSH,		ARG_MEM,	ARG_NONE},
	{OP_PUSH_PMEM,		INS_PUSH,		ARG_PMEM,	ARG_NONE},
	{OP_PUSH_OFFSET,	INS_PUSH,		ARG_DATA,	ARG_NONE},

	{OP_POP_REG,		INS_POP,		ARG_REG,	ARG_NONE},
	{OP_POP_MEM,		INS_POP,		ARG_MEM,	ARG_NONE},

	{OP_MOV_REG_REG,	INS_MOV,		ARG_REG,	ARG_REG},
	{OP_MOV_REG_IMM,	INS_MOV,		ARG_REG,	ARG_IMM},
	{OP_MOV_REG_MEM,	INS_MOV,		ARG_REG,	ARG_PMEM},
	{OP_MOV_MEM_IMM,	INS_MOV,		ARG_PMEM,	ARG_IMM},
	{OP_MOV_MEM_REG,	INS_MOV,		ARG_PMEM,	ARG_REG},
	{OP_MOV_MEM_MEM,	INS_MOV,		ARG_PMEM,	ARG_PMEM},
	{OP_MOV_REG_PREG,	INS_MOV,		ARG_REG,	ARG_PREG},
	{OP_MOV_PREG_REG,	INS_MOV,		ARG_PREG,	ARG_REG},

	{OP_ADD_REG_IMM,	INS_ADD,		ARG_REG,	ARG_IMM},
	{OP_ADD_REG_REG,	INS_ADD,		ARG_REG,	ARG_REG},
	{OP_ADD_REG_MEM,	INS_ADD,		ARG_REG,	ARG_PMEM},
	{OP_ADD_MEM_IMM,	INS_ADD,		ARG_PMEM,	ARG_IMM},
	{OP_ADD_MEM_REG,	INS_ADD,		ARG_PMEM,	ARG_REG},
	{OP_ADD_MEM_MEM,	INS_ADD,		ARG_PMEM,	ARG_PMEM},

	{OP_SUB_REG_IMM,	INS_SUB,		ARG_REG,	ARG_IMM},
	{OP_SUB_REG_REG,	INS_SUB,		ARG_REG,	ARG_REG},
	{OP_SUB_REG_MEM,	INS_SUB,		ARG_REG,	ARG_PMEM},
	{OP_SUB_MEM_IMM,	INS_SUB,		ARG_PMEM,	ARG_IMM},
	{OP_SUB_MEM_REG,	INS_SUB,		ARG_PMEM,	ARG_REG},
	{OP_SUB_MEM_MEM,	INS_SUB,		ARG_PMEM,	ARG_PMEM},

	{OP_CMP_REG_IMM,	INS_CMP,		ARG_REG,	ARG_IMM},
	{OP_CMP_REG_REG,	INS_CMP,		ARG_REG,	ARG_REG},
	{OP_CMP_REG_MEM,	INS_CMP,		ARG_REG,	ARG_PMEM},
	{OP_CMP_MEM_IMM,	INS_CMP,		ARG_PMEM,	ARG_IMM},
	{OP_CMP_MEM_REG,	INS_CMP,		ARG_PMEM,	ARG_REG},
	{OP_CMP_MEM_MEM,	INS_CMP,		ARG_PMEM,	ARG_PMEM},

	{OP_TEST_REG_REG,	INS_TEST,		ARG_REG,	ARG_REG},

	{OP_MUL_REG_REG,	INS_MUL,		ARG_REG,	ARG_REG},
	{OP_DIV_REG_REG,	INS_DIV,		ARG_REG,	ARG_REG},
	{OP_MOD_REG_REG,	INS_MOD,		ARG_REG,	ARG_REG},
	{OP_SHL_REG_REG,	INS_SHL,		ARG_REG,	ARG_REG},
	{OP_SHR_REG_REG,	INS_SHR,		ARG_REG,	ARG_REG},
	{OP_OR_REG_REG,		INS_OR,			ARG_REG,	ARG_REG},
	{OP_AND_REG_REG,	INS_AND,		ARG_REG,	ARG_REG},
	{OP_XOR_REG_REG,	INS_XOR,		ARG_REG,	ARG_REG},

	{OP_INC_REG,		INS_INC,		ARG_REG,	ARG_NONE},
	{OP_DEC_REG,		INS_DEC,		ARG_REG,	ARG_NONE},

	{OP_CALL,			INS_CALL,		ARG_MEM,	ARG_NONE},
//	{OP_RCALL,			INS_RCALL,		ARG_MEM,	ARG_NONE},
	{OP_DBG_PRINT,		INS_DBG_PRINT,	ARG_NONE,	ARG_NONE},
	{OP_NOP,			INS_NOP,		ARG_NONE,	ARG_NONE},

//	{OP_PUSH_RM32,		INS_PUSH,		REG6,	RMEM},
//	{OP_PUSH_RD,		INS_PUSH,		_REG,	NONE},
//	{OP_PUSH_RD,		INS_PUSH,		IMM,	NONE},
//	{OP_PUSH_RD,		INS_PUSH,		_REG,	NONE},

	{NULL,				NULL,		NULL,	NULL}
};

unsigned long cl_push_op(cl_parser_params *pp, unsigned char code, zlval *arg1, zlval *arg2)
{
//	T_REGISTER, T_REGISTER	= _REG;
//	T_REGISTER, NONE		= REG0 | REG1 | REG2 | REG4 | REG5 | REG6 | REG7 = one of MODRM;
//	T_INTEGER, NONE			= IMM;

	unsigned long i;
	i = 0;

	while(	zl_opcodes[i].opcode
			&&	(zl_opcodes[i].group != code
				|| (arg1 && zl_opcodes[i].arg1 != arg1->flags)
				|| (arg2 && zl_opcodes[i].arg2 != arg2->flags)
				)
	)
	{
		i++;
	}

	if(zl_opcodes[i].opcode)
	{
		cl_code_add(pp, &(zl_opcodes[i].opcode), ZL_INSTRUCTION_LENGTH);
		if(arg1)
		{
			switch(arg1->flags)
			{
				case ARG_REG:
					i = 1;
					break;
				default:
					i = 4;
			}
			
			cl_code_add(pp, (unsigned char *) &(arg1->value), i);
		}

		if(arg2)
		{
			switch(arg2->flags)
			{
				case ARG_REG:
					i = 1;
					break;
				default:
					i = 4;
			}

			cl_code_add(pp, (unsigned char *) &(arg2->value), i);
		}
		return 0L;
		//return cl_data_add(hardcode, &(zl_opcodes[i].opcode), 1);
	}
//*/
	return 0L;
}

long cl_strtol(char *text, unsigned long len)
{
	return strtol(text, NULL, 10);
}

unsigned long cl_strtoul(char *text, unsigned long len)
{
	return strtoul(text, NULL, 16);
}

double cl_strtod(char *text, unsigned long len)
{
	return strtod(text, NULL);
}

char *cl_strndup(char *text, unsigned long len)
{
	return alloc_strncpy(text, len);
}

char *cl_strdup(char *text)
{
	return alloc_string(text);
}

char *cl_sprintf(char *fmt, ...)
{
	va_list marker;
	char *temp;

	va_start(marker, fmt);
	temp = (char *) zalloc(1024);
	vsprintf(temp, fmt, marker);
	va_end(marker);

	return temp;
}

unsigned long cl_do_op(cl_parser_params *pp, unsigned char op, zlval *ss, zlval *s1, zlval *s2)
{
	cl_push(pp, op);

	return 0L;
}
