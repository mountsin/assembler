#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "second_scan.h"
#include "compile_and_write_output.h"
#include "AllTests.h"

//#define TEST
#define FILENAME_MAX 100

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

	//TODO: remove (testing)

	getchar();
}

void dispose_all()
{
	//free all symbols lists
	free_all_symbols_lists();

	
}
#endif