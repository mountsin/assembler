/* Status = define current command status (can be extended) */
enum error_type 
{
	INPUT_FILE_FAILURE,					/* One of the input file could not been opened */
	UNKNOWN_COMMAND,					/* Assembly command is not recognized */
	LABEL_ALREADY_USED,					/* Trying to set a value to the same label already defined */
	TOO_MUCH_PARAMS,					/* Number of parameters exceeding assembly command rules */
	MISSING_PARAMS,						/* Number of parameters is not enough for assembly command rules */
	LABEL_NOT_DEFINED,					/* Trying to use a label which not defined in the file */
	INVALID_LABEL,						/* The label name is not followning naming rules */
	ILLEGAL_DATA_ADDRESS,				/* The label is pointing to Illegal address */
	UNKNOWN_ERROR = 99					/* Other error */
};

/**
* ErrorCollector (linked list of structs) of all errors
*/
typedef struct error
{
	struct error *next;  
	enum error_type type;			/* type of error */
	int line_number;				/* assembly (input) file line number */
}	
Error;

void add_error(int line_number, enum error_type error);
Error *get_errors_list();