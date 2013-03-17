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
#include "test_utils.h"

int test_assembler_compile(const char* szTestFile,
						   symbol_table_arr_t symbol_expected,
						   memory_section_t *code_expected,
						   memory_section_t *data_expected,
						   int expected_result);

int test_assembler()
{
	static symbol_table_arr_t symbols_expected;
	static memory_section_t code_expected;
	static memory_section_t data_expected;

	symbol_t *pSymbol = NULL;
	instruction_t instruction;
	instruction_t *pInstruction = &instruction;

	printf("Testing assembler module:\n");

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	Empty file: ");
	assert(0 == test_assembler_compile("tests/empty.as",
									   symbols_expected,
									   &code_expected,
									   &data_expected,
									   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
					  &code_expected,
					  &data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	File only with comments: ");
	assert(0 == test_assembler_compile("tests/comments.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_EXTERNAL;
	strcpy(pSymbol->name, "ThisIsMySymbol!");

	printf("	File only with extern: ");
	assert(0 == test_assembler_compile("tests/extern.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
					  &code_expected,
					  &data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_ABSOLUTE;
	strcpy(pSymbol->name, "ThisIsMySymbol!");

	/*mov/0 #3,r1*/
	pInstruction->comb = INST_COMB_MSB_MSB;
	pInstruction->type = INST_TYPE_20_BIT;
	pInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pInstruction->dest_reg = R1;
	pInstruction->opcode = MOV;
	pInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;

	section_write(&code_expected, instruction.raw, ADDR_ABSOLUTE);
	section_write(&code_expected, 3, ADDR_ABSOLUTE);

	printf("	File with entry and instruction: ");
	assert(0 == test_assembler_compile("tests/entry.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	Syntax error: ");
	assert(0 == test_assembler_compile("tests/syntax_err.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										-1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	Invalid extern: ");
	assert(0 == test_assembler_compile("tests/invalid_extern.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										-1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	Invalid entry: ");
	assert(0 == test_assembler_compile("tests/invalid_entry.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										-1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_RELOCATABLE;
	strcpy(pSymbol->name, "LOL");
	pSymbol = &symbols_expected[1];
	pSymbol->locality = ADDR_RELOCATABLE;
	pSymbol->address = 5;
	strcpy(pSymbol->name, "rofl");

	section_write(&data_expected, -5, ADDR_ABSOLUTE);
	section_write(&data_expected, 1, ADDR_ABSOLUTE);
	section_write(&data_expected, 3, ADDR_ABSOLUTE);
	section_write(&data_expected, 14, ADDR_ABSOLUTE);
	section_write(&data_expected, 100, ADDR_ABSOLUTE);
	section_write(&data_expected, 'H',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'e',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'l',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'l',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'o',  ADDR_ABSOLUTE);
	section_write(&data_expected, '\0',  ADDR_ABSOLUTE);


	printf("	.data and .string: ");
	assert(0 == test_assembler_compile("tests/data_and_string.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	printf("	.data error ");
	assert(0 == test_assembler_compile("tests/data_error.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										-1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);

	printf("	.string error ");
	assert(0 == test_assembler_compile("tests/string_error.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										-1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	pSymbol = &symbols_expected[2];
	pSymbol->locality = ADDR_ABSOLUTE;
	pSymbol->address = 0;
	strcpy(pSymbol->name, "ThisIsMySymbol!");

	pInstruction->comb = INST_COMB_MSB_MSB;
	pInstruction->type = INST_TYPE_20_BIT;
	pInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pInstruction->dest_reg = R1;
	pInstruction->opcode = MOV;
	pInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;

	section_write(&code_expected, pInstruction->raw, ADDR_ABSOLUTE);
	section_write(&code_expected, 3, ADDR_ABSOLUTE);

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_RELOCATABLE;
	pSymbol->address = 2;
	strcpy(pSymbol->name, "LOL");
	pSymbol = &symbols_expected[1];
	pSymbol->locality = ADDR_RELOCATABLE;
	pSymbol->address = 7;
	strcpy(pSymbol->name, "rofl");

	section_write(&data_expected, -5, ADDR_ABSOLUTE);
	section_write(&data_expected, 1, ADDR_ABSOLUTE);
	section_write(&data_expected, 3, ADDR_ABSOLUTE);
	section_write(&data_expected, 14, ADDR_ABSOLUTE);
	section_write(&data_expected, 100, ADDR_ABSOLUTE);
	section_write(&data_expected, 'H',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'e',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'l',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'l',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'o',  ADDR_ABSOLUTE);
	section_write(&data_expected, '\0',  ADDR_ABSOLUTE);

	printf("	instruction and data: ");
	assert(0 == test_assembler_compile("tests/instruction_and_data.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	init_program_data(symbols_expected,
								&code_expected,
								&data_expected);
	memset(pInstruction, 0, sizeof(*pInstruction));

	pSymbol = &symbols_expected[0];
	pSymbol->locality = ADDR_RELOCATABLE;
	pSymbol->address = 9;
	strcpy(pSymbol->name, "lol");

	pSymbol = &symbols_expected[1];
	pSymbol->locality = ADDR_EXTERNAL;
	pSymbol->address = 0;
	strcpy(pSymbol->name, "Hallo");

	pSymbol = &symbols_expected[2];
	pSymbol->locality = ADDR_RELOCATABLE;
	pSymbol->address = 10;
	strcpy(pSymbol->name, "a");

	pSymbol = &symbols_expected[3];
	pSymbol->locality = ADDR_ABSOLUTE;
	pSymbol->address = 6;
	strcpy(pSymbol->name, "kicks");

	pInstruction->comb = INST_COMB_MSB_MSB;
	pInstruction->type = INST_TYPE_20_BIT;
	pInstruction->dest_addressing = OPERAND_ADDR_DIRECT;
	pInstruction->opcode = MOV;
	pInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;

	section_write(&code_expected, pInstruction->raw, ADDR_ABSOLUTE);
	section_write(&code_expected, 1, ADDR_ABSOLUTE);
	section_write(&code_expected, 9, ADDR_RELOCATABLE); /* fixme: entry is relocatable? */

	pInstruction->comb = INST_COMB_MSB_MSB;
	pInstruction->type = INST_TYPE_20_BIT;
	pInstruction->dest_addressing = OPERAND_ADDR_DIRECT;
	pInstruction->opcode = MOV;
	pInstruction->src_addressing = OPERAND_ADDR_DIRECT;

	section_write(&code_expected, pInstruction->raw, ADDR_ABSOLUTE);
	section_write(&code_expected, 0, ADDR_EXTERNAL);
	section_write(&code_expected, 9, ADDR_RELOCATABLE);

	pInstruction->comb = INST_COMB_MSB_MSB;
	pInstruction->type = INST_TYPE_20_BIT;
	pInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pInstruction->dest_reg = R2;
	pInstruction->opcode = MOV;
	pInstruction->src_addressing = OPERAND_ADDR_INDEX;

	section_write(&code_expected, pInstruction->raw, ADDR_ABSOLUTE);
	section_write(&code_expected, 10, ADDR_RELOCATABLE);
	section_write(&code_expected, 9, ADDR_RELOCATABLE); /* fixme: entry is relocatable? */

	section_write(&data_expected, -20, ADDR_ABSOLUTE);
	section_write(&data_expected, 'h',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'm',  ADDR_ABSOLUTE);
	section_write(&data_expected, 'z',  ADDR_ABSOLUTE);
	section_write(&data_expected, '\0',  ADDR_ABSOLUTE);

	printf("	instruction address resolution: \n");
	assert(0 == test_assembler_compile("tests/instruction_address_resolution.as",
										symbols_expected,
										&code_expected,
										&data_expected,
										   0));
	/**********************************************/

	/*
	 * #syntax error
	 * #invalid extern
	 * #invalid entry
	 * #.data + .string
	 * #.data error
	 * #.string error
	 * #instruction + .data (data after text)
	 * #instruction address resolution
	 */
	printf("PASSED.\n");
	return 0;
}


int test_assembler_compile(const char* szTestFile,
						   symbol_table_arr_t symbol_expected,
						   memory_section_t *code_expected,
						   memory_section_t *data_expected,
						   int expected_result)
{
	static symbol_table_arr_t symbols;
	static memory_section_t code;
	static memory_section_t data;
	int compile_res = 0;

	init_program_data(symbols, &code, &data);

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

	/* Don't forget to close the test file */
	fclose(fd);

	if (expected_result == 0)
	{
		if (compile_res != 0)
		{
			return -2;
		}
	}
	else
	{
		if (compile_res == 0)
		{
			return -2;
		}

		return 0;
	}

	/* Check the code section */
	if (compare_memory_sections(code_expected, &code) != 0)
	{
		printf("Code sections don't match.\n");
		return -1;
	}

	/* Check the data section */
	if (compare_memory_sections(data_expected, &data) != 0)
	{
		printf("Data sections don't match.\n");
		return -1;
	}

	/* Check the symbol table */
	{
		unsigned i;
		for (i = 0; i < MAX_SYMBOLS; ++i)
		{
			if (symbol_expected[i].address != symbols[i].address ||
				symbol_expected[i].locality != symbols[i].locality ||
				0 != strcmp(symbol_expected[i].name, symbols[i].name))
			{
				printf("Symbol in index %d doesn't match expected.\n", i);
				return -3;
			}
		}
	}

	return 0;
}
