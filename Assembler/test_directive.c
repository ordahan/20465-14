/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>

#include "directive.h"

int test_compile_dummy_instruction();
int test_compile_extern();

int test_directive()
{
	assert(test_compile_dummy_instruction() == 0);

	return 0;
}

int test_compile_dummy_instruction()
{
	return -1;
}

int test_compile_extern()
{
	/* todo: extern test cases
	 * new valid extern
	 * valid with label
	 * without any params
	 * name too long (?)
	 * symbol exists
	 */
	return -1;
}
