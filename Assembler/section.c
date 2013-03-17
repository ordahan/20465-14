/*
 * section.c
 *
 *  Created on: Mar 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "section.h"
#include <stdlib.h>

memory_address_t section_write(memory_section_t* pSection,
							   unsigned int nValue,
				   	   	   	   address_locality_t locAddress)
{
	if (pSection == NULL)
		return MEMORY_ADDRESS_INVALID;

	/* Make sure not to overflow the section content */
	if (pSection->counter_a >= SECTION_MAX_SIZE)
		return MEMORY_ADDRESS_INVALID;

	/* Place the value in the section */
	pSection->content_a[pSection->counter_a].val = nValue;
	pSection->localities_a[pSection->counter_a] = locAddress;

	/* Count it as added */
	pSection->counter_a++;

	return 0;
}

unsigned int section_get_size(const memory_section_t* pSection)
{
	if (pSection == NULL)
			return 0;

	return pSection->counter_a;
}
