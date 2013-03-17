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
/* Implementation */
int symbol_add_to_table(symbol_table_arr_t table,
						address_locality_t 	locality,
						const char* szName,
						memory_address_t		address,
						address_section_t	section)
{
	int i;

	if (table == NULL ||
		szName == NULL)
		return -2;

	/* Check if the symbol exists already */
	if (symbol_get_from_table_by_name(table, szName) != NULL)
	{
		printf("Error! symbol %s already exists in symbol table.\n",
				szName);
		return -1;
	}

	/* Make sure the name is valid */
	if (parser_check_symbol_syntax(szName) == 0)
		return -3;

	/* Go symbol-by-symbol until we either find the first empty spot
	 * or we reached the end of the table
	 */
	for (i = 0; i < sizeof(symbol_table_arr_t) / sizeof(table[0]); ++i)
	{
		/* Is this a free spot? */
		if (table[i].locality == ADDR_INVALID)
		{
			/* Add to the table */
			symbol_t *pSymbol = &table[i];
			pSymbol->entry = ADDR_ENTRY_STATUS_NON_ENTRY;
			pSymbol->locality = locality;
			pSymbol->section = section;
			strcpy(pSymbol->name, szName);

			/* In case its an external symbol, the address is
			 * its index in the symbol table for future use.
			 */
			if (locality == ADDR_EXTERNAL)
			{
				pSymbol->address = i;
			}
			else
			{
				pSymbol->address = address;
			}

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
	for (i = 0; i < sizeof(symbol_table_arr_t) / sizeof(table[0]); ++i)
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

void symbol_move_section(symbol_table_arr_t table,
						 address_section_t section,
						 unsigned int offset)
{
	unsigned int i;
	for (i = 0; i < sizeof(symbol_table_arr_t) / sizeof(table[0]); ++i)
	{
		if (table[i].locality != ADDR_INVALID &&
				table[i].section == section)
		{
			table[i].address += offset;
		}
	}
}

const symbol_t* symbol_get_next_by_address(const symbol_table_arr_t table,
										   memory_address_t address)
{
	unsigned int i;
	memory_address_t addrMinimalCandidate = MEMORY_ADDRESS_INVALID;
	unsigned int indexMinimalCandidate = 0;

	for (i = 0; i < sizeof(symbol_table_arr_t) / sizeof(table[0]); ++i)
	{
		if (table[i].locality != ADDR_INVALID &&
			table[i].address > address &&
			table[i].address < addrMinimalCandidate)
		{
			addrMinimalCandidate = table[i].address;
			indexMinimalCandidate = i;
		}
	}

	if (addrMinimalCandidate == MEMORY_ADDRESS_INVALID)
	{
		return NULL;
	}
	else
	{
		return &table[indexMinimalCandidate];
	}
}
