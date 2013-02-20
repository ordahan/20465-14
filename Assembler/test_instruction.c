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

#include "instruction.h"
#include "parser.h"

int test_compile_instruction();

int test_instruction()
{
	assert(test_compile_instruction() == 0);

	return 0;
}

int test_compile_instruction()
{
	code_section_t code;
	statement_t statement;

	/*
	 * # Simple
	 * Type set
	 * Invalid type
	 * No operands
	 * One operand
	 * Two operands
	 * Register
	 * One extra data
	 * Two extra data
	 * Three extra data
	 * Four extra data
	 */

	printf("Testing compiling externals:\n");

	/**********************************************/
	printf("	Simple instruction: ");
	memset(&code, 0, sizeof(code));
	memset(&statement, 0, sizeof(statement));
	strcpy(statement.szContent, "");
	assert(0 == parser_get_statement(&statement));
	assert(0 == instruction_compile(&statement, &code));
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
