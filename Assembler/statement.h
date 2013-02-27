/*
 * statement.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

#define MAX_LINE_LENGTH 80

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
	int			name;
	char*       modifiers;
	unsigned 	memory_size;
}instruction_statement_t;

typedef struct
{
	int name;
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

#endif /* STATEMENT_H_ */
