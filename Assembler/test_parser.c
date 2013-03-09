/*
 * test_assembler.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */


#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "parser.h"
#include "directive.h"
#include "instruction.h"

int test_get_statement();
int test_get_items_from_list();

int test_parser()
{
	assert(test_get_statement() == 0);
	assert(test_get_items_from_list() == 0);
	return 0;
}

int test_get_statement()
{
	statement_t stResult;

	printf("Testing parser module:\n");

	/**********************************************/
	printf("	Empty line: ");
	memset(&stResult, 0, sizeof(stResult));
	stResult.szLabel = "lalala";
	strcpy(stResult.szContent, "   				");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_EMPTY == stResult.type);
	assert(stResult.szLabel == NULL);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Commented line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "; dsad  		dsa mov extern entry 	LAbde:	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_COMMENT == stResult.type);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Illegal directive line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".fu abc");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".extern ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	strcpy(stResult.szContent, ".extern");
	assert(0 == parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".extern ");
	assert(0 == parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".entry ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_ENTRY == stResult.info.directive.name);
	assert(&stResult.szContent[7] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	strcpy(stResult.szContent, ".entry");
	assert(0 == parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".entry ");
	assert(0 == parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Data line single: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".data 7");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Data line multiple: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".data +7, -57    ,17,9 ");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(17 == strlen(stResult.szOperationData));
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/* Not handled here, should be ok */
	/**********************************************/
	printf("	Data line invalid number format: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".data +7-57");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/* Not handled here, should be ok */
	/**********************************************/
	printf("	Data line multiple number not separated: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".data +7 -57");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	String line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".string \"ABC\"");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_STRING == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	String line missing quote: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".string \"ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_STRING == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	String line missing quotes: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".string ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_STRING == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Ignoring white-spaces on directive: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "   .extern   	    ABC     ");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[11] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	/* Should pass as we don't parse to deeply here */
	printf("	Extra shit in directive: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ".extern dasd ABC");
	assert(0 == parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Labeled line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "H3LL0: .extern foo");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[15] == stResult.szOperationData);
	assert(stResult.szContent == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label not first in line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "   HELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label with bad syntax: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "8ELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ",ELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, ":HELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label too long: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "HELLOHELLOHELLOHELLOHELLOHELLO: .extern foo");
	assert(0 == parser_get_statement(&stResult));
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "HELLOHELLOHELLOHELLOHELLOHELL31: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label with reserved name: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "r5: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "rts: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, No operands: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "rts/0");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(RTS == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(NULL == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, One operand: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "clr/0 r2");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(CLR == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, Two operands: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with label: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[11] == stResult.szOperationData);
	assert(stResult.szContent == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with type=1: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[10] == stResult.szOperationData);
	assert(NULL == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with everything: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[15] == stResult.szOperationData);
	assert(stResult.szContent == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	/* Should pass as we don't parse to deeply here */
	printf("	Instruction line with more operands than should: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1, x5");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[15] == stResult.szOperationData);
	assert(stResult.szContent == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with weird spacing: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:      mov/1/0/1 		A,		r1	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCTION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[15] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[21] == stResult.szOperationData);
	assert(9 == strlen(stResult.szOperationData));
	assert(stResult.szContent == stResult.szLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line illegal opcode:");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:m00f/1/0/1 A,r1");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}

int run_get_items_from_list(char* szList, char** arrItems, unsigned int nListSize)
{
	char szCurrTestCase[MAX_LIST_LENGTH];

	strcpy(szCurrTestCase, szList);

	return parser_get_items_from_list(szCurrTestCase,
									  arrItems,
									  nListSize);
}
#define MAX_NUM_ITEMS 5
int test_get_items_from_list()
{
	/*
	 * For each test, expect less,equal,more than list size
	 * #Empty list (no less)
	 * #Empty list with whitespaces
	 * #Only delimiter
	 * #Delimiter and whitespaces
	 * #One value
	 * #Two values
	 * #N values
	 * #Whitespaces inside list
	 * #Leading whitespaces before list
	 * #Trailing whitespaces after list
	 * #Items separated by illegal delimiter
	 * #Item contains whitespaces
	 * #Items separated by illegal delimiter with whitespaces
	 * #Empty item in start of list
	 * #Empty item in middle of list
	 * #Empty item in end of list
	 * #Empty item in start of list with spaces
	 * #Empty item in middle of list with spaces
	 * #Empty item in end of list with spaces
	 * Null ptrs
	 */
	char* arrItems[MAX_NUM_ITEMS];
	char szList[MAX_LIST_LENGTH];

	/**********************************************/
	printf("	Empty list:");
	strcpy(szList, "");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 == run_get_items_from_list(szList,
										arrItems,
										0));
	assert(arrItems[0] == NULL);
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty list with whitespaces:");
	strcpy(szList, "   		 	");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   0));
	assert(arrItems[0] == NULL);
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Only delimiter:");
	strcpy(szList, ",");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   2));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Delimiter with whitespaces before:");
	strcpy(szList, "   	,");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   2));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Delimiter with whitespaces after:");
	strcpy(szList, ", 	");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   2));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Delimiter with whitespaces both sides:");
	strcpy(szList, "   	, 	");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   1));
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   2));
	printf("PASSED.\n");
	/**********************************************/


	/**********************************************/
	printf("	One item in list:");
	strcpy(szList, "hello");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   1));
	assert(0 == strcmp(arrItems[0],"hello"));

	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   2));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Two item in list:");
	strcpy(szList, "hello,love");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   0));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   2));
	assert(0 == strcmp(arrItems[0], "hello"));
	assert(0 == strcmp(arrItems[1], "love"));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	N items in list:");
	strcpy(szList, "hello,love,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   5));
	assert(0 == strcmp(arrItems[0], "hello"));
	assert(0 == strcmp(arrItems[1], "love"));
	assert(0 == strcmp(arrItems[2], "nice"));
	assert(0 == strcmp(arrItems[3], "to"));
	assert(0 == strcmp(arrItems[4], "meet"));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Whitespaces in list:");
	strcpy(szList, "hello ,love	,nice,  	to, meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   5));
	assert(0 == strcmp(arrItems[0], "hello"));
	assert(0 == strcmp(arrItems[1], "love"));
	assert(0 == strcmp(arrItems[2], "nice"));
	assert(0 == strcmp(arrItems[3], "to"));
	assert(0 == strcmp(arrItems[4], "meet"));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Leading whitespaces before list:");
	strcpy(szList, " hello,love,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   5));
	assert(0 == strcmp(arrItems[0], "hello"));
	assert(0 == strcmp(arrItems[1], "love"));
	assert(0 == strcmp(arrItems[2], "nice"));
	assert(0 == strcmp(arrItems[3], "to"));
	assert(0 == strcmp(arrItems[4], "meet"));

	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Trailing whitespaces after list:");
	strcpy(szList, "hello,love,nice,to,meet	");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 == run_get_items_from_list(szList,
										   arrItems,
										   5));
	assert(0 == strcmp(arrItems[0], "hello"));
	assert(0 == strcmp(arrItems[1], "love"));
	assert(0 == strcmp(arrItems[2], "nice"));
	assert(0 == strcmp(arrItems[3], "to"));
	assert(0 == strcmp(arrItems[4], "meet"));

	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Illegal delimiter:");
	strcpy(szList, "hello,love;nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Item contains whitespaces:");
	strcpy(szList, "hello,love nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   4));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in start:");
	strcpy(szList, ",hello,love,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in middle:");
	strcpy(szList, "hello,love,,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* Might consider equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   7));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in end:");
	strcpy(szList, "hello,love,nice,to,meet,");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in start with spaces:");
	strcpy(szList, " 	,hello,love,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in middle with spaces:");
	strcpy(szList, "hello,love, 	,nice,to,meet");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* Might consider equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   7));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Empty in end with spaces:");
	strcpy(szList, "hello,love,nice,to,meet, 	");
	memset(arrItems, 0, sizeof(arrItems) / sizeof(arrItems[0]));
	/* Less */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   3));
	/* Equal */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   5));
	/* More */
	assert(0 != run_get_items_from_list(szList,
										   arrItems,
										   6));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Null ptrs:");
	assert(0 != parser_get_items_from_list(NULL,
										   arrItems,
										   3));
	assert(0 != run_get_items_from_list(szList,
										   NULL,
										   3));
	printf("PASSED.\n");
	/**********************************************/

	printf ("PASSED.\n");
	return 0;
}
