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
#include "parser.h"

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
					   const memory_section_t *pCode,
		  	  	  	   const data_section_t *pData)
{
	const char* szHeader = "Base 4 Address	Base 4 machine code	Absolute, relocatable or external";
	FILE* fObjectFile = NULL;
	unsigned int i;

	if (szFileName == NULL ||
		pCode == NULL ||
		pData == NULL)
		return -1;

	/* Open the object file for writing */
	fObjectFile = open_file_with_ext(szFileName,
									 FILE_OBJECT_EXT,
									 "w");
	if (fObjectFile == NULL)
		return -1;

	/* Print the header */
	fprintf(fObjectFile, "%s\n", szHeader);

	/* Print the sizes of sections */
	fprintf(fObjectFile,
			"%s ",
			parser_int_to_string_base_4(section_get_size(pCode), 0));
	fprintf(fObjectFile,
			"%s\n",
			parser_int_to_string_base_4(pData->DC, 0));

	/* Go over the code section and print it */
	for (i = 0; i < section_get_size(pCode); ++i)
	{
		fprintf(fObjectFile,
				"%s\t",
				parser_int_to_string_base_4(i, 0));

		/* Put 0 instead of externals for the sake of consistency */
		if (pCode->localities_a[i] == ADDR_EXTERNAL)
		{
			fprintf(fObjectFile,
					"%s\t",
					parser_int_to_string_base_4(0,
												MACHINE_CELL_NUM_BASE_4_CHARACTERS));
		}
		else
		{
			fprintf(fObjectFile,
					"%s\t",
					parser_int_to_string_base_4(pCode->content_a[i].val,
												MACHINE_CELL_NUM_BASE_4_CHARACTERS));
		}

		fprintf(fObjectFile,
				"%c\n",
				parser_get_locality_letter(pCode->localities_a[i]));
	}

	/* Go over the data section and print it */
	for (i = 0; i < pData->DC; ++i)
	{
		fprintf(fObjectFile,
				"%s\t",
				parser_int_to_string_base_4(section_get_size(pCode) + i, 0));
		fprintf(fObjectFile,
				"%s\n",
				parser_int_to_string_base_4(pData->content[i].val,
											MACHINE_CELL_NUM_BASE_4_CHARACTERS));
	}

	fclose(fObjectFile);

	return 0;
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
					  	  const memory_section_t *pCode)
{
	unsigned int i;
	FILE* fExternalsFile = NULL;

	if (szFileName == NULL ||
		arrSymbols == NULL ||
		pCode == NULL)
		return -1;

	/* Go over all code section and locate externals
	 */
	for (i = 0; i < section_get_size(pCode); ++i)
	{
		/* Is this an external? */
		if (pCode->localities_a[i] == ADDR_EXTERNAL)
		{
			/* Lazy open, is this the first one? */
			if (fExternalsFile == NULL)
			{
				fExternalsFile = open_file_with_ext(szFileName,
													FILE_EXTERNALS_EXT,
													"w");

				if (fExternalsFile == NULL)
					return -1;
			}

			/* The value of the current code cell is the index in the
			 * symbol table for the external symbol.
			 */
			fprintf(fExternalsFile,
					"%s %s\n",
					arrSymbols[pCode->content_a[i].val].name,
					parser_int_to_string_base_4(i, 0));
		}
	}

	/* Close if needed */
	if (fExternalsFile != NULL)
		fclose(fExternalsFile);

	return 0;
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
