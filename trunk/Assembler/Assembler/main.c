#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "second_scan.h"
#include "compile_and_write_output.h"
#include "AllTests.h"
#include "error.h"


//#define TEST
#define FILENAME_MAX 100
#define FILES_PER_INPUT 3

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
	char filename[FILENAME_MAX];

	for(i = 1; i < argc; i++)					/* files loop */
	{
		strcpy(filename, argv[i]);
		strcat(filename, ".as");
		first_scan(filename); 
		second_scan();
		compile_and_write_output(filename);
		dispose_all();
	}

	printf("%s %d %s", "Compilation Successful.", (i-1)*FILES_PER_INPUT, "Files were Created.");

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