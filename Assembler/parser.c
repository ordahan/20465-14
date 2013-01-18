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
#define INSTRUCTION_MODIFIER_SEPARATOR "/"
#define ITEM_LIST_DELIMITERS ", \t" /* todo: is there a way to combine with blanks? */

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

/**
 * Checks whether or not the given string is comprised
 * of only 0 or more whitespaces
 * @param szString String to check if empty
 * @return 1 if empty, 0 if not
 */
int parser_is_string_empty(const char* szString);

/* Implementations */
/* fixme: shorter this one */
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
		io_pLine->szLabel = io_pLine->szContent;

		nLabelSectionLength = parser_check_label_syntax(io_pLine->szLabel);

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
			/* Get the directive type */
			io_pLine->info.directive.name =
					parser_string_to_directive_type(szOperation + 1);
			if (io_pLine->info.directive.name == DIRECTIVE_ILLEGAL)
			{
				printf("Error! Illegal directive: %s\n", szOperation);
				return -3;
			}

			/* Check values for the directive later,
			 * this is valid enough for this stage */
			io_pLine->type = STATEMENT_TYPE_DIRECTIVE;
			return 0;
		}

		/* The operation must be some sort of instruction then */

		/* Split instruction to 'type' and 'name' */
		szOperation = strtok(szOperation, INSTRUCTION_MODIFIER_SEPARATOR);
		io_pLine->info.instruction.modifiers =
				strtok(NULL, INSTRUCTION_MODIFIER_SEPARATOR);

		io_pLine->info.instruction.name =
				parser_string_to_instruction_type(szOperation);
		if (io_pLine->info.instruction.name == ILLEGAL)
		{
			/* Style errors generaly? */
			printf("Error! Illegal instruction: %s\n", szOperation);
			return -4;
		}

		/* Check modifiers and operands in evaluation,
		 * this is valid enough for this stage */
		io_pLine->type = STATEMENT_TYPE_INSTRUCTION;
		return 0;
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
		 "not",
		 "clr",
		 "lea",
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

typedef enum
{
	PARSER_LIST_STATE_INIT,
	PARSER_LIST_STATE_IN_ITEM,
	PARSER_LIST_STATE_AFTER_ITEM,
	PARSER_LIST_STATE_AFTER_DELIMITER
}parser_list_state_t;
int parser_get_items_from_list(char* szList,
							   char** o_arrItems,
							   size_t nListSize)
{
	size_t nCurrItem = 0;
	size_t nCurrCharIdx = 0;
	parser_list_state_t state = PARSER_LIST_STATE_INIT;

	/* Empty list expected */
	if (nListSize == 0)
	{
		/* Really is empty */
		if (parser_is_string_empty(szList) == 1)
			return 0;
		/* Garbage found */
		else
			return -1;
	}

	/* fixme: null-termination define */
	for (nCurrCharIdx = 0; szList[nCurrCharIdx] != '\0'; ++nCurrCharIdx)
	{
		/* In the middle of an item */
		if (state == PARSER_LIST_STATE_IN_ITEM)
		{
			/* Whitespace */
			if (isspace(szList[nCurrCharIdx]) != 0)
			{
				state = PARSER_LIST_STATE_AFTER_ITEM;
			}
			/* Delimiter */
			else if (szList[nCurrCharIdx] == ',')/* fixme: delimiter */
			{
				state = PARSER_LIST_STATE_AFTER_DELIMITER;
			}
			/* else, keep advancing in the item */
		}
		/* Not inside an item */
		else
		{
			/* Delimiter found */
			if (szList[nCurrCharIdx] == ',') /* fixme: delimiter */
			{
				/* Invalid location */
				if (state == PARSER_LIST_STATE_AFTER_DELIMITER ||
					state == PARSER_LIST_STATE_INIT)
					return -1;

				state = PARSER_LIST_STATE_AFTER_DELIMITER;
			}
			/* Started a new item */
			else if (isspace(szList[nCurrCharIdx]) == 0)
			{
				/* Make sure this is either the first one,
				 * or there is a delimiter preceding us
				 */
				if (state != PARSER_LIST_STATE_INIT &&
					state != PARSER_LIST_STATE_AFTER_DELIMITER)
					return -1;

				/* Save the item, make sure not to
				 * go out of bounds */
				if (nCurrItem >= nListSize)
					return -1;
				o_arrItems[nCurrItem] = &szList[nCurrCharIdx];
				nCurrItem++;

				/* Signal we started parsing a new item */
				state = PARSER_LIST_STATE_IN_ITEM;
			}
			/* else, just a whitespace.. doesn't change anything */
		}
	}

	/* The list must end with either a whitespace,
	 * an item or nothing at all if nothing is supposed
	 * to be found.
	 * Also, Make sure we got the number of items
	 * we were looking for
	 */
	if ((state == PARSER_LIST_STATE_AFTER_ITEM ||
		 state == PARSER_LIST_STATE_IN_ITEM ||
		(state == PARSER_LIST_STATE_INIT &&
		 nListSize == 0)) &&
		nListSize == nCurrItem)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int parser_is_string_empty(const char* szString)
{
	size_t nCurrChar = 0;
	int fNonWhiteFound = 1;

	/* Look char by char */
	while (szString[nCurrChar] != '\0' &&
		   fNonWhiteFound == 1)
	{
		/* Is this non space? */
		if (isspace(szString[nCurrChar]) == 0)
			fNonWhiteFound = 0;

		nCurrChar++;
	}

	return fNonWhiteFound;
}
