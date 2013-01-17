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
	assert(test_parser() == 0);
	return 0;
}

int test_get_statement()
{
	statement_t stResult;
	statement_t stExpected;

	printf("Testing parser module:\n");

	/**********************************************/
	printf("	Empty line: ");
	strcpy(stResult.szContent, "   				");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_EMPTY == stResult.type);
	printf("PASSED.\n");
	/**********************************************/

	/* todo: Can a comment appear only in a line by itself? */
	/**********************************************/
	printf("	Commented line: ");
	strcpy(stResult.szContent, "; dsad  		dsa mov extern entry 	LAbde:	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_COMMENT == stResult.type);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Illegal directive line: ");
	strcpy(stResult.szContent, ".fu abc");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extern line: ");
	strcpy(stResult.szContent, ".extern ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);
	strcpy(stResult.szContent, ".extern");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".extern ");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".extern abcd abc");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Entry line: ");
	strcpy(stResult.szContent, ".entry ABC");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_ENTRY == stResult.info.directive.name);
	assert(&stResult.szContent[7] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);
	strcpy(stResult.szContent, ".entry");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".entry ");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".entry ABC Adsax");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Data line single: ");
	strcpy(stResult.szContent, ".data 7");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Data line multiple: ");
	strcpy(stResult.szContent, ".data +7, -57    ,17,9 ");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_DATA == stResult.info.directive.name);
	assert(&stResult.szContent[6] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);

	strcpy(stResult.szContent, ".data +7-57");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".data +7 -57");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	String line: ");
	strcpy(stResult.szContent, ".string \"ABC\"");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_STRING == stResult.info.directive.name);
	assert(&stResult.szContent[9] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);
	strcpy(stResult.szContent, ".string ABC\"");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".string ABC");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Ignoring whitespaces on directive: ");
	strcpy(stResult.szContent, "   .extern   	    ABC     ");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.info.directive.value);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Extra shit in directive: ");
	strcpy(stResult.szContent, ".extern dasd ABC");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ".extern ABC dasdgd");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Labeled line: ");
	strcpy(stResult.szContent, "H3LL0: .extern foo");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_DIRECTIVE == stResult.type);
	assert(DIRECTIVE_EXTERN == stResult.info.directive.name);
	assert(&stResult.szContent[8] == stResult.info.directive.value);
	assert(stResult.szContent == stResult.pLabel);
	assert(5 == stResult.lenbLabel); /* Not including ':' separator */
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label not first in line: ");
	strcpy(stResult.szContent, "   HELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label with bad syntax: ");
	strcpy(stResult.szContent, "8ELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ",ELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	strcpy(stResult.szContent, ":HELLO: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Label too long: ");
	strcpy(stResult.szContent, "HELLOHELLOHELLOHELLOHELLOHELLO31: .extern foo");
	assert(0 == parser_get_statement(&stResult));
	strcpy(stResult.szContent, "HELLOHELLOHELLOHELLOHELLOHELLO32: .extern foo");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/* fixme: Is it ok to parse the operands?*/
	/**********************************************/
	printf("	Instruction line, No operands: ");
	strcpy(stResult.szContent, "rts/0");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(RTS == stResult.info.instruction.name);
	assert(&stResult.szContent[3] == stResult.info.instruction.modifiers);
	assert(NULL == stResult.info.instruction.operand_first);
	assert(NULL == stResult.info.instruction.operand_second);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, One operand: ");
	strcpy(stResult.szContent, "clr/0 r2");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(CLR == stResult.info.instruction.name);
	assert(&stResult.szContent[3] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.info.instruction.operand_first);
	assert(NULL == stResult.info.instruction.operand_second);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line, Two operands: ");
	strcpy(stResult.szContent, "mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[3] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[6] == stResult.info.instruction.operand_first);
	assert(&stResult.szContent[8] == stResult.info.instruction.operand_second);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with label: ");
	strcpy(stResult.szContent, "MOFO:mov/0 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[8] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[11] == stResult.info.instruction.operand_first);
	assert(&stResult.szContent[13] == stResult.info.instruction.operand_second);
	assert(stResult.szContent == stResult.pLabel);
	assert(4 == stResult.lenbLabel); /* Not including ':' separator */
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with type=1: ");
	strcpy(stResult.szContent, "mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[3] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[10] == stResult.info.instruction.operand_first);
	assert(&stResult.szContent[12] == stResult.info.instruction.operand_second);
	assert(NULL == stResult.pLabel);
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with everything: ");
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[8] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[15] == stResult.info.instruction.operand_first);
	assert(&stResult.szContent[17] == stResult.info.instruction.operand_second);
	assert(stResult.szContent == stResult.pLabel);
	assert(4 == stResult.lenbLabel); /* Not including ':' separator */
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with weird spacing: ");
	strcpy(stResult.szContent, "MOFO:      mov/1/0/1 		A,		r1	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_INSTRUCION == stResult.type);
	assert(MOV == stResult.info.instruction.name);
	assert(&stResult.szContent[14] == stResult.info.instruction.modifiers);
	assert(&stResult.szContent[23] == stResult.info.instruction.operand_first);
	assert(&stResult.szContent[27] == stResult.info.instruction.operand_second);
	assert(stResult.szContent == stResult.pLabel);
	assert(4 == stResult.lenbLabel); /* Not including ':' separator */
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line illegal opcode:");
	strcpy(stResult.szContent, "MOFO:m00f/1/0/1 A,r1");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line with garbage (middle):");
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 	dasd A,r1	");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Instruction line garbage (end): ");
	strcpy(stResult.szContent, "MOFO:mov/1/0/1 A,r1	dasd41");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	return 0;
}
