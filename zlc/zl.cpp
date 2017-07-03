//#include "stdafx.h"
#include "zl.h"
#include "zl_opcodes.h"
#include <string.h>
//#include <zalloc.h>
#include <stdio.h>
#include "../utstrings.h"
#include <windows.h>

unsigned long zl_load_functions(unsigned char *import_section, zl_map_section *map_section, unsigned long count, zl_funcs_list *fn_list, unsigned long **modules)
{
	// на выходе возвращает список библиотек, которые надо освобождать

	unsigned long i, x, cursor, offset;

	cursor = 0;
	x = 0;
	offset = 0;

	if(modules)
	{
		*modules = (unsigned long *) zalloc(4);
		**modules = NULL;
	}

	while(count)
	{
		count--;
		if(map_section[x].flags)
		{
			if(map_section[x].library[0] != '\0')
			{
				if(modules)
				{
					pdw(dw(*modules) + offset) = dw(LoadLibrary(map_section[x].library));
					if(pdw(dw(*modules) + offset))
					{
						*modules = (unsigned long *) zrealloc(*modules, offset+8);

						*((unsigned long *)(import_section + cursor)) = dw(GetProcAddress((HMODULE) pdw(dw(*modules) + offset), map_section[x].name));

						offset += 4;
						pdw(dw(*modules) + offset) = NULL;
					}
				}
			}
			else
			{
				if(strcmp(map_section[x].name, "LoadLibrary") == 0)
				{
					*((unsigned long *)(import_section + cursor)) = (unsigned long) LoadLibrary;
				}
				else if(strcmp(map_section[x].name, "GetProcAddress") == 0)
				{
					*((unsigned long *)(import_section + cursor)) = (unsigned long) GetProcAddress;
				}
				else
				{
					i = 0;
					while(fn_list[i].name)
					{
						if(strcmp(fn_list[i].name, map_section[x].name) == 0)
						{
							*((unsigned long *)(import_section + cursor)) = (unsigned long) fn_list[i].proc;
							//*((unsigned long *)(import_section + cursor + 4)) = fn_list[i].params;
							//*((unsigned long *)(import_section + cursor + 8)) = fn_list[i].flags;
							break;
						}
						i++;
					}
				}
			}
			cursor += 12;
		}
		x++;
	}

	return 0;
}

unsigned long zl_unload_modules(unsigned long *modules)
{
	HMODULE *hModules;

	if(modules)
	{
		hModules = (HMODULE *) modules;
		while(*hModules)
		{
			FreeLibrary(*hModules);
			hModules = (HMODULE *) (dw(hModules) + 4);
		}

		zfree(modules);
	}

	return 0L;
}

unsigned long zl_set(unsigned char *reloc_section, zl_map_section *map_section, unsigned long count, char *name, void *ext_var)
{
	unsigned long x, cursor;

	cursor = 0;
	x = 0;
	while(count)
	{
		count--;
		if(!map_section[x].flags)
		{
			if(strcmp(name, map_section[x].name) == 0)
			{
				*((unsigned long *)(reloc_section + cursor)) = (unsigned long) ext_var;
				return 0;
			}
			cursor += 4;
		}
		x++;
	}

	return 1;
}

/* revision 1
unsigned long zl_set(void **maptable, zl_names_map *vars_map, char *name, void *ext_var)
{
	unsigned long offset;

	offset = 0;
	while(vars_map && strcmp(name, vars_map->name))
	{
		vars_map = vars_map->next_node;
		offset += 4;
	}

	if(vars_map)
	{
		pdw(dw(maptable) + offset) = dw(ext_var);
		//pdw(dw(maptable) + vars_map->offset*4) = dw(ext_var);
		return 0;
	}

	return 1;
}

unsigned long zl_memtable_make2(void ***mem_table, zl_names_map *vars_map)
{
	unsigned long vt_size;
	unsigned long count;
	unsigned long *vars_table;
	zl_names_map *temp_node;

	*mem_table = NULL;

	count = 0;
	vt_size = 0;
	temp_node = vars_map;

	while(temp_node)
	{
		count++;
		vt_size += temp_node->size;
		temp_node = temp_node->next_node;
	}

	if(!count)
	{
		return 0L;
	}

	*mem_table = (void **) zalloc(count * (sizeof(void *) + vt_size));
	if(*mem_table)
	{
		vars_table = (unsigned long *)(dw(*mem_table) + count * sizeof(void *));
		memset(vars_table, 0, vt_size);

		count = 0;
		vt_size = 0;
		temp_node = vars_map;

		while(temp_node)
		{
			//vars_table[vt_size] = 0;
			(*mem_table)[count] = (void *)(dw(vars_table)+vt_size);

			count++;
			vt_size += temp_node->size;
			temp_node = temp_node->next_node;
		}
	}

	return 0L;
}

unsigned long zl_memtable_make(void ***mem_table, unsigned long **vars_table, unsigned long count)
{
	*mem_table = NULL;
	*vars_table = NULL;

	if(!count)
	{
		return 0L;
	}

	*mem_table = (void **) zalloc(count * (sizeof(void *) + sizeof(unsigned long)));
	if(*mem_table)
	{
		*vars_table = (unsigned long *)(dw(*mem_table) + count * sizeof(void *));

		unsigned long i;
		for(i = 0; i < count; i++)
		{
			(*vars_table)[i] = 0;
			(*mem_table)[i] = &((*vars_table)[i]);
		}
	}

	return 0L;
}
*/

/* revision 1
unsigned long zl_free(unsigned char **hardcode, void ***mem_table, unsigned char **data_table, zl_names_map **mapvars)
{
	if(mem_table && *mem_table)
	{
		zfree(*mem_table);
		*mem_table = NULL;
	}

	if(hardcode && *hardcode)
	{
		zfree(*hardcode);
		*hardcode = NULL;
	}

	if(data_table && *data_table)
	{
		zfree(*data_table);
		*data_table = NULL;
	}

	if(mapvars)
	{
		zl_names_map *temp_node;
		
		while(*mapvars)
		{
			temp_node = (*mapvars)->next_node;
			free_str((*mapvars)->name);
			zfree(*mapvars);
			*mapvars = temp_node;
		}
	}

	return 0L;
}
*/

unsigned long zl_free(unsigned char *hardcode, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect, unsigned char *export_sect, unsigned char *map_sect)
{
	if(isalloc(hardcode))
	{
		zfree(hardcode);
	}
	
	if(isalloc(data_sect))
	{
		zfree(data_sect);
	}
	
	if(isalloc(const_sect))
	{
		zfree(const_sect);
	}
	
	if(isalloc(reloc_sect))
	{
		zfree(reloc_sect);
	}
	
	if(isalloc(import_sect))
	{
		zfree(import_sect);
	}

	if(isalloc(export_sect))
	{
		zfree(export_sect);
	}

	if(isalloc(map_sect))
	{
		zfree(map_sect);
	}

	return 0L;
}

unsigned long zl_offset(char *name, zl_export_section *export_section, unsigned long count)
{
	while(count)
	{
		count--;
		if(strcmp(name, export_section[count].name) == 0)
		{
			return export_section[count].offset;
		}
	}

	return 0xFFFFFFFFL;
}

unsigned long zl_init(unsigned long offset, unsigned char *hardcode, unsigned long *stack, unsigned long *regs, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect)
{
	for(unsigned long i = 0; i < REG_COUNT; i++)
	{
		regs[i] = 0L;
	}

	regs[REG_EIP] = dw(hardcode) + offset;
	regs[REG_ESP] = dw(stack);

	if(!const_sect)
	{
		regs[SECT_CONST] = dw(hardcode);
	}
	else
	{
		regs[SECT_CONST] = dw(const_sect);
	}

	if(!data_sect)
	{
		regs[SECT_DATA] = dw(hardcode);
	}
	else
	{
		regs[SECT_DATA] = dw(data_sect);
	}

	if(!reloc_sect)
	{
		regs[SECT_RELOC] = dw(hardcode);
	}
	else
	{
		regs[SECT_RELOC] = dw(reloc_sect);
	}

	if(!import_sect)
	{
		regs[SECT_IMPORT] = dw(hardcode);
	}
	else
	{
		regs[SECT_IMPORT] = dw(import_sect);
	}

	if(offset == 0xFFFFFFFFL)
	{
		return 0xFFFFFFFFL;
	}

	return 0L;
}

unsigned long zl_push(unsigned long *regs, unsigned long value)
{
	push(regs[REG_ESP]);
	pdw(regs[REG_ESP]) = value;

	return regs[REG_ESP];
}

unsigned long zl_pop(unsigned long *regs, unsigned long value)
{
	regs[REG_ESP] -= value*4;

	return regs[REG_ESP];
}

unsigned long zl_call(unsigned long *regs)
{
	unsigned char end_code;

	end_code = OP_EOF;

	// in this order
	// pdw(zl_esp) = param1;
	// push(zl_esp)
	// pdw(zl_esp) = param2;
	// push(zl_esp)
	// pdw(zl_esp) = param3;
	// push(zl_esp)

	push(regs[REG_ESP]);
	pdw(regs[REG_ESP]) = dw(&end_code);

	zl_execute(regs);

	// sub esp, params_count*4

	return regs[REG_EAX];
}

//unsigned long zl_execute(unsigned long *regs, unsigned char *const_sect, unsigned char *data_sect, unsigned char *reloc_sect, unsigned char *import_sect)
unsigned long zl_execute(unsigned long *regs)
{
	unsigned long &zl_eflags = regs[ZL_EFLAGS];
	unsigned long &zl_eip = regs[REG_EIP];
	unsigned long &zl_esp = regs[REG_ESP];

	/*
	unsigned char *&const_sect = (unsigned char *&) regs[SECT_CONST];
	unsigned char *&data_sect = (unsigned char *&) regs[SECT_DATA];
	unsigned char *&reloc_sect = (unsigned char *&) regs[SECT_RELOC];
	unsigned char *&import_sect = (unsigned char *&) regs[SECT_IMPORT];
	*/

	unsigned long zl_over;

	while(instr(zl_eip))
	{
		switch(instr(zl_eip))
		{
			case OP_SIZE_OVERRIDE_1:
				zl_over = 1;
				nextinstr(zl_eip);
				break;
			case OP_SIZE_OVERRIDE_2:
				zl_over = 2;
				nextinstr(zl_eip);
				break;
			default:
				zl_over = 0;
		}

		switch(instr(zl_eip))
		{
			case OP_PUSH_REG:
				nextinstr(zl_eip);
				push(zl_esp);
				pdw(zl_esp) = regs[pdb(zl_eip)];
				zl_eip++;
				continue;
			case OP_PUSH_IMM:
				nextinstr(zl_eip);
				push(zl_esp);
				pdw(zl_esp) = pdw(zl_eip);
				zl_eip += 4;
				continue;
			case OP_PUSH_MEM:
				nextinstr(zl_eip);
				push(zl_esp);
				//pdw(zl_esp) = mem(zl_eip);	// revision 1
				pdw(zl_esp) = dw(dw(regs[SECT_DATA]) + dw(pdw(zl_eip)));
				zl_eip += 4;
				continue;
			case OP_PUSH_PMEM:
				nextinstr(zl_eip);
				push(zl_esp);
				//pdw(zl_esp) = pmem(zl_eip);	// revision 1
				pdw(zl_esp) = pdw(dw(regs[SECT_RELOC]) + dw(pdw(zl_eip)));
				zl_eip += 4;
				continue;
			case OP_PUSH_OFFSET:
				nextinstr(zl_eip);
				push(zl_esp);
				//pdw(zl_esp) = dw(datatable + pdw(zl_eip)); // revision 1
				pdw(zl_esp) = dw(regs[SECT_CONST] + pdw(zl_eip));
				zl_eip += 4;
				continue;

			case OP_POP_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_POP_MEM:
				nextinstr(zl_eip);
				mem(zl_eip) = pdw(zl_esp);
				pop(zl_esp);
				zl_eip += 4;
				continue;

			case OP_ADD_REG_IMM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] += pdw(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_ADD_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] += regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_ADD_REG_MEM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] += mem(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_ADD_MEM_IMM:
				nextinstr(zl_eip);
				mem(zl_eip) += pdw(zl_eip+4);
				zl_eip += 8;
				continue;
			case OP_ADD_MEM_REG:
				nextinstr(zl_eip);
				mem(zl_eip) += regs[pdb(zl_eip+4)];
				zl_eip += 5;
				continue;
			case OP_ADD_MEM_MEM:
				nextinstr(zl_eip);
				mem(zl_eip) += mem(zl_eip+4);
				zl_eip += 8;
				continue;

			case OP_SUB_REG_IMM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] -= pdw(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_SUB_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] -= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_SUB_REG_MEM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] -= mem(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_SUB_MEM_IMM:
				nextinstr(zl_eip);
				mem(zl_eip) -= pdw(zl_eip+4);
				zl_eip += 8;
				continue;
			case OP_SUB_MEM_REG:
				nextinstr(zl_eip);
				mem(zl_eip) -= regs[pdb(zl_eip+4)];
				zl_eip += 5;
				continue;
			case OP_SUB_MEM_MEM:
				nextinstr(zl_eip);
				mem(zl_eip) -= mem(zl_eip+4);
				zl_eip += 8;
				continue;

			case OP_MOV_REG_IMM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = pdw(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_MOV_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_MOV_PREG_REG:
				nextinstr(zl_eip);
				switch(zl_over)
				{
					case 1:
						pdb(regs[pdb(zl_eip)]) = db(regs[pdb(zl_eip+1)]);
						break;
					case 2:
						pw(regs[pdb(zl_eip)]) = w(regs[pdb(zl_eip+1)]);
						break;
					default:
						pdw(regs[pdb(zl_eip)]) = regs[pdb(zl_eip+1)];
				}
				zl_eip += 2;
				continue;
			case OP_MOV_REG_PREG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = pdw(regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOV_REG_MEM:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = mem(zl_eip+1);
				zl_eip += 5;
				continue;
			case OP_MOV_MEM_IMM:
				nextinstr(zl_eip);
				mem(zl_eip) = pdw(zl_eip+4);
				zl_eip += 8;
				continue;
			case OP_MOV_MEM_REG:
				nextinstr(zl_eip);
				mem(zl_eip) = regs[pdb(zl_eip+4)];
				zl_eip += 5;
				continue;
			case OP_MOV_MEM_MEM:
				nextinstr(zl_eip);
				mem(zl_eip) = mem(zl_eip+4);
				zl_eip += 8;
				continue;

			case OP_CMP_REG_IMM:
				nextinstr(zl_eip);

				__asm
				{
					mov eax, dword ptr regs[REG_EIP*4]
					mov eax, [eax]					
					and eax, 0xFF
					shl eax, 2
					lea ecx, regs
					add eax, ecx
					mov eax, [eax]
					mov ecx, dword ptr regs[REG_EIP*4]
					inc ecx
					mov ecx, [ecx]
					cmp eax, ecx
					pushfd
					pop eax
					mov dword ptr regs[ZL_EFLAGS*4], eax
				}

				zl_eip += 5;
				continue;

			 case OP_TEST_REG_REG:
				nextinstr(zl_eip);

				__asm
				{
					lea ebx, regs
					mov ecx, dword ptr regs[REG_EIP*4]
					mov eax, [ecx]
					and eax, 0xFF
					shl eax, 2
					add eax, ebx
					mov eax, [eax]
					inc ecx
					mov ecx, [ecx]
					and ecx, 0xFF
					shl ecx, 2
					add ecx, ebx
					mov ecx, [ecx]
					test eax, ecx
					pushfd
					pop eax
					mov dword ptr regs[ZL_EFLAGS*4], eax
				}

				zl_eip += 2;
				continue;

			case OP_JMP:
				nextinstr(zl_eip);
				//zl_eip = dw(hardcode + pdw(zl_eip));	// revision 1 (absolute jump)
				zl_eip += pdw(zl_eip);					// revision 2 (relative jump)
				continue;

			case OP_JA:
			case OP_JNBE:
				nextinstr(zl_eip);
				if(~zl_eflags & (EF_ZF | EF_CF))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNA:
			case OP_JBE:
				nextinstr(zl_eip);
				if(zl_eflags & (EF_ZF | EF_CF))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JAE:
			case OP_JNB:
				nextinstr(zl_eip);
				if(~zl_eflags & EF_CF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNAE:
			case OP_JB:
			case OP_JC:
				nextinstr(zl_eip);
				if(zl_eflags & EF_CF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNP:
			case OP_JPO:
				nextinstr(zl_eip);
				if(~zl_eflags & EF_PF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JP:
			case OP_JPE:
				nextinstr(zl_eip);
				if(zl_eflags & EF_PF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNE:
			case OP_JNZ:
				nextinstr(zl_eip);
				if(~zl_eflags & EF_ZF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JE:
			case OP_JZ:
				nextinstr(zl_eip);
				if(zl_eflags & EF_ZF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JECXZ:
				nextinstr(zl_eip);
				if(!regs[REG_ECX])
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;

			case OP_JG:
			case OP_JNLE:
				nextinstr(zl_eip);
				if(!((zl_eflags & EF_ZF) | (((zl_eflags & EF_OF) >> 11) ^ ((zl_eflags & EF_SF) >> 7))))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JLE:
			case OP_JNG:
				nextinstr(zl_eip);
				if((zl_eflags & EF_ZF) | (((zl_eflags & EF_OF) >> 11) ^ ((zl_eflags & EF_SF) >> 7)))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JGE:
			case OP_JNL:
				nextinstr(zl_eip);
				if(!(((zl_eflags & EF_OF) >> 11) ^ ((zl_eflags & EF_SF) >> 7)))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JL:
			case OP_JNGE:
				nextinstr(zl_eip);
				if(((zl_eflags & EF_OF) >> 11) ^ ((zl_eflags & EF_SF) >> 7))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNO:
				nextinstr(zl_eip);
				if(~zl_eflags & EF_OF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JO:
				nextinstr(zl_eip);
				if(zl_eflags & EF_OF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNS:
				nextinstr(zl_eip);
				if(~zl_eflags & EF_SF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JS:
				nextinstr(zl_eip);
				if(zl_eflags & EF_SF)
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;

			case OP_CALL:
				{
					nextinstr(zl_eip);
					push(zl_esp);
					pdw(zl_esp) = zl_eip+4;
					zl_eip += pdw(zl_eip);
				}
				continue;
			case OP_CALL_FAR:
				{
					nextinstr(zl_eip);

					__asm
					{
						//mov		ebx, dword ptr [import_sect]
						mov		ebx, dword ptr [regs]
						add		ebx, SECT_IMPORT*4
						mov		ebx, [ebx]
						//mov		eax, dword ptr [regs + REG_EIP*4]
						mov		eax, regs
						add		eax, REG_EIP*4
						mov		eax, [eax]			// regs[REG_EIP] value
						mov		eax, [eax]
						add		ebx, eax			// address of import_sect[func_numb]

						mov		eax, ebx
						add		eax, 4
						mov		eax, [eax]
						//mov		ecx, dword ptr regs[REG_ESP*4]
						mov		ecx, regs
						add		ecx, REG_ESP*4
						mov		ecx, [ecx]				// regs[REG_ESP] value
					lb_loop:
						test	eax, eax
						jz		lb_exec
						dec		eax
						add		ecx, 4
						push	[ecx]
						jmp		lb_loop
					lb_exec:
						call	[ebx]
						//mov		dword ptr regs[REG_EAX*4], eax
						mov		ecx, regs
						add		ecx, REG_EAX*4
						mov		[ecx], eax				// result to regs[REG_EAX]
						mov		eax, ebx
						add		eax, 8
						mov		eax, [eax]
						test	eax, eax
						jz		lb_end
						mov		eax, ebx
						add		eax, 4
						mov		eax, [eax]
						shl		eax, 2
						add		esp, eax
					lb_end:
					}

					zl_eip += 4;
				}
				continue;
			case OP_RCALL_FAR:
				{
					nextinstr(zl_eip);

					//import_sect + pwd(zl_eip+4); // address
					//import_sect + pwd(zl_eip+4)+4; // params
					//import_sect + pwd(zl_eip+4)+8; // __stdcall or __cdecl
					__asm
					{
						//mov		ebx, dword ptr [import_sect]
						mov		ebx, dword ptr [regs]
						add		ebx, SECT_IMPORT*4
						mov		ebx, [ebx]
						//mov		eax, dword ptr [regs + REG_EIP*4]
						mov		eax, regs
						add		eax, REG_EIP*4
						mov		eax, [eax]			// regs[REG_EIP] value
						mov		eax, [eax]
						add		ebx, eax			// address of import_sect[func_numb]

						mov		eax, ebx
						add		eax, 4
						mov		eax, [eax]
						mov		ecx, eax
						shl		ecx, 2
						//mov		ecx, dword ptr regs[REG_ESP*4]
						mov		ecx, regs
						add		ecx, REG_ESP*4
						mov		ecx, [ecx]			// regs[REG_ESP] value
					lb_rc_loop:
						test	eax, eax
						jz		lb_rc_exec
						dec		eax
						push	[ecx]
						sub		ecx, 4
						jmp		lb_rc_loop
					lb_rc_exec:
						//int		3
						call	[ebx]
						//mov		dword ptr regs[REG_EAX*4], eax
						mov		ecx, regs
						add		ecx, REG_EAX*4
						mov		[ecx], eax			// result to regs[REG_EAX]
						mov		eax, ebx
						add		eax, 8
						mov		eax, [eax]
						test	eax, eax
						jz		lb_rc_end
						mov		eax, ebx
						add		eax, 4
						mov		eax, [eax]
						shl		eax, 2
						add		esp, eax
					lb_rc_end:
					}

					zl_eip += 4;
				}
				continue;
			/* revision 1
			case OP_CALL:
				{
					register zl_funcs_list *fn;

					nextinstr(zl_eip);

					fn = (zl_funcs_list *) (dw(fn_list) + pdw(zl_eip));

					if(fn->flags & ZLF_NEAR_CALL)
					{
						push(zl_esp);
						pdw(zl_esp) = zl_eip+4;
						zl_eip += pdw(zl_eip);
					}
					else if(fn->proc)
					{
						zl_esp -= fn->params*4;
						__asm
						{
							mov		eax, dword ptr [fn]
							add		eax, 4
							mov		eax, [eax]
							mov		ecx, dword ptr regs[REG_ESP*4]
						lb_loop:
							test	eax, eax
							jz		lb_exec
							dec		eax
							add		ecx, 4
							push	[ecx]
							jmp		lb_loop
						lb_exec:
							mov		eax, dword ptr [fn]
							add		eax, 8
							call	[eax]
							mov		dword ptr regs[REG_EAX*4], eax
							mov		eax, dword ptr [fn]
							add		eax, 12
							mov		eax, [eax]
							test	eax, eax
							jz		lb_end
							mov		eax, dword ptr [fn]
							add		eax, 4
							mov		eax, [eax]
							shl		eax, 2
							add		esp, eax
						lb_end:
						}
					}
					else
					{
						printf("error: unknown function: 0x%.8X\n", pdw(zl_eip));
					}

					zl_eip += 4;
				}
				continue;

			case OP_RCALL:
				{
					register zl_funcs_list *fn;

					nextinstr(zl_eip);

					fn = (zl_funcs_list *) (dw(fn_list) + pdw(zl_eip));

					if(fn->flags & ZLF_NEAR_CALL)
					{
						printf("error: rcall unsupported for near call: 0x%.8X\n", pdw(zl_eip));
					}
					else if(fn->proc)
					{
						zl_esp -= fn->params*4;
						__asm
						{
							mov		eax, dword ptr [fn]
							add		eax, 4
							mov		eax, [eax]
							mov		ecx, eax
							shl		ecx, 2
							add		ecx, dword ptr regs[REG_ESP*4]
						lb_rc_loop:
							test	eax, eax
							jz		lb_rc_exec
							dec		eax
							push	[ecx]
							sub		ecx, 4
							jmp		lb_rc_loop
						lb_rc_exec:
							mov		eax, dword ptr [fn]
							add		eax, 8
							call	[eax]
							mov		dword ptr regs[REG_EAX*4], eax
							mov		eax, dword ptr [fn]
							add		eax, 12
							mov		eax, [eax]
							test	eax, eax
							jz		lb_rc_end
							mov		eax, dword ptr [fn]
							add		eax, 4
							mov		eax, [eax]
							shl		eax, 2
							add		esp, eax
						lb_rc_end:
						}
					}
					else
					{
						printf("error: unknown function: 0x%.8X\n", pdw(zl_eip));
					}

					zl_eip += 4;
				}
				continue;
			//*/

			case OP_RET:
				{
					unsigned long zl_new_point;
					nextinstr(zl_eip);
					zl_new_point = pdw(zl_esp);
					pop(zl_esp);
					zl_esp -= pdb(zl_eip);
					zl_eip = zl_new_point;
	
					if(zl_eip == 0xFFFFFFFF)
					{
						goto lb_exit;
					}
				}
				continue;

			case OP_G_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] > regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_GE_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] >= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_L_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] < regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_LE_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] <= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_E_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] == regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_NE_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = regs[pdb(zl_eip)] != regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;

			case OP_OR_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] |= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_AND_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] &= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_XOR_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] ^= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_LNOT_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = !regs[pdb(zl_eip)];
				zl_eip++;
				continue;
			case OP_BNOT_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] = ~regs[pdb(zl_eip)];
				zl_eip++;
				continue;
			case OP_MINUS_REG:
				nextinstr(zl_eip);
				(*(signed long *) &regs[pdb(zl_eip)]) = - ((signed long) regs[pdb(zl_eip)]);
				zl_eip++;
				continue;
			case OP_INC_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)]++;
				zl_eip++;
				continue;
			case OP_DEC_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)]--;
				zl_eip++;
				continue;

			case OP_MUL_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] *= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_MOD_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] %= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_DIV_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] /= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_SHL_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] <<= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;
			case OP_SHR_REG_REG:
				nextinstr(zl_eip);
				regs[pdb(zl_eip)] >>= regs[pdb(zl_eip+1)];
				zl_eip += 2;
				continue;

			// direct short stack operations
			case OP_ADD_STK_STK:
				pdw(zl_esp-4) += pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_SUB_STK_STK:
				pdw(zl_esp-4) -= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_MUL_STK_STK:
				pdw(zl_esp-4) *= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_MOD_STK_STK:
				pdw(zl_esp-4) %= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_DIV_STK_STK:
				pdw(zl_esp-4) /= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_SHL_STK_STK:
				pdw(zl_esp-4) <<= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_SHR_STK_STK:
				pdw(zl_esp-4) >>= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_OR_STK_STK:
				pdw(zl_esp-4) |= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_XOR_STK_STK:
				pdw(zl_esp-4) ^= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_AND_STK_STK:
				pdw(zl_esp-4) &= pdw(zl_esp);
				pop(zl_esp);
				break;

			case OP_E_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) == pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_NE_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) != pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_G_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) > pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_GE_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) >= pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_L_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) < pdw(zl_esp);
				pop(zl_esp);
				break;
			case OP_LE_STK_STK:
				pdw(zl_esp-4) = pdw(zl_esp-4) <= pdw(zl_esp);
				pop(zl_esp);
				break;

			case OP_LNOT_STK:
				pdw(zl_esp) = !pdw(zl_esp);
				break;
			case OP_BNOT_STK:
				pdw(zl_esp) = ~pdw(zl_esp);
				break;
			case OP_MINUS_STK:
				(*(signed long *) &pdw(zl_esp)) = - ((signed long) pdw(zl_esp));
				break;
			case OP_JZ_POP_STK:
				nextinstr(zl_eip);
				if(!pdw(zl_esp))
				{
					zl_eip += pdw(zl_eip);
					pop(zl_esp);
					continue;
				}
				zl_eip += 4;
				pop(zl_esp);
				continue;
			case OP_JNZ_POP_STK:
				nextinstr(zl_eip);
				if(pdw(zl_esp))
				{
					zl_eip += pdw(zl_eip);
					pop(zl_esp);
					continue;
				}
				zl_eip += 4;
				pop(zl_esp);
				continue;
			case OP_JZ_STK:
				nextinstr(zl_eip);
				if(!pdw(zl_esp))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			case OP_JNZ_STK:
				nextinstr(zl_eip);
				if(pdw(zl_esp))
				{
					zl_eip += pdw(zl_eip);
					continue;
				}
				zl_eip += 4;
				continue;
			// end short stack operations

			case OP_NOP:
				break;
			case OP_DBG_PRINT:
				//printf("debug print:\nEAX: 0x%.8X (%u)\nESP: 0x%.8X (%u)\nEIP: 0x%.8X (%u)\nZF: %u, CF: %u, PF: %u, OF: %u, SF: %u\n", regs[REG_EAX], regs[REG_EAX], regs[REG_ESP]-dw(stack), regs[REG_ESP]-dw(stack), regs[REG_EIP]-dw(hardcode), regs[REG_EIP]-dw(hardcode), (zl_eflags & EF_ZF)?1:0, (zl_eflags & EF_CF)?1:0, (zl_eflags & EF_PF)?1:0, (zl_eflags & EF_OF)?1:0, (zl_eflags & EF_SF)?1:0);
				printf("debug print:\nEAX: 0x%.8X (%u)\nESP: 0x%.8X (%u)\nEIP: 0x%.8X (%u)\nEBP: 0x%.8X (%u)\nZF: %u, CF: %u, PF: %u, OF: %u, SF: %u\n", regs[REG_EAX], regs[REG_EAX], regs[REG_ESP], regs[REG_ESP], regs[REG_EIP], regs[REG_EIP], regs[REG_EBP], regs[REG_EBP], (zl_eflags & EF_ZF)?1:0, (zl_eflags & EF_CF)?1:0, (zl_eflags & EF_PF)?1:0, (zl_eflags & EF_OF)?1:0, (zl_eflags & EF_SF)?1:0);
				break;
			default:
				printf("error: unknown instruction: 0x%.2x\n", instr(zl_eip));
				goto lb_exit;
		}

		nextinstr(zl_eip);
	}

lb_exit:

	return regs[REG_EAX];
}

unsigned long zl_decompile(unsigned char *hardcode, unsigned long hard_code_size)
{
	char *regs[] = {"eip", "esp", "eax", "ecx", "ebx", "edx", "ebp", "esd", "ebd", "esi", "edi", "eflags", "error"};
	unsigned long zl_eip;
	unsigned long zl_eof;

	zl_eip = dw(hardcode);
	zl_eof = dw(hardcode) + hard_code_size;

	while(zl_eip < zl_eof)
	{
		switch(instr(zl_eip))
		{
			case OP_POP_REG:
				nextinstr(zl_eip);
				printf("%.4u:   pop %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)]);
				zl_eip++;
				continue;
			case OP_POP_MEM:
				nextinstr(zl_eip);
				printf("%.4u:   pop [%.4Xh]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_PUSH_REG:
				nextinstr(zl_eip);
				printf("%.4u:   push %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)]);
				zl_eip++;
				continue;
			case OP_PUSH_IMM:
				nextinstr(zl_eip);
				printf("%.4u:   push %d\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_PUSH_OFFSET:
				nextinstr(zl_eip);
				printf("%.4u:   push data offset [%.4Xh]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_PUSH_MEM:
				nextinstr(zl_eip);
				printf("%.4u:   push var value [%.4Xh]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_PUSH_PMEM:
				nextinstr(zl_eip);
				printf("%.4u:   push dword ptr to value [%.4Xh]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_TEST_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   test %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_ADD_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   add %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_SUB_REG_IMM:
				nextinstr(zl_eip);
				printf("%.4u:   sub %s, %u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], pdw(zl_eip+1));
				zl_eip += 5;
				continue;
			case OP_SUB_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   sub %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MUL_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   mul %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_DIV_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   div %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOD_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   mod %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_G_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   g %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_GE_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   ge %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_LE_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   le %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_L_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   l %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_E_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   e %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_NE_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   ne %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_OR_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   or %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_AND_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   and %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_SHR_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   and %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_SHL_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   and %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_XOR_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   xor %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOV_REG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   mov %s, %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOV_PREG_REG:
				nextinstr(zl_eip);
				printf("%.4u:   mov [%s], %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOV_REG_PREG:
				nextinstr(zl_eip);
				printf("%.4u:   mov %s, [%s]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], regs[pdb(zl_eip+1)]);
				zl_eip += 2;
				continue;
			case OP_MOV_MEM_REG:
				nextinstr(zl_eip);
				printf("%.4u:   mov [%.4Xh], %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip), regs[pdb(zl_eip+4)]);
				zl_eip += 5;
				continue;
			case OP_MOV_REG_MEM:
				nextinstr(zl_eip);
				printf("%.4u:   mov %s, [%.4Xh]\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], pdw(zl_eip+1));
				zl_eip += 5;
				continue;
			case OP_MOV_REG_IMM:
				nextinstr(zl_eip);
				printf("%.4u:   mov %s, %u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], pdw(zl_eip+1));
				zl_eip += 5;
				continue;
			case OP_ADD_REG_IMM:
				nextinstr(zl_eip);
				printf("%.4u:   add %s, %u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)], pdw(zl_eip+1));
				zl_eip += 5;
				continue;
			case OP_INC_REG:
				nextinstr(zl_eip);
				printf("%.4u:   inc %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)]);
				zl_eip++;
				continue;
			case OP_DEC_REG:
				nextinstr(zl_eip);
				printf("%.4u:   dec %s\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, regs[pdb(zl_eip)]);
				zl_eip++;
				continue;
			case OP_JZ:
				nextinstr(zl_eip);
				printf("%.4u:   jz %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_JNZ:
				nextinstr(zl_eip);
				printf("%.4u:   jnz %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_JMP:
				nextinstr(zl_eip);
				printf("%.4u:   jmp %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_CALL:
				nextinstr(zl_eip);
				printf("%.4u:   call near %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_CALL_FAR:
				nextinstr(zl_eip);
				printf("%.4u:   call far %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_RCALL_FAR:
				nextinstr(zl_eip);
				printf("%.4u:   rcall far %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, pdw(zl_eip));
				zl_eip += 4;
				continue;

			case OP_ADD_STK_STK:
				printf("%.4u:   add_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_SUB_STK_STK:
				printf("%.4u:   sub_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_MUL_STK_STK:
				printf("%.4u:   mul_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_MOD_STK_STK:
				printf("%.4u:   mod_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_DIV_STK_STK:
				printf("%.4u:   div_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_OR_STK_STK:
				printf("%.4u:   or_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_AND_STK_STK:
				printf("%.4u:   and_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_XOR_STK_STK:
				printf("%.4u:   xor_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_SHL_STK_STK:
				printf("%.4u:   shl_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_SHR_STK_STK:
				printf("%.4u:   shr_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_E_STK_STK:
				printf("%.4u:   e_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_NE_STK_STK:
				printf("%.4u:   ne_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_L_STK_STK:
				printf("%.4u:   l_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_LE_STK_STK:
				printf("%.4u:   le_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_G_STK_STK:
				printf("%.4u:   g_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_GE_STK_STK:
				printf("%.4u:   ge_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_LNOT_STK:
				printf("%.4u:   lnot_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_BNOT_STK:
				printf("%.4u:   not_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_MINUS_STK:
				printf("%.4u:   minus_stk\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_JZ_STK:
				nextinstr(zl_eip);
				printf("%.4u:   jz_stk %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_JNZ_STK:
				nextinstr(zl_eip);
				printf("%.4u:   jnz_stk %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_JZ_POP_STK:
				nextinstr(zl_eip);
				printf("%.4u:   jz_pop_stk %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;
			case OP_JNZ_POP_STK:
				nextinstr(zl_eip);
				printf("%.4u:   jnz_pop_stk %.4u\n", dw(zl_eip) - dw(hardcode) - ZL_INSTRUCTION_LENGTH, dw(zl_eip) - dw(hardcode) + pdw(zl_eip));
				zl_eip += 4;
				continue;

			case OP_SIZE_OVERRIDE_1:
				printf("%.4u:   size override to 1 byte for next operation\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_SIZE_OVERRIDE_2:
				printf("%.4u:   size override to 2 bytes for next operation\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_DBG_PRINT:
				printf("%.4u:   dbg_print_regs\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_RET:
				printf("%.4u:   ret\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_NOP:
				printf("%.4u:   nop\n", dw(zl_eip) - dw(hardcode));
				break;
			case OP_EOF:
				printf("%.4u:   $end\n", dw(zl_eip) - dw(hardcode));
				break;
			default:
				printf("%.4u:   %Xh - unknown instruction\n", dw(zl_eip) - dw(hardcode), instr(zl_eip));
		}
		nextinstr(zl_eip);
	}

	//printf("%.4u:   $end\n", dw(zl_eip) - dw(hardcode));

	return 0L;
}

/*

// *** demo funcs ****************************

unsigned long __stdcall fn_test(unsigned long prm1, unsigned long prm2, unsigned long prm3)
{
	printf("fn_test: prm1: %u, prm2: %u, prm3: %u\n", prm1, prm2, prm3);

	return prm1 + prm2 + prm3;
}

unsigned long __stdcall fn_text(char *message)
{
	printf("fn_text: message: %s\n", message);

	return 0;
}

unsigned long fn_test_loop(char *message, unsigned long itr)
{
	printf("fn_test_loop: in loop [%u], message: %s\n", itr, message);

	return (itr >= 99)?1:0;
}

// *** end demo funcs ************************

int main(int argc, char* argv[])
{
	printf("Hello World!\n");

	// external variable
	unsigned long ext_var = 0;

	// user defined table
	funcs_list fn_list[] = {
		{"fn_test",			3,		fn_test,			0},
		{"fn_text",			1,		fn_text,			0},
		{"fn_loop",			2,		fn_test_loop,		ZLF_CLEAR_STACK},
		{NULL,				NULL,	NULL,				NULL}
		};

	// data generated by zl_compile
	//unsigned char *hardcode;
	//unsigned char *data_table;
	//unsigned long vars_count;

	//zl_names_map *vars_map;

	char data_table[] = {"Hello, User!\0More data!\0Next block internal vars\0"};
	unsigned long vars_count = 1;

	unsigned char hardcode[] = {
		OP_PUSH_REG, REG_EAX,
		OP_PUSH_REG, REG_ECX,

		OP_MOV_REG_MEM, REG_ECX, sdw(0),

		//OP_PRINT_DBG,
		OP_PUSH_REG, REG_ECX,
		OP_PUSH_OFFSET, sdw(13),
		OP_CALL, sdw(2*16),

		OP_ADD_REG_IMM, REG_ECX, sdw(1),

		OP_MOV_MEM_REG, sdw(0), REG_ECX,

		OP_CMP_REG_IMM, REG_EAX, sdw(1),
		OP_JNE, sdw(10),

		OP_POP_REG, REG_ECX,
		OP_POP_REG, REG_EAX,

		OP_PRINT_DBG,

		OP_EOF
	};

	zl_names_map mapvars[] = {
		{"ext_var",		0},
		{NULL,			NULL}
		};


	// temporary variables
	unsigned long *vars_table;
	void **map_table;

	//zl_compile(&hardcode, &data_table, &vars_count, fn_list, "code here: expr = expr + expr;", &vars_map);
	//zl_load_file(&hardcode, &data_table, &vars_count, "filename.zlc"[, &vars_map]);

	zl_memtable_make(&map_table, &vars_table, vars_count);
	zl_set(map_table, mapvars, "ext_var", &ext_var);

	zl_execute(hardcode, data_table, map_table, fn_list);
	
	zl_free(NULL, &map_table, NULL, NULL);
	//zl_memtable_free(&hardcode, &map_table, &data_table, &vars_map);

	printf("after execute: ext_var = %u\n", ext_var);

	return 0;
}

*/

/* zlanguage like this:

	fn_test(1, 2, 3);

	push 3
	push 2
	push 1
	call fn_test
	
	var_test = var_test + 1 + 2;

	push	eax
	mov		eax, [var_test]
	add		eax, 1
	add		eax, 2
	mov		[var_test], eax
	pop		eax

	header = "text";

	push	eax
	push	[const_string_1]
	call	alloc_string
	mov		[header], eax
	pop		eax


	header = "text" + "text";

	push	eax
	push	[const_string_1]
	call	alloc_string
	push	[const_string_2]
	push	eax
	call	alloc_strcat
	mov		[header], eax
	pop		eax

	if(isset(method))
	{
		len++;
	}

	push	eax
	push	[method]
	call	isset
	test	eax, eax
	jz		[lb_skip]
	inc		[len]
	
lb_skip:
	pop eax


*/
