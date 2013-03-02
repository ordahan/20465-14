/*
 * symbol.c
 *
 *  Created on: Feb 20, 2013
 *      Author: Or Dahan 201644929
 */


#include "symbol.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

int symbol_add_to_table(symbol_table_arr_t table,
						const symbol_t *symbol)
{
	int i;

	if (table == NULL ||
		symbol == NULL)
		return -2;

	/* Check if the symbol exists already */
	if (symbol_get_from_table_by_name(table, symbol->name) != NULL)
	{
		printf("Error! symbol %s already exists in symbol table.\n", symbol->name);
		return -1;
	}

	/* Make sure the name is valid */
	if (parser_check_symbol_syntax(symbol->name) == 0)
		return -3;

	/* Go symbol-by-symbol until we either find the first empty spot
	 * or we reached the end of the table
	 */
	for (i = 0; i < sizeof(table); ++i)
	{
		/* Is this a free spot? */
		if (table[i].locality == ADDR_INVALID)
		{
			/* Add to the table */
			memcpy(&table[i], symbol, sizeof(table[i]));
			return 0;
		}
	}

	return -3;
}

symbol_t* symbol_get_from_table_by_name(symbol_table_arr_t table,
										const char* szName)
{
	int i;

	if (table == NULL ||
		szName == NULL)
		return NULL;

	/* Go symbol-by-symbol until we either find the requested
	 * name or we reached the end of the table
	 */
	for (i = 0; i < sizeof(table); ++i)
	{
		/* Is this a valid symbol? */
		if (table[i].locality != ADDR_INVALID)
		{
			/* Check if it matches the requested one */
			if (strcmp(szName, table[i].name) == 0)
			{
				return &table[i];
			}
		}
	}

	return NULL;
}
