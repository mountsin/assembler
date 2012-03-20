#include <stdio.h>
#include <string.h>
#include "first_scan.h"
#include "AllTests.h"

#define TEST
#define FILENAME_MAX 100


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
		//second_scan();//TODO: implement
		//compile_and_write_output(filename);//TODO: implement
	}

	//TODO: remove (testing)
	getchar();
}

void dispose_all()
{
	//TODO: dispose compiler node list and symbols list
}
#endif