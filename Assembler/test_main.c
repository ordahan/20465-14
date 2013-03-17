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
	char* args[5] = {"Assembler"};

	/**********************************************/
	printf("	example from book: ");
	args[1] = "inputs/ps";
	args[2] = "inputs/ps_with_errors";
	args[3] = "inputs/no_externs";
	args[4] = "inputs/no_entries";
	assert(0 == main(5, args));
	assert(0 == compare_files("outputs/ps.ent", "inputs/ps.ent"));
	assert(0 == compare_files("outputs/ps.ext", "inputs/ps.ext"));
	assert(0 == compare_files("outputs/ps.ob", "inputs/ps.ob"));

	assert(NULL != fopen("inputs/no_externs.ent", "r"));
	assert(NULL == fopen("inputs/no_externs.ext", "r"));
	assert(NULL != fopen("inputs/no_externs.ob", "r"));

	assert(NULL == fopen("inputs/no_entries.ent", "r"));
	assert(NULL != fopen("inputs/no_entries.ext", "r"));
	assert(NULL != fopen("inputs/no_entries.ob", "r"));

	assert(NULL == fopen("inputs/ps_with_errors.ent", "r"));
	assert(NULL == fopen("inputs/ps_with_errors.ext", "r"));
	assert(NULL == fopen("inputs/ps_with_errors.ob", "r"));

	printf("PASSED.\n");
	/**********************************************/

	return 0;
}
