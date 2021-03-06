/*
 * directive.h
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef DIRECTIVE_H_
#define DIRECTIVE_H_

#include "assembler.h"
#include "statement.h"

typedef enum
{
	DIRECTIVE_DATA,
	DIRECTIVE_STRING,
	DIRECTIVE_ENTRY,
	DIRECTIVE_EXTERN,

	/* This must be last */
	DIRECTIVE_ILLEGAL
}directive_type_t;

/**
 * Compiles the given 'dummy instruction' directive statement and
 * adds it to the given data section (and symbol table if needed)
 *
 * @param pDummyInst Dummy instruction statement
 * @param io_pData Data section to place the result it
 * @param io_pSymbols Symbol table to hold the directive's label if exists
 * @return 0 on successful compilation, any other value on error.
 */
int directive_compile_dummy_instruction(const statement_t *pDummyInst,
									    data_section_t* io_pData,
									    symbol_table_arr_t io_pSymbols);

/**
 * Compiles the given extern directive, places it in the
 * symbol table.
 * In case the symbol already exists in the given table,
 * this will cause an error.
 *
 * @param pExtern Extern statement
 * @param io_pSymbols Symbol table to add to
 * @return 0 if successful, any other value on error.
 */
int directive_compile_extern(const statement_t *pExtern,
							 symbol_table_arr_t io_pSymbols);

/**
 * Compiles the given entry directive, modifies a symbols
 * addressing type in the symbol table if needed.
 * In case the symbol doesn't exist in the given table,
 * this will cause an error.
 *
 * @param pExtern Entry statement
 * @param io_pSymbols Symbol table to search the symbol in.
 * @return 0 if successful, any other value on error.
 */
int directive_compile_entry(const statement_t *pEntry,
							symbol_table_arr_t io_pSymbols);

#endif /* DIRECTIVE_H_ */
