#ifndef _ZL_HELPER_H_
#define _ZL_HELPER_H_

#include "zl.h"

//extern zl_funcs_list fn_list[];

unsigned long zl_load_predefined_functions(unsigned char *import_section, zl_map_section *map_section, unsigned long count, unsigned long **modules);

#endif // _ZL_HELPER_H_
