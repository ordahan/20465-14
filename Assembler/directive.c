/*
 * directive.c
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */


#include "directive.h"
#include "symbol.h"
#include "parser.h"
#include <string.h>
#include "assembler.h"

/* Internal functions */
/**
 * Retrieves the data fields specified in the list into the
 * given data section.
 *
 * @param io_pData Data section to place the values in.
 * @param arrFields List of data fields as strings.
 * @param nFields Number of fields on the list.
 * @return 0 if everything is ok, any other number otherwise.
 */
int retrieve_data_fields(data_section_t* io_pData,
					  	 char** arrFields,
					  	 unsigned int nFields);

/**
 * Compiles the given data directive directly into the data section given.
 * @param pDataDirective Directive to compile.
 * @param io_pData Data section to put the results in.
 * @return 0 if ok, anything else otherwise to signal error.
 */
int compile_data(const statement_t *pDataDirective,
				 data_section_t* io_pData);

/**
 * Compiles the given string directive directly into the data section given.
 * @param pStringDirective Directive to compile.
 * @param io_pData Data section to put the results in.
 * @return 0 if ok, anything else otherwise to signal error.
 */
int compile_string(const statement_t *pStringDirective,
				   data_section_t* io_pData);

/* Implementations */
int directive_compile_dummy_instruction(const statement_t *pDummyInst,
									    data_section_t* io_pData,
									    symbol_table_arr_t io_pSymbols)
{
	if (pDummyInst == NULL ||
		io_pSymbols == NULL ||
		io_pData == NULL)
		return -1;

	/* Add the instruction's label if exists */
	if (pDummyInst->szLabel != NULL)
	{
		/* Save the label for the instruction,
		 * its address is the DC before the instruction
		 */
		if (symbol_add_to_table(io_pSymbols,
							    ADDR_RELOCATABLE,
							    pDummyInst->szLabel,
							    io_pData->DC,
							    ADDR_SECTION_DATA) != 0)
		{
			return -1;
		}
	}

	if (pDummyInst->info.directive.name == DIRECTIVE_DATA)
	{
		return compile_data(pDummyInst, io_pData);
	}
	else if (pDummyInst->info.directive.name == DIRECTIVE_STRING)
	{
		return compile_string(pDummyInst, io_pData);
	}
	else
	{
		/* Internal error.. must be a bug in the code itself. */
		return -99;
	}

	return 0;
}

int directive_compile_extern(const statement_t *pExtern,
							 symbol_table_arr_t io_pSymbols)
{
	char *pName;

	if (pExtern == NULL ||
		io_pSymbols == NULL)
		return -1;

	/* Get the label intended to add */
	if (parser_get_items_from_list(pExtern->szOperationData,
								   &pName,
								   1) != 0)
		return -2;

	/* Add the label to the table */
	return symbol_add_to_table(io_pSymbols,
							   ADDR_EXTERNAL,
							   pName,
							   0,
							   ADDR_SECTION_EXTERNAL); /*fixme: external section? */
}

int directive_compile_entry(const statement_t *pEntry,
							symbol_table_arr_t io_pSymbols)
{
	char *pName = NULL;

	if (pEntry == NULL ||
		io_pSymbols == NULL)
		return -1;

	/* Get the label intended to make an entry */
	if (parser_get_items_from_list(pEntry->szOperationData,
								   &pName,
								   1) != 0)
		return -2;

	return symbol_set_as_entry(io_pSymbols, pName);
}

int retrieve_data_fields(data_section_t* io_pData,
					  	 char** arrFields,
					  	 unsigned int nFields)
{
	unsigned i;

	/* Go over the list and retrieve the numbers from it */
	for (i = 0; i < nFields; ++i)
	{
		/* fixme: add this test to instructions as well */
		/* Make sure we don't have too much data already */
		if (io_pData->DC >=  ASSEMBLER_DATA_MAX_SIZE_CELLS)
		{
			return -2;
		}

		if (0 == parser_get_number(arrFields[i],
						  	  	   &io_pData->content[io_pData->DC].val))
		{
			/* Error retrieving fields */
			return -1;
		}
		/* Count the number retrieved */
		io_pData->DC++;
	}

	return 0;
}

int compile_data(const statement_t *pDataDirective,
				 data_section_t* io_pData)
{
	char *arrDataFields[ASSEMBLER_DATA_MAX_SIZE_CELLS];
	unsigned int nNumDataFields = 0;

	/* Get the number of expected data fields */
	nNumDataFields = parser_get_num_items_in_list(pDataDirective->szOperationData);
	if (nNumDataFields == 0)
	{
		printf("Error! data directive must contain at least one element.");
		return -2;
	}

	/* Get the data field to add */
	if (parser_get_items_from_list(pDataDirective->szOperationData,
								   arrDataFields,
								   nNumDataFields) != 0)
		return -3;

	/* Add the data fields to the data section */
	if (retrieve_data_fields(io_pData,
							 arrDataFields,
							 nNumDataFields) != 0)
		return -4;

	return 0;
}

int compile_string(const statement_t *pStringDirective,
				   data_section_t* io_pData)
{
	const char *pStart;
	const char *pEnd;

	/* Get the string represented in the directive */
	if (parser_get_string(pStringDirective->szOperationData,
					  &pStart,
					  &pEnd) != 0)
	{
		return -1;
	}

	/* Place all of its chars in the data section */
	while (pStart <= pEnd)
	{
		/* No more room for data */
		/* fixme: make the code / data section more
		 * abstract by giving them a correct API
		 */
		if (io_pData->DC >= ASSEMBLER_DATA_MAX_SIZE_CELLS)
		{
			return -1;
		}

		/* Add the null-terminator */
		if (pStart == pEnd)
		{
			io_pData->content[io_pData->DC].val = '\0';
		}
		else
		{
			io_pData->content[io_pData->DC].val = *pStart;
		}

		io_pData->DC++;
		pStart++;
	}


	return 0;
}
