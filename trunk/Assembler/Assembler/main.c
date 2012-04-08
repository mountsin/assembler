#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "second_scan.h"
#include "compile_and_write_output.h"
#include "AllTests.h"
#include "error.h"
#include "global_constants.h"

//#define TEST


/*free all memory allocations*/
void dispose_all();

#ifdef TEST

void main()
{
	RunAllTests();
	getchar();
}
#else 
void main(int argc, char *argv[])
{
	int i;
	int successfulComp = 0;
	Boolean isAllOK = TRUE;

	char filename[FILENAME_MAX];

	for(i = 1; i < argc; i++)					/* files loop */
	{
		strcpy(filename, argv[i]);
		//strcat(filename_withEX, INPUT_FILE_EXT);

		first_scan(filename); 
		second_scan();
		compile_and_write_output(filename);

		if(get_errors_list() == NULL) successfulComp++; /*if OK - count as successful compilation*/

		dispose_all();
	}

	if(successfulComp == (i-1)) /*All Files successful*/
		printf("\n%s %d %s\n", "Compilation Successful.", successfulComp*FILES_PER_INPUT, "Files were Created.");
	else
		printf("\n%s %d %s\n", "Some files have failed compilation.", successfulComp*FILES_PER_INPUT, "Files were Created.");

	getchar();
}

void dispose_all()
{
	/* free memory allocations */
	free_all_symbols_lists();

	free_compiler_node_list();

	free_errors_collector();
	
}
#endif