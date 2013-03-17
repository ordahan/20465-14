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

	if (pMem1->counter_a != pMem2->counter_a)
	{
		printf("Sizes not as expected: [%d != %d]\n",
				pMem1->counter_a,
				pMem2->counter_a);
		return -1;
	}

	for (i = 0; i < pMem2->counter_a; ++i)
	{
		if (pMem1->content_a[i].val != pMem2->content_a[i].val)
		{
			printf("Memory in index %d doesn't match: [%d != %d]\n",
					i,
					pMem1->content_a[i].val,
					pMem2->content_a[i].val);
			return -2;
		}
		else if (pMem1->localities_a[i] != pMem2->localities_a[i])
		{
			printf("Locality in index %d doesn't match: [%d != %d]\n",
					i,
					pMem1->localities_a[i],
					pMem2->localities_a[i]);
			return -3;
		}
	}

	return 0;
}
