/*
 * symbol.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#define MAX_SYMBOLS 1000

/* fixme: A line that contains only a label and ':'
 * is considered legal(?), and it might take the entire line
 */
/* Assume label's total length is 30 (instructions not exactly
 * clear wether its 30 or 31) */
#define MAX_LABEL_LENGTH ((unsigned int)30)

/* Typedefs */
typedef enum
{
	ADDR_INVALID = 0,
	ADDR_ABSOLUTE,
	ADDR_RELOCATABLE,
	ADDR_EXTERNAL,
	ADDR_ENTRY /*todo: is this true? whats going on with these..*/
}address_locality_t;

typedef struct
{
	address_locality_t 	locality;
	char			  	name[MAX_LABEL_LENGTH + 1];
	unsigned long		address;
}symbol_t;

typedef symbol_t symbol_table_arr_t[MAX_SYMBOLS];

/**
 * Adds the given symbol to the table.
 * Makes sure the symbol name is unique to the table.
 * @param table Table to add the symbol into
 * @param symbol Symbol to add into the table
 * @return 0 if everything ok, -1 if not unique, anything else for error.
 */
int symbol_add_to_table(symbol_table_arr_t table,
						const symbol_t *symbol);

/**
 * Searches the given symbol table for a symbol with the given name.
 * @param table Table to search in.
 * @param szName Symbol name to look for.
 * @return NULL if not found, the symbol requested otherwise.
 */
symbol_t* symbol_get_from_table_by_name(symbol_table_arr_t table,
										const char* szName);

#endif /* SYMBOL_H_ */
