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

Boolean is_valid_label(char *token, CompilerNode *stmt);
Cmd parse_command(char *command_name);
void process_statement(CompilerNode stmt);
void parse_and_load_data(CompilerNode *stmt, int *dc);
AddressingMethod get_addressing_for(char *source_operand);
void read_line_and_build_statement_struct(char *, CompilerNode *);
void extract_symbol(char *str,char *result[]);
void extract_index(char *str,char *result[]);
void set_addressing_and_register(char *operand,AddressingMethod *addressing ,int *reg);
void add_second_word(Cmd cmd_type, AddressingMethod source_addressing,char *source_operand, int ic);
void add_third_word(Cmd cmd_type,AddressingMethod targetAddressing,char *target_operand, int ic);
Boolean is_register(char *str);
Boolean is_literal(char *str);
Boolean is_index(char *str);
Boolean is_double_index(char *str);
Boolean is_comment(char* line);

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
	CompilerNode stmt;														/* Each code line will be parsed and stored in this temporary struct */
	CommandStruct command_struct_from_validation_list;
	Boolean label_exist = FALSE;

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

			if(stmt.cmd_type == DATA || stmt.cmd_type == STRING)			/* Check if it's .data or .string instruction */
			{
				if(stmt.label) 
					add_data_symbol(stmt.label, dc);
				parse_and_load_data(&stmt, &dc);
				continue;
			}

			if(stmt.cmd_type == EXTERN)										/* Check if it's .extern instruction */
			{
				add_external_symbol(stmt.target_operand,ic); 
				continue;
			}

			if(stmt.label)
				add_code_symbol(stmt.label,ic);

			set_addressing_and_register(stmt.source_operand,&stmt.sourceAddressing,&stmt.source_register);
			set_addressing_and_register(stmt.target_operand,&stmt.targetAddressing,&stmt.target_register);
			add_second_word(stmt.cmd_type,stmt.sourceAddressing,stmt.source_operand,++ic);
			add_third_word(stmt.cmd_type,stmt.targetAddressing,stmt.target_operand,++ic);

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

/* This function extracts the symbl out of the operand in case of index or double index adressing method */
void extract_symbol(char *str,char *result)
{
	char *start_of_symbol;
	start_of_symbol = strchr(str,']')+1;
	if(strlen(start_of_symbol) > 1)
		strncpy(result,start_of_symbol,strchr(start_of_symbol,'[')-start_of_symbol);
	else
		strncpy(result,str,strchr(str,'[')-str);
}

/* This function extracts the index out of the operand in case of index or double index adressing method */
void extract_index(char *str,char *result)
{
	char *start_of_index = strchr(str,'[')+1;
	char *end_of_index = strchr(str,']');
	strncpy(result,start_of_index,end_of_index - start_of_index);
}

/* This function get the correct adressing method of the operand */
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

/* This funcitons handale data instructions such as .data and .string and increment the data counter (dc) */
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

/* This function accepts an operand string and checks if it is a valid machine register (0 - 7) */
Boolean is_register(char *str)
{
	if (str[0] == 'r' && (str[1] >= '0' && str[1] <= '7') && str[2] == '\0')
		return TRUE;
	return FALSE;
}

/* This function accepts an operand string and checks if it is a literal value (a number) */
Boolean is_literal(char *str)
{
	if(str[0] == '#')
		return TRUE;
	return FALSE;
}

/* This function accepts an operand string and checks if it is an index */
Boolean is_index(char *str)
{
	if(strchr(str,'['))
		return TRUE;
	return FALSE;
}

/* This function accepts an operand string and checks if it is a double index */
Boolean is_double_index(char *str)
{
	if(str[0] == '[')
		return TRUE;
	return FALSE;
}

/* Accept assembly code line and populate the CompilerNode struct used by the second scan function */
void read_line_and_build_statement_struct(char *line, CompilerNode *stmt)
{
	char *token;
	debug_output(line);	

	if(is_comment(line))
	{
		stmt->cmd_type = COMMENT;
		debug_output("DEBUG: Comment line ignored");
		return;
	}

	token =  get_first_token(line);
	if(token)
	{
		if(is_valid_label(token, stmt))
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
	else
		add_error(line_number,UNKNOWN_COMMAND);
}

/* Accepts a token and checks if it is a valid label. If so, copy the token to the lable field of the CompilerNode struct and advance to the next token */
Boolean is_valid_label(char *token, CompilerNode *stmt)
{
	int length_without_colon = strlen(token)-1;
	if(length_without_colon > 29)						/* label length is less or equal to 30 */
		return FALSE;
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


/* This function accepts the command token from the assembly code line and return the correct Cmd enum value of it */
Cmd parse_command(char *command_name)
{
	CommandStruct *tmp;
	for(tmp = commands_list; tmp->name && strcmp(command_name, tmp->name); tmp++);
	return tmp->cmd_type;
}





/* public function used be the second scan function to get the instrctions counter */
int get_IC()
{
	return ic;
}

/* public function used be the second scan function to get the data counter */
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

/* This function is used for adding the second word of the assembly command if a second word is neccessery */
void add_second_word(Cmd cmd_type, AddressingMethod source_addressing,char *source_operand, int ic)
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
					dec2bin(atoi(&source_operand[1]),second_word.binary_machine_code,8);
					break;
				case DIRECT:
					strcpy(second_word.binary_machine_code,source_operand);
					second_word.is_second_scan_needed = TRUE;
					break;
				case INDEX:
					extract_symbol(source_operand,second_word.binary_machine_code);
					second_word.is_second_scan_needed = TRUE;
					break;
				case DOUBLE_INDEX:
					extract_symbol(source_operand,second_word.binary_machine_code);
					second_word.is_second_scan_needed = TRUE;
					break;
			}
			add_compiler_node(&second_word);
		}
}

/* This function is used for adding the third word of the assembly command if a third word is neccessery */
void add_third_word(Cmd cmd_type,AddressingMethod targetAddressing,char *target_operand,int ic)
{
	CompilerNode third_word;
	third_word.address = ic;
	if(cmd_type != RTS && cmd_type != STOP)
		{
			switch(targetAddressing)
			{
				case IMMEDIATE:
					dec2bin(atoi(&target_operand[1]),third_word.binary_machine_code,8); /* The second operand is a number so I convert it's valu to binary code */
					break;
				case DIRECT:
					strcpy(third_word.binary_machine_code,target_operand);	/* The second operand is a symbol that should be translated to it's addresss value in the second scan phase */
					third_word.is_second_scan_needed = TRUE;				/* I save the symble name as is in the binary machine code field and tell the second scan that it needs to translate it to the address value of the symbol */
					break;
				case INDEX:
					third_word.address = ++ic;
					extract_index(target_operand,third_word.binary_machine_code);
					add_compiler_node(&third_word);
					third_word.is_second_scan_needed = TRUE;
					break;
				case DOUBLE_INDEX:
					extract_index(target_operand,third_word.binary_machine_code);
					third_word.is_second_scan_needed = TRUE;
					break;
			}
		}
}

/* This function check if the code line is a comment */
Boolean is_comment(char* line)
{
	if(line[0] == ';')
		return TRUE;
	return FALSE;
}