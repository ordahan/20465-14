/*
 * files.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "files.h"
#include <stdio.h>
#include <string.h>

/* Internal functions */
/**
 * Adds the given extension to the file name
 * and places the result in the dest string.
 * Will not perform the operation if there isn't
 * enough space in dest for the file name, the extension
 * separator and the extension itself.
 *
 * @param szDest Where to place the result
 * @param lenbDest Length in bytes of dest
 * @param szFileName Filename to place
 * @param szExt Extension to place
 * @return 0 if ok, anything else on error.
 */
int concat_extension(char* szDest,
					 size_t lenbDest,
					 const char* szFileName,
					 const char* szExt);

/* Implementations */
FILE* file_open_input(const char *szFileName)
{
	char szInputFile[FILE_NAME_MAX_LENGTH + FILE_NAME_MAX_EXT_LENGTH];
	if (concat_extension(szInputFile,
						 sizeof(szInputFile),
						 szFileName,
						 FILE_ASSEMBLY_EXT) != 0)
	{
		return NULL;
	}
	else
	{
		return fopen(szInputFile, "r");
	}
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
	return -1;
}

int file_create_externals(const char *szFileName,
						  const symbol_table_arr_t arrSymbols,
					  	  const code_section_t *pCode)
{
	return -1;
}

int concat_extension(char* szDest,
					 size_t lenbDest,
					 const char* szFileName,
					 const char* szExt)
{
	if (strlen(szFileName) + FILE_NAME_SEPARATOR_LENGTH + strlen(szExt) + 1 > lenbDest)
		return -1;

	strncpy(szDest, szFileName, lenbDest);
	strncat(szDest, FILE_EXT_SEPARATOR, lenbDest);
	strncat(szDest, szExt, lenbDest);
	return 0;
}
