/*
 * parser.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#include "parser.h"

#include <stdlib.h>
#include <string.h>

int parser_get_statement(statement_t* io_pLine)
{
	char* szOperation = NULL;
	size_t nLabelLength = 0;

	/* Don't crash the program if we aren't given a
	 * valid ptr
	 */
	if (io_pLine == NULL)
		return -1;

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
		nLabelLength = strlen(io_pLine->pLabel);
		/* fixme: Check label length here? assert it is legal? */
	}

	/* Split the line according to whitespaces, find the first
	 * word after the label (if exists) */
	/* fixme: Assume that the token ':' was found only once? it isn't
	 * legal in any of the other parts of the line?
	 */
	szOperation = strtok(io_pLine->szContent + nLabelLength, "\t ");

	/* Given line is empty*/
	if (szOperation == NULL)
	{
		io_pLine->type = STATEMENT_TYPE_EMPTY;
		return 0;
	}
	else
	{
		/* Directive line */

		/* Instruction line */
	}

	return -1;
}
