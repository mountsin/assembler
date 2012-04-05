#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "first_scan.h"
#include "symbols.h"
#include "error.h"
#include "global_functions.h"
#include "pre_compiled.h"
#include "data_structs.h"
#include "global_constants.h"

#define LINE_SIZE 100

#define ASM_COMMAND_BITS	 4
#define ASM_ADDRESSING_BITS	 3
#define ASM_REGISTER_BITS	 3

Boolean is_valid_label(char *token, CompilerNode *stmt);
void parst_and_set_command_type(char* command_name, CompilerNodePtr node);
void process_statement(CompilerNode stmt);
void parse_and_load_data(CompilerNode *stmt, int *dc);
AddressingMethod get_addressing_for(char *source_operand);
void read_line_and_set_compiler_node(char *, CompilerNode *);
void extract_symbol(char *str,char *result[]);
void extract_index(char *str,char *result[]);
void set_addressing_and_register(char *operand,AddressingMethod *addressing ,int *reg);
void add_operand_nodes(Cmd cmd_type, AddressingMethod source_addressing,char *source_operand);
Boolean is_register(char *str);
Boolean is_literal(char *str);
Boolean is_index(char *str);
Boolean is_double_index(char *str);
Boolean is_comment(char* line);
void set_binary_code(CompilerNodePtr stmt);

/* Commands_list - table(array) contains each assembly command and its rules */
CommandStruct commands_list[] = 
{
/*	cmd_type	name		how many operands		source_addressing_options	dest_addressing_options */
	MOV,		"mov", 		TWO_OPERANDS,			{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	CMP,		"cmp", 		TWO_OPERANDS,			{0,1,2,3,4},				 {0,1,2,3,4},
	ADD,		"add", 		TWO_OPERANDS,			{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	SUB,		"sub", 		TWO_OPERANDS,			{0,1,2,3,4},				 {1,2,3,4,EMPTY},
	NOT,		"not", 		NO_OPERANDS,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	CLR,		"clr", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	LEA,		"lea", 		TWO_OPERANDS,			{1,2,3,EMPTY,EMPTY},		 {1,2,3,4,EMPTY},
	INC,		"inc", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	DEC,		"dec", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	JMP,		"jmp", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	BNE,		"bne", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	RED,		"red", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,2,3,4,EMPTY},
	PRN,		"prn", 		ONE_OPERAND,			EMPTY_ARRAY,				 {0,1,2,3,4},
	JSR,		"jsr", 		ONE_OPERAND,			EMPTY_ARRAY,				 {1,EMPTY,EMPTY,EMPTY,EMPTY},
	RTS,		"rts", 		NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,
	STOP,		"stop",		NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,
	DATA,		".data",	NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,
	STRING,		".string",	NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,
	ENTRY,		".entry",	NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,	
	EXTERN,		".extern",	NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY,	
	UNKNOWN_CMD,"\0"	  ,	NO_OPERANDS,			EMPTY_ARRAY,				 EMPTY_ARRAY
};

int ic = 0;								/* Instructions counter */
int dc = 0;								/* Data counter */
int line_number = 0;					/* Line number in the assembly file for the errors report */

/* Read the assembly file, line by line and process the statements */
void first_scan(char *filename)
{
	FILE *fp;
	char line[LINE_SIZE];
	CompilerNodePtr stmt;														/* Each code line will be parsed and stored in this struct */
	CommandStruct command_struct_from_validation_list; //TODO: check if needed
	Boolean label_exist = FALSE;

	fp = fopen(filename,"r");
	if(fp)
	{
		while(fgets(line,LINE_SIZE,fp))
		{
			stmt = create_compiler_node();
			stmt->line_number = ++line_number;
			read_line_and_set_compiler_node(line, stmt);
			if(stmt->cmd_type == COMMENT || stmt->cmd_type == UNKNOWN_CMD)
				continue;

			if(stmt->cmd_type == DATA || stmt->cmd_type == STRING)			/* Check if it's .data or .string instruction */
			{
				if(stmt->label) 
					add_data_symbol(stmt->label, dc,line_number);
				parse_and_load_data(stmt, &dc);
				continue;
			}

			if(stmt->cmd_type == EXTERN)									/* Check if it's .extern instruction */
			{
				add_external_symbol(stmt->target_operand, UNDEFINED_ADDRESS, line_number); 
				continue;
			}
			if(stmt->cmd_type == ENTRY)										/* Check if it's .entry instruction */
			{
				add_entries_symbol(stmt->target_operand, UNDEFINED_ADDRESS); 
				continue;
			}

			if(strlen(stmt->label) > 0)
				add_code_symbol(stmt->label, ic, line_number);

			set_addressing_and_register(stmt->source_operand, &stmt->sourceAddressing, &stmt->source_register);
			set_addressing_and_register(stmt->target_operand, &stmt->targetAddressing, &stmt->target_register);

			build_binary_machine_code(stmt);
			ic++;
			add_compiler_node(stmt);

			add_operand_nodes(stmt->cmd_type,stmt->sourceAddressing,stmt->source_operand);
			add_operand_nodes(stmt->cmd_type,stmt->targetAddressing,stmt->target_operand);
		}
		fclose(fp);
	}
	else
	{
		add_error(line_number,INPUT_FILE_FAILURE);
	}
}

void set_binary_code(CompilerNodePtr stmt)
{
	char temp[5];
	dec2bin(stmt->cmd_type,temp,4);
	temp[4]= NULL;
	strcat(stmt->binary_machine_code,temp);
	dec2bin(stmt->sourceAddressing,temp,3);
	temp[3] = NULL;
	strcat(stmt->binary_machine_code,temp);
	dec2bin(stmt->source_register,temp,3);
	temp[3] = NULL;
	strcat(stmt->binary_machine_code,temp);
	dec2bin(stmt->targetAddressing ,temp,3);
	temp[3] = NULL;
	strcat(stmt->binary_machine_code,temp);
	dec2bin(stmt->target_register,temp,3);
	temp[3] = NULL;
	strcat(stmt->binary_machine_code,temp);
}

/* This function extracts the symbl out of the operand in case of index or double index adressing method */
void extract_symbol(char *str,char *result)
{
	char *start_of_symbol;
	int length;
	start_of_symbol = strchr(str,']')+1;
	
	if(strlen(start_of_symbol) > 1)
	{
		length = strchr(start_of_symbol,'[')-start_of_symbol;
		strncpy(result,start_of_symbol,length);
	}
	else
	{
		length = strchr(str,'[')-str;
		strncpy(result,str,length);
	}
	result[length] = NULL;
}

/* This function extracts the index out of the operand in case of index or double index adressing method */
void extract_index(char *str,char *result, SecondScanType *scan_type)
{
	char *start_of_index = strchr(str,'[')+1;
	char *end_of_index = strchr(str,']');
	int length = end_of_index - start_of_index;
	if('%' == start_of_index[0])
	{
		start_of_index++;
		length--;
		*scan_type = LABEL_OFFSET;
	}
	else
	{
		*scan_type = LABEL;
	}
	strncpy(result,start_of_index,length);
	result[length] = NULL;
}

/* This function get the correct adressing method of the operand */
enum addressing_method get_addressing_for(char *operand)
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

Boolean build_binary_machine_code(CompilerNodePtr cn_ptr)
{
	/*machin code word structure:*/
	/* command(4) + source addressing(3) + source registry(3) + target addressing(3) + target registry(3) */

	char *command =						(char *)malloc(sizeof(char)*(ASM_COMMAND_BITS+1));		/*command code - 4 bits*/
	char *source_addressing_method =	(char *)malloc(sizeof(char)*(ASM_ADDRESSING_BITS+1));	/*source operand addressing method code  - 3 bits*/
	char *source_register =				(char *)malloc(sizeof(char)*(ASM_REGISTER_BITS+1));		/*source operand register  - 3 bits*/
	char *target_addressing_method =	(char *)malloc(sizeof(char)*(ASM_ADDRESSING_BITS+1));	/*target operand addressing method code  - 3 bits*/
	char *target_register =				(char *)malloc(sizeof(char)*(ASM_REGISTER_BITS+1));		/*source operand register  - 3 bits*/
	
	/*TODO: check that cmd_type<=15 if not - add error and return FALSE*/
	dec2bin(cn_ptr->cmd_type,			command,					ASM_COMMAND_BITS);
	dec2bin(cn_ptr->sourceAddressing,	source_addressing_method,	ASM_ADDRESSING_BITS);
	dec2bin(cn_ptr->source_register,	source_register,			ASM_REGISTER_BITS);
	dec2bin(cn_ptr->targetAddressing,	target_addressing_method,	ASM_ADDRESSING_BITS);
	dec2bin(cn_ptr->target_register,	target_register,			ASM_REGISTER_BITS);
	
	strcat(cn_ptr->binary_machine_code, command);					/* add comman bits*/
	strcat(cn_ptr->binary_machine_code, source_addressing_method);	/* add source addressing method bits*/
	strcat(cn_ptr->binary_machine_code, source_register);			/* add source register bits*/
	strcat(cn_ptr->binary_machine_code, target_addressing_method);	/* add target addressing method bits*/
	strcat(cn_ptr->binary_machine_code, target_register);			/* add target register register bits*/
	
	//TODO: use dipose instead
	free(command);
	free(source_addressing_method);
	free(source_register);
	free(target_addressing_method);
	free(target_register);
	return TRUE;
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
void read_line_and_set_compiler_node(char *line, CompilerNodePtr stmt)
{
	char *token;

#if DUBUG
	debug_output(line);	
#endif

	if(is_comment(line))
	{
#if DEBUG
		debug_output("DEBUG: Comment line ignored");
#endif

		stmt->cmd_type = COMMENT;
		return;
	}

	token =  get_first_token(line);
	if(token)
	{
		if(is_valid_label(token, stmt))
			token = get_next_token();		

		parst_and_set_command_type(token,stmt);

		if(stmt->cmd_type == UNKNOWN_CMD)
		{
			add_error(line_number,UNKNOWN_COMMAND);
		}
		stmt->source_operand = get_next_token();
		stmt->target_operand = get_next_token();

		/* swap operands if this command type has only one operand */
		if(stmt->target_operand == NULL)
		{
			stmt->target_operand = stmt->source_operand;
			stmt->source_operand = NULL;
		}
	}
	//set_binary_code(stmt); 
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
		stmt->label[length_without_colon] = NULL;
		return TRUE;
	}
	return FALSE;
}


/* This function accepts the command token from the assembly code line and return the correct Cmd enum value of it */
void parst_and_set_command_type(char* command_name, CompilerNodePtr node)
{
	CommandStruct* tmp;
	for(tmp = commands_list; tmp->name && strcmp(command_name, tmp->name); tmp++);
	if(tmp->cmd_type == UNKNOWN_CMD)
		add_error(line_number, UNKNOWN_COMMAND);
	node->cmd_type = tmp->cmd_type;
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
void set_addressing_and_register(char *operand, enum addressing_method *addressing ,int *reg)
{
	if(operand)
	{
		*addressing = get_addressing_for(operand);
		*reg = *addressing == REGISTER? atoi(&(operand[1])):0;
	}
}

/* This function is used for adding nodes of the operand if neccessery */
void add_operand_nodes(Cmd cmd_type, AddressingMethod addressing,char *operand)
{
	CompilerNodePtr node1 = create_compiler_node();
	CompilerNodePtr node2 = create_compiler_node();

	node1->line_number = line_number;
	node2->line_number = line_number;

	switch(addressing)
	{
		case UNKNOWN:
		case REGISTER:
			break;
		case IMMEDIATE:
			dec2bin(atoi(&operand[1]),node1->binary_machine_code,8);	/* The second operand is a number so I convert it's valu to binary code */
			break;
		case DIRECT:
			strcpy(node1->binary_machine_code,operand);					/* The operand is a symbol that should be translated to it's addresss value in the second scan phase */
			node1->second_scan_type = LABEL;							/* I save the symble name as is in the label field and tell the second scan that it needs to translate it to the address value of the symbol */
			node1->address = ++ic;
			add_compiler_node(node1);
			break;
		default:
			extract_symbol(operand,node1->label);
			extract_index(operand,node2->label,&node2->second_scan_type);
			node1->address = ++ic;
			node2->address = ++ic;
			add_compiler_node(node1);
			add_compiler_node(node2);
			break;
	}
}

/* This function check if the code line is a comment */
Boolean is_comment(char* line)
{
	if(line[0] == ';')
		return TRUE;
	return FALSE;
}