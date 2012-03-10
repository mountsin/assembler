#include <stdio.h>

void process_file(char *filename);
void process_line(char *line);

void main(int argc, char *argv[])
{
	int i;

	for(i = 1; i < argc; i++)
	{
		process_file(argv[i]);
	}

	getchar();
}

void process_file(char *filename)
{
	FILE *file;
	char line[81];

	file = fopen(filename,"r");
	while(fgets(line,80,file))
	{
		process_line(line); /* first scan */
	}
	fclose(file);
}

void process_line(char *line)
{
	puts(line);
}
