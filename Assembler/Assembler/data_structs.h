#define EMPTY -1
#define EMPTY_ARRAY {-1,-1,-1,-1,-1}

//basic data types


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


//cmd = define command / instruction type
enum cmd 
{
	MOV, 
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE, 
	RED,
	PRN,
	JSR,
	RTS,
	STOP,
	DATA,				//.data
	STRING,				//.string
	ENTRY,				//.entry
	EXTERNAL,			//.extern
	UNKNOWN_CMD = 99
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
//CmdParam - represent a parameter of assembly command
typedef struct
{
	enum paramTypeEnum paramType;
	char *paramData;
}
CmdParam;

//AssemblyStatement - represent an assembly command
typedef struct
{
	char label[20];					//string or null
	enum cmd command;				//cmd enum
	enum status (*covert2MachineCode)(char *, enum cmd ,CmdParam,CmdParam);	//HERE IS WHEN YOU PUT YOU FUNCTION
	char *source_operand;			//first parameter (nullable)
	char *target_operand;			//second parameter (nullable)
} 
AssemblyStatement;


// CommandRule - define an assembly command binary code and legitimate addressing methods (0-4)
typedef struct
{
	enum cmd cmd_type;
	char *name;
	char *binary_code;
	int source_addressing_options[5]; //legal source addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
	int dest_addressing_options[5];	 //legal destination addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
} CommandStruct;

//===================  Tables & Collectors ==========================

//commandRules - table(array) contains each assembly command and its rules
CommandStruct commands_list[] = 
{
	MOV,		"mov", "0000", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	CMP,		"cmp", "0001", {0,1,2,3,4}, {0,1,2,3,4},
	ADD,		"add", "0010", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	SUB,		"sub", "0011", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	NOT,		"not", "0100", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	CLR,		"clr", "0101", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	LEA,		"lea", "0110", {1,2,3,EMPTY,EMPTY},{1,2,3,4,EMPTY},
	INC,		"inc", "0111", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	DEC,		"dec", "1000", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	JMP,		"jmp", "1001", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	BNE,		"bne", "1010", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	RED,		"red", "1011", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	PRN,		"prn", "1100", EMPTY_ARRAY, {0,1,2,3,4},
	JSR,		"jsr", "1101", EMPTY_ARRAY, {1,EMPTY,EMPTY,EMPTY,EMPTY},
	RTS,		"rts", "1110", EMPTY_ARRAY, EMPTY_ARRAY,
	STOP,		"stop","1111", EMPTY_ARRAY, EMPTY_ARRAY,
	UNKNOWN_CMD,""	  ,""	 , EMPTY_ARRAY, EMPTY_ARRAY

};

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
compiler_Node;
