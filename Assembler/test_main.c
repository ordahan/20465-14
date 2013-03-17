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
extern int test_files();

int test_main();

int test_env_main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	assert(test_files() == 0);
	assert(test_main() == 0);
	assert(test_directive() == 0);
	assert(test_parser() == 0);
	assert(test_instruction() == 0);
	assert(test_assembler() == 0);

	return 0;
}

int test_main()
{
	return -1;
}
