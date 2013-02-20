/*
 * instruction.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "machine.h"
#include "statement.h"
#include "assembler.h"

/* Typedefs */
typedef enum
{
	INST_COMB_LSB_LSB = 0,
	INST_COMB_LSB_MSB = 1,
	INST_COMB_MSB_LSB = 2,
	INST_COMB_MSB_MSB = 3
}instruction_comb_t;

typedef enum
{
	INST_TYPE_20_BIT = 0,
	INST_TYPE_10_BIT = 1
}instruction_type_t;

typedef enum
{
	OPERAND_ADDR_IMMEDIATE = 0,
	OPERAND_ADDR_DIRECT = 1,
	OPERAND_ADDR_INDEX = 2,
	OPERAND_ADDR_REGISTER = 3
}operand_addressing_t;

typedef enum
{
	MOV = 0,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP,
}opcode_t;
#define ILLEGAL (STOP+1)

typedef struct
{
	instruction_comb_t 	 comb 			: 2;
	machine_registers_t  dest_reg		: 3;
	operand_addressing_t dest_addressing: 2;
	machine_registers_t  src_reg		: 3;
	operand_addressing_t src_addressing	: 2;
	opcode_t 			 opcode 		: 4;
	instruction_type_t	 type			: 1;
	unsigned char		 rfu			: 3;
}instruction_t;

/**
 * Compile the instruction statement into an actual instruction.
 * Leave all the addresses unresolved.
 * @param pInstruction Statement to compile.
 * @param io_pCode Code section to put the compiled result into.
 * @return 0 on success, anything else on error.
 */
int instruction_compile(const statement_t *pInstruction,
						code_section_t* io_pCode);

#endif /* INSTRUCTION_H_ */
