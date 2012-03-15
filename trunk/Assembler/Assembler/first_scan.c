#include <stdio.h>
#include <string.h>
#include "first_scan.h"

#define LINE_SIZE 100

void first_scan(char *filename)
{

}

void read_file(char *filename)
{
	FILE *fp;
	char line[LINE_SIZE];

	fp = fopen(filename,"r");
	while(fgets(line,LINE_SIZE,fp))
	{
		read_line_and_build_statement_struct(line);							/* first scan */
	}
	fclose(fp);
}

void read_line_and_build_statement_struct(char *line)
{
	char *delimiters = " ,\t\n\r";
	char *token;
	AssemblyStatement stmt;
	
	debug_output(line);	

	if(line[0] == ';')
	{
		debug_output("DEBUG: Comment line");
		return;
	}

	token = strtok(line, delimiters);
	if(token)
	{
		//TODO: remove (testing)
		debug_output(token);
		validate_label(token, delimiters, &stmt);
		stmt.command = parse_command(token);
		stmt.source_operand = get_next_token(delimiters);
		stmt.target_operand = get_next_token(delimiters);
		process_statement(stmt);
	}
}

void validate_label(char *token, char *delimiters, AssemblyStatement *stmt)
{
	int length_without_colon = strlen(token)-1;
	if(token[length_without_colon] == ':')
	{
		strncpy(stmt->label, token, length_without_colon);			
		token = get_next_token(delimiters);
	}
}

void process_statement(AssemblyStatement *stmt)
{
	//TODO:Implement
}

char *get_next_token(char *delimiters)
{
	char *temp = 0;
	temp = strtok(NULL, delimiters);
	if(temp)
		debug_output(temp);
	return temp;
}

enum cmd parse_command(char *command_name)
{
	CommandStruct *tmp;
	for(tmp = commands_list; tmp->name && strcmp(command_name, tmp->name); tmp++);
	return tmp->cmd_type;
}

void debug_output(char *what)
{
	puts(what);
}