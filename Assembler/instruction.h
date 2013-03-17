/*
 * instruction.h
 *
 *  Defines and handles the instruction set in the language.
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
	OPERAND_ADDR_INDEX = 2,
	OPERAND_ADDR_REGISTER  = 3
}operand_addressing_t;
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

typedef union
{
	struct
	{
		instruction_comb_t 	 comb 			: 2;
		machine_registers_t  dest_reg		: 3;
		operand_addressing_t dest_addressing: 2;
		machine_registers_t  src_reg		: 3;
		operand_addressing_t src_addressing	: 2;
		opcode_t 			 opcode 		: 4;
		instruction_type_t	 type			: 1;
		unsigned char		 rfu			: 3;
		unsigned int 		 pad			: 12;
	};
	unsigned int raw;
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
 *
 * * Makes sure the operand's addressing methods are valid for the
 * opcodes.
 *
 * * Places the compiled instruction in the current IC of the code
 * section given.
 *
 * * Advances the IC accordingly.
 *
 * @param pInstruction Statement to compile.
 * @param io_pCode Code section to put the compiled result into.
 * @return 0 on success, anything else on error.
 */
int instruction_compile(const statement_t *pInstruction,
						memory_section_t* io_pCode,
						symbol_table_arr_t io_pSymbols);

/**
 * Parses the given instruction just a bit to find out if
 * its ok.
 * @param pInstructionStatement
 * @return number of memory cells the instruction takes
 * (1 + num of cells for the operands data). -1 is given on error.
 */
int instruction_shallow_parse(const statement_t *pInstructionStatement);

/**
 * Add an extra data cell to the given instruction.
 * @param pInstruction Instruction to add to
 * @param val Value of the data cell
 * @param locality Locality of the data cell
 */
void instruction_add_data(instruction_with_operands_t* pInstruction,
						  unsigned int val,
						  address_locality_t locality);

/**
 * Sets the given instruction's values as given,
 * and zeroes the remaining bits (rfu, pad)
 * @param pInstruction
 * @param comb
 * @param dest_reg
 * @param dest_addressing
 * @param src_reg
 * @param src_addressing
 * @param opcode
 * @param type
 */
void instruction_set_values(instruction_t* pInstruction,
					 instruction_comb_t 	 comb,
					 machine_registers_t  dest_reg,
					 operand_addressing_t dest_addressing,
					 machine_registers_t  src_reg,
					 operand_addressing_t src_addressing,
					 opcode_t 			 opcode,
					 instruction_type_t	 type);

#endif /* INSTRUCTION_H_ */
