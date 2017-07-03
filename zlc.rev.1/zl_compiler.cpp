#include "zl_compiler.h"
#include "zl_opcodes.h"
#include <windows.h>
#include <stdio.h>
#include "../utstrings.h"
#include "strtod.h"

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
	{OP_RCALL,			INS_RCALL,		ARG_MEM,	ARG_NONE},
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
