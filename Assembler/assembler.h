/*
 * assembler.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>

#include "machine.h"
#include "symbol.h"

/* TODO: Is this rly the restriction we need?
 * can a program weigh more?
 */
#define ASSEMBLER_CODE_MAX_SIZE_CELLS 2000
#define ASSEMBLER_DATA_MAX_SIZE_CELLS 2000

/* Typedefs */
typedef machine_cell_t code_segment_arr_t[ASSEMBLER_CODE_MAX_SIZE_CELLS];
typedef machine_cell_t data_segment_arr_t[ASSEMBLER_CODE_MAX_SIZE_CELLS];

/* External functions */
/**
 * Reads a program from the given file and compiles it.
 * @param pAssemblyFile Text file containing the program to compile.
 * @param o_pSymbols All the symbols referenced / defined in the code.
 * @param o_pCode .text section (instructions) of the program.
 * @param o_pData .data of the program.
 * @return 0 if succeeds, anything else for a compilation error.
 */
int assembler_compile(const FILE* pAssemblyFile,
					  symbol_table_arr_t *o_pSymbols,
					  code_segment_arr_t *o_pCode,
					  data_segment_arr_t *o_pData);

#endif /* ASSEMBLER_H_ */
