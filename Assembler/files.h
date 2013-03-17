/*
 * files.h
 *
 * 	Responsible for everything regarding input / output file naming
 * 	and creating output files for a given program.
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef FILES_H_
#define FILES_H_

#include <stdio.h>
#include "assembler.h"
#include "symbol.h"

/* Defines for the assembler i/o interface */
#define FILE_EXT_SEPARATOR "."
#define FILE_ASSEMBLY_EXT "as"
#define FILE_OBJECT_EXT "ob"
#define FILE_ENTIRES_EXT "ent"
#define FILE_EXTERNALS_EXT "ext"
#define FILE_NAME_MAX_LENGTH (252)
#define FILE_NAME_MAX_EXT_LENGTH (3)
#define FILE_NAME_SEPARATOR_LENGTH (1)
#define FILE_FULL_NAME_MAX_LENGTH (FILE_NAME_MAX_LENGTH +\
								   FILE_NAME_SEPARATOR_LENGTH +\
								   FILE_NAME_MAX_EXT_LENGTH)

/* API */
/**
 * Locates the given input file and opens it for read access.
 * @param szFileName Filename without extension
 * @return NULL if not found, File object if its ok.
 */
FILE* file_open_input(const char *szFileName);

/**
 * Creates an object file from the given sections.
 * If the file szFileName + ".ob" exists, overrides it.
 * @param szFileName Filename without extension
 * @param pCode Code section of the file.
 * @param pData Data section of the file.
 * @param 0 on success, anything else on error.
 */
int file_create_object(const char *szFileName,
					   const memory_section_t *pCode,
		  	  	  	   const data_section_t *o_pData);

/**
 * Creates an entry file from the given symbol table,
 * if there are any entries in it.
 * If the file szFileName + ".ob" exists, overrides it.
 * @param szFileName Filename without extension
 * @param arrSymbols  Symbol table
 * @param 0 on success, anything else on error.
 */
int file_create_entry(const char *szFileName,
					  const symbol_table_arr_t arrSymbols);

/**
 * Creates an externals file from the given symbol table
 * and code section, if there are any external symbols in it.
 * If the file szFileName + ".ob" exists, overrides it.
 * @param szFileName Filename without extension
 * @param arrSymbols  Symbol table
 * @param pCode Code section that has references to externals.
 * @param 0 on success, anything else on error.
 */
int file_create_externals(const char *szFileName,
						  const symbol_table_arr_t arrSymbols,
					  	  const memory_section_t *pCode);

#endif /* FILES_H_ */
