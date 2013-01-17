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
#include <ctype.h>

#include "machine.h"

/* Defines */
#define BLANKS "\t "
#define LABEL_SEPARATOR ':'

/* fixme: A line that contains only a label and ':'
 * is considered legal(?), and it might take the entire line
 */
/* fixme: 30 chars .. or 31? not sure */
#define MAX_LABEL_LENGTH ((unsigned int)30)

/* Internal functions declarations */
/**
 * Finds out which directive code corresponds to
 * the given string.
 *
 * @param szDirective String representing a directive
 * (without the leading '.' character)
 *
 * @return type of directive found, or DIRECTIVE_ILLEGAL if
 * the given string isn't legal.
 */
directive_type_t parser_string_to_directive_type(const char* szDirective);

/**
 * Finds out which instruction code corresponds to
 * the given string.
 *
 * @param szInstruction String representing an instruction
 * (including its modifiers, e.g 'mov/1/0/1')
 *
 * @return type of instruction found, or ILLEGAL if
 * the given string isn't legal.
 */
opcode_t parser_string_to_instruction_type(const char* szInstruction);

/**
 * Finds out which register code corresponds to
 * the given string.
 *
 * @param szRegister String representing a valid GP register
 *
 * @return type of register found, or REGISTERS_LAST if
 * the given string isn't legal.
 */
machine_registers_t parser_string_to_register_type(const char* szRegister);

/**
 * Makes sure that the given label is valid.
 * @param szLabel Label to check
 * @return Length of the label, 0 if label have any syntax errors
 */
int parser_check_label_syntax(const char* szLabel);

/* Implementations */
int parser_get_statement(statement_t* io_pLine)
{
	char* szOperation = NULL;
	char* pLabelEnd = NULL;
	size_t nLabelSectionLength = 0;

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
	/* Is there a label in the line? */
	pLabelEnd = strchr(io_pLine->szContent, LABEL_SEPARATOR);
	if (pLabelEnd != NULL)
	{
		/* Replace the end separator with \0,
		 * like strtok would.
		 */
		*pLabelEnd = '\0';

		/* Essentialy we have split the content into 2 parts */
		io_pLine->pLabel = io_pLine->szContent;

		nLabelSectionLength = parser_check_label_syntax(io_pLine->pLabel);

		/* Make sure the label is valid */
		if (nLabelSectionLength == 0)
		{
			return -4;
		}

		/* Count the \0 as it is a part of the label's section
		 * in the content.
		 */
		nLabelSectionLength++;
	}

	/* Split the line according to whitespaces, fetch the first
	 * word after the label (if exists) */
	szOperation = strtok(io_pLine->szContent + nLabelSectionLength, BLANKS);

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
directive_type_t parser_string_to_directive_type(const char* szDirective)
{
	if (szDirective == NULL)
		return DIRECTIVE_ILLEGAL;

	/* fixme: Is this case sensitive? */
	/* Match to a directive */
	if (strcmp("data", szDirective) == 0)
	{
		return DIRECTIVE_DATA;
	}
	else if (strcmp("string", szDirective) == 0)
	{
		return DIRECTIVE_STRING;
	}
	else if (strcmp("entry", szDirective) == 0)
	{
		return DIRECTIVE_ENTRY;
	}
	else if (strcmp("extern", szDirective) == 0)
	{
		return DIRECTIVE_EXTERN;
	}
	else
	{
		return DIRECTIVE_ILLEGAL;
	}
}

opcode_t parser_string_to_instruction_type(const char* szInstruction)
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

	if (szInstruction == NULL)
		return ILLEGAL;

	/* fixme: Is this case sensitive? */
	/* Match to an instruction */
	for (currOpcode = (opcode_t)0; currOpcode < ILLEGAL; currOpcode++)
	{
		if (strcmp(szInstruction, arrInstructionNames[currOpcode]) == 0)
		{
			return currOpcode;
		}
	}

	return ILLEGAL;
}

machine_registers_t parser_string_to_register_type(const char* szRegister)
{
	static const char* arrRegisterNames[REGISTERS_LAST] =
		{
		"r0",
		"r1",
		"r2",
		"r3",
		"r4",
		"r5",
		"r6",
		"r7"
		};
	machine_registers_t currRegister;

	if (szRegister == NULL)
		return ILLEGAL;

	/* fixme: Is this case sensitive? */
	/* Match to an instruction */
	for (currRegister = (opcode_t)0;
		 currRegister < REGISTERS_LAST;
		 currRegister++)
	{
		if (strcmp(szRegister, arrRegisterNames[currRegister]) == 0)
		{
			return currRegister;
		}
	}

	return REGISTERS_LAST;
}

int parser_check_label_syntax(const char* szLabel)
{
	size_t nLabelLength = 0;

	if (szLabel == NULL)
		return 0;

	/* Must begin with a lower / upper case letter */
	if (isalpha(szLabel[0]) == 0)
	{
		printf("Error! label '%s' doesn't begin with a letter.\n",
				szLabel);
		return 0;
	}

	/* Must not be too long */
	nLabelLength = strlen(szLabel);
	if (nLabelLength == 0)
	{
		printf("Error! label must contain at least 1 character.\n");
		return 0;
	}
	else
	{
		if (nLabelLength > MAX_LABEL_LENGTH)
		{
			printf("Error! label '%s' is %u characters long, max length is %u.\n",
					szLabel,
					nLabelLength,
					MAX_LABEL_LENGTH);
			return 0;
		}
	}

	/* Make sure it's not an instruction name */
	if (parser_string_to_instruction_type(szLabel) != ILLEGAL)
	{
		printf("Error! name '%s' is an instruction name, cannot be a label.\n",
				szLabel);
		return 0;
	}

	/* Make sure its not a register name */
	if (parser_string_to_register_type(szLabel) != REGISTERS_LAST)
	{
		printf("Error! name '%s' is a register name, cannot be a label.\n",
				szLabel);
		return 0;
	}

	return nLabelLength;
}
