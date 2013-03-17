/*
 * test_files.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "test_utils.h"
#include "files.h"
#include "symbol.h"

int test_files()
{
	const char* szFileName;
	const char* szFileExpected;
	const char* szFileExpectedName;
	symbol_table_arr_t symbols;
	code_section_t code;
	data_section_t data;

	/* todo: tests for files
	 * #no input file
	 * #input file exists
	 * #no entry symbols
	 * valid entries
	 * no externals
	 * valid externals
	 */

	/**********************************************/
	init_program_data(symbols,
					  &code,
					  &data);

	printf("	no input file: ");
	szFileName = "dsadasdas";
	assert(NULL == file_open_input(szFileName));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols,
					  &code,
					  &data);

	printf("	input exists: ");
	szFileName = "tests/entry";
	assert(NULL != file_open_input(szFileName));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols,
					  &code,
					  &data);

	printf("	no entry symbols: ");
	szFileName = "tests/entry";
	szFileExpectedName = "tests/entry.ent";
	szFileExpected = "expecteds/entry.ent";
	symbol_add_to_table(symbols, ADDR_ABSOLUTE, "abcd", 123, ADDR_SECTION_CODE);
	assert(0 == file_create_entry(szFileName, symbols));
	assert(NULL == fopen(szFileExpectedName, "r"));
	printf("PASSED.\n");
	/**********************************************/

	return 0;
}
