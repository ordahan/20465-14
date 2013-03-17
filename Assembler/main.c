/*
 * main.c
 *
 *  Created on: Feb 5, 2013
 *      Author: Or Dahan 201644929
 */

#include "assembler.h"
#include "files.h"

int main(int argc, char** argv)
{
	unsigned int i;
	symbol_table_arr_t symbols;
	memory_section_t   code;
	memory_section_t   data;

	/* Go over the input files one by one and
	 * try to compile them
	 */
	for (i = 1; i < argc; ++i)
	{
		/* Try to compile the file */
		if (assembler_compile(file_open_input(argv[i]),
							  symbols,
							  &code,
							  &data) != 0)
		{
			printf("Aborting compilation of %s\n", argv[i]);

			/* No need to create outputs, next file */
			continue;
		}

		/* Create outputs for it */
		if (file_create_entry(argv[i], symbols) != 0)
		{
			printf("Error! cannot create an entry file for %s", argv[i]);
		}

		if (file_create_externals(argv[i],
								  symbols,
								  &code) != 0)
		{
			printf("Error! cannot create an externs file for %s", argv[i]);
		}

		if (file_create_object(argv[i],
							   &code,
							   &data) != 0)
		{
			printf("Error! cannot create an object file for %s", argv[i]);
		}
	}

	return 0;
}
