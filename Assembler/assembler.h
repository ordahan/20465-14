/*
 * assembler.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdlib.h>

#include "machine.h"
#include "symbol.h"

/* TODO: Is this rly the restriction we need?
 * can a program weigh more?
 */
#define ASSEMBLER_CODE_MAX_SIZE_CELLS 2000
#define ASSEMBLER_DATA_MAX_SIZE_CELLS 2000

/* Typedefs */
typedef struct
{
	machine_cell_t content[ASSEMBLER_CODE_MAX_SIZE_CELLS];
	size_t		   length;
}code_section_t;

typedef struct
{
	machine_cell_t content[ASSEMBLER_CODE_MAX_SIZE_CELLS];
	size_t		   length;
}data_section_t;

/* todo: Add tests for the lengths of the given sections */

/* External functions */
/**
 * Reads a program from the given file and compiles it.
 * @param pAssemblyFile Text file containing the program to compile.
 * @param o_pSymbols All the symbols referenced / defined in the code.
 * @param o_pCode .text section (instructions) of the program.
 * @param o_pData .data of the program.
 * @return 0 if succeeds, anything else for a compilation error.
 */
int assembler_compile(FILE* pAssemblyFile,
					  symbol_table_arr_t o_arrSymbols,
					  code_section_t *o_pCode,
					  data_section_t *o_pData);

#endif /* ASSEMBLER_H_ */
