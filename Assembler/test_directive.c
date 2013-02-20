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

int test_directive()
{
	assert(test_compile_extern() == 0);

	return 0;
}

int test_compile_dummy_instruction()
{
	return -1;
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
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern HelloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 == directive_compile_extern(&statement, arrSymbols));
	assert(0 == strcmp(statement.szOperationData , arrSymbols[0].name));
	assert(ADDR_EXTERNAL == arrSymbols[0].address);
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
	assert(ADDR_EXTERNAL == arrSymbols[0].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern without params: ");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[0].address);
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
	assert(ADDR_EXTERNAL == arrSymbols[0].address);
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[1].address);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern with invalid syntax: ");
	memset(arrSymbols, 0, sizeof(arrSymbols));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, ".extern @elloMoto");
	assert(0 == parser_get_statement(&statement));
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[0].address);
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
	assert(ADDR_EXTERNAL == arrSymbols[0].address);
	strcpy(statement.szContent, ".extern A23456789012345678901234567890000");
	assert(0 != directive_compile_extern(&statement, arrSymbols));
	assert(ADDR_INVALID == arrSymbols[1].address);
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
