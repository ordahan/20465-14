/*
 * parser.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 *
 *  This module is responsible for translating the given assembly
 *  program (as text) to data structures the assembler can handle.
 *  Any syntax error that isn't related to the instruction set,
 *  addresses etc will be caught here. Also, anything related to
 *  changing formats for numbers is handled here.
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "statement.h"
#include "instruction.h"
#include <stdlib.h>

#define MAX_LIST_LENGTH MAX_LINE_LENGTH

/* For ease of implementation */
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
 * If the number of expected elements is 0, szList must be NULL
 * in order to succeed.
 * Each separator in the list is replaced with the null-termination
 * char (thus splitting the given list to all of its members.
 *
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
/**
 * Makes sure that the given label is valid.
 * @param szLabel Label to check
 * @return Length of the label, 0 if label have any syntax errors
 */
int parser_check_symbol_syntax(const char* szSymbol);

/**
 * Gets the type of the given modifiers string.
 * @param szModifiers Modifiers of the instruction.
 * @return What type (if legal) is the instruction,
 * or INVALID_TYPE if its invalid.
 */
instruction_type_t parser_get_instruction_type(const char* szModifiers);

/**
 * Gets the comb value of the given modifiers string,
 * if there is any.
 *
 * @param szModifiers Modifiers of the instruction.
 * @return NO_COMB if no comb exists,
 * otherwise: what comb (if legal) is the instruction,
 * or INVALID_COMB if its invalid.
 */
instruction_comb_t parser_get_instruction_comb(const char* szModifiers);

/**
 * Parses the given operand (both gets the addressing type and
 * places data in the given instruction while updating the number of
 * extra cells used). If the operand is a register, the value of the
 * register is temporarily placed in the "dest" register.
 * This function might change the value of the string szOperand!
 *
 * @param szOperand string representing the operand value.
 * @param pInstruction Where to place the extra data that the operand takes.
 * @param arrSymbols Where to retrieve symbols from.
 * @return Addressing type for the operand.
 */
operand_addressing_t parser_get_operand(char* szOperand,
										instruction_with_operands_t* pInstruction,
										const symbol_table_arr_t arrSymbols);

/**
 * Performs only a shallow parsing of an operand, finds out
 * which type it is (doesn't check in case of a label, if it
 * is legal).
 * @param szOperand Operand to parse
 * @return size of the operand in memory cells.
 */
unsigned int parser_shallow_parse_operand(char* szOperand);

/**
 * Calculates the number of items that comprise the list.
 * Doesn't check that the list format is valid at all.
 * @param szList List of items to count.
 * @return negative number if the ptr given isn't valid, otherwise
 * the number of items found on the list (0 is a valid number)
 */
unsigned int parser_get_num_items_in_list(char* szList);

/**
 * Gets the number represented in the string
 * @param szNumber String of a number.
 * @param o_pNum Number found.
 * @return 1 if number retrieved ok, 0 otherwise.
 */
unsigned char parser_get_number(const char* szNumber, int *o_pNum);

/**
 * Extracts the string representation in the given string.
 * If the given string contains anything other than the representation
 * or its syntax is incorrect - this function will fail.
 *
 * @param szString String that contains the representation.
 * @param o_pStart Where to place the ptr to the start of the representation.
 * @param o_pEnd  Where to place the ptr to the end of the representation.
 * @return 0 on success, anything else on error. If found, o_pStart
 * points to the first char in the string and o_pEnd points to the first
 * byte after the last char in the string. In case of an error the values
 * for the given ptr pair is un-determined and should not be relied upon.
 * If the string is empty, o_pStart == o_pEnd (first byte after the empty string)
 */
int parser_get_string(const char* szString, const char** o_pStart, const char** o_pEnd);

/**
 * Finds consecutive strings in memory starting from
 * pStart until reaching pEnd (all strings found must be
 * in this range. A string that surpasses the range is
 * treated as an error.)
 * @param pStart Where to start looking.
 * @param pEnd First address that is out of range.
 * @param arrStrings Where to place ptrs to the strings found.
 * @param nMaxResults Max num of strings to be found
 * @return 0 on success, anything else on error.
 */
int parser_get_consecutive_strings(const char* pStart,
								   const char* pEnd,
								   const char** arrStrings,
								   unsigned int nMaxResults);

/**
 * Returns a string representing the given int in base 4.
 * The string belongs to the parsing function! Do not rely
 * on its value after it has been called again.
 *
 * @param num Number to convert
 * @param fExtend Number of chars to be extended to the left.
 * @return String representing the given number extended by the amount
 * requested.
 */
const char* parser_int_to_string_base_4(int num, unsigned int numToExtend);

/**
 * Converts locality value to an ascii letter
 * @param locality Locality to convert
 * @return an ascii character for the given locality
 */
char parser_get_locality_letter(address_locality_t locality);

#endif /* PARSER_H_ */
