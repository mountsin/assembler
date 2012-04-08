#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_functions.h"

#include "compile_and_write_output.h"
#include "error.h"


/*bit usage*/
#define ONEBYTE_BITSNUM		8
#define TWOBYTES_BITSNUM	16
#define BYTE_MAX_NUM		255
#define HALFBYTE_MAX_NUM	15

/*binary string usage*/
#define MAX_BINARY_STR 33
#define MAX_LINKER_STR 33

/*linker flag*/
#define ABS_STRING		"a"
#define REL_STRING		"r"
#define EXT_STRING		"e"
#define EMPTY_STRING	""

/*files extensions*/
#define OBJECT_FILE_EXT		".ob"
#define ENTRIES_FILE_EXT	".ent"
#define EXTERNAL_FILE_EXT	".ext"
#define INPUT_FILE_EXT		".as"

/* error collector messages*/
#define GENERAL_ERROR_MESSAGE		"One or more errors occurred while trying to compile the assembly file"
#define INPUT_FILE_OPEN_FAIL		"One of the input file could not been opened"
#define UNKNOWN_ASSEMBLY_COMMAND	"Assembly command is not recognized"
#define LABEL_DOUBLE_DEFINITION		"Trying to set a value to the same label already defined"
#define TOO_MUCH_PARAMETERS			"Number of parameters exceeding assembly command rules"
#define PARAMETERS_MISSING			"Number of parameters is not enough for assembly command rules"
#define LABEL_UNDEFINED				"Trying to use a label which not defined in the file"
#define INVALID_LABEL_USED			"The label name is not following naming rules"
#define ILLEGAL_ADDRESS				"The label is pointing to Illegal address"
#define	MULTIPLE_ENTRYLABEL_USE_MSG	"entry row should have a unique label and cannot be used more then once"
#define SYMBOL_ALREADY_EXISTS_MSG	"The symbol is defined more than once"
#define ENTRY_LABEL_NOT_SET_MSG		"label defined but not been used in code"
#define	INVALID_SOURCE_OPERNAD_MSG	"The first operand is illegal for this command type"
#define	INVALID_TARGET_OPERNAD_MSG	"The second operand is illegal for this command type"
#define	INVALID_SECOND_INDEX_MSG	"The second index in a double indexed addressing method must be a register"
#define	INVALID_DATA_MSG			"The data argument is not a valid number"
#define	INVALID_STRING_MSG			"The string argument is not a valid string"
#define UNKNOWN_ERROR_MESSAGE		"Unknown error occurred"

/*file stream error messages*/
#define OK_MSG						"Compilation successful."
#define CREATE_FILE_ERR__MSG		"One or more errors occurred while trying to create file"
#define CLOSE_FILE_ERR__MSG			"Error occurred while trying to finalize file"

/*printf formats*/
#define OBJECT_ROW_FORMAT		"%30s%30s%25s\n"
#define EXT_ENT_ROW_FORMAT		"%32s \t %32s\n"

/*functions return value*/
#define ERROR -1

/**

*/
void print_errors_report(Error *errors_collector, char *filename);

int create_file_ob(char *filename, CompilerNode *cn_list);
int create_file_ent(char *filename, Symbol *entries_list);
int create_file_ext(char *filename, Symbol *entries_symbols_list);
void get_linker_flag_str(enum linker_enum linker, char *external_symbols_list);

int compile_and_write_output(char *filename)
{
	int filesCreated = 1;
	Error *errors_collector = get_errors_list();

	if(errors_collector != NULL)
	{
		print_errors_report(errors_collector, filename);
		return ERROR;
	}
	else
	{
		create_file_ob(filename, get_code_list_head());

		if(get_entries_symbols_list() != NULL) /*create entries file if needed*/
		{
			create_file_ent(filename, get_entries_symbols_list());
			filesCreated++;
		}

		if(get_externalFile_head() != NULL) /*create externals file if needed*/
		{	
			create_file_ext(filename, get_externalFile_head());
			filesCreated++;
		}

		return filesCreated;
	}
}

void print_errors_report(Error *errors_collector, char *filename)
{
	char *current_err_msg;

	printf("%s %s%s :\n",GENERAL_ERROR_MESSAGE, filename,INPUT_FILE_EXT);
	
	while(errors_collector)
	{
		switch(errors_collector->type)
		{
			case INPUT_FILE_FAILURE: 		current_err_msg = INPUT_FILE_OPEN_FAIL;break;		 /* One of the input file could not been opened */
			case UNKNOWN_COMMAND:			current_err_msg = UNKNOWN_ASSEMBLY_COMMAND;break;	 /* Assembly command is not recognized */
			case LABEL_ALREADY_USED:		current_err_msg = LABEL_DOUBLE_DEFINITION;break;	 /* Trying to set a value to the same label already defined */
			case TOO_MUCH_PARAMS:			current_err_msg = TOO_MUCH_PARAMETERS;break;		 /* Number of parameters exceeding assembly command rules */
			case MISSING_PARAMS:			current_err_msg = PARAMETERS_MISSING;break;			 /* Number of parameters is not enough for assembly command rules */
			case LABEL_NOT_DEFINED:			current_err_msg = LABEL_UNDEFINED;break;			 /* Trying to use a label which not defined in the file */
			case INVALID_LABEL:				current_err_msg = INVALID_LABEL_USED;break;			 /* The label name is not followning naming rules */
			case ILLEGAL_DATA_ADDRESS:		current_err_msg = ILLEGAL_ADDRESS;break;			 /* The label is pointing to Illegal address */
			case SYMBOL_ALREADY_EXISTS:		current_err_msg = SYMBOL_ALREADY_EXISTS_MSG;break;	 /* The symbol is definded more than once */
			case MULTIPLE_ENTRYLABEL_USE:	current_err_msg = MULTIPLE_ENTRYLABEL_USE_MSG;break; /* entry row should have a uniqe label and cannot be used more then once*/
			case ENTRY_LABEL_NOT_SET:		current_err_msg = ENTRY_LABEL_NOT_SET_MSG;break;	 /*"label defined but not been used in code"*/
			case INVALID_SOURCE_OPERNAD:	current_err_msg = INVALID_SOURCE_OPERNAD_MSG;break;	 /* The first operand is illegal for this command type */
			case INVALID_TARGET_OPERNAD:	current_err_msg = INVALID_TARGET_OPERNAD_MSG;break;	 /* The second operand is illegal for this command type */
			case INVALID_SECOND_INDEX:		current_err_msg = INVALID_SECOND_INDEX_MSG;break;	 /* The second index in a double indexed addressing method must be a register */
			case INVALID_DATA:				current_err_msg = INVALID_DATA_MSG;break;			 /* The data argument is not a valid number */
			case INVALID_STRING:			current_err_msg = INVALID_STRING_MSG;break;			 /* The string argument is not a valid string */
			case UNKNOWN_ERROR:				current_err_msg = UNKNOWN_ERROR_MESSAGE;break;		 /*Unknown error occurred */
			default:						current_err_msg = "Unknown error occurred - error could not be recognized";
		}

		printf("line %d : %s\n", errors_collector->line_number, current_err_msg);
		
		errors_collector = errors_collector->next;
	}
}
/*
*
*/
int create_file_ob(char *filename, CompilerNode *cn_list)
{	
	int IC,DC;
	int how_many_bits_to_use;

	char *code_length_in_binary;
	char *data_length_in_binary;
	char address_binary_string[MAX_BINARY_STR];
	char linker_flag_string[MAX_LINKER_STR];

	FILE *fp;


	char *filefullname =  (char *)calloc(strlen(filename) + 1, sizeof(char)); /*allocate memory for filename */
	strcpy(filefullname,filename);
	
	filefullname = strcat(filefullname, OBJECT_FILE_EXT);	 /* add extension to filename */
	filefullname[strlen(filefullname)] = '\0';
		
	fp = fopen(filefullname, "w");
	if(!fp)
		return CREATE_FILE_ERR; /* error occurred while trying to create new file*/


	/*
	 * write 1th row - columns headers  - can be removed
	 * fprintf(fp, OBJECT_ROW_FORMAT, HEADER_ADDRESS, HEADER_MACHINE_CODE, HEADER_LINKER_FLAG);*/
	
	IC = get_instruction_counter();
	DC = get_data_counter();


	how_many_bits_to_use = (IC <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM;	/*how many chars will the string need*/
	code_length_in_binary = (char *)calloc(how_many_bits_to_use + 1, sizeof(char));
	dec2bin(IC, code_length_in_binary, how_many_bits_to_use);							/*translate decimal to binary string*/

	how_many_bits_to_use = (DC <= HALFBYTE_MAX_NUM) ?  (ONEBYTE_BITSNUM/2) : ONEBYTE_BITSNUM;	/*how many chars will the string need*/
	data_length_in_binary = (char *)calloc(how_many_bits_to_use + 1, sizeof(char));
	dec2bin(DC, data_length_in_binary, how_many_bits_to_use);									/*translate decimal to binary string*/
	
	/* write 2th row - data and instructions size in binary */
	fprintf(fp, "%45s%s %s\n", "", code_length_in_binary, data_length_in_binary);
	
	while(cn_list != NULL)
	{
		how_many_bits_to_use = (cn_list->address <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM; /*set how many chars will the string need*/
		dec2bin(cn_list->address, address_binary_string, how_many_bits_to_use);							/*translate decimal address to binary string*/

		get_linker_flag_str(cn_list->linker_flag, linker_flag_string);	/* get */

		fprintf(fp, OBJECT_ROW_FORMAT, address_binary_string, cn_list->binary_machine_code, linker_flag_string);
		cn_list = cn_list->next;  /* point to next node*/
	}

	/*free memory allocations*/
	free(code_length_in_binary);
	free(data_length_in_binary);


	if(fclose(fp) != OK)
			return	CLOSE_FILE_ERR; /*Error occured while trying to close the file Stream*/
	else
			return OK; /* everthing is OK*/ 

}

int create_file_ent(char *filename, Symbol *entries_symbols_list)
{
	int how_many_bits_to_use;
	char address_binary_string[MAX_BINARY_STR];

	FILE *fp;

	char *filefullname =  (char *)calloc(strlen(filename) + 1, sizeof(char)); /*allocate memory for filefullname */
	strcpy(filefullname,filename);
	
	filefullname = strcat(filefullname, ENTRIES_FILE_EXT);	 /*add extention to filename */
	filefullname[strlen(filefullname)] = '\0';


	fp = fopen(filefullname, "w");
	if(!fp)
		return CREATE_FILE_ERR; /* error occured while trying to creat new file*/
	
	while(entries_symbols_list != NULL)
	{	
		how_many_bits_to_use = (entries_symbols_list->address <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM; /*set how many chars will the string need*/
		dec2bin(entries_symbols_list->address, address_binary_string, how_many_bits_to_use);							/*translate decimal address to binary string*/

		fprintf(fp, EXT_ENT_ROW_FORMAT, entries_symbols_list->name, address_binary_string);
		
		entries_symbols_list = entries_symbols_list->next; /* point to next node*/
	}

	if(fclose(fp) != OK)
			return	CLOSE_FILE_ERR; /*Error occured while trying to close the file Stream*/
	else
			return OK; /* everthing is OK*/ 
}

int create_file_ext(char *filename, Symbol *external_symbols_list)
{
	int how_many_bits_to_use;
	char address_binary_string[MAX_BINARY_STR];

	FILE *fp;

	char *filefullname =  (char *)calloc(strlen(filename) + 1, sizeof(char)); /*allocate memory for filefullname */
	strcpy(filefullname,filename);
	
	filefullname = strcat(filefullname, EXTERNAL_FILE_EXT);	 /*add extention to filename */
	filefullname[strlen(filefullname)] = '\0';


	fp = fopen(filefullname, "w");
	if(!fp)
		return CREATE_FILE_ERR; /* error occured while trying to creat new file*/
	
	while(external_symbols_list != NULL)
	{	
		how_many_bits_to_use = (external_symbols_list->address <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM; /*set how many chars will the string need*/
		dec2bin(external_symbols_list->address, address_binary_string, how_many_bits_to_use);							/*translate decimal address to binary string*/

		fprintf(fp, EXT_ENT_ROW_FORMAT, external_symbols_list->name, address_binary_string);
		
		external_symbols_list = external_symbols_list->next; /* point to next node*/
	}

	if(fclose(fp) != OK)
			return	CLOSE_FILE_ERR; /*Error occured while trying to close the file Stream*/
	else
			return OK; /* everthing is OK*/ 
}

void get_linker_flag_str(enum linker_enum linker, char *result_string)
{
	switch(linker)
	{
		case ABSOLUTE:
				{
					strcpy(result_string,ABS_STRING);
					result_string[strlen(ABS_STRING)] = '\0'; /*set string end to null*/
					break;
				}
		case RELOCATABLE:
				{
					strcpy(result_string,REL_STRING);
					result_string[strlen(REL_STRING)] = '\0'; /*set string end to null*/
					break;
				}
		case EXTERNAL:
				{
					strcpy(result_string,EXT_STRING);
					result_string[strlen(EXT_STRING)] = '\0'; /*set string end to null*/
					break;
				}
		default:
				{
					strcpy(result_string,EMPTY_STRING);
					result_string[strlen(EMPTY_STRING)] = '\0'; /*set string end to null*/
					break;
				}
	}
}



