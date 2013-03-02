/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "assembler.h"
#include "instruction.h"

int test_assembler_compile(const char* szTestFile,
						   symbol_table_arr_t *symbol_expected,
						   code_section_t *code_expected,
						   data_section_t *data_expected);

void init_test_assembler_compile(symbol_table_arr_t *symbol_expected,
						   	   	 code_section_t *code_expected,
						   	   	 data_section_t *data_expected);

int test_assembler()
{
	static symbol_table_arr_t symbols_expected;
	static code_section_t code_expected;
	static data_section_t data_expected;

	symbol_t *pSymbol = NULL;
	instruction_t *pInstruction = NULL;

	printf("Testing assembler module:\n");

	/**********************************************/
	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	printf("	Empty file: ");
	assert(0 == test_assembler_compile("tests/empty.as",
									   &symbols_expected,
									   &code_expected,
									   &data_expected));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	printf("	File only with comments: ");
	assert(0 == test_assembler_compile("tests/comments.as",
										&symbols_expected,
										&code_expected,
										&data_expected));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_EXTERNAL;
	strcpy(pSymbol->name, "ThisIsMySymbol!");

	printf("	File only with extern: ");
	assert(0 == test_assembler_compile("tests/extern.as",
										&symbols_expected,
										&code_expected,
										&data_expected));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_ENTRY;
	strcpy(pSymbol->name, "ThisIsMySymbol!");

	printf("	File only with entry: ");
	assert(0 == test_assembler_compile("tests/entry.as",
										&symbols_expected,
										&code_expected,
										&data_expected));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_test_assembler_compile(&symbols_expected,
								&code_expected,
								&data_expected);

	pInstruction = (instruction_t*)&code_expected.content[0];
	pInstruction->comb = 0;
	pInstruction->dest_reg = R1;
	pInstruction->dest_addressing =  OPERAND_ADDR_REGISTER;
	pInstruction->src_reg = R0; /* NOT REFERENCED, CONVENTION */
	pInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;
	pInstruction->opcode = MOV;
	pInstruction->type = 0;
	pInstruction->rfu = 0; /* NOT REFERENCED, CONVENTION */
	code_expected.content[1].val = 3;

	printf("	File only with one instruction: ");
	assert(0 == test_assembler_compile("tests/instruction.as",
										&symbols_expected,
										&code_expected,
										&data_expected));
	printf("PASSED.\n");
	/**********************************************/

	/* todo: test cases for the assembler
	 * instruction with label
	 * .data
	 * .string
	 * .data + .string
	 * instruction + .data
	 * mix n' match (appropriate program)
	 */
	return 0;
}


void init_test_assembler_compile(symbol_table_arr_t *symbol_expected,
						   	   	 code_section_t *code_expected,
						   	   	 data_section_t *data_expected)
{
	memset(*symbol_expected, 0, sizeof(*symbol_expected));
	memset(code_expected, 0, sizeof(*code_expected));
	memset(data_expected, 0, sizeof(*data_expected));
}

int test_assembler_compile(const char* szTestFile,
						   symbol_table_arr_t *symbol_expected,
						   code_section_t *code_expected,
						   data_section_t *data_expected)
{
	static symbol_table_arr_t symbols;
	static code_section_t code;
	static data_section_t data;
	int compile_res = 0;

	init_test_assembler_compile(&symbols, &code, &data);

	/* Open the test file */
	FILE* fd = fopen(szTestFile, "r");
	if(fd == NULL)
	{
		return -1;
	}

	/* Compile it */
	compile_res = assembler_compile(fd,
									 symbols,
									 &code,
									 &data);
	if (compile_res != 0)
	{
		return -2;
	}

	/* Don't forget to close the test file */
	fclose(fd);

	/* Check that we got what we expected */
	if (0 != memcmp(*symbol_expected,
				    symbols,
				    sizeof(symbols)))
		return -3;

	if (0 != memcmp(code_expected,
					&code,
					sizeof(code)))
		return -4;
	if (0 != memcmp(data_expected,
					&data,
					sizeof(data)))
		return -5;

	return 0;
}
