/*
 * symbol.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#define MAX_SYMBOLS 1000

/* Typedefs */
typedef enum
{
	ADDR_ABSOLUTE,
	ADDR_RELOCATABLE,
	ADDR_EXTERNAL
}address_locality_t;

typedef struct
{
	address_locality_t 	address;
	char*			  	name;/*TODO: Ptr or array?*/
}symbol_t;

typedef symbol_t symbol_table_t[MAX_SYMBOLS];

int symbol_add_to_table(symbol_table_t table, symbol_t symbol);

const symbol_t* symbol_get_from_table_by_name(const symbol_table_t table, const char* szName);

#endif /* SYMBOL_H_ */
