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

	f1 = fopen(szFile1, "r");
	f2 = fopen(szFile2, "r");

	if (f1 == NULL || f2 == NULL)
	{
		printf("Cannot open files for comparison: f1 = %x, f2 = %x\n",
				(int)f1,
				(int)f2);
		return -1;
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
			   feof(f1) == 0 &&
			   fgets(szLine2,
					 sizeof(szLine2),
					 f2) != NULL &&
			   feof(f2) == 0)
		{
			i++;

			if (strcmp(szLine1, szLine2) != 0)
			{
				printf("Line %d doesn't match:\n%s\n%s\n",
					   i,
					   szLine1,
					   szLine2);
				return -1;
			}
		}

		/* Check lengths */
		if (feof(f1) != feof(f2))
		{
			return -1;
		}
	}

	return 0;
}

void init_program_data(symbol_table_arr_t symbol_expected,
						   	   	 code_section_t *code_expected,
						   	   	 data_section_t *data_expected)
{
	memset(symbol_expected, 0, sizeof(symbol_expected));
	memset(code_expected, 0, sizeof(*code_expected));
	memset(data_expected, 0, sizeof(*data_expected));
}