/*
 * test_main.c
 *
 *  Created on: Feb 5, 2013
 *      Author: Or Dahan 201644929
 */

#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "test_utils.h"

extern int main(int argc, char** argv);

int test_main()
{
	char* args[] = {"Assembler", "tests/ps"};

	/* todo:
	 * example from book
	 * without externs
	 * without entries
	 * with errors (first pass)
	 * with errors (second pass)
	 */

	/**********************************************/


	printf("	example from book: ");
	assert(0 == main(sizeof(args) / sizeof(args[0]), args));
	assert(0 == compare_files("expecteds/ps.ent", "tests/ps.ent"));
	assert(0 == compare_files("expecteds/ps.ext", "tests/ps.ext"));
	assert(0 == compare_files("expecteds/ps.ob", "tests/ps.ob"));
	printf("PASSED.\n");
	/**********************************************/

	return 0;
}
