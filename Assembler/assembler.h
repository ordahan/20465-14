/*
 * assembler.h
 *
 *	This module handles the compilation of file objects into
 *	logical program outputs.
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
#include "section.h"

#define BASE_ADDRESS (100)

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
					  memory_section_t *o_pCode,
					  memory_section_t *o_pData);

/* fixme */
void init_program_data(symbol_table_arr_t symbols,
					   memory_section_t *pCode,
					   memory_section_t *pData);

#endif /* ASSEMBLER_H_ */
