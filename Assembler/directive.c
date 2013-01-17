/*
 * directive.c
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "directive.h"

int directive_compile_dummy_instruction(directive_type_t type,
										char* szValue,
										const char* szLabel,
									    data_section_t* io_pData,
									    symbol_table_arr_t io_pSymbols)
{
	return -1;
}
