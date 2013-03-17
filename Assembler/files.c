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

/**
 * Prints the given memory section to the object file.
 * @param pObjectFile File to print to
 * @param pSection Memory section to print
 * @param fPrintLocality Whether or not to print the locality letter.
 * @return 0 if ok, anything else on error.
 */
int print_section_to_object(FILE* pObjectFile,
							const memory_section_t* pSection,
						    unsigned char fPrintLocality);

/* Implementations */
FILE* file_open_input(const char *szFileName)
{
	return open_file_with_ext(szFileName, FILE_ASSEMBLY_EXT, "r");
}

int file_create_object(const char *szFileName,
					   const memory_section_t *pCode,
		  	  	  	   const memory_section_t *pData)
{
	const char* szHeader = "Base 4 Address	Base 4 machine code	Absolute, relocatable or external";
	FILE* fObjectFile = NULL;

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
			parser_int_to_string_base_4(section_get_size(pData), 0));

	/* Print the code section */
	if (print_section_to_object(fObjectFile,
								pCode,
								1) != 0)
	{
		return -1;
	}

	/* Print the data section */
	if (print_section_to_object(fObjectFile,
								pData,
								0) != 0)
	{
		return -1;
	}

	fclose(fObjectFile);

	return 0;
}

int file_create_entry(const char *szFileName,
					  const symbol_table_arr_t arrSymbols)
{
	FILE* fEntryFile = NULL;
	const symbol_t* pCurrSymbol = symbol_get_next_by_address(arrSymbols, 0);

	if (szFileName == NULL ||
		arrSymbols == NULL)
		return -1;

	/* Go over all the symbols and print out the
	 * entries
	 */
	while (pCurrSymbol != NULL)
	{
		/* Is this an entry symbol? */
		if (pCurrSymbol->entry == ADDR_ENTRY_STATUS_ENTRY)
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
					pCurrSymbol->name,
					parser_int_to_string_base_4(pCurrSymbol->address, 0));
		}

		/* Get next one */
		pCurrSymbol = symbol_get_next_by_address(arrSymbols, pCurrSymbol->address);
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
	const memory_section_cell_t* pCell;
	memory_address_t addrCell;


	if (szFileName == NULL ||
		arrSymbols == NULL ||
		pCode == NULL)
		return -1;

	/* Go over all code section and locate externals
	 */
	for (i = 0; i < section_get_size(pCode); ++i)
	{
		/* Read the current code cell */
		addrCell = section_read(pCode, &pCell, i);

		/* Make sure we retrieved a valid memory cell */
		if (addrCell == MEMORY_ADDRESS_INVALID)
		{
			return -2;
		}

		/* Is this an external? */
		if (pCell->locality == ADDR_EXTERNAL)
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
					arrSymbols[pCell->content.val].name,
					parser_int_to_string_base_4(addrCell, 0));
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

int print_section_to_object(FILE* fObjectFile,
						    const memory_section_t* pSection,
						    unsigned char fPrintLocality)
{
	unsigned int i;
	const memory_section_cell_t* pCell;
	memory_address_t addrCell;

	if (fObjectFile == NULL ||
		pSection == NULL)
		return -1;

	/* Go over the section and print it */
	for (i = 0; i < section_get_size(pSection); ++i)
	{
		/* Read the current code cell */
		addrCell = section_read(pSection, &pCell, i);

		/* Make sure we retrieved a valid memory cell */
		if (addrCell == MEMORY_ADDRESS_INVALID)
		{
			return -2;
		}

		/* Address of the cell */
		fprintf(fObjectFile,
				"%s",
				parser_int_to_string_base_4(addrCell, 0));

		/* Value of the cell */
		if (pCell->locality == ADDR_EXTERNAL)
		{
			/* Put 0 instead of externals for the sake of consistency
			 * as we don't know the real address */
			fprintf(fObjectFile,
					"\t%s",
					parser_int_to_string_base_4(0,
												MACHINE_CELL_NUM_BASE_4_CHARACTERS));
		}
		else
		{
			fprintf(fObjectFile,
					"\t%s",
					parser_int_to_string_base_4(pCell->content.val,
												MACHINE_CELL_NUM_BASE_4_CHARACTERS));
		}

		/* Locality of the cell */
		if (fPrintLocality == 1)
		{
			fprintf(fObjectFile,
					"\t%c",
					parser_get_locality_letter(pCell->locality));
		}

		fprintf(fObjectFile, "\n");
	}

	return 0;
}
