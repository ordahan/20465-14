/*
 * directive.h
 *
 *  Created on: Jan 17, 2013
 *      Author: Or Dahan 201644929
 */

#ifndef DIRECTIVE_H_
#define DIRECTIVE_H_

typedef enum
{
	DIRECTIVE_DATA,
	DIRECTIVE_STRING,
	DIRECTIVE_ENTRY,
	DIRECTIVE_EXTERN,

	/* This must be last */
	DIRECTIVE_ILLEGAL
}directive_type_t;


#endif /* DIRECTIVE_H_ */
