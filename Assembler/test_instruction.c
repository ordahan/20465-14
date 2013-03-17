/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <strings.h>

#include "instruction.h"
#include "parser.h"
#include "test_utils.h"

int test_compile_instruction();

int run_test_compile_instruction(const char		*line,
								 memory_section_t *code,
								 const memory_section_t *expected,
								 symbol_table_arr_t symbols,
								 char fShouldSucceed)
{
	statement_t statement;

	/* Retrieve the line to compile */
	strcpy(statement.szContent, line);

	/* Create a statement from the given line */
	assert(0 == parser_get_statement(&statement));

	/* Shallow parse the statement */
	instruction_shallow_parse(&statement); /* fixme: maybe error value? */

	/* Check that the compilation returns as expected */
	assert(!((0 == instruction_compile(&statement, code, symbols)) ^
			 fShouldSucceed));

	if (fShouldSucceed == 1)
	{
		/* Check the label if exists */
		if (statement.szLabel != NULL)
		{
			assert(0 == strcmp(statement.szLabel, symbols[0].name));
			assert(code->counter_a == symbols[0].address);
			assert(ADDR_ABSOLUTE == symbols[0].locality);
		}

		if (compare_memory_sections(expected, code) != 0)
		{
			printf("Code sections don't match.\n");
			return -1;
		}
	}

	printf("PASSED.\n");
	return 0;
}

int test_instruction()
{
	assert(test_compile_instruction() == 0);
	/* todo: test first pass instruction
	 */
#if 0
	printf("\t\t");
	assert(0 == run_test_compile_instruction("stop/0 r0",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("stop/0 r0,r1",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("inc/0 r7,r0",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("inc/0",
											 &code,
											 &expected,
											 0));

	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,r1,r2",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0",
											 &code,
											 &expected,
											 0));

#endif

	return 0;
}

int test_compile_instruction()
{
	memory_section_t code,expected;
	symbol_table_arr_t symbols;
	instruction_t instruction;
	instruction_t * const pExpectedInstruction = &instruction;

	/*
	 * #No operands
	 * #One operand
	 * #Two operands
	 * #Consecutive instructions
	 * #Label
	 * #One extra data
	 * #Two extra data
	 * #Three extra data
	 * #Four extra data
	 * #Invalid addressing modes
	 * #Type set
	 * #Invalid type
	 */

	printf("Testing compiling instructions:\n");

	/**********************************************/
	printf("	No operands: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->opcode = STOP;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	assert(0 == run_test_compile_instruction("stop/0",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	One operand: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->dest_reg = R7;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = INC;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	assert(0 == run_test_compile_instruction("inc/0 r7",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Two operands: \n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 1));
	printf("\t\t");
	memset(&code, 0, sizeof(code));
	assert(0 == run_test_compile_instruction("mov/0 r0 ,r1",
												 &code,
												 &expected,
												 symbols,
												 1));
	printf("\t\t");
	memset(&code, 0, sizeof(code));
	assert(0 == run_test_compile_instruction("mov/0 r0, r1",
												 &code,
												 &expected,
												 symbols,
												 1));
	/**********************************************/

	/**********************************************/
	printf("	Consecutive instructions: \n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 1));

	instruction_set_values(pExpectedInstruction,
						   0,
						   R0,
						   OPERAND_ADDR_REGISTER,
						   0,
						   0,
						   INC,
						   0);
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	printf("\t\t");
	assert(0 == run_test_compile_instruction("inc/0 r0",
											 &code,
											 &expected,
											 symbols,
											 1));

	/**********************************************/

	/**********************************************/
	printf("	Label: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 1));
	instruction_set_values(pExpectedInstruction,
						   0,
						   R0,
						   OPERAND_ADDR_REGISTER,
						   0,
						   0,
						   INC,
						   0);
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	assert(0 == run_test_compile_instruction("inc/0 r0",
											 &code,
											 &expected,
											 symbols,
											 1));

	/**********************************************/

	/**********************************************/
	printf("	One extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	memset(symbols, 0, sizeof(symbols));
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_DIRECT;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	strcpy(symbols[0].name, "x");
	symbols[0].locality = ADDR_RELOCATABLE;
	assert(0 == run_test_compile_instruction("mov/0 x,r1",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Two extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	memset(symbols, 0, sizeof(symbols));
	instruction_set_values(pExpectedInstruction,
						   0,
						   0,
						   OPERAND_ADDR_DIRECT,
						   0,
						   OPERAND_ADDR_IMMEDIATE,
						   MOV,
						   0);
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 3, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	strcpy(symbols[0].name, "y");
	symbols[0].locality = ADDR_RELOCATABLE;
	assert(0 == run_test_compile_instruction("mov/0 #3,y",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Three extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	memset(symbols, 0, sizeof(symbols));
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_DIRECT;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	section_write(&expected, 5, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	strcpy(symbols[0].name, "x");
	symbols[0].locality = ADDR_RELOCATABLE;
	strcpy(symbols[1].name, "y");
	symbols[1].locality = ADDR_RELOCATABLE;
	assert(0 == run_test_compile_instruction("mov/0 x{5},y",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Four extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	memset(symbols, 0, sizeof(symbols));
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->opcode = MOV;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	section_write(&expected, 5, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	strcpy(symbols[0].name, "x");
	symbols[0].locality = ADDR_RELOCATABLE;
	strcpy(symbols[1].name, "y");
	symbols[1].locality = ADDR_RELOCATABLE;
	strcpy(symbols[2].name, "z");
	symbols[2].locality = ADDR_RELOCATABLE;
	assert(0 == run_test_compile_instruction("mov/0 x{5},y{z}",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Index with register: \n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	memset(symbols, 0, sizeof(symbols));
	pExpectedInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->opcode = MOV;
	pExpectedInstruction->dest_reg = R2;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 3, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	strcpy(symbols[0].name, "y");
	symbols[0].locality = ADDR_RELOCATABLE;
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 #3,y{r2}",
											 &code,
											 &expected,
											 symbols,
											 1));
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->src_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R6;
	pExpectedInstruction->dest_reg = R1;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	section_write(&expected, 0, ADDR_RELOCATABLE);
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 y{r6},r1",
												 &code,
												 &expected,
												 symbols,
												 1));
	/**********************************************/

	/**********************************************/
	printf("	Invalid addressing:\n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,#0",
											 &code,
											 &expected,
											 symbols,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("jsr/0 #31",
											 &code,
											 &expected,
											 symbols,
											 0));
	/**********************************************/
	printf("	Type'd instruction: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	pExpectedInstruction->comb = INST_COMB_MSB_LSB;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	pExpectedInstruction->type = INST_TYPE_10_BIT;
	section_write(&expected, pExpectedInstruction->raw, ADDR_ABSOLUTE);
	assert(0 == run_test_compile_instruction("mov/1/0/1 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Invalid type'd instruction:\n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/1 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0/1/1 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/1/2/1 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/2/0/1 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/3 r0,r1",
											 &code,
											 &expected,
											 symbols,
											 0));
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
