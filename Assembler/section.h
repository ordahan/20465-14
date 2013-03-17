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

/*
 *  Large enough for any normal program
 */
#define SECTION_MAX_SIZE 2000

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

/**
 * Writes the given value to the given section and
 * specifies it's locality.
 * @param pSection Section to write to
 * @param nValue Value to write
 * @param locAddress Locality of value
 * @return The absolute address of the cell written
 * (base offset + inner offset)
 */
memory_address_t section_write(memory_section_t* pSection,
				   	   	   	   unsigned int nValue,
				   	   	   	   address_locality_t locAddress);

/**
 * Gets size of section.
 * @param pSection Section to get info about
 * @return How many cells occupy the section
 */
unsigned int section_get_size(const memory_section_t* pSection);

/**
 * Sets the size of a section.
 * Doesn't wipe anything, just moves the offset marker.
 * @param pSection Section to set size.
 * @param nSize New size.
 */
void section_set_size(memory_section_t* pSection,
					  unsigned int nSize);

/**
 * Reads the cell at the given address offset inside the sector.
 * @param pSection Section to read from
 * @param o_pCell Will be filled with read result
 * @param offset Where to read inside the section
 * @return Absolute address that was read from.
 */
memory_address_t section_read(const memory_section_t* pSection,
							  const memory_section_cell_t** o_pCell,
							  unsigned int offset);

#endif /* SECTIONS_H_ */
