/*
 * instruction.c
 *
 *  Created on: Feb 20, 2013
 *      Author: Or Dahan 201644929
 */


#include "instruction.h"
#include "parser.h"

#include <stdio.h>
#include <memory.h>

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
 * If there is no need to check one of the addressing types,
 * mark the type by giving it a value of "OPERAND_ADDR_NUM".
 *
 * @param opcode Opcode to test according to.
 * @param src Addressing type for the src operand.
 * @param dest Addressing type for the dest operand.
 * @return 1 if valid, 0 if it doesn't.
 */
unsigned int is_addressing_valid_for_opcode(opcode_t opcode,
								   operand_addressing_t src,
								   operand_addressing_t dest);

/**
 *
 * @param opcode To retrieve for.
 * @return Number of operands required for the opcode, -1 if
 * there is no such opcode.
 */
int get_num_operands_for_opcode(opcode_t opcode);

/**
 * Retrieves the operands for the instruction from the given
 * list of operands. The resulting operand data is saved
 * within the given instruction and the number of extra
 * data cells used is updated as well as well as the
 * address locality for each data cell.
 *
 * @param pInstruction The instruction that the operands belong to.
 * @param arrOperands List of operand strings (code).
 * @param nOperands Number of operands in the list.
 * @return 0 on success, anything else on failure
 */
int retrieve_operands(instruction_with_operands_t* pInstruction,
					  char** arrOperands,
					  unsigned int nOperands);
/* fixme: const char**?*/

/* Implementation */
int instruction_compile(const statement_t *pInstructionStatement,
						code_section_t* io_pCode)
{
	char* arrOperands[MAX_NUM_OPERANDS];
	unsigned int type,comb;
	int nOperands;
	instruction_with_operands_t complete_instruction;
	instruction_t* pInst = &complete_instruction.instruction;

	if (pInstructionStatement == NULL ||
		io_pCode == NULL)
		return -1;

	/* Set the locality for the instruction code itself */
	memset(complete_instruction.localities,
			0,
			sizeof(complete_instruction.localities));
	complete_instruction.localities[0] = ADDR_ABSOLUTE;

	/* Set the opcode */
	pInst->opcode = pInstructionStatement->info.instruction.name;

	/* Retrieve opcode's configuration */
	nOperands = get_num_operands_for_opcode(pInst->opcode);

	/* Must be a valid number */
	if (nOperands < 0)
		return -1;

	/* Set the type */
	type = parser_get_instruction_type(pInstructionStatement->info.instruction.modifiers);
	if (type == INVALID_TYPE)
	{
		printf("Error! Invalid type: ");
		return -1;
	}
	else
	{
		pInst->type = type;
	}

	/* Set the comb */
	pInst->comb = INST_COMB_MSB_MSB;
	comb = parser_get_instruction_comb(pInstructionStatement->info.instruction.modifiers);
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
		pInst->comb = comb;
	}

	/* Get the operands strings */
	if (parser_get_items_from_list(
			pInstructionStatement->szOperationData,
		    arrOperands,
		    nOperands) != 0)
    {
		printf("Error! Invalid number of operands: ");
		return -1;
    }

	/* Retrieve the operands from their respective strings */
	if (retrieve_operands(&complete_instruction,
						  arrOperands,
						  nOperands) != 0)
	{
		/* todo: any error msg? */
		return -1;
	}

	/* fixme: make this more elegant */
	/* Update the code section with the compiled data */
	memcpy(&io_pCode->content[io_pCode->IC],
		   pInst,
		   sizeof(io_pCode->content[io_pCode->IC]));
	io_pCode->localities[io_pCode->IC] = complete_instruction.localities[0];
	io_pCode->IC++;
	memcpy(&io_pCode->content[io_pCode->IC],
		   complete_instruction.extra_data,
		   complete_instruction.num_extra_data *
		   sizeof(io_pCode->content[io_pCode->IC]));
	memcpy(&io_pCode->localities[io_pCode->IC],
		   &complete_instruction.localities[1],
		   complete_instruction.num_extra_data *
		   sizeof(io_pCode->localities[io_pCode->IC]));
	io_pCode->IC += complete_instruction.num_extra_data;

	return 0;
}

unsigned int is_addressing_valid_for_opcode(opcode_t opcode,
								   operand_addressing_t src,
								   operand_addressing_t dest)
{
	unsigned int i;

	/* Search for the given opcode */
	for (i = 0; i < ILLEGAL; ++i)
	{
		if (g_arrInstructionSyntax[i].opcode == opcode)
		{
			/* Check that both types for src and dest are marked
			 * as valid (if there are any types specified for either)*/
			if  (((OPERAND_ADDR_NUM == src) ||
				  (g_arrInstructionSyntax[i].src.types[src] == 1)) &&
				 ((OPERAND_ADDR_NUM == dest) ||
				  (g_arrInstructionSyntax[i].dest.types[dest] == 1)))
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

int retrieve_operands(instruction_with_operands_t* pInstruction,
					  char** arrOperands,
					  unsigned int nOperands)
{
	operand_addressing_t addresingSrc, addressingDest;
	addresingSrc = addressingDest = OPERAND_ADDR_NUM;

	/* Binary operation */
	if (nOperands == 2)
	{
		/* fixme: is the first always src ? */
		/* Get the source operand */
		addresingSrc = parser_get_operand(arrOperands[0],
							   pInstruction);
		/* If it was a register, it was stored in the dest.
		 * Copy it to the src.
		 * If it wasn't, it won't matter anyway.
		 */
		pInstruction->instruction.src_reg = pInstruction->instruction.dest_reg;

		/* Get the destination operand */
		addressingDest = parser_get_operand(arrOperands[1],
								pInstruction);
	}
	/* Unary operation */
	else if(nOperands == 1)
	{
		/* Get the destination operand */
		addressingDest = parser_get_operand(arrOperands[0],
								pInstruction);
	}

	/* Test addressing types for the opcode */
	if (0 == is_addressing_valid_for_opcode(
							   pInstruction->instruction.opcode,
							   addresingSrc,
							   addressingDest))
	{
		printf("Error! Operands addressing is invalid for instruction: ");
		return -1;
	}

	pInstruction->instruction.src_addressing = addresingSrc;
	pInstruction->instruction.dest_addressing = addressingDest;

	return 0;
}
