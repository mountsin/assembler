#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structs.h"
#include "first_scan.h"
#include "symbols.h"
#include "error.h"
#include "global_functions.h"
#include "pre_compiled.h"

#define LINE_SIZE 100


Boolean validate_label(char *token, CompilerNode *stmt);
enum cmd parse_command(char *command_name);
char *get_first_token(char *text);
char *get_next_token(void);
char *get_token(char *text);
void process_statement(CompilerNode stmt);
void debug_output(char *what);
void parse_and_load_data(CompilerNode *stmt, int *dc);
enum addressing_method get_addressing_for(char *source_operand);
void read_line_and_build_statement_struct(char *, CompilerNode *);
void set_symbol(char *str,char *result[]);
void set_index(char *str,char *result[]);
void set_addressing_and_register(char *operand,AddressingMethod *addressing ,int *reg);
void add_second_word(Cmd cmd_type, AddressingMethod source_addressing,char *source_operand, char *binary_machine_code, int ic);
void add_third_word(Cmd cmd_type,AddressingMethod targetAddressing,char *target_operand);



/* Commands_list - table(array) contains each assembly command and its rules */
CommandStruct commands_list[] = 
{
/*	cmd_type	name		source_addressing_options	dest_addressing_options */
	MOV,		"mov", 		{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	CMP,		"cmp", 		{0,1,2,3,4},				 {0,1,2,3,4},
	ADD,		"add", 		{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	SUB,		"sub", 		{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	NOT,		"not", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	CLR,		"clr", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	LEA,		"lea", 		{1,2,3,EMPTY				,EMPTY},{1,2,3,4,EMPTY},
	INC,		"inc", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	DEC,		"dec", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	JMP,		"jmp", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	BNE,		"bne", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	RED,		"red", 		EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	PRN,		"prn", 		EMPTY_ARRAY,				 {0,1,2,3,4},
	JSR,		"jsr", 		EMPTY_ARRAY,				 {1,EMPTY,EMPTY,EMPTY,EMPTY},
	RTS,		"rts", 		EMPTY_ARRAY,				 EMPTY_ARRAY,
	STOP,		"stop",		EMPTY_ARRAY,				 EMPTY_ARRAY,
	DATA,		".data",	EMPTY_ARRAY,				 EMPTY_ARRAY,
	STRING,		".string",	EMPTY_ARRAY,				 EMPTY_ARRAY,
	ENTRY,		".entry",	EMPTY_ARRAY,				 EMPTY_ARRAY,	
	EXTERN,		".extern",	EMPTY_ARRAY,				 EMPTY_ARRAY,	
	UNKNOWN_CMD,"\0"	  ,	EMPTY_ARRAY,				 EMPTY_ARRAY
};

int ic = 0;								/* Instructions counter */
int dc = 0;								/* Data counter */
int line_number = 0;					/* Line number in the assembly file for the errors report */

/* Read the assembly file, line by line and process the statements */
void first_scan(char *filename)
{
	FILE *fp;
	char line[LINE_SIZE];
	CompilerNode stmt;													/* Each code line will be parsed and stored in this temporary struct */
	CommandStruct command_struct_from_validation_list;
	int label_exist = 0;

	fp = fopen(filename,"r");
	if(fp)
	{
	while(fgets(line,LINE_SIZE,fp))
	{
		line_number++;
		read_line_and_build_statement_struct(line, &stmt);
		if(stmt.cmd_type == COMMENT)
			continue;
		if(stmt.cmd_type == UNKNOWN_CMD)
			add_error(line_number, UNKNOWN_COMMAND);
		label_exist = stmt.label != NULL;								/* Check if label exists */
		if(stmt.cmd_type == DATA || stmt.cmd_type == STRING)			/* Check if it's .data or .string instruction */
		{
			if(label_exist) 
				add_data_symbol(stmt.label, dc);
			parse_and_load_data(&stmt, &dc);
			continue;
		}

		if(stmt.cmd_type == EXTERN)										/* Check if it's .extern instruction */
		{
			add_external_symbol(stmt.target_operand,ic); 
			continue;
		}

		if(label_exist)
			add_data_symbol(stmt.label,ic);

		//TODO: cancel duplication
		set_addressing_and_register(stmt.source_operand,&stmt.sourceAddressing,&stmt.source_register);
		set_addressing_and_register(stmt.target_operand,&stmt.targetAddressing,&stmt.target_register);
		add_second_word(stmt.cmd_type,stmt.sourceAddressing,stmt.source_operand,stmt.binary_machine_code,++ic);
		add_third_word(stmt.cmd_type,stmt.targetAddressing,stmt.target_operand);

		stmt.line_number = line_number;
		add_compiler_node(&stmt);
		ic++;
	}
	fclose(fp);
	}
	else
	{
		add_error(line_number,INPUT_FILE_FAILURE);
	}
}

void set_symbol(char *str,char *result)
{
	strncpy(result,str,strchr(str,'[')-str+1);
}
void set_index(char *str,char *result)
{
	char *start_of_index = strchr(str,'%');
	char *end_of_index = strchr(str,']');
	strncpy(result,start_of_index,end_of_index - start_of_index);
}

AddressingMethod get_addressing_for(char *operand)
{
	if(is_literal(operand))
		return IMMEDIATE;
	if(is_register(operand))
		return REGISTER;
	if(is_double_index(operand))
		return DOUBLE_INDEX;
	if(is_index(operand))
		return INDEX;
	return DIRECT;
}

void parse_and_load_data(CompilerNode *stmt, int *dc)
{
	//TODO: Input validtaion and Error handling
	char *tmp;

	switch(stmt->cmd_type)
	{
		case DATA:
			tmp = get_first_token(stmt->target_operand);
			while(tmp != NULL)
			{
				*dc++;
				tmp = get_next_token();
			}
			break;
		case STRING:
			*dc += strlen(stmt->target_operand)+1;
			break;
	}
}

Boolean is_register(char *str)
{
	if (str[0] == 'r' && (str[1] >= '0' && str[1] <= '7') && str[2] == '\0')
		return TRUE;
	return FALSE;
}

Boolean is_literal(char *str)
{
	if(str[0] == '#')
		return TRUE;
	return FALSE;
}

Boolean is_index(char *str)
{
	if(strchr(str,'['))
		return TRUE;
	return FALSE;
}

Boolean is_double_index(char *str)
{
	if(str[0] == '[')
		return TRUE;
	return FALSE;
}

/* Accept assembly code line and populate an CompilerNode struct */
void read_line_and_build_statement_struct(char *line, CompilerNode *stmt)
{
	char *token;
	Boolean result;
	debug_output(line);	

	if(line[0] == ';')
	{
		stmt->cmd_type = COMMENT;
		debug_output("DEBUG: Comment line");
		return;
	}

	token =  get_first_token(line);
	if(token)
	{
		result = validate_label(token, stmt);
		if(result)
			token = get_next_token();		
		stmt->cmd_type = parse_command(token);
		if(stmt->cmd_type == UNKNOWN_CMD)
		{
			add_error(line_number,UNKNOWN_COMMAND);
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
Boolean validate_label(char *token, CompilerNode *stmt)
{
	int length_without_colon = strlen(token)-1;
	if(token[length_without_colon] == ':')
	{
		if(token[0] < 'A' || token[0] > 'z')
		{
			add_error(line_number, INVALID_LABEL);
			return FALSE;
		}
		strncpy(stmt->label, token, length_without_colon);			
		return TRUE;
	}
	return FALSE;
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
	//TODO: remove
	if(temp)
		debug_output(temp);
	return temp;
}

void debug_output(char *what)
{
	puts(what);
}

int get_IC()
{
	return ic;
}
int get_DC()
{
	return dc;
}

/* Set the addressing method and the register value according to the operand */
void set_addressing_and_register(char *operand,AddressingMethod *addressing ,int *reg)
{
	if(operand)
	{
		*addressing = get_addressing_for(operand);
		*reg = *addressing == REGISTER? atoi(&(operand[1])):0;
	}
}

void add_second_word(Cmd cmd_type, AddressingMethod source_addressing,char *source_operand, char *binary_machine_code, int ic)
{
	CompilerNode second_word;
	second_word.address = ic;
	if(cmd_type == MOV || 
	   cmd_type == CMP || 
	   cmd_type == ADD || 
	   cmd_type == SUB ||
	   cmd_type == LEA)
		{
			switch(source_addressing)
			{
				case IMMEDIATE:
					dec2bin(atoi(&source_operand[1]),binary_machine_code,8);
					break;
				case DIRECT:
					strcpy(binary_machine_code,source_operand);
					break;
				case INDEX:
					set_symbol(source_operand,binary_machine_code);
					break;
				case DOUBLE_INDEX:
					break;
			}
			add_compiler_node(&second_word);
		}
}

void add_third_word(Cmd cmd_type,AddressingMethod targetAddressing,char *target_operand)
{
	CompilerNode third_word;
	if(cmd_type != RTS && cmd_type != STOP)
		{
			switch(targetAddressing)
			{
				case IMMEDIATE:
					dec2bin(atoi(&target_operand[1]),third_word.binary_machine_code,8);
					break;
				case DIRECT:
					strcpy(third_word.binary_machine_code,target_operand);
					break;
				case INDEX:
					third_word.address = ++ic;
					set_index(target_operand,third_word.binary_machine_code);
					add_compiler_node(&third_word);
					break;
				case DOUBLE_INDEX:
					break;
			}
		}
}