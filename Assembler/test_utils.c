/*
 * test_utils.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "test_utils.h"
#include <memory.h>

int compare_files(const char* szFile1 , const char* szFile2)
{
	return 1;
}

void init_program_data(symbol_table_arr_t symbol_expected,
						   	   	 code_section_t *code_expected,
						   	   	 data_section_t *data_expected)
{
	memset(symbol_expected, 0, sizeof(symbol_expected));
	memset(code_expected, 0, sizeof(*code_expected));
	memset(data_expected, 0, sizeof(*data_expected));
}
