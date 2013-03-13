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

int test_compile_instruction();

int run_test_compile_instruction(const char		*line,
								 code_section_t *code,
								 const code_section_t *expected,
								 char fShouldSucceed)
{
	unsigned i;
	statement_t statement;
	symbol_table_arr_t symbols;

	memset(symbols, 0, sizeof(symbols));

	/* Retrieve the line to compile */
	strcpy(statement.szContent, line);

	/* Create a statement from the given line */
	assert(0 == parser_get_statement(&statement));

	/* Check that the compilation returns as expected */
	assert(!((0 == instruction_compile(&statement, code, symbols)) ^
			 fShouldSucceed));

	/* Check the IC */
	assert(expected->IC == code->IC);

	/* Check the label if exists */
	if (statement.szLabel != NULL)
	{
		assert(0 == strcmp(statement.szLabel, symbols[0].name));
		assert(code->IC == symbols[0].address);
		assert(ADDR_ABSOLUTE == symbols[0].locality);
	}

	/* Check the code generated */
	for (i = 0; i < expected->IC; ++i)
	{
		assert(0 == memcmp(&expected->content[i],
						   &code->content[i],
						   sizeof(expected->content[0])));
		assert(expected->localities[i] == code->localities[i]);
	}

	printf("PASSED.\n");
	return 0;
}

int test_instruction()
{
	assert(test_compile_instruction() == 0);

	return 0;
}

int test_compile_instruction()
{
	code_section_t code,expected;
	instruction_t * const pExpectedInstruction = (instruction_t*)&expected.content[0];

	/*
	 * #No operands
	 * #One operand
	 * #Two operands
	 * #Consecutive instructions
	 * Label
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
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->opcode = STOP;
	expected.IC = 1;
	assert(0 == run_test_compile_instruction("stop/0",
											 &code,
											 &expected,
											 1));
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
	/**********************************************/

	/**********************************************/
	printf("	One operand: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_reg = R7;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = INC;
	expected.IC = 1;
	assert(0 == run_test_compile_instruction("inc/0 r7",
											 &code,
											 &expected,
											 1));
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
	/**********************************************/

	/**********************************************/
	printf("	Two operands: \n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	expected.IC = 1;
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 1));
	printf("\t\t");
	memset(&code, 0, sizeof(code));
	assert(0 == run_test_compile_instruction("mov/0 r0 ,r1",
												 &code,
												 &expected,
												 1));
	printf("\t\t");
	memset(&code, 0, sizeof(code));
	assert(0 == run_test_compile_instruction("mov/0 r0, r1",
												 &code,
												 &expected,
												 1));
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
	/**********************************************/

	/**********************************************/
	printf("	Consecutive instructions: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	expected.IC = 1;
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 1));
	expected.localities[1] = ADDR_ABSOLUTE;
	((instruction_t*)&expected.content[1])->dest_reg = R0;
	((instruction_t*)&expected.content[1])->dest_addressing = OPERAND_ADDR_REGISTER;
	((instruction_t*)&expected.content[1])->opcode = INC;
	expected.IC = 2;
	assert(0 == run_test_compile_instruction("inc/0 r0",
											 &code,
											 &expected,
											 1));

	/**********************************************/

	/**********************************************/
	printf("	Label: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	expected.IC = 1;
	assert(0 == run_test_compile_instruction("mov/0 r0,r1",
											 &code,
											 &expected,
											 1));
	expected.localities[1] = ADDR_ABSOLUTE;
	((instruction_t*)&expected.content[1])->dest_reg = R0;
	((instruction_t*)&expected.content[1])->dest_addressing = OPERAND_ADDR_REGISTER;
	((instruction_t*)&expected.content[1])->opcode = INC;
	expected.IC = 2;
	assert(0 == run_test_compile_instruction("inc/0 r0",
											 &code,
											 &expected,
											 1));

	/**********************************************/

	/**********************************************/
	printf("	One extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_DIRECT;
	pExpectedInstruction->opcode = MOV;
	expected.localities[1] = ADDR_RELOCATABLE;
	expected.content[1].val = 0;
	expected.IC = 2;
	assert(0 == run_test_compile_instruction("mov/0 x,r1",
											 &code,
											 &expected,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Two extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_DIRECT;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_IMMEDIATE;
	pExpectedInstruction->opcode = MOV;
	expected.localities[1] = ADDR_ABSOLUTE;
	expected.content[1].val = 3;
	expected.localities[2] = ADDR_RELOCATABLE;
	expected.content[2].val = 0;
	expected.IC = 3;
	assert(0 == run_test_compile_instruction("mov/0 #3,y",
											 &code,
											 &expected,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Three extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_DIRECT;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->opcode = MOV;
	expected.localities[1] = ADDR_RELOCATABLE;
	expected.content[1].val = 0;
	expected.localities[2] = ADDR_ABSOLUTE;
	expected.content[2].val = 5;
	expected.localities[3] = ADDR_RELOCATABLE;
	expected.content[3].val = 0;
	expected.IC = 4;
	assert(0 == run_test_compile_instruction("mov/0 x{5},y",
											 &code,
											 &expected,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Four extra data: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_INDEX;
	pExpectedInstruction->opcode = MOV;
	expected.localities[1] = ADDR_RELOCATABLE;
	expected.content[1].val = 0;
	expected.localities[2] = ADDR_ABSOLUTE;
	expected.content[2].val = 5;
	expected.localities[3] = ADDR_RELOCATABLE;
	expected.content[3].val = 0;
	expected.localities[4] = ADDR_RELOCATABLE;
	expected.content[4].val = 0;
	expected.IC = 5;
	assert(0 == run_test_compile_instruction("mov/0 x{5},y{z}",
											 &code,
											 &expected,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Invalid addressing:\n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	expected.IC = 0;
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0 r0,#0",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("jsr/0 #31",
											 &code,
											 &expected,
											 0));
	/**********************************************/
	printf("	Type'd instruction: ");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	pExpectedInstruction->comb = INST_COMB_MSB_LSB;
	pExpectedInstruction->dest_reg = R1;
	pExpectedInstruction->dest_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->src_reg = R0;
	pExpectedInstruction->src_addressing = OPERAND_ADDR_REGISTER;
	pExpectedInstruction->opcode = MOV;
	pExpectedInstruction->type = INST_TYPE_10_BIT;
	expected.IC = 1;
	assert(0 == run_test_compile_instruction("mov/1/0/1 r0,r1",
											 &code,
											 &expected,
											 1));
	/**********************************************/

	/**********************************************/
	printf("	Invalid type'd instruction:\n");
	memset(&code, 0, sizeof(code));
	memset(&expected, 0, sizeof(expected));
	expected.localities[0] = ADDR_ABSOLUTE;
	expected.IC = 0;
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/1 r0,r1",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/0/1/1 r0,r1",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/1/2/1 r0,r1",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/2/0/1 r0,r1",
											 &code,
											 &expected,
											 0));
	printf("\t\t");
	assert(0 == run_test_compile_instruction("mov/3 r0,r1",
											 &code,
											 &expected,
											 0));
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
