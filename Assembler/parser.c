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
#include "instruction.h"
#include "directive.h"
#include "symbol.h"

/* Defines */
#define BLANKS "\t \n\r"
#define LABEL_SEPARATOR ':'
#define INSTRUCTION_MODIFIER_SEPARATOR '/'
#define ITEM_LIST_DELIMITERS ", \t" /* todo: is there a way to combine with blanks? */
#define NUMBERS_BASE	(10)
#define NULL_TERMINATOR '\0'
#define IMMEDIATE_PREFIX '#'
#define INDEX_OFFSET_OPEN_DELIMITER "{"
#define INDEX_OFFSET_CLOSE_DELIMITER "}"

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
 * If there label given has an index offset to it, retrieves a ptr
 * to the index. Any syntax-used separator for the index are replaced
 * with a null-termination character (thus practically "splitting" the
 * label from it's index).
 *
 * @param szLabel Label that might contain an index offset.
 *
 * @return szLabel if there is no index, NULL for any error with
 * the index, index string if its ok.
 */
char* parser_get_index_from_label(char* szLabel);

/* Internal definitions */
typedef enum
{
	PARSER_LIST_STATE_INIT,
	PARSER_LIST_STATE_IN_ITEM,
	PARSER_LIST_STATE_AFTER_ITEM,
	PARSER_LIST_STATE_AFTER_DELIMITER
}parser_list_state_t;

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

	/* Is there a label in the line? */
	io_pLine->szLabel = NULL;
	pLabelEnd = strchr(io_pLine->szContent, LABEL_SEPARATOR);
	if (pLabelEnd != NULL)
	{
		/* Replace the end separator with \0,
		 * like strtok would.
		 */
		*pLabelEnd = NULL_TERMINATOR;

		/* Essentially we have split the content into 2 parts */
		io_pLine->szLabel = io_pLine->szContent;

		nLabelSectionLength = parser_check_symbol_syntax(io_pLine->szLabel);

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

		/* Assert where the modifiers begin */
		io_pLine->info.instruction.modifiers =
				strchr(szOperation, INSTRUCTION_MODIFIER_SEPARATOR);

		/* Make sure there are modifiers */
		if (io_pLine->info.instruction.modifiers == NULL)
		{
			printf("Error! Instruction doesn't have a valid modifier: %s\n",
					szOperation);
			return -5;
		}

		/* Split instruction from modifiers by chopping
		 * out the separator */
		io_pLine->info.instruction.modifiers[0] = NULL_TERMINATOR;
		io_pLine->info.instruction.modifiers++;

		/* Shallow-parsing of the instruction */
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

/* fixme: make this more elegant - like instruction */
directive_type_t parser_string_to_directive_type(const char* szDirective)
{
	if (szDirective == NULL)
		return DIRECTIVE_ILLEGAL;

	/* Match to a directive,
	 * assume case sensitive */
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
	unsigned i;

	if (szInstruction == NULL)
		return ILLEGAL;

	/* Match to an instruction, assume it is case
	 * sensitive */
	for (i = 0; i < ILLEGAL; i++)
	{
		if (strcmp(szInstruction,
				   g_arrInstructionSyntax[i].name) == 0)
		{
			return g_arrInstructionSyntax[i].opcode;
		}
	}

	return ILLEGAL;
}

machine_registers_t parser_string_to_register_type(const char* szRegister)
{
	static const char* arrRegisterNames[REGISTER_INVALID] =
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
		 currRegister < REGISTER_INVALID;
		 currRegister++)
	{
		if (strcmp(szRegister, arrRegisterNames[currRegister]) == 0)
		{
			return currRegister;
		}
	}

	return REGISTER_INVALID;
}

/* fixme: belongs to symbol.h only? check validity only there...
 * even in index addressing etc we don't care.. address resolution
 * process will not be able to find the symbol anyway (because it wasn't
 * legal and didn't get into the symbol table) */
int parser_check_symbol_syntax(const char* szSymbol)
{
	size_t nSymbolLength = 0;

	if (szSymbol == NULL)
		return 0;

	/* Must begin with a lower / upper case letter */
	if (isalpha(szSymbol[0]) == 0)
	{
		printf("Error! symbol '%s' doesn't begin with a letter.\n",
				szSymbol);
		return 0;
	}

	/* Must not be too long */
	nSymbolLength = strlen(szSymbol);
	if (nSymbolLength == 0)
	{
		printf("Error! symbol must contain at least 1 character.\n");
		return 0;
	}
	else
	{
		if (nSymbolLength > MAX_LABEL_LENGTH)
		{
			printf("Error! symbol '%s' is %u characters long, max length is %u.\n",
					szSymbol,
					nSymbolLength,
					MAX_LABEL_LENGTH);
			return 0;
		}
	}

	/* Make sure it's not an instruction name */
	if (parser_string_to_instruction_type(szSymbol) != ILLEGAL)
	{
		printf("Error! name '%s' is an instruction name, cannot be a symbol.\n",
				szSymbol);
		return 0;
	}

	/* Make sure its not a register name */
	if (parser_string_to_register_type(szSymbol) != REGISTER_INVALID)
	{
		printf("Error! name '%s' is a register name, cannot be a symbol.\n",
				szSymbol);
		return 0;
	}

	return nSymbolLength;
}

int parser_get_items_from_list(char* szList,
							   char** o_arrItems,
							   size_t nListSize)
{
	size_t nCurrItem = 0;
	size_t nCurrCharIdx = 0;
	parser_list_state_t state = PARSER_LIST_STATE_INIT;

	/* Special case - empty list */
	if (szList == NULL && nListSize == 0)
	{
		return 0;
	}

	if (szList == NULL ||
		o_arrItems == NULL)
		return -1;

	for (nCurrCharIdx = 0; szList[nCurrCharIdx] != NULL_TERMINATOR; ++nCurrCharIdx)
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

		/* Found a delimiter or whitespace, remove it */
		if (state == PARSER_LIST_STATE_AFTER_DELIMITER ||
			state == PARSER_LIST_STATE_AFTER_ITEM)
			szList[nCurrCharIdx] = NULL_TERMINATOR;
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

/* todo: test this separately */
instruction_type_t parser_get_instruction_type(const char* szModifiers)
{
	unsigned long type;
	char * pEnd;

	/* The first character in the modifiers string is the type */
	type = strtol(szModifiers, &pEnd, NUMBERS_BASE);

	/* Nothing was parsed, error */
	if (pEnd == szModifiers)
	{
		return INVALID_TYPE;
	}
	/* Type isn't right */
	else if (type >= INVALID_TYPE)
	{
		return INVALID_TYPE;
	}
	else
	{
		return (instruction_type_t)type;
	}
}

/* todo: test this separately */
/* fixme: magic numbers */
instruction_comb_t parser_get_instruction_comb(const char* szModifiers)
{
	unsigned long comb1, comb2;
	char *pEnd1, *pEnd2;

	/* Should there be any comb at all? */
	if (szModifiers[1] == NULL_TERMINATOR)
	{
		/* Default value */
		return NO_COMB;
	}

	/* Make sure proper delimiters were placed */
	if (szModifiers[1] != INSTRUCTION_MODIFIER_SEPARATOR ||
		szModifiers[3] != INSTRUCTION_MODIFIER_SEPARATOR)
	{
		return INVALID_COMB;
	}

	/* Make sure the comb ends after its second part,
	 * 5 chars max should make up the modifiers string */
	if (strlen(szModifiers) > 5)
		return INVALID_COMB;

	/* Get the combination parts */
	comb1 = strtol(&szModifiers[2], &pEnd1, NUMBERS_BASE);
	comb2 = strtol(&szModifiers[4], &pEnd2, NUMBERS_BASE);

	/* Nothing was parsed, error */
	if (pEnd1 == szModifiers || pEnd2 == szModifiers)
	{
		return INVALID_COMB;
	}
	else
	{
		/* Make sure they are valid in their range */
		if (comb1 > 1 || comb2 > 1)
			return INVALID_COMB;

		/* Left bit is comb1, right bit is comb2 */
		return (instruction_comb_t)(comb1 << 1 | comb2);
	}
}

/* todo: test this separately */
/* todo: split code to more funcs */
operand_addressing_t parser_get_operand(char* szOperand,
										instruction_with_operands_t* pInstruction)
{
	machine_registers_t reg = parser_string_to_register_type(szOperand);

	/* Immediate value */
	if (szOperand[0] == IMMEDIATE_PREFIX)
	{
		char* pEnd;
		long int val;

		/* Parse number starting from the prefix */
		val = strtol(&szOperand[1], &pEnd, NUMBERS_BASE);

		/* Check that there aren't any 'leftovers' */
		if (pEnd == strchr(szOperand, NULL_TERMINATOR))
		{
			/* Place the value as an extra data */
			pInstruction->extra_data[pInstruction->num_extra_data].val = val;
			pInstruction->num_extra_data++;
			pInstruction->localities[pInstruction->num_extra_data] = ADDR_ABSOLUTE;
			return OPERAND_ADDR_IMMEDIATE;
		}
		else
		{
			printf("Error! Invalid immediate value: %s\n", szOperand);
			return OPERAND_ADDR_NUM;
		}
	}
	/* Register */
	else if (reg != REGISTER_INVALID)
	{
		/* Temporarily place the register in the
		 * destination register
		 */
		pInstruction->instruction.dest_reg = reg;
		return OPERAND_ADDR_REGISTER;
	}
	/* Must be some sort of label */
	else
	{
		/* Split the index from the label, if there is any index */
		const char* szIndexValue = parser_get_index_from_label(szOperand);

		/* The label must be valid anyway */
		if (parser_check_symbol_syntax(szOperand) == 0)
		{
			printf("Error! Label %s has syntax errors.\n",
					szOperand);
			return OPERAND_ADDR_NUM;
		}

		/* Save room for the label, resolve addressing later */
		pInstruction->extra_data[pInstruction->num_extra_data].val = 0;
		pInstruction->num_extra_data++;
		pInstruction->localities[pInstruction->num_extra_data] = ADDR_RELOCATABLE;

		/* Does it have an index? */
		if (szIndexValue != szOperand &&
			szIndexValue != NULL)
		{
			long int index;
			char* pEnd;
			address_locality_t locality_for_index = ADDR_ABSOLUTE;

			/* Might be a number */
			/* fixme: a negative index is possible? */
			index = strtol(szIndexValue, &pEnd, NUMBERS_BASE);

			/* Might be a register */ /* fixme: register index is just reg num? */
			reg = parser_string_to_register_type(szIndexValue);

			/* Index is a register */
			if (reg != REGISTER_INVALID)
			{
				index = reg;
			}
			/* Not an immediate number, must be a label */
			else if (pEnd != strchr(szIndexValue, NULL_TERMINATOR))
			{
				/* Must be a valid label */
				if (parser_check_symbol_syntax(szIndexValue) == 0)
				{
					printf("Error! Index %s has syntax errors.\n",
							szIndexValue);
					return OPERAND_ADDR_NUM;
				}

				locality_for_index = ADDR_RELOCATABLE;
			}
			/* A number */

			/* Save the index value as well */
			pInstruction->extra_data[pInstruction->num_extra_data].val = index;
			pInstruction->num_extra_data++;
			pInstruction->localities[pInstruction->num_extra_data] = locality_for_index;

			return OPERAND_ADDR_INDEX;
		}
		/* No index, plain old label */
		else
		{
			return OPERAND_ADDR_DIRECT;
		}
	}

	return OPERAND_ADDR_NUM;
}

/* fixme: test me!!! */
char* parser_get_index_from_label(char* szLabel)
{
	char* szStartingDelim = strstr(szLabel, INDEX_OFFSET_OPEN_DELIMITER);

	/* Index found */
	if (szStartingDelim != NULL)
	{
		char* szClosingDelim = NULL;

		/* Look for the closing delimiter */
		szClosingDelim = strstr(szStartingDelim, INDEX_OFFSET_CLOSE_DELIMITER);

		/* Make sure its closed ok */
		if (szClosingDelim != NULL &&
			(szClosingDelim[1]) == NULL_TERMINATOR)
		{
			/* Remove delimiters */
			szStartingDelim[0] = NULL_TERMINATOR;
			szClosingDelim[0] = NULL_TERMINATOR;

			/* Index starts right after starting delimiter */
			return szStartingDelim+1;
		}
		else
		{
			printf("Error! Index syntax isn't valid: %s.\n", szStartingDelim);
			return NULL;
		}
	}
	/* No index */
	else
	{
		return szLabel;
	}
}
