#define EMPTY -1
#define EMPTY_ARRAY {-1,-1,-1,-1,-1}

//basic data types


//===== enums ==================

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
	enum cmd  cmdType;
	char binary_code[5];
	int sourceAddressing[5]; //legal source addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
	int destAddressing[5];	 //legal destination addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
} CommandRule;


/* Used to find parse the command name from the file and convert it to the apropriate enum value */
typedef struct
{
	char *name;
	enum cmd command;
} ConvertCommand; 

ConvertCommand commands[] = 
{
	"mov", MOV,
	"cmp", CMP,
	"add", ADD,
	"sub", SUB,
	"not", NOT,
	"clr", CLR,
	"lea", LEA,
	"inc", INC,
	"dec", DEC,
	"jmp", JMP,
	"bne", BNE,
	"red", RED,
	"prn", PRN,
	"jsr", JSR,
	"rts", RTS,
	"stop", STOP,
	0,UNKNOWN_CMD
};


//===================  Tables & Collectors ==========================

//commandRules - table(array) contains each assembly command and its rules
CommandRule commandRules[] = 
{
	MOV, "0000", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	CMP, "0001", {0,1,2,3,4}, {0,1,2,3,4},
	ADD, "0010", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	SUB, "0011", {0,1,2,3,4}, {1,2,3,4,EMPTY},
	NOT, "0100", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	CLR, "0101", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	LEA, "0110", {1,2,3,EMPTY,EMPTY},{1,2,3,4,EMPTY},
	INC, "0111", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	DEC, "1000", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	JMP, "1001", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	BNE, "1010", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	RED, "1011", EMPTY_ARRAY, {1,2,3,4,EMPTY},
	PRN, "1100", EMPTY_ARRAY, {0,1,2,3,4},
	JSR, "1101", EMPTY_ARRAY, {1,EMPTY,EMPTY,EMPTY,EMPTY},
	RTS, "1110", EMPTY_ARRAY, EMPTY_ARRAY,
	STOP, "1111",EMPTY_ARRAY, EMPTY_ARRAY
};

//Table (linked list of structs) of all assembly Labels
typedef struct labelNode
{
	struct labelNode *next;  
	enum cmd type;			//can get enum values 16 - 19 (data, string...)
	int decimalAddr;
}
LabelNode;

//ErrorCollector (linked list of structs) of all errors
typedef struct error
{
	struct error *next;  
	enum status errType;			//type of error
	enum cmd cmdType;				//type of command caused the error
	int lineNumber;					//assembly (input) file line number
}	
ErrorCollector;