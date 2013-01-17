/*
 * parser.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#include "parser.h"

#include <stdlib.h>
#include <string.h>

#define BLANKS "\t "

int parser_get_statement(statement_t* io_pLine)
{
	char* szOperation = NULL;
	size_t nLabelLength = 0;

	/* Don't crash the program if we aren't given a
	 * valid ptr
	 */
	if (io_pLine == NULL)
		return -1;

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
		/* Check if the operation is a directive */
		io_pLine->info.directive.name = parser_get_directive(szOperation);
		if (io_pLine->info.directive.name != DIRECTIVE_ILLEGAL)
		{
			io_pLine->type = STATEMENT_TYPE_DIRECTIVE;

			/* The next part right after the directive should
			 * be at least one value that it refers to
			 */
			/* fixme: this part is actually common to the directive
			 * and to the instruction.. maybe change the statement struct
			 * and support this?
			 */
			io_pLine->info.directive.value = strtok(NULL, BLANKS);

			/* No value.. syntax error */
			if (io_pLine->info.directive.value == NULL)
			{
				return -2;
			}

			return 0;
		}

		/* Instruction line */
		io_pLine->info.instruction.name = parser_get_instruction(szOperation);
		if (io_pLine->info.instruction.name == ILLEGAL)
		{
			/* Style errors generaly? */
			printf("Error! Illegal instruction: %d\n", szOperation);
		}
		else
		{
			/* todo: continue with parsing the type and the operands */
		}
	}

	return -1;
}

/* fixme: make this more elegant */
directive_type_t parser_get_directive(const char* szDirectiveString)
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

instruction_type_t parser_get_instruction(const char* szInstructionString)
{
	return ILLEGAL;
}
