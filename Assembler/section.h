/*
 * sections.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef SECTIONS_H_
#define SECTIONS_H_

#include "machine.h"
#include "symbol.h"

/* TODO: Is this rly the restriction we need?
 * can a program weigh more?
 */
#define SECTION_MAX_SIZE 2000

/* todo: Add tests for the lengths of the given sections */

/* Typedefs */
typedef struct
{
	machine_cell_t 	   content;
	address_locality_t locality;
}memory_section_cell_t;

typedef struct
{
	memory_address_t   		_base_offset;
	unsigned int 	   		_counter;
	memory_section_cell_t 	cells[SECTION_MAX_SIZE];
}memory_section_t;

memory_address_t section_write(memory_section_t* pSection,
				   	   	   	   unsigned int nValue,
				   	   	   	   address_locality_t locAddress);

unsigned int section_get_size(const memory_section_t* pSection);

void section_set_size(memory_section_t* pSection,
					  unsigned int nSize);

memory_address_t section_read(const memory_section_t* pSection,
							  const memory_section_cell_t** o_pCell,
							  unsigned int offset);

#endif /* SECTIONS_H_ */
