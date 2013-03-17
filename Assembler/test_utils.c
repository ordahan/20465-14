/*
 * test_utils.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "test_utils.h"
#include <memory.h>
#include "statement.h"

int compare_files(const char* szFile1 , const char* szFile2)
{
	FILE *f1, *f2;
	unsigned int result = 0;

	f1 = fopen(szFile1, "r");
	f2 = fopen(szFile2, "r");

	if (f1 == NULL || f2 == NULL)
	{
		printf("Cannot open files for comparison: f1 = %x, f2 = %x\n",
				(int)f1,
				(int)f2);
		result = -1;
	}
	else
	{
		char szLine1[MAX_LINE_LENGTH];
		char szLine2[MAX_LINE_LENGTH];
		unsigned int i = 0;

		/* Compare line by line */
		while (fgets(szLine1,
					 sizeof(szLine1),
					 f1) != NULL &&
			   fgets(szLine2,
					 sizeof(szLine2),
					 f2) != NULL)
		{
			i++;

			if (strcmp(szLine1, szLine2) != 0)
			{
				printf("Line %d don't match:\n%s%s",
					   i,
					   szLine1,
					   szLine2);
				result = -1;
			}
		}
	}

	return result;
}

void init_program_data(symbol_table_arr_t symbol_expected,
					   memory_section_t *pMem1,
					   memory_section_t *data_expected)
{
	memset(symbol_expected, 0, sizeof(symbol_table_arr_t));
	memset(pMem1, 0, sizeof(*pMem1));
	memset(data_expected, 0, sizeof(*data_expected));
}


int compare_memory_sections(const memory_section_t* pMem1,
							const memory_section_t* pMem2)
{
	unsigned int i;

	if (section_get_size(pMem1) != section_get_size(pMem2))
	{
		printf("Sizes not as expected: [%d != %d]\n",
				section_get_size(pMem1),
				section_get_size(pMem2));
		return -1;
	}

	for (i = 0; i < section_get_size(pMem1); ++i)
	{
		const memory_section_cell_t* pCell1;
		const memory_section_cell_t* pCell2;
		section_read(pMem1, &pCell1, i);
		section_read(pMem1, &pCell2, i);

		if (pCell1->content.val != pCell2->content.val)
		{
			printf("Memory in index %d doesn't match: [%d != %d]\n",
					i,
					pCell1->content.val,
					pCell2->content.val);
			return -2;
		}
		else if (pCell1->locality != pCell1->locality)
		{
			printf("Locality in index %d doesn't match: [%d != %d]\n",
					i,
					pCell1->locality,
					pCell1->locality);
			return -3;
		}
	}

	return 0;
}
