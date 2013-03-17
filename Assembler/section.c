/*
 * section.c
 *
 *  Created on: Mar 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "section.h"
#include <stdlib.h>

/* fixme: check return code in instruction */
memory_address_t section_write(memory_section_t* pSection,
							   unsigned int nValue,
				   	   	   	   address_locality_t locAddress)
{
	if (pSection == NULL)
		return MEMORY_ADDRESS_INVALID;

	/* Make sure not to overflow the section content */
	if (pSection->_counter >= SECTION_MAX_SIZE)
		return MEMORY_ADDRESS_INVALID;

	/* Place the value in the section */
	pSection->cells[pSection->_counter].content.val = nValue;
	pSection->cells[pSection->_counter].locality = locAddress;

	/* Count it as added */
	pSection->_counter++;

	/* Return its absolute address */
	return pSection->_base_offset + pSection->_counter - 1;
}

unsigned int section_get_size(const memory_section_t* pSection)
{
	if (pSection == NULL)
			return 0;

	return pSection->_counter;
}


memory_address_t section_read(const memory_section_t* pSection,
							  const memory_section_cell_t** o_pCell,
							  unsigned int offset)
{
	if (o_pCell == NULL ||
		pSection == NULL)
		return MEMORY_ADDRESS_INVALID;

	if (offset > section_get_size(pSection))
		return MEMORY_ADDRESS_INVALID;

	/* Get the requested cell */
	*o_pCell = &pSection->cells[offset];

	/* Return its absolute address */
	return offset + pSection->_base_offset;
}


void section_set_size(memory_section_t* pSection,
					  unsigned int nSize)
{
	if (pSection == NULL)
		return;

	pSection->_counter = nSize;
}
