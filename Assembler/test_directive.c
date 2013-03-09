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

int test_compile_dummy_instruction();
int test_compile_extern();
int test_compile_entry();

void init_object_blocks(symbol_table_arr_t *symbol_expected,
					code_section_t *code_expected,
					data_section_t *data_expected)
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
						   data_section_t *data,
						   const data_section_t *expected,
						   char fShouldSucceed)
{
	unsigned i;
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

	if (fShouldSucceed == 0)
		return 0;

	/* Check the IC */
	assert(expected->DC == data->DC);

	/* Check the label if exists */
	if (statement.szLabel != NULL)
	{
		assert(0 == strcmp(statement.szLabel, symbols[0].name));
		assert(data->DC == symbols[0].address);
		assert(ADDR_RELOCATABLE == symbols[0].locality);
	}

	/* Check the data generated */
	for (i = 0; i < expected->DC; ++i)
	{
		assert(0 == memcmp(&expected->content[i],
						   &data->content[i],
						   sizeof(expected->content[0])));
	}

	printf("PASSED.\n");
	return 0;
}

int test_compile_dummy_instruction()
{
	/*todo:
	 * one data
	 * two data
	 * N data
	 * no data
	 * too much data (no more room)
	 * element is not a valid number
	 * number out of range
	 * list not separated properly
	 * label for data
	 * multiple data parts
	 * no chars in string
	 * one char string
	 * N chars string
	 * no '"' at start / end / both
	 * string too long (null-terminator out of bounds, more than 1 out of bounds)
	 * another field before/after the string
	 */
	data_section_t     data, data_expected;

	printf("Testing compiling dummy directives:\n");

	/**********************************************/
	printf("	one data: ");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	data_expected.DC = 1;
	data_expected.content[0].val = 13;
	assert(0 == test_dummy_instruction_compile(".data 13",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	two data: ");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	data_expected.DC = 2;
	data_expected.content[0].val = 13;
	data_expected.content[1].val = -3;
	assert(0 == test_dummy_instruction_compile(".data 13, -3",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	N data: ");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	data_expected.DC = 5;
	data_expected.content[0].val = 13;
	data_expected.content[1].val = 0;
	data_expected.content[2].val = -4;
	data_expected.content[3].val = 100000;
	data_expected.content[4].val = -1000000;
	assert(0 == test_dummy_instruction_compile(".data 13, 0, -4, 100000, -1000000",
											   &data,
											   &data_expected,
											   1));
	/**********************************************/

	/**********************************************/
	printf("	no data: ");
	init_object_blocks(NULL, NULL, &data_expected);
	init_object_blocks(NULL, NULL, &data);
	assert(0 == test_dummy_instruction_compile(".data 13",
											   &data,
											   &data_expected,
											   0));
	/**********************************************/
	printf("	too much data (no more room): ");
	printf("	element is not a valid number: ");
	printf("	number out of range: ");
	printf("	list not separated properly: ");
	printf("	label for data: ");
	printf("	multiple data parts: ");
	printf("	no chars in string: ");
	printf("	one char string: ");
	printf("	N chars string: ");
	printf("	no '\"' at start / end / both: ");
	printf("	string too long (null-terminator out of bounds, more than 1 out of bounds): ");
	printf("	another field before/after the string: ");

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
	printf("	Valid extern: ");
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
	printf("	Valid extern with label: ");
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
	printf("	Extern without params: ");
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
	printf("	Extern existing: ");
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
	printf("	Extern with invalid syntax: ");
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
	printf("	Extern long name: ");
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
	printf("	Existing symbol entry: ");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_ABSOLUTE;
	arrSymbols[0].address = 0;
	strcpy(arrSymbols[0].name, "HelloMoto");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_entry(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_ENTRY == arrSymbols[0].locality);
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry for external symbol: ");
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
	printf("	Entry for non existing symbol: ");
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
	printf("	Entry for already an entry symbol: ");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	arrSymbols[0].locality = ADDR_ABSOLUTE;
	arrSymbols[0].address = 0;
	strcpy(arrSymbols[0].name, "HelloMoto");
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".entry HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_entry(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData, arrSymbols[0].name));
	assert(ADDR_ENTRY == arrSymbols[0].locality);
	assert(0 != directive_compile_entry(&statement, arrSymbols));
	assert(0 == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
