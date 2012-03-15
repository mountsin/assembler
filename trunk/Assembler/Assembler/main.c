#include <stdio.h>
#include <string.h>
#include "first_scan.h"

#define FILENAME_MAX 100

void main(int argc, char *argv[])
{
	int i;
	char filename[FILENAME_MAX];

	for(i = 1; i < argc; i++)					/* files loop */
	{

		strcpy(filename, argv[i]);
		strcat(filename, ".as");
		first_scan(filename); //TODO: implement
		//second_scan();//TODO: implement
		//compile_and_write_output(filename);//TODO: implement
	}

	//TODO: remove (testing)
	getchar();
}