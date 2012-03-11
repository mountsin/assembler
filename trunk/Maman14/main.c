#include <stdio.h>
#include <string.h>

void process_file(char *filename);
void process_line(char *line);

void main(int argc, char *argv[])
{
	/*test*/
	int i;
	char filename[100];

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
	char line[81];

	fp = fopen(filename,"r");
	while(fgets(line,80,fp))
	{
		process_line(line);							/* first scan */
	}
	fclose(fp);
}

void process_line(char *line)
{
	char *delimiters = "\t\n\r ,";
	char *token;

	puts(line);	/* for debug */

	token = strtok(line, delimiters);
	if(line[0] == ';')
		puts("DEBUG: Comment line");
	
}
