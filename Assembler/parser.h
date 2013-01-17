/*
 * parser.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 *
 *  This module is responsible for translating the given assembly
 *  program (as text) to data structures the assembler can handle.
 *  Any syntax error that isn't related to the instruction set,
 *  addresses etc will be caught here.
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "instruction.h"
#include "directive.h"

#define MAX_LINE_LENGTH 80

/* todo: decide on a good number.. maybe use some other #define
 * for memory size etc?
 */
#define MAX_STATEMENTS_IN_PROGRAM 4096

/* Typedefs */
typedef enum
{
	STATEMENT_TYPE_EMPTY,
	STATEMENT_TYPE_COMMENT,
	STATEMENT_TYPE_DIRECTIVE,
	STATEMENT_TYPE_INSTRUCTION,

	STATEMENT_TYPE_ERROR
}statement_type_t;

typedef struct
{
	opcode_t	name;
	const char* modifiers;
}instruction_statement_t;

typedef struct
{
	directive_type_t name;
}directive_statement_t;

typedef struct
{
	statement_type_t type;

	char* szLabel;			/* NULL if there is no label */

	union
	{
		directive_statement_t 	directive;
		instruction_statement_t instruction;
	}info;

	char* szOperationData;

	char szContent[MAX_LINE_LENGTH]; /* Original line */
}statement_t;

/**
 * Parses a line of assembly code into its different fields.
 * Makes sure the syntax of the label (if exists) is valid.
 * Doesn't parse the syntax of the directive's value
 * (valid number, valid string etc) or the instruction's operands
 * (number of operands, register names or even syntax of memory addressing etc),
 * only makes sure the command exists in the language.
 *
 * @param io_pLine Statement to parse - szContent should be filled with the
 * string that represents the line.
 * @return 0 if valid statement, any other val otherwise.
 */
int parser_get_statement(statement_t* io_pLine);

#endif /* PARSER_H_ */
