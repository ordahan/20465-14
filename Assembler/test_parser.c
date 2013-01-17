/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "parser.h"

int test_parser()
{
	statement_t st;

	printf("Testing parser module:\n");

	/**********************************************/
	printf("	Empty line: ");
	assert(STATEMENT_TYPE_EMPTY == parser_get_statement("  			 ", &st));
	printf("PASSED.\n");
	/**********************************************/


	return 0;
}
