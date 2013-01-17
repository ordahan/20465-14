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

int test_parser()
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

	/**********************************************/
	printf("	Commented line: ");
	strcpy(stResult.szContent, "; dsad  		dsa mov extern entry 	LAbde:	");
	assert(0 == parser_get_statement(&stResult));
	assert(STATEMENT_TYPE_COMMENT == stResult.type);
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
	strcpy(stResult.szContent, ".entry ABC ARC");
	assert(0 != parser_get_statement(&stResult));
	printf("PASSED.\n");
	/**********************************************/

	/**********************************************/
	printf("	Data line: ");
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

	/* todo: test instruction syntax...*/
	return 0;
}
