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

int test_main()
{
	assert(test_assembler() == 0);

	return 0;
}
