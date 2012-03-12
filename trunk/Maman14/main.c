#include <stdio.h>
#include <string.h>
#include "CuTest.h"

#define FILENAME_MAX 100
#define LINE_SIZE 100

void process_file(char *filename);
void process_line(char *line);



void main(int argc, char *argv[])
{
	int i;
	char filename[FILENAME_MAX];

	for(i = 1; i < argc; i++)					/* files loop */
	{
		strcpy(filename, argv[i]);
		strcat(filename, ".as");
		process_file(filename);
	}

	getchar();
}

void process_file(char *filename)
{
	FILE *fp;
	char line[LINE_SIZE];

	fp = fopen(filename,"r");
	while(fgets(line,LINE_SIZE,fp))
	{
		process_line(line);							/* first scan */
	}
	fclose(fp);
}
/*sefi test*/
void process_line(char *line)
{
	char *delimiters = " ,\t\n\r";
	char *token;

	puts(line);	/* for debug */

	if(line[0] == ';')
		puts("DEBUG: Comment line");
	else
	{
		token = strtok(line, delimiters);
		while(token)
		{
			puts(token);
			token = strtok(NULL, delimiters);
		}
	}
}
