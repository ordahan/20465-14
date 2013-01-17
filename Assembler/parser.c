/*
 * parser.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Defines */
#define BLANKS "\t "

/* Internal functions declarations */
/**
 * Finds out which directive code corresponds to
 * the given string.
 *
 * @param szDirectiveString String representing a directive
 * (without the leading '.' character)
 *
 * @return type of directive found, or DIRECTIVE_ILLEGAL if
 * the given string isn't legal.
 */
directive_type_t parser_string_to_directive_type(const char* szDirectiveString);

/**
 * Finds out which instruction code corresponds to
 * the given string.
 *
 * @param szInstructionString String representing an instruction
 * (including its modifiers, e.g 'mov/1/0/1')
 *
 * @return type of instruction found, or ILLEGAL if
 * the given string isn't legal.
 */
instruction_type_t parser_string_to_instruction_type(const char* szInstructionString);

/* Implementations */
int parser_get_statement(statement_t* io_pLine)
{
	char* szOperation = NULL;
	size_t nLabelLength = 0;

	/* Don't crash the program if we aren't given a
	 * valid ptr
	 */
	if (io_pLine == NULL)
		return 1;

	/* Assume we didn't find anything */
	io_pLine->type = STATEMENT_TYPE_ERROR;

	/* Given line is a comment */
	if (io_pLine->szContent[0] == ';')
	{
		io_pLine->type = STATEMENT_TYPE_COMMENT;
		return 0;
	}

	/* fixme: can there be a label before a comment line?,
	 * 		  in an empty line?
	 */
	/* Search for a label in the beginning of line */
	io_pLine->pLabel = strtok(io_pLine->szContent, ":");

	/* How long is the label? */
	if (io_pLine->pLabel != NULL)
	{
		/* Is this really a label or was it just not found? */
		if (strtok(NULL, ":") == NULL)
		{
			io_pLine->pLabel = NULL;
		}
		else
		{
			nLabelLength = strlen(io_pLine->pLabel);
		}

		/* fixme: Check label length here? assert it is legal? */
	}

	/* Split the line according to whitespaces, find the first
	 * word after the label (if exists) */
	/* fixme: Assume that the token ':' was found only once? it isn't
	 * legal in any of the other parts of the line?
	 */
	szOperation = strtok(io_pLine->szContent + nLabelLength, BLANKS);

	/* Given line is empty*/
	if (szOperation == NULL)
	{
		io_pLine->type = STATEMENT_TYPE_EMPTY;
		return 0;
	}
	else
	{
		/* fixme: directive / instruction must appear right after label
		 * or can there be any whitespaces separating them?
		 */
		io_pLine->szOperationData = strtok(NULL, BLANKS);

		/* Check if the operation is a directive */
		if (szOperation[0] == '.')
		{
			io_pLine->type = STATEMENT_TYPE_DIRECTIVE;
			/* Convert the directive type without the '.' */
			io_pLine->info.directive.name =
					parser_string_to_directive_type(szOperation + 1);
			if (io_pLine->info.directive.name == DIRECTIVE_ILLEGAL)
			{
				printf("Error! Illegal directive: %s\n", szOperation);
				return -3;
			}

			return 0;
		}

		/* The operation must be some sort of instruction then */
		io_pLine->info.instruction.name = parser_string_to_instruction_type(szOperation);
		if (io_pLine->info.instruction.name == ILLEGAL)
		{
			/* Style errors generaly? */
			printf("Error! Illegal instruction: %s\n", szOperation);
			return -4;
		}
		else
		{
			/* todo: continue with parsing the type and the operands */
		}
	}

	return -1;
}

/* fixme: make this more elegant */
directive_type_t parser_string_to_directive_type(const char* szDirectiveString)
{
	if (szDirectiveString == NULL)
		return DIRECTIVE_ILLEGAL;

	/* fixme: Is this case sensitive? */
	/* Match to a directive */
	if (strcmp("data", szDirectiveString) == 0)
	{
		return DIRECTIVE_DATA;
	}
	else if (strcmp("string", szDirectiveString) == 0)
	{
		return DIRECTIVE_STRING;
	}
	else if (strcmp("entry", szDirectiveString) == 0)
	{
		return DIRECTIVE_ENTRY;
	}
	else if (strcmp("extern", szDirectiveString) == 0)
	{
		return DIRECTIVE_EXTERN;
	}
	else
	{
		return DIRECTIVE_ILLEGAL;
	}
}

instruction_type_t parser_string_to_instruction_type(const char* szInstructionString)
{
	static const char* arrInstructionNames[ILLEGAL] =
		{"mov",
		 "cmp",
		 "add",
		 "sub",
		 "lea",
		 "not",
		 "clr",
		 "inc",
		 "dec",
		 "jmp",
		 "bne",
		 "red",
		 "prn",
		 "jsr",
		 "rts",
		 "stop"
		};
	opcode_t currOpcode;

	if (szInstructionString == NULL)
		return ILLEGAL;

	/* fixme: Is this case sensitive? */
	/* Match to an instruction */
	for (currOpcode = (opcode_t)0; currOpcode < ILLEGAL; currOpcode++)
	{
		if (strcmp(szInstructionString, arrInstructionNames[currOpcode]) == 0)
		{
			return currOpcode;
		}
	}

	return ILLEGAL;
}
