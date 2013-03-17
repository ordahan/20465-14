/*
 * files.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "files.h"
#include <stdio.h>
#include <string.h>
#include "symbol.h"

/* Internal functions */
/**
 * Adds the given extension to the file name
 * and tries to open the resulting name with
 * the given mode.
 *
 * @param szFileName File to open
 * @param szExt Extension to use
 * @param szMode Mode to open in
 * @return 0 if ok, anything else on error.
 */
FILE* open_file_with_ext(const char* szFileName,
					 	 const char* szExt,
					 	 const char* szMode);

/* Implementations */
FILE* file_open_input(const char *szFileName)
{
	return open_file_with_ext(szFileName, FILE_ASSEMBLY_EXT, "r");
}

int file_create_object(const char *szFileName,
					   const code_section_t *pCode,
		  	  	  	   const data_section_t *o_pData)
{
	return -1;
}

int file_create_entry(const char *szFileName,
					  const symbol_table_arr_t arrSymbols)
{
	unsigned int i;
	FILE* fEntryFile = NULL;

	if (szFileName == NULL ||
		arrSymbols == NULL)
		return -1;

	/* Go over all the symbols and print out the
	 * entries
	 */
	for (i = 0; i < MAX_SYMBOLS; ++i)
	{
		/* Is this an entry symbol? */
		if (arrSymbols[i].locality != ADDR_INVALID &&
			arrSymbols[i].entry == ADDR_ENTRY_STATUS_ENTRY)
		{
			/* Lazy open, is this the first one? */
			if (fEntryFile == NULL)
			{
				fEntryFile = open_file_with_ext(szFileName,
												FILE_ENTIRES_EXT,
												"w");

				if (fEntryFile == NULL)
					return -1;
			}

			/* Add the symbol to file */
			fprintf(fEntryFile,
					"%s %s\n",
					arrSymbols[i].name,
					parser_int_to_string_base_4(arrSymbols[i].address, 0));
		}
	}

	/* Close if needed */
	if (fEntryFile != NULL)
		fclose(fEntryFile);

	return 0;
}

int file_create_externals(const char *szFileName,
						  const symbol_table_arr_t arrSymbols,
					  	  const code_section_t *pCode)
{
	return -1;
}

FILE* open_file_with_ext(const char* szFileName,
					 	 const char* szExt,
					 	 const char* szMode)
{
	char szFile[FILE_FULL_NAME_MAX_LENGTH];
	FILE* f = NULL;

	if (strlen(szFileName) + FILE_NAME_SEPARATOR_LENGTH + strlen(szExt) + 1 >
		sizeof(szFile))
	{
		printf("Error! filename %s is too long (%d max)",
			   szFileName,
			   sizeof(szFile));
		return NULL;
	}

	/* Create the final filename */
	strncpy(szFile, szFileName, sizeof(szFile));
	strncat(szFile, FILE_EXT_SEPARATOR, sizeof(szFile));
	strncat(szFile, szExt, sizeof(szFile));

	f = fopen(szFile, szMode);
	if (f == NULL)
	{
		printf("Error! cannot open file '%s'.\n", szFile);
		return NULL;
	}

	return f;
}
