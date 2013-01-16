/*
 * test_main.c
 *
 *  Created on: Feb 5, 2013
 *      Author: Or Dahan 201644929
 */

#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "assembler.h"

int test_assembler();

int test_main()
{
	assert(test_assembler() == 0);

	return 0;
}

void init_test_assembler_compile(symbol_table_t *symbol_expected,
						   	   	 code_segment_t *code_expected,
						   	   	 data_segment_t *data_expected)
{
	memset(*symbol_expected, 0, sizeof(*symbol_expected));
	memset(*code_expected, 0, sizeof(*code_expected));
	memset(*data_expected, 0, sizeof(*data_expected));
}

int test_assembler_compile(const char* szTestFile,
						   symbol_table_t *symbol_expected,
						   code_segment_t *code_expected,
						   data_segment_t *data_expected)
{
	static symbol_table_t symbols;
	static code_segment_t code;
	static data_segment_t data;

	init_test_assembler_compile(&symbols, &code, &data);

	/* Open the test file */
	FILE* fd = fopen(szTestFile, "r");
	assert(fd != NULL);

	/* Compile it */
	assert(assembler_compile(fd,
							 &symbols,
							 &code,
							 &data) == 0);

	/* Don't forget to close the test file */
	fclose(fd);

	/* Check that we got what we expected */
	assert(0 == memcmp(*symbol_expected,
					   symbols,
					   sizeof(symbols)));
	assert(0 == memcmp(*code_expected,
					   code,
					   sizeof(code)));
	assert(0 == memcmp(*data_expected,
					   data,
					   sizeof(data)));
	return 0;
}

int test_assembler()
{
	static symbol_table_t symbols_expected;
	static code_segment_t code_expected;
	static data_segment_t data_expected;

	printf("Testing assembler module:\n");

	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	printf("	Empty file: \n");
	test_assembler_compile("tests/empty.as", &symbols_expected, &code_expected, &data_expected);

	printf("	File only with comments: \n");
	test_assembler_compile("tests/comments.as", &symbols_expected, &code_expected, &data_expected);

	return 0;
}
