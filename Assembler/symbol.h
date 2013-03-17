/*
 * symbol.h
 *
 *  Handles everything related to symbols and the symbol table.
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "machine.h"

#define MAX_SYMBOLS 1000

/* Assume label's total length is 30 (instructions not exactly
 * clear whether its 30 or 31) */
#define MAX_LABEL_LENGTH ((unsigned int)30)

/* Typedefs */
typedef enum
{
	ADDR_INVALID = 0,
	ADDR_ABSOLUTE,
	ADDR_RELOCATABLE,
	ADDR_EXTERNAL
}address_locality_t;

typedef enum
{
	ADDR_ENTRY_STATUS_NON_ENTRY = 0,
	ADDR_ENTRY_STATUS_ENTRY
}address_entry_status_t;

typedef enum
{
	ADDR_SECTION_CODE,
	ADDR_SECTION_DATA,
	ADDR_SECTION_EXTERNAL
}address_section_t;

typedef struct
{
	address_locality_t 	locality;
	char			  	name[MAX_LABEL_LENGTH + 1];
	unsigned long		address;
	address_entry_status_t 	entry;
	address_section_t	section;
}symbol_t;

typedef symbol_t symbol_table_arr_t[MAX_SYMBOLS];

/**
 * Adds the given symbol to the table.
 * Makes sure the symbol name is unique to the table.
 * In case of an external symbol - its address
 * will be its index in the symbol table.
 *
 * @param table Table to add the symbol into
 * @param locality What is the addressing type for the symbol.
 * @param szName Symbol name.
 * @param address Address it resides in.
 * @param section What section does the address belong to.
 * @return 0 if everything ok, -1 if not unique, anything else for error.
 */
int symbol_add_to_table(symbol_table_arr_t  table,
						address_locality_t 	locality,
						const char* 		szName,
						memory_address_t	address,
						address_section_t	section);

/**
 * Searches the given symbol table for a symbol with the given name.
 * @param table Table to search in.
 * @param szName Symbol name to look for.
 * @return NULL if not found, the symbol requested otherwise.
 */
const symbol_t* symbol_get_from_table_by_name(const symbol_table_arr_t table,
											  const char* szName);

/**
 * Sets the given symbol as an entry.
 * Fails if the given name doesn't exist or it is already
 * an existing entry / external symbol.
 * @param table Symbol table containing the given name.
 * @param szName Symbol name
 * @return 0 on success, anything else on error.
 */
int symbol_set_as_entry(symbol_table_arr_t table, const char* szName);

/**
 * Move the given section forward by the given offset amount.
 * @param io_arrSymbols Symbol table with section symbols
 * @param section Which section to move
 * @param offset How many cells forward to shift them.
 */
void symbol_move_section(symbol_table_arr_t io_arrSymbols,
						 address_section_t section,
						 unsigned int offset);

/**
 * Gets the symbol with the minimal address that is
 * greater than this one.
 * @param table Symbol table
 * @param address Address to start the search
 * @return NULL if there aren't any, symbol if there are.
 */
const symbol_t* symbol_get_next_by_address(const symbol_table_arr_t table,
										   memory_address_t address);

#endif /* SYMBOL_H_ */
