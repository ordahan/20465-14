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
#define ASSEMBLER_CODE_MAX_SIZE_CELLS SECTION_MAX_SIZE
#define ASSEMBLER_DATA_MAX_SIZE_CELLS SECTION_MAX_SIZE

/* todo: Add tests for the lengths of the given sections */
/* todo: API for code / data sections */

/* Typedefs */
typedef struct
{
	machine_cell_t content[ASSEMBLER_DATA_MAX_SIZE_CELLS];
	unsigned	   DC;
}data_section_t;

typedef struct
{
	memory_address_t   base_offset;
	unsigned int 	   counter_a;
	machine_cell_t 	   content_a[ASSEMBLER_CODE_MAX_SIZE_CELLS];
	address_locality_t localities_a[ASSEMBLER_CODE_MAX_SIZE_CELLS];
}memory_section_t;

memory_address_t section_write(memory_section_t* pSection,
				   	   	   	   unsigned int nValue,
				   	   	   	   address_locality_t locAddress);

unsigned int section_get_size(const memory_section_t* pSection);
#endif /* SECTIONS_H_ */
