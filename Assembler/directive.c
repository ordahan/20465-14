/*
 * directive.c
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "directive.h"
#include "symbol.h"
#include "parser.h"
#include <string.h>

int directive_compile_dummy_instruction(const statement_t *pDummyInst,
									    data_section_t* io_pData,
									    symbol_table_arr_t io_pSymbols)
{
	return -1;
}

int directive_compile_extern(const statement_t *pExtern,
							 symbol_table_arr_t io_pSymbols)
{
	char *pName;
	symbol_t symbol;
	symbol.address = 0;
	symbol.locality = ADDR_EXTERNAL;

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

int directive_compile_entry(const statement_t *pEntry,
							symbol_table_arr_t io_pSymbols)
{
	char *pName = NULL;
	symbol_t *pSymbol = NULL;

	if (pEntry == NULL ||
		io_pSymbols == NULL)
		return -1;

	/* Get the label intended to make an entry */
	if (parser_get_items_from_list(pEntry->szOperationData,
								   &pName,
								   1) != 0)
		return -2;

	/* Make sure the label is valid */
	if (parser_check_symbol_syntax(pName) == 0)
		return -3;

	/* Look for the label in the symbol table */
	pSymbol = symbol_get_from_table_by_name(io_pSymbols, pName);
	if (pSymbol == NULL)
	{
		printf("Error! cannot set %s as an entry point, symbol doesn't exist!\n",
				pName);
		return -4;
	}
	/* Check if its already an entry */
	else if (pSymbol->locality == ADDR_ENTRY)
	{
		printf("Error! %s is already an entry point!\n",
				pName);
		return -5;
	}
	/* Check if its an external symbol */
	else if (pSymbol->locality == ADDR_EXTERNAL)
	{
		printf("Error! cannot set %s as an entry point, it's an external symbol!\n",
				pName);
		return -5;
	}
	else
	{
		/* Make it an entry */
		pSymbol->locality = ADDR_ENTRY;
	}

	return 0;
}
