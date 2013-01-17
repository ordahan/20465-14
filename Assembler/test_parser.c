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

int test_get_statement();

int test_parser()
{
	assert(test_get_statement() == 0);
	return 0;
}

int test_get_statement()
{
	statement_t stResult;

	printf("Testing parser module:\n");

	/**********************************************/
	printf("	Empty line: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "   				");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_EMPTY == stResult.type);
	printf("PASSED.\n");
	/**********************************************/

	/* todo: Can a comment appear only in a line by itself? */
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
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
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Ignoring white-spaces on directive: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "   .extern   	    ABC     ");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[18] == stResult.szOperationData);
	assert(NULL == stResult.pLabel);
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
	assert(stResult.szContent == stResult.pLabel);
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
	/* fixme: 30 chars .. or 31? not sure */
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
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(RTS == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(NULL == stResult.szOperationData);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, One operand: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "clr/0 r2");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(CLR == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, Two operands: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.szOperationData);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with label: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[11] == stResult.szOperationData);
	assert(stResult.szContent == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with type=1: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[4] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[10] == stResult.szOperationData);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with everything: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[15] == stResult.szOperationData);
	assert(stResult.szContent == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	/* Should pass as we don't parse to deeply here */
	printf("	Instruction line with more operands than should: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1, x5");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[9] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[15] == stResult.szOperationData);
	assert(stResult.szContent == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with weird spacing: ");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:      mov/1/0/1 		A,		r1	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[15] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[23] == stResult.szOperationData);
	assert(stResult.szContent == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line illegal opcode:");
	memset(&stResult, 0, sizeof(stResult));
	strcpy(stResult.szContent, "MOFO:m00f/1/0/1 A,r1");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	return 0;
}
