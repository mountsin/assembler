#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "pre_compiled.h"
#include "error.h"

#define OBJECT_FILE_EXT		".ob"
#define ENTRIES_FILE_EXT	".ent"
#define EXTERNAL_FILE_EXT	".ext"

#define GENERAL_ERROR_MESSAGE		"Error occurred while trying to compile the assembly file:"
#define INPUT_FILE_OPEN_FAIL		"One of the input file could not been opened"
#define UNKNOWN_ASSEMBLY_COMMAND	"Assembly command is not recognized"
#define LABEL_DOUBLE_DEFINITION		"Trying to set a value to the same label already defined"
#define TOO_MUCH_PARAMETERS			"Number of parameters exceeding assembly command rules"
#define PARAMETERS_MISSING			"Number of parameters is not enough for assembly command rules"
#define LABEL_UNDEFINED				"Trying to use a label which not defined in the file"
#define UNKNOWN_ERROR_MESSAGE		"Unknown error occurred"

void print_errors_report(Error *errors_collector);
void create_file_ob(char *filename, CompilerNode *cn_list);
void create_file_ent(char *filename, CompilerNode *cn_list);
void create_file_ext(char *filename, CompilerNode *cn_list);

void compile_and_write_output(char *filename)
{
	CompilerNode *h = get_compiler_nodes_list();

	Error *errors_collector = get_errors_list();

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

void create_file_ob(char *filename, CompilerNode *cn_list)
{
	FILE *fp;
}

void create_file_ent(char *filename, CompilerNode *cn_list)
{
	FILE *fp;
}

void create_file_ext(char *filename, CompilerNode *cn_list)
{
	FILE *fp;
}



