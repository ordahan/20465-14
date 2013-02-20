/*
 * parser.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 *
 *  This module is responsible for translating the given assembly
 *  program (as text) to data structures the assembler can handle.
 *  Any syntax error that isn't related to the instruction set,
 *  addresses etc will be caught here.
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "statement.h"
#include <stdlib.h>

#define MAX_LIST_LENGTH MAX_LINE_LENGTH

/* todo: decide on a good number.. maybe use some other #define
 * for memory size etc?
 */
#define MAX_STATEMENTS_IN_PROGRAM 4096

/**
 * Parses a line of assembly code into its different fields.
 * Makes sure the syntax of the label (if exists) is valid.
 * Doesn't parse the syntax of the directive's value
 * (valid number, valid string etc) or the instruction's operands
 * (number of operands, register names or even syntax of memory addressing etc),
 * only makes sure the command exists in the language.
 *
 * @param io_pLine Statement to parse - szContent should be filled with the
 * string that represents the line.
 * @return 0 if valid statement, any other val otherwise.
 */
int parser_get_statement(statement_t* io_pLine);

/**
 * Takes a string list of items and fills the given array of items
 * with ptrs to each of the items on the list.
 * The number of elements that should be in the output list
 * is given, and if the list doesn't contain that exact number
 * of items, an error occurs.
 * The cells in the array of items will be filled in the range
 * of [0,nListSize) (when of-course [0,0) is an empty group of cells)
 * @param szList String containing a comma-separated items.
 * @param o_arrItems Items found on the list (each item will be a
 * null-terminated string and any leading or trailing whitespaces
 * will be truncated from it).
 * @param nListSize Number of items that should be on the list.
 * @return 0 for success, anything else on error.
 */
int parser_get_items_from_list(char* szList,
							   char** o_arrItems,
							   size_t nListSize);

#endif /* PARSER_H_ */
