/*
 * machine.h
 *
 *  Describes the machine that we are compiling to.
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

typedef struct
{
	unsigned int val;
}machine_cell_t;

#define MACHINE_CELL_NUM_BITS (20)
/* A pair of bits is 1 base 4 number,
 * as 2^2 = 4 (base^k = new_base), k = 2
 * Round up the number of chars.
 */
#define MACHINE_CELL_NUM_BASE_4_CHARACTERS (((MACHINE_CELL_NUM_BITS - 1) / 2) + 1)

typedef enum
{
	R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7
}machine_registers_t;
#define REGISTER_INVALID (R7+1)

#endif /* DATATYPES_H_ */
