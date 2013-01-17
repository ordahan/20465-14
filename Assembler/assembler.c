/*
 * assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include "assembler.h"
#include "parser.h"

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
 * @return -1 if any error happened during the parsing, 0 otherwise.
 */
int first_pass(FILE* flProgram,
			   statement_t *o_arrStatements,
			   size_t nMaxNumOfStatements,
			   size_t *o_pNumStatements,
			   symbol_table_arr_t *o_pSymbols,
			   data_section_t *o_pData);

int second_pass();

/* Function Implementation */
int assembler_compile(FILE* flProgram,
					  symbol_table_arr_t *o_pSymbols,
					  code_section_t *o_pCode,
					  data_section_t *o_pData)
{
	/* fixme: Should this be placed elsewhere? */
	static statement_t arrProgramStatements[MAX_STATEMENTS_IN_PROGRAM];
	size_t	nNumOfStatements = 0;

	/* First of all, init the given output-bound array types */
	memset(*o_pSymbols, 0, sizeof(*o_pSymbols));
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
				   o_pSymbols,
				   o_pData) != 0)
		return -1; /* fixme: errors in initial parsing lead to abortion of 2nd pass? */

	return 0;
}

int first_pass(FILE* flProgram,
			   statement_t *o_arrStatements,
			   size_t nMaxNumOfStatements,
			   size_t *o_pNumStatements,
			   symbol_table_arr_t *o_pSymbols,
			   data_section_t *o_pData)
{
	size_t nCurrLine = 0;
	int nErrorCode = 0;
	statement_t* pCurrStatement = &o_arrStatements[nCurrLine];

	/* Making sure ptrs valid */
	if (flProgram == NULL ||
		o_arrStatements == NULL ||
		o_pNumStatements == NULL ||
		o_pSymbols == NULL ||
		o_pData == NULL)
		return -1;

	/* Read each line from the input file */
	while (fgets(pCurrStatement->szContent,
				 sizeof(pCurrStatement->szContent),
				 flProgram))
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
					if (directive_compile_dummy_instruction(
											pCurrStatement->info.directive.name,
											pCurrStatement->szOperationData,
											pCurrStatement->szLabel,
											o_pData,
											o_pSymbols) != 0)
						nErrorCode = -1;
					break;
				}
				case (DIRECTIVE_EXTERN):
				{
					/* todo: Externals */
					/* fixme: can symbol be .extern'ed twice? */
					break;
				}
				default:
					break;
			}
		}
		/* An instruction */
		else if (pCurrStatement->type == STATEMENT_TYPE_INSTRUCTION)
		{
			/* todo: Handle instruction */

			/* Label */
		}
	}

	*o_pNumStatements = nCurrLine;

	return nErrorCode;
}
