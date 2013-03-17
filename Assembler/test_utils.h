/*
 * test_utils.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include "symbol.h"
#include "assembler.h"

int compare_files(const char* szFile1 , const char* szFile2);

int compare_memory_sections(const memory_section_t* pMem1,
							const memory_section_t* pMem2);


#endif /* TEST_UTILS_H_ */
