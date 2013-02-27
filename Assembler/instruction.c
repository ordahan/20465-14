/*
 * instruction.c
 *
 *  Created on: Feb 20, 2013
 *      Author: Or Dahan 201644929
 */


#include "instruction.h"
#include "parser.h"

#include <stdio.h>

/* fixme: warning regarding size..*/
instruction_syntax_t g_arrInstructionSyntax[ILLEGAL] =
{
 {MOV,"mov",2,{1,1,1,1},{0,1,1,1}},
 {CMP,"cmp",2,{1,1,1,1},{1,1,1,1}},
 {ADD,"add",2,{1,1,1,1},{0,1,1,1}},
 {SUB,"sub",2,{1,1,1,1},{0,1,1,1}},
 {NOT,"not",1,{0,0,0,0},{0,1,1,1}},
 {CLR,"clr",1,{0,0,0,0},{0,1,1,1}},
 {LEA,"lea",2,{0,1,1,1},{0,1,1,1}},
 {INC,"inc",1,{0,0,0,0},{0,1,1,1}},
 {DEC,"dec",1,{0,0,0,0},{0,1,1,1}},
 {JMP,"jmp",1,{0,0,0,0},{0,1,1,1}},
 {BNE,"bne",1,{0,0,0,0},{0,1,1,1}},
 {RED,"red",1,{0,0,0,0},{0,1,1,1}},
 {PRN,"prn",1,{0,0,0,0},{1,1,1,1}},
 {JSR,"jsr",1,{0,0,0,0},{0,1,0,0}},
 {RTS,"rts",0,{0,0,0,0},{0,0,0,0}},
 {STOP,"stop",0,{0,0,0,0},{0,0,0,0}},
};

/* Internal functions */
/**
 * Tests if the given addressing types are valid
 * for the opcode.
 * @param opcode Opcode to test according to.
 * @param src Addressing type for the src operand.
 * @param dest Addressing type for the dest operand.
 * @return 1 if valid, 0 if it doesn't.
 */
unsigned int is_addressing_valid_for_opcode(opcode_t opcode,
								   operand_addressing_t src,
								   operand_addressing_t dest,
								   unsigned char fCheckSrc);

/**
 *
 * @param opcode To retrieve for.
 * @return Number of operands required for the opcode, -1 if
 * there is no such opcode.
 */
int get_num_operands_for_opcode(opcode_t opcode);

/**
 * Retrieves the operands for the instruction from the given
 * list of operands.
 *
 * @param pInstruction The instruction that the operands belong to.
 * @param arrOperands List of operand strings (code).
 * @param nOperands Number of operands in the list.
 * @param io_pCode Code segment to place the found operands values (if needed)
 * @return 0 on success, anything else on failure
 */
int retrieve_operands(instruction_t* pInstruction,
					  char** arrOperands,
					  unsigned int nOperands,
					  code_section_t* io_pCode);
/* fixme: const char**?*/

/**
 * Retrieve the given operand (type and places data in the given
 * code section).
 * @param io_pCode Where to place the extra data that the operand takes
 * @return Addressing type for the operand.
 */
operand_addressing_t retrieve_operand(char* szOperand,
									  code_section_t* io_pCode);

/* Implementation */
int instruction_compile(const statement_t *pInstruction,
						code_section_t* io_pCode)
{
	char* arrOperands[MAX_NUM_OPERANDS];
	unsigned int type,comb;
	int nOperands;
	instruction_t* pCompiledInstruction;

	if (pInstruction == NULL ||
		io_pCode == NULL)
		return -1;

	/* Point to the result area */
	pCompiledInstruction = (instruction_t*)&io_pCode->content[io_pCode->IC];
	io_pCode->localities[io_pCode->IC] = ADDR_RELOCATABLE;
	io_pCode->IC++;

	/* Set the opcode */
	pCompiledInstruction->opcode = pInstruction->info.instruction.name;

	/* Retrieve opcode's configuration */
	nOperands = get_num_operands_for_opcode(pCompiledInstruction->opcode);

	/* Must be a valid number */
	if (nOperands < 0)
		return -1;

	/* Set the type */
	type = parser_get_instruction_type(pInstruction->info.instruction.modifiers);
	if (type == INVALID_TYPE)
	{
		printf("Error! Invalid type: ");
		return -1;
	}
	else
	{
		pCompiledInstruction->type = type;
	}

	/* Set the comb */
	pCompiledInstruction->comb = INST_COMB_MSB_MSB;
	comb = parser_get_instruction_comb(pInstruction->info.instruction.modifiers);
	if (comb == INVALID_COMB)
	{
		printf("Error! Invalid comb: ");
		return -1;
	}
	else if (type == INST_TYPE_20_BIT)
	{
		/* Shouldn't have any comb set */
		if (comb != NO_COMB)
		{
			printf("Error! type is %d, there should be no comb: ",
					INST_TYPE_20_BIT);
			return -1;
		}
	}
	else
	{
		pCompiledInstruction->comb = comb;
	}

	/* Get the operands themselves */
	if (parser_get_items_from_list(
			pInstruction->szOperationData,
		    arrOperands,
		    nOperands) != 0)
    {
		printf("Error! Invalid number of operands: ");
		return -1;
    }

	/* Retrieve the operands associated with this instruction */
	if (retrieve_operands(pCompiledInstruction,
						  arrOperands,
						  nOperands,
						  io_pCode) != 0)
	{
		/* todo: any error msg? */
		return -1;
	}

	/* Update the code section with the compiled data */


	return 0;
}

unsigned int is_addressing_valid_for_opcode(opcode_t opcode,
								   operand_addressing_t src,
								   operand_addressing_t dest,
								   unsigned char fCheckSrc)
{
	unsigned int i;

	/* Search for the given opcode */
	for (i = 0; i < ILLEGAL; ++i)
	{
		if (g_arrInstructionSyntax[i].opcode == opcode)
		{
			/* Check that both types for src and dest are mared
			 * as valid */
			if  ((g_arrInstructionSyntax[i].src.types[src] == 1) &&
				 (g_arrInstructionSyntax[i].dest.types[dest] == 1))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

int get_num_operands_for_opcode(opcode_t opcode)
{
	unsigned int i;

	/* Search for the given opcode */
	for (i = 0; i < ILLEGAL; ++i)
	{
		if (g_arrInstructionSyntax[i].opcode == opcode)
		{
			return g_arrInstructionSyntax[i].numOperands;
		}
	}

	return -1;
}

int retrieve_operands(instruction_t* pInstruction,
					  char** arrOperands,
					  unsigned int nOperands,
					  code_section_t* io_pCode)
{
	unsigned char fValidAddressing = 1;

	/* Binary operation */
	if (nOperands == 2)
	{
		pInstruction->src_addressing = retrieve_operand(arrOperands[0],
														io_pCode);
		pInstruction->dest_addressing = retrieve_operand(arrOperands[1],
														 io_pCode);
		fValidAddressing = is_addressing_valid_for_opcode(
							   pInstruction->opcode,
							   pInstruction->src_addressing,
							   pInstruction->dest_addressing,
							   1);
	}
	/* Unary operation */
	else if(nOperands == 1)
	{
		pInstruction->dest_addressing = retrieve_operand(arrOperands[1],
														 io_pCode);
		fValidAddressing = is_addressing_valid_for_opcode(
							   pInstruction->opcode,
							   pInstruction->src_addressing,
							   pInstruction->dest_addressing,
							   0);
	}

	/* Test addressing types for the opcode */
	if (0 == fValidAddressing)
	{
		printf("Error! Operands addressing is invalid for instruction: ");
		return -1;
	}

	return 0;
}

operand_addressing_t retrieve_operand(char* szOperand,
									  code_section_t* io_pCode)
{
	return OPERAND_ADDR_NUM;
}
