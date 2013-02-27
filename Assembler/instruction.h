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

/* Defines */
#define MAX_NUM_OPERANDS (2)
#define MAX_INSTRUCTION_EXTRA_DATA_CELLS (4)

/* Typedefs */
typedef enum
{
	INST_COMB_MSB_MSB =	0,
	INST_COMB_MSB_LSB = 1,
	INST_COMB_LSB_MSB = 2,
	INST_COMB_LSB_LSB = 3
}instruction_comb_t;
#define INVALID_COMB (4)
#define NO_COMB (5)

typedef enum
{
	INST_TYPE_20_BIT = 0,
	INST_TYPE_10_BIT = 1
}instruction_type_t;
#define INVALID_TYPE (2)

typedef enum
{
	OPERAND_ADDR_IMMEDIATE = 0,
	OPERAND_ADDR_DIRECT = 1,
	OPERAND_ADDR_INDEX_IMMEDIATE = 2,
	OPERAND_ADDR_INDEX_DIRECT = 2,
	OPERAND_ADDR_INDEX_REGISTER = 2,
	OPERAND_ADDR_REGISTER  = 3
}operand_addressing_t;/* fixme: remove sub types for index */
#define OPERAND_ADDR_NUM (OPERAND_ADDR_REGISTER+1)

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
	unsigned char types[OPERAND_ADDR_NUM];
}operand_legal_address_types_t;

typedef struct
{
	opcode_t 	opcode;
	const char* name;
	unsigned char numOperands;
	operand_legal_address_types_t src;
	operand_legal_address_types_t dest;
}instruction_syntax_t;

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

typedef struct
{
	/* How many actual extra data does the instruction have */
	unsigned char		 num_extra_data;

	/* Memory cells for the entire instruction */
	instruction_t		 instruction;
	machine_cell_t		 extra_data[MAX_INSTRUCTION_EXTRA_DATA_CELLS];

	/* Extra first locality is reserved for the instruction itself */
	address_locality_t   localities[MAX_INSTRUCTION_EXTRA_DATA_CELLS + 1];
}instruction_with_operands_t;

/* Externals */
extern instruction_syntax_t g_arrInstructionSyntax[ILLEGAL];

/* API */

/**
 * Compile the instruction statement into an actual instruction.
 * Leave all the addresses unresolved.
 *
 * * Makes sure the operand's addressing methods are valid for the
 * opcode.
 *
 * * Calculates the number of memory cells needed for
 * the instruction (the opcode + memory cells for operands).
 *
 * * Places the compiled instruction in the current IC of the code
 * section given (including 0'd cells for unresolved addresses in
 * the operands).
 *
 * * Advances the IC accordingly.
 *
 * * Sets the 'type' of the instruction (which parts of the operands
 * the processor will actually use at runtime).
 *
 * @param pInstruction Statement to compile.
 * @param io_pCode Code section to put the compiled result into.
 * @return 0 on success, anything else on error.
 */
int instruction_compile(const statement_t *pInstruction,
						code_section_t* io_pCode);

#endif /* INSTRUCTION_H_ */
