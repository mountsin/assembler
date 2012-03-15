
//===== enums ==================

//addressingMethod - addresing method (0-4)
enum addressingMethod
{	
	IMMEDIATE,
	DIRECT,
	INDEX,
	DOUBLE_INDEX,
	REGISTER,
};



//status = define current command status (can be extended)
enum status 
{
	OK,								//OK
	UNKNOWN_COMMAND,				//assembly command is not recognized
	LABEL_ALREADY_USED,				//trying to set a value to the same label already defined 
	TOO_MUCH_PARAMS,				//number of parameters exceeding assembly command rules
	MISSING_PARAMS,					//number of parameters is not enough for assembly command rules
	LABEL_NOT_DEFINED,				//trying to use a label which not defined in the file
	UNKNOWN_ERROR = 99				//other error
};

//paramType = define assembly command param type
enum paramTypeEnum 
{
	INT = 0,
	CHAR = 1, 
	STR = 2,
	ASSEMBLY_LABEL = 3
};


//=========== Complex Elements ==================================






//===================  Tables & Collectors ==========================



//Table (linked list of structs) of all assembly Labels
typedef struct labelNode
{
	struct labelNode *next;  
	enum cmd type;			//can get enum values 16 - 19 (data, string...)
	int decimalAddr;
}
LabelNode;

/**
* ErrorCollector (linked list of structs) of all errors
*/
typedef struct error
{
	struct error *next;  
	enum status errType;			//type of error
	enum cmd cmdType;				//type of command caused the error
	int lineNumber;					//assembly (input) file line number
}	
ErrorCollector;

/**
* compiler_Node (linked list of structs) table of nodes
* each represent a compiled machine code line
*/
typedef struct compiler_Node
{
	struct compiler_Node *next;  
	char *label;
	int address;
	enum cmd cmdType;							//type of command
	enum addressingMethod sourceAddressing;
	enum addressingMethod targetAddressing;
	char *source_operand;			//first parameter (nullable)
	char *target_operand;			//second parameter (nullable)
	char *binary_machine_code; //"101010110" "STR" "&L STR"
}	
CompilerNode;
