/*
 * test_main.c
 *
 *  Created on: Feb 5, 2013
 *      Author: Or Dahan 201644929
 */

#include <stdio.h>
#include <assert.h>
#include <memory.h>

extern int test_assembler();
extern int test_parser();
extern int test_directive();
extern int test_instruction();

int test_main()
{
	assert(test_parser() == 0);
	assert(test_instruction() == 0);
	assert(test_directive() == 0);
	assert(test_assembler() == 0);
	return 0;
}
