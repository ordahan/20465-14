/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <strings.h>

#include "directive.h"
#include "parser.h"
#include "assembler.h"
#include "test_utils.h"

int test_compile_dummy_instruction();
int test_compile_extern();
int test_compile_entry();

void init_object_blocks(symbol_table_arr_t *symbol_expected,
					memory_section_t *code_expected,
					memory_section_t *data_expected)
{
	if (symbol_expected != NULL)
		memset(*symbol_expected, 0, sizeof(*symbol_expected));
	if (code_expected != NULL)
		memset(code_expected, 0, sizeof(*code_expected));
	if (data_expected != NULL)
		memset(data_expected, 0, sizeof(*data_expected));
}

int test_directive()
{
	assert(test_compile_dummy_instruction() == 0);
	assert(test_compile_extern() == 0);
	assert(test_compile_entry() == 0);

	return 0;
}

int test_dummy_instruction_compile(const char		*line,
						   memory_section_t *data,
						   const memory_section_t *expected,
						   char fShouldSucceed)
{
	statement_t statement;
	symbol_table_arr_t symbols;

	memset(symbols, 0, sizeof(symbols));

	/* Retrieve the line to compile */
	strcpy(statement.szContent, line);

	/* Create a statement from the given line */
	assert(0 == parser_get_statement(&statement));

	/* Check that the compilation returns as expected */
	assert(!((0 == directive_compile_dummy_instruction(&statement, data, symbols)) ^
			 fShouldSucceed));

	if (fShouldSucceed != 0)

	{
		/* Check the label if exists */
		if (statement.szLabel != NULL)
		{
			/* Find last symbol */
			unsigned i;
			symbol_t* pSymb = &symbols[0];
			for (i = 1; symbols[i].address != ADDR_INVALID;++i )
			{
				pSymb = &symbols[i];
			}

			assert(0 == strcmp(statement.szLabel, pSymb->name));
			assert(section_get_size(data) - 1 == pSymb->address);
			assert(ADDR_RELOCATABLE == pSymb->locality);
		}

		/* Check the data section */
		if (compare_memory_sections(expected, data) != 0)
		{
			printf("Data sections don't match.\n");
			return -1;
		}
	}

	printf("PASSED.\n");
	return 0;
}

int test_compile_dummy_instruction()
{
	/*todo:
	 * #one data
	 * #two data
	 * #N data
	 * #no data
	 * #too much data (no more room)
	 * #element is not a valid number
	 * #number out of range
	 * #list not separated properly
	 * #label for data
	 * #multiple data parts
	 * #no chars in string
	 * #one char string
	 * #N chars string
	 * #no '"' at start / end / both
	 * #string too long (null-terminator out of bounds, more than 1 out of bounds)
	 * #another field before/after the string
	 */
	memory_section_t     data, data_expected;

	printf("Testing compiling dummy directives:\n");

	/**********************************************/
	printf("	one data: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	section_write(&data_expected, 13, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".data 13",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	two data: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	section_write(&data_expected, 13, ADDR_ABSOLUTE);
	section_write(&data_expected, -3, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".data 13, -3",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	N data: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	section_write(&data_expected, 13, ADDR_ABSOLUTE);
	section_write(&data_expected, 0, ADDR_ABSOLUTE);
	section_write(&data_expected, -4, ADDR_ABSOLUTE);
	section_write(&data_expected, 100000, ADDR_ABSOLUTE);
	section_write(&data_expected, -1000000, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".data 13, 0, -4, 100000, -1000000",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	no data: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	assert(0 == test_dummy_instruction_compile(".data ",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	too much data (no more room): \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	data._counter = SECTION_MAX_SIZE;
	assert(0 == test_dummy_instruction_compile(".data 13",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	element is not a valid number: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data #13",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data abc",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data 15, A4",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data 15 %%2",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data 1..2",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".data a",
												   &data,
												   &data_expected,
												   0));
	/**********************************************/

	/**********************************************/
	printf("	number out of range: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	assert(0 == test_dummy_instruction_compile(".data 9999999999999999999999",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	list not separated properly: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	assert(0 == test_dummy_instruction_compile(".data 13 . 5",
											   &data,
											   &data_expected,
											   0));
	assert(0 == test_dummy_instruction_compile(".data 13,,5",
												   &data,
												   &data_expected,
												   0));
	assert(0 == test_dummy_instruction_compile(".data 13 #5",
											   &data,
											   &data_expected,
											   0));
	assert(0 == test_dummy_instruction_compile(".data 13\t5",
											   &data,
											   &data_expected,
											   0));
	assert(0 == test_dummy_instruction_compile(".data 13 5",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	label for data: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);

	section_write(&data_expected, 13, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile("X:.data 13",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	multiple data parts: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	section_write(&data_expected, 13, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".data 13",
											   &data,
											   &data_expected,
											   1));
	section_write(&data_expected, 29, ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".data 29",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	no chars in string: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);

	section_write(&data_expected, '\0', ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".string \"\"",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	one char string: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);

	section_write(&data_expected, 'x', ADDR_ABSOLUTE);
	section_write(&data_expected, '\0', ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".string \"x\"",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	N chars string: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);

	section_write(&data_expected, '1', ADDR_ABSOLUTE);
	section_write(&data_expected, '2', ADDR_ABSOLUTE);
	section_write(&data_expected, '3', ADDR_ABSOLUTE);
	section_write(&data_expected, '4', ADDR_ABSOLUTE);
	section_write(&data_expected, '\0', ADDR_ABSOLUTE);
	assert(0 == test_dummy_instruction_compile(".string \"1234\"",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	no '\"' at start / end / both: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"hi",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	Too many \": \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"hi\"\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \" \"hi\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"\"hi\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"hi\" \"",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	Multiple strings \": \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	assert(0 == test_dummy_instruction_compile(".string \"hi\" \"lol\"",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	string too long (null-terminator out of bounds, more than 1 out of bounds): \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	data._counter = SECTION_MAX_SIZE;
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"\"",
											   &data,
											   &data_expected,
											   0));
	data._counter = SECTION_MAX_SIZE-1;
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"a\"",
											   &data,
											   &data_expected,
											   0));
	data._counter = SECTION_MAX_SIZE;
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"a\"",
											   &data,
											   &data_expected,
											   0));
	data._counter = SECTION_MAX_SIZE-2;
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\"",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	/**********************************************/
	printf("	another field before/after the string: \n");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string h\"abc\"",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi\"abc\"",
												   &data,
												   &data_expected,
												   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi \"abc\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi,\"abc\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string hi, \"abc\"",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\"a",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\"aa",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\",a",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\", a",
											   &data,
											   &data_expected,
											   0));
	printf("\t\t");
	assert(0 == test_dummy_instruction_compile(".string \"abc\" ,a",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/

	printf("PASSED.\n");
	return 0;
}

int test_compile_extern()
{
	/*
	 * #new valid extern
	 * #valid with label
	 * #without any params
	 * #name too long (?)
	 * #symbol exists (can symbol be .extern'ed twice? no!)
	 * #name with syntax error
	 */
	symbol_table_arr_t arrSymbols;
	statement_t		   statement;

	printf("Testing compiling externals:\n");

	/**********************************************/
	printf("	Valid extern: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].address = 1;
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_extern(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Valid extern with label: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, "GREETINGS: .extern HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_extern(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern without params: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern existing: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_extern(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[1].locality);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern with invalid syntax: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern @elloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern long name: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern A23456789012345678901234567890");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_extern(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	strcpy(statement.szContent, ".extern A23456789012345678901234567890000");
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[1].locality);
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}

int test_compile_entry()
{
	/*
	 * #Entry for existing symbol
	 * #Entry for external
	 * #Entry for non-existing symbol
	 * #Entry for an already entry marked symbol
	 */
	symbol_table_arr_t arrSymbols;
	statement_t		   statement;

	printf("Testing compiling externals:\n");

	/**********************************************/
	printf("	Existing symbol entry: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_ABSOLUTE;
	arrSymbols[0].address = 0;
	strcpy(arrSymbols[0].name, "HelloMoto");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_entry(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_ABSOLUTE == arrSymbols[0].locality);
	assert(ADDR_ENTRY_STATUS_ENTRY == arrSymbols[0].entry);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry for external symbol: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_EXTERNAL;
	arrSymbols[0].address = 0;
	strcpy(arrSymbols[0].name, "HelloMoto");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_entry(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry for non existing symbol: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_EXTERNAL;
	arrSymbols[0].address = 0;
	strcpy(arrSymbols[0].name, "HelloM0t0");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_entry(&statement, arrSymbols));
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry for already an entry symbol: \n");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_ABSOLUTE;
	arrSymbols[0].address = 0;
	arrSymbols[0].entry = ADDR_ENTRY_STATUS_NON_ENTRY;
	strcpy(arrSymbols[0].name, "HelloMoto");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_entry(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData, arrSymbols[0].name));
	assert(ADDR_ENTRY_STATUS_ENTRY == arrSymbols[0].entry);
	assert(0 != directive_compile_entry(&statement, arrSymbols));
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
