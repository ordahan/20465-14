/*
 * directive.c
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "directive.h"
#include "symbol.h"
#include "parser.h"

int directive_compile_dummy_instruction(const statement_t *pDummyInst,
									    data_section_t* io_pData,
									    symbol_table_arr_t io_pSymbols)
{
	return -1;
}

int directive_compile_extern(const statement_t *pExtern,
							 symbol_table_arr_t io_pSymbols)
{
	symbol_t symbol;
	symbol.address = ADDR_EXTERNAL;
	const char *pName;

	if (pExtern == NULL ||
		io_pSymbols == NULL)
		return -1;

	/* Get the label intended to add */
	if (parser_get_items_from_list(pExtern->szOperationData,
								   &pName,
								   1) != 0)
		return -2;

	/* Make sure the label is valid */
	if (parser_check_symbol_syntax(pName) == 0)
		return -3;

	/* Copy the content of the label */
	strncpy(symbol.name, pName, sizeof(symbol.name));

	/* Add the label to the table */
	return symbol_add_to_table(io_pSymbols, &symbol);
}
