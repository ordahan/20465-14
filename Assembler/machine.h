/*
 * machine.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

typedef struct
{
	unsigned int val : 20;
}machine_cell_t;

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
#define REGISTERS_LAST (R7+1)

#endif /* DATATYPES_H_ */
