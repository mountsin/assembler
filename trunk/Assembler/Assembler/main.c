#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "second_scan.h"
#include "compile_and_write_output.h"
#include "AllTests.h"
#include "error.h"
#include "global_constants.h"

#define STD_QUIT_MSG "Press any key to quit."
/* #define TEST */


/*free all memory allocations*/
void dispose_all();

#ifdef TEST

void main()
{
	RunAllTests();
	getchar();
}
#else 
int main(int argc, char *argv[])
{
	int i;
	int filesCreated = 0;
	Boolean isAllOK = TRUE;

	char filename[MAX_FILENAME];

	for(i = 1; i < argc; i++)					/* files loop */
	{
		strcpy(filename, argv[i]);

		first_scan(filename); 
		second_scan();
		filesCreated += compile_and_write_output(filename); /*compile, count how many files were created*/

		if(get_errors_list() != NULL) isAllOK = FALSE; /*if not OK - Mark error*/

		dispose_all(); /*free all memory allocations*/
	}

	if(isAllOK == TRUE) /*All Files successful*/
	{
		printf("\n%s %d %s\n", "Compilation Successful.", filesCreated, "Files were Created.");
		printf("%s\n", STD_QUIT_MSG);
		getchar();
		return 0;
	}
	else
	{
		printf("\n%s %d %s\n", "Some files have failed compilation.", filesCreated, "Files were Created.");
		printf("%s\n", STD_QUIT_MSG);
		getchar();
		return -1;
	}
}

void dispose_all()
{
	/* free memory allocations */
	free_all_symbols_lists();

	free_compiler_node_list();

	free_errors_collector();
	
}
#endif
