/*
 * assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "assembler.h"
#include "parser.h"
#include "directive.h"
#include "instruction.h"

#include <memory.h>

/* Internal functions */
/**
 * Parses the given file into an array of logical statements.
 * Also, constructs the data section of the program and places
 * some of the symbols (data symbols and external symbols) in
 * the given symbol table.
 *
 * @param flProgram File with the assembly code to parse.
 * @param o_arrStatements An output bound array that will be filled
 * with statements from the program.
 * @param nMaxNumOfStatements Size of the given array of statements.
 * @param o_pNumStatements An output bound ptr that returns the number
 * of actual statements found in the program.
 * @param o_pSymbols Symbol table to store the found symbols so far.
 * @param o_pData Data section of the program, here the data
 * will be stored.
 * @return 0 on success, anything else on any error.
 */
int first_pass(FILE* flProgram,
			   statement_t *o_arrStatements,
			   size_t nMaxNumOfStatements,
			   size_t *o_pNumStatements,
			   symbol_table_arr_t o_arrSymbols,
			   data_section_t *o_pData,
			   memory_section_t *o_pCode);

/**
 * Adds entry symbols to the table, and finalizes any
 * addressing issues.
 * @param arrStatements Statements that comprise the program.
 * @param nNumOfStatements Num of statements given.
 * @param io_pSymbols Symbol table for the program.
 * @param io_pCode Code section for the program.
 * @return 0 on success, anything else on any error.
 */
int second_pass(const statement_t *arrStatements,
				size_t nNumOfStatements,
				symbol_table_arr_t io_arrSymbols,
				memory_section_t *io_pCode);

/* Function Implementation */
int assembler_compile(FILE* flProgram,
					  symbol_table_arr_t o_arrSymbols,
					  memory_section_t *o_pCode,
					  data_section_t *o_pData)
{
	/* fixme: Should this be placed elsewhere? */
	static statement_t arrProgramStatements[MAX_STATEMENTS_IN_PROGRAM];
	size_t	nNumOfStatements = 0;

	/* First of all, init the given output-bound array types */
	memset(o_arrSymbols, 0, sizeof(symbol_table_arr_t));
	memset(o_pCode, 0, sizeof(*o_pCode));
	memset(o_pData, 0, sizeof(*o_pData));

	/* Make a first pass that completes parsing
	 * of the code, without resolving addresses
	 * yet
	 */
	if (first_pass(flProgram,
				   arrProgramStatements,
				   sizeof(arrProgramStatements) / sizeof(arrProgramStatements[0]),
				   &nNumOfStatements,
				   o_arrSymbols,
				   o_pData,
				   o_pCode) != 0)
		return -1;

	/* Move the data section to its final location so we
	 * can resolve data symbols properly
	 */
	symbol_move_section(o_arrSymbols, ADDR_SECTION_DATA, o_pCode->counter_a);

	/* First pass completed, resolve addresses */
	if (second_pass(arrProgramStatements,
					nNumOfStatements,
					o_arrSymbols,
					o_pCode) != 0)
		return -2;

	return 0;
}

int first_pass(FILE* flProgram,
			   statement_t *o_arrStatements,
			   size_t nMaxNumOfStatements,
			   size_t *o_pNumStatements,
			   symbol_table_arr_t o_arrSymbols,
			   data_section_t *o_pData,
			   memory_section_t *o_pCode)
{
	size_t nCurrLine = 0;
	int nErrorCode = 0;
	statement_t* pCurrStatement = &o_arrStatements[nCurrLine];

	/* Making sure ptrs valid */
	if (flProgram == NULL ||
		o_arrStatements == NULL ||
		o_pNumStatements == NULL ||
		o_arrSymbols == NULL ||
		o_pData == NULL ||
		o_pCode == NULL)
		return -1;

	/* Start the counters */
	o_pCode->counter_a = 0;
	o_pData->DC = 0;

	/* Read each line from the input file */
	while (fgets(pCurrStatement->szContent,
			 sizeof(pCurrStatement->szContent),
			 flProgram) != NULL &&
			 feof(flProgram) == 0)
	{
		/* Count the line as read */
		nCurrLine++;

		/* Parse the statement from the line */
		if (parser_get_statement(pCurrStatement) != 0)
		{
			/* Once there was an error found, continue with the
			 * parsing to find as many errors as possible, but
			 * make sure to notify this is not a valid program
			 * that should not be compiled into an .obj
			 */
			nErrorCode = -1;
		}

		/* Some kind of directive */
		if (pCurrStatement->type == STATEMENT_TYPE_DIRECTIVE)
		{
			/* Handle data/string directive */
			switch(pCurrStatement->info.directive.name)
			{
				case (DIRECTIVE_DATA):
				case (DIRECTIVE_STRING):
				{
					if (directive_compile_dummy_instruction(pCurrStatement,
															o_pData,
															o_arrSymbols) != 0)
						nErrorCode = -1;
					break;
				}
				case (DIRECTIVE_EXTERN):
				{
					if (directive_compile_extern(pCurrStatement,
												 o_arrSymbols) != 0)
						nErrorCode = -1;
					break;
				}
				/* Ignore for now, 2nd pass will handle it */
				case (DIRECTIVE_ENTRY):
				default:
					break;
			}
		}
		/* An instruction */
		else if (pCurrStatement->type == STATEMENT_TYPE_INSTRUCTION)
		{
			/* fixme: re-factor out the label stuff from data and string */
			/* Add the instruction's label if exists */
			if (pCurrStatement->szLabel != NULL)
			{
				/* Save the label for the instruction,
				 * its address is the IC before the instruction
				 */
				if (symbol_add_to_table(o_arrSymbols,
										ADDR_ABSOLUTE,
										pCurrStatement->szLabel,
										o_pCode->counter_a,
										ADDR_SECTION_CODE) != 0)
				{
					return -1;
				}
			}

			/* Reserve it a number of cells */
			o_pCode->counter_a += instruction_shallow_parse(pCurrStatement);
		}
		else if (pCurrStatement->type == STATEMENT_TYPE_ERROR)
		{
			/*todo: Handle error */
		}

		/* Set the curr statement ptr to the next one
		 * on the list.
		 */
		pCurrStatement = &o_arrStatements[nCurrLine];
	}

	*o_pNumStatements = nCurrLine;

	return nErrorCode;
}

int second_pass(const statement_t *arrStatements,
				size_t nNumOfStatements,
				symbol_table_arr_t io_arrSymbols,
				memory_section_t *io_pCode)
{
	size_t nCurrLine = 0;
	int nErrorCode = 0;
	const statement_t* pCurrStatement = NULL;

	/* Making sure ptrs valid */
	if (arrStatements == NULL ||
		io_arrSymbols == NULL ||
		io_pCode == NULL)
		return -1;

	/* Start counting the instructions again */
	io_pCode->counter_a = 0;

	/* Read each line from the input file */
	for (nCurrLine = 0; nCurrLine < nNumOfStatements; ++nCurrLine)
	{
		pCurrStatement = &arrStatements[nCurrLine];

		/* Some kind of directive */
		if (pCurrStatement->type == STATEMENT_TYPE_DIRECTIVE)
		{
			/* Handle data/string directive */
			switch(pCurrStatement->info.directive.name)
			{
				case (DIRECTIVE_ENTRY):
				{
					if (directive_compile_entry(pCurrStatement,
												io_arrSymbols) != 0)
						return -2;
					break;
				}
				case (DIRECTIVE_DATA):
				case (DIRECTIVE_STRING):
				default:
					break;
			}
		}
		/* An instruction */
		else if (pCurrStatement->type == STATEMENT_TYPE_INSTRUCTION)
		{
			/* Compile the instruction fully and evaluate its operands */
			if (instruction_compile(pCurrStatement,
									io_pCode,
									io_arrSymbols) != 0)
			{
				nErrorCode = -1;
			}
		}
		else
		{
			/* Assume everything else is already dealt with */
		}
	}

	return nErrorCode;
}
