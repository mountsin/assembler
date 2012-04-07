/* Status = define current command status (can be extended) */
typedef enum error_type 
{
	INPUT_FILE_FAILURE,					/* One of the input file could not been opened */
	UNKNOWN_COMMAND,					/* Assembly command is not recognized */
	LABEL_ALREADY_USED,					/* Trying to set a value to the same label already defined */
	TOO_MUCH_PARAMS,					/* Number of parameters exceeding assembly command rules */
	MISSING_PARAMS,						/* Number of parameters is not enough for assembly command rules */
	LABEL_NOT_DEFINED,					/* Trying to use a label which not defined in the file */
	INVALID_LABEL,						/* The label name is not followning naming rules */
	ILLEGAL_DATA_ADDRESS,				/* The label is pointing to Illegal address */
	SYMBOL_ALREADY_EXISTS,				/* The symbol is definded more than once */
	MULTIPLE_ENTRYLABEL_USE,			/* entry row should have a uniqe label and cannot be used more then once*/
	INVALID_DATA,						/* The data argumnet is not a valid number */
	INVALID_STRING,						/* The string argument is not a valid string */
	INVALID_SOURCE_OPERNAD,				/* The first operand is illegal for this command type */
	INVALID_TARGET_OPERNAD,				/* The second operand is illegal for this command type */
	ENTRY_LABEL_UNDEFINED,				/* label should be defined as entry*/
	ENTRY_LABEL_NOT_SET,				/* label defined but not been used in code*/
	UNKNOWN_ERROR = 99					/* Other error */
} ErrorType;

/**
* ErrorCollector (linked list of structs) of all errors
*/
typedef struct error
{
	struct error *next;  
	ErrorType type;					/* type of error */
	int line_number;				/* assembly (input) file line number */
} Error;

void add_error(int line_number, enum error_type error);
Error *get_errors_list();