/*
 * assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "assembler.h"

#include <memory.h>

/* Internal data types */

/* Internal functions */
int first_pass();
int second_pass();

/* Function Implementation */
int assembler_compile(const FILE* flProgram,
					  symbol_table_arr_t *o_pSymbols,
					  code_segment_arr_t *o_pCode,
					  data_segment_arr_t *o_pData)
{
	/* First of all, init the given output-bound array types */
	memset(*o_pSymbols, 0, sizeof(*o_pSymbols));
	memset(*o_pCode, 0, sizeof(*o_pCode));
	memset(*o_pData, 0, sizeof(*o_pData));

	return 0;
}
