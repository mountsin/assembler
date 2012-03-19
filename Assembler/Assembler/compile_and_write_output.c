#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "global_functions.h"
#include "compile_and_write_output.h"
#include "symbols.h"
#include "error.h"
#include "pre_compiled.h"


#define ONEBYTE_BITSNUM		8
#define TWOBYTES_BITSNUM	16
#define BYTE_MAX_NUM		255
#define HALFBYTE_MAX_NUM	15

#define ABS_STRING		"a"
#define REL_STRING		"r"
#define EXT_STRING		"e"
#define EMPTY_STRING	""

#define OBJECT_FILE_EXT		".ob"
#define ENTRIES_FILE_EXT	".ent"
#define EXTERNAL_FILE_EXT	".ext"

/* error collector messages*/
#define GENERAL_ERROR_MESSAGE		"Error occurred while trying to compile the assembly file:"
#define INPUT_FILE_OPEN_FAIL		"One of the input file could not been opened"
#define UNKNOWN_ASSEMBLY_COMMAND	"Assembly command is not recognized"
#define LABEL_DOUBLE_DEFINITION		"Trying to set a value to the same label already defined"
#define TOO_MUCH_PARAMETERS			"Number of parameters exceeding assembly command rules"
#define PARAMETERS_MISSING			"Number of parameters is not enough for assembly command rules"
#define LABEL_UNDEFINED				"Trying to use a label which not defined in the file"
#define UNKNOWN_ERROR_MESSAGE		"Unknown error occurred"

/*file stream error messages*/
#define OK_MSG						"Compilation successful."
#define CREATE_FILE_ERR__MSG		"Error occurred while trying to create file"
#define CLOSE_FILE_ERR__MSG			"Error occurred while trying to finalize file"


#define OBHEADER_ADDRESS		"Base 2 adress"
#define OBHEADER_MACHINE_CODE	"Base 2 Machine code"
#define OBHEADER_LINKER_FLAG	"linker flag (a/r/e)"
#define OBJECT_ROW_FORMAT		"%24s%24s%20s\n"

void print_errors_report(Error *errors_collector);
int create_file_ob(char *filename, CompilerNode *cn_list);
int create_file_ent(char *filename, CompilerNode *cn_list);
int create_file_ext(char *filename, CompilerNode *cn_list);
void get_linker_flag_str(enum linker_enum linker, char *result_string);

void compile_and_write_output(char *filename)
{
	CompilerNode *h = get_compiler_nodes_list();
	Error *errors_collector = get_errors_list();
	
	//TODO: functions 2 get ext_symbols, ent_symbols
	//Symbol *entries_symbols_list = NULL;
	//Symbol *external_symbols_list = NULL;


	if(errors_collector != NULL)
		print_errors_report(errors_collector);
	else
	{
		create_file_ob(filename, h);

		create_file_ent(filename, h);
		
		create_file_ext(filename, h);
	}
}

void print_errors_report(Error *errors_collector)
{
	char *current_err_msg;

	printf("%s\n",GENERAL_ERROR_MESSAGE);
	
	while(errors_collector)
	{
		switch(errors_collector->type)
		{
			case INPUT_FILE_FAILURE: 	current_err_msg = INPUT_FILE_OPEN_FAIL;break;		/* One of the input file could not been opened */
			case UNKNOWN_COMMAND:		current_err_msg = UNKNOWN_ASSEMBLY_COMMAND;break;	/* Assembly command is not recognized */
			case LABEL_ALREADY_USED:	current_err_msg = LABEL_DOUBLE_DEFINITION;break;	/* Trying to set a value to the same label already defined */
			case TOO_MUCH_PARAMS:		current_err_msg = TOO_MUCH_PARAMETERS;break;		/* Number of parameters exceeding assembly command rules */
			case MISSING_PARAMS:		current_err_msg = PARAMETERS_MISSING;break;			/* Number of parameters is not enough for assembly command rules */
			case LABEL_NOT_DEFINED:		current_err_msg = LABEL_UNDEFINED;break;			/* Trying to use a label which not defined in the file */
			case UNKNOWN_ERROR:			current_err_msg = UNKNOWN_ERROR_MESSAGE;break;		/*Unknown error occurred */
		}

		printf("line: %s : %s\n", errors_collector->line_number, current_err_msg);
		
		errors_collector = errors_collector->next;
	}
}

int create_file_ob(char *filename, CompilerNode *cn_list)
{	
	int IC,DC;
	int how_many_bits_to_use;

	char *code_length_in_binary;
	char *data_length_in_binary;
	char *address_binary_string;
	char *linker_flag_string;
	
	FILE *fp;


	char *filefullname = filename;
	filefullname = strcat(filefullname, OBJECT_FILE_EXT);	
	filefullname[strlen(filefullname)] = '\0';

	fp = fopen(filename, "w");
	if(!fp)
		return CREATE_FILE_ERR; /* error occured while trying to creat new file*/


	/* write 1th row - columns headers*/
	fprintf(fp, OBJECT_ROW_FORMAT, OBHEADER_ADDRESS, OBHEADER_MACHINE_CODE, OBHEADER_LINKER_FLAG);
	
	IC = get_instruction_counter();
	DC =  get_data_counter();


	how_many_bits_to_use = (IC <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM;	/*how many chars will the string need*/
	dec2bin(IC, code_length_in_binary, how_many_bits_to_use);							/*translate decimal to binary string*/

	how_many_bits_to_use = (DC <= HALFBYTE_MAX_NUM) ?  (ONEBYTE_BITSNUM/2) : ONEBYTE_BITSNUM;	/*how many chars will the string need*/
	dec2bin(DC, data_length_in_binary, how_many_bits_to_use);									/*translate decimal to binary string*/
	
	/* write 2th row - data and instructions size in binary */
	fprintf(fp, "%24s%s %s\n", "", code_length_in_binary, data_length_in_binary);
	
	while(cn_list)
	{
		how_many_bits_to_use = (cn_list->address <= BYTE_MAX_NUM) ? ONEBYTE_BITSNUM : TWOBYTES_BITSNUM; /*set how many chars will the string need*/
		dec2bin(cn_list->address, address_binary_string, how_many_bits_to_use);							/*translate decimal address to binary string*/

		get_linker_flag_str(cn_list->linker_flag, linker_flag_string);	/* get */

		fprintf(fp, OBJECT_ROW_FORMAT, address_binary_string, cn_list->binary_machine_code, linker_flag_string);
		cn_list = cn_list->next;
	}

	if(fclose(fp) != OK)
			return	CLOSE_FILE_ERR; //Error occured while trying to close the file Stream
	else
			return OK; /* everthing is OK*/ 

}

int create_file_ent(char *filename, Symbol *entries_symbols_list)
{
	FILE *fp;

	char *filefullname = filename;
	filefullname = strcat(filefullname, ENTRIES_FILE_EXT);	
	filefullname[strlen(filefullname)] = '\0';

	fp = fopen(filefullname, "w");
	if(!fp)
		return -1; /* error occured while trying to creat new file*/
	
	while(entries_symbols_list)
	{
		entries_symbols_list = entries_symbols_list->next;
	}
}

int create_file_ext(char *filename, Symbol *external_symbols_list)
{
	FILE *fp;

	char *filefullname = filename;
	filefullname = strcat(filefullname, EXTERNAL_FILE_EXT);	
	filefullname[strlen(filefullname)] = '\0';

	fp = fopen(filefullname, "w");
	if(!fp)
		return -1; /* error occured while trying to creat new file*/
	
	while(external_symbols_list)
	{
		external_symbols_list = external_symbols_list->next;
	}
}

void get_linker_flag_str(enum linker_enum linker, char *result_string)
{
	switch(linker)
	{
		case ABSOLUTE:
				{result_string = ABS_STRING; break;}
		case RELOCATABLE:
				{result_string = REL_STRING; break;}
		case EXTERNAL:
				{result_string = EXT_STRING; break;}
		case NONE:
				{result_string = EMPTY_STRING; break;}
	}
}



