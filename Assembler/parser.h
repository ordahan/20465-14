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

#define MAX_LINE_LENGTH 80

/* A line that contains only a label and ':'
 * is considered legal, and it might take the entire line
 */
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH-1)

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
	STATEMENT_TYPE_INSTRUCION
}statement_type_t;

typedef enum
{
	DIRECTIVE_DATA,
	DIRECTIVE_STRING,
	DIRECTIVE_ENTRY,
	DIRECTIVE_EXTERN
}directive_t;

typedef struct
{
	statement_type_t type;
	char szContent[MAX_LINE_LENGTH];
	char* pLabel;			/* NULL if there is no label */
	unsigned int lenbLabel;	/* Length of the label in bytes,
							 * 0 if there is no label */
}statement_t;

int parser_translate_to_statements(const FILE* flProgram);

#endif /* PARSER_H_ */
