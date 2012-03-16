#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "first_scan.h"
#include "symbols.h"
#include "error.h"


#define LINE_SIZE 100

//TODO: check if needed
typedef enum boolean
{
	FALSE,
	TRUE
} Boolean;

void read_line_and_build_statement_struct(char *line);
void validate_label(char *token, AssemblyStatement *stmt);
enum cmd parse_command(char *command_name);
char *get_next_token();
char *get_token(char *text);
void process_statement(AssemblyStatement stmt);
void debug_output(char *what);
void parse_and_load_data(AssemblyStatement *stmt, int *dc);

//commands_list - table(array) contains each assembly command and its rules
CommandStruct commands_list[] = 
{
	MOV,		"mov", "0000", 3 , {0,1,2,3,4}, {1,2,3,4,EMPTY},
	CMP,		"cmp", "0001", 3 , {0,1,2,3,4}, {0,1,2,3,4},
	ADD,		"add", "0010", 3 , {0,1,2,3,4}, {1,2,3,4,EMPTY},
	SUB,		"sub", "0011", 3 , {0,1,2,3,4}, {1,2,3,4,EMPTY},
	NOT,		"not", "0100", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	CLR,		"clr", "0101", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	LEA,		"lea", "0110", 3 , {1,2,3,EMPTY,EMPTY},{1,2,3,4,EMPTY},
	INC,		"inc", "0111", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	DEC,		"dec", "1000", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	JMP,		"jmp", "1001", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	BNE,		"bne", "1010", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	RED,		"red", "1011", 2 , EMPTY_ARRAY, {1,2,3,4,EMPTY},
	PRN,		"prn", "1100", 2 , EMPTY_ARRAY, {0,1,2,3,4},
	JSR,		"jsr", "1101", 2 , EMPTY_ARRAY, {1,EMPTY,EMPTY,EMPTY,EMPTY},
	RTS,		"rts", "1110", 1 , EMPTY_ARRAY, EMPTY_ARRAY,
	STOP,		"stop","1111", 1 , EMPTY_ARRAY, EMPTY_ARRAY,
	UNKNOWN_CMD,""	  ,""	 , 0 , EMPTY_ARRAY, EMPTY_ARRAY
};

/* Read the assembly file, line by line and process the statements */
void first_scan(char *filename)
{
	FILE *fp;
	char line[LINE_SIZE];
	int ic = 0,				/* Instructions counter */
		dc = 0,				/* Data counter */
		line_number = 1;	/* line counter for the errors report */
	AssemblyStatement stmt; /* Each code line will be parsed and stored in this temporary struct */

	fp = fopen(filename,"r");
	while(fgets(line,LINE_SIZE,fp))
	{
		read_line_and_build_statement_struct(line, &stmt);
		if(stmt.command == UNKNOWN_CMD)
			add_error(line_number, UNKNOWN_COMMAND);
		if(stmt.command == DATA || stmt.command == STRING) /* Check if it's .data or .string instruction */
		{
			if(stmt.label)
				add_symbol(stmt.label, dc, INSTRUCTION);
			parse_and_load_data(&stmt, &dc);
			continue;
		}

		if(stmt.command == EXTERN) /* Check if it's .extern instruction */
		{
			add_external_symbol(stmt.target_operand);
			continue;
		}

		if(stmt.label)
			add_symbol(stmt.label,ic,OPCODE);

		


		ic += commands_list[stmt.command].number_of_words;
		line_number++;
	}
	fclose(fp);
}

void parse_and_load_data(AssemblyStatement *stmt, int *dc)
{
	//TODO: Input validtaion and Error handling
	char *tmp;

	switch(stmt->command)
	{
		case DATA:
			tmp = get_first_token(stmt->target_operand);
			while(tmp != NULL)
				*dc++;
			break;
		case STRING:
			*dc += strlen(stmt->target_operand)+1;
			break;
	}
}

/* Accept assembly code line and populate an AssemblyStatement struct */
void read_line_and_build_statement_struct(char *line, AssemblyStatement *stmt)
{
	char *token;
	debug_output(line);	

	if(line[0] == ';')
	{
		debug_output("DEBUG: Comment line");
		return;
	}

	token =  get_next_token();
	if(token)
	{
		//TODO: remove (testing)
		debug_output(token);
		validate_label(token, stmt);
		stmt->command = parse_command(token);
		if(stmt->command == UNKNOWN_COMMAND)
		{
			//TODO: Add error with ilne number
		}
		stmt->source_operand = get_next_token();
		stmt->target_operand = get_next_token();
		if(stmt->target_operand == NULL)
		{
			stmt->target_operand = stmt->source_operand;
			stmt->source_operand = NULL;
		}
	}
}

/* Accept a token and check if it is a valid label. If so, copy the token to the lable field of the struct and advance to the next token */
void validate_label(char *token, AssemblyStatement *stmt)
{
	int length_without_colon = strlen(token)-1;
	if(token[length_without_colon] == ':')
	{
		strncpy(stmt->label, token, length_without_colon);			
		token = get_next_token();
	}
}

enum cmd parse_command(char *command_name)
{
	CommandStruct *tmp;
	for(tmp = commands_list; tmp->name && strcmp(command_name, tmp->name); tmp++);
	return tmp->cmd_type;
}

char *get_first_token(char *text)
{
	return get_token(text);
}

char *get_next_token(void)
{
	return get_token(NULL);
}

char *get_token(char *text)
{
	char *delimiters = " ,\t\n\r";
	char *temp;
	temp = strtok(text, delimiters);
	return temp;
}

void debug_output(char *what)
{
	puts(what);
}