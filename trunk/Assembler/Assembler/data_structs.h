#define EMPTY -1
#define EMPTY_ARRAY {-1,-1,-1,-1,-1}

/*===== enums ================== */
/* cmd = define command / instruction type */
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
	DATA,				/* .data */
	STRING,				/* .string */
	ENTRY,				/* .entry */
	EXTERN,				/* .extern */
	COMMENT,			/* comment line the assmbler will ignore */
	UNKNOWN_CMD = 99	//TODO: duplicate with stasus enum values
};

/* addressing_method - addresing method (0-4) */
enum addressing_method
{	
	IMMEDIATE,
	DIRECT,
	INDEX,
	DOUBLE_INDEX,
	REGISTER
};

/* linker info enum */
enum linker_enum
{	
	ABSOLUTE,
	RELOCATABLE,
	EXTERNAL
};



/* paramType = define assembly command param type */
enum paramTypeEnum 
{
	INT = 0,
	CHAR = 1, 
	STR = 2,
	ASSEMBLY_LABEL = 3
};


/* CmdParam - represent a parameter of assembly command */
typedef struct
{
	enum paramTypeEnum paramType;
	char *paramData;
}
CmdParam;

/* CommandRule - define an assembly command binary code and legitimate addressing methods (0-4) */
typedef struct
{
	enum cmd cmd_type;
	char *name;
	char *binary_code;
	char number_of_words;
	int source_addressing_options[5]; /* legal source addressing typr for that command (cell can be 0-4 or EMPTY if not needed) */
	int dest_addressing_options[5];	  /* legal destination addressing typr for that command (cell can be 0-4 or EMPTY if not needed) */
} CommandStruct;

//=========== Complex Elements ==================================
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

//===================  Tables & Collectors ==========================




//Table (linked list of structs) of all assembly Labels
typedef struct labelNode
{
	struct labelNode *next;  
	enum cmd type;			//can get enum values 16 - 19 (data, string...)
	int decimalAddr;
}
LabelNode;


