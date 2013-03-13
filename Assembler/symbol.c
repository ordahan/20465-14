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

/* Internal functions */
/**
 * Finds the index of the requested symbol if it exists.
 * @param table Symbol table to look in
 * @param szName Name of the requested symbol
 * @return -1 if not found, its index otherwise.
 */
int find_symbol_index_by_name(const symbol_table_arr_t table,
							  const char* szName);

/* fixme: change the api to get the proper values and not just the type.. */
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

const symbol_t* symbol_get_from_table_by_name(const symbol_table_arr_t table,
											 const char* szName)
{
	int index = -1;
	if (table == NULL ||
		szName == NULL)
		return NULL;

	index = find_symbol_index_by_name(table, szName);

	if (index == -1)
	{
		return NULL;
	}
	else
	{
		return &table[index];
	}
}

int symbol_set_as_entry(symbol_table_arr_t table, const char* szName)
{
	int index = find_symbol_index_by_name(table, szName);

	if (index == -1 ||
		table[index].entry == ADDR_ENTRY_STATUS_ENTRY ||
		table[index].locality == ADDR_EXTERNAL)
	{
		return -1;
	}
	else
	{
		table[index].entry = ADDR_ENTRY_STATUS_ENTRY;
		return 0;
	}
}

int find_symbol_index_by_name(const symbol_table_arr_t table,
							  const char* szName)
{
	int i;

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
				return i;
			}
		}
	}

	return -1;
}
