/*===== enums ================== */

typedef enum boolean
{
	FALSE,
	TRUE
} Boolean;

typedef enum boolean_ex
{	
	NO,
	YES,
	NEITHER 
} boolean_ex;


/* cmd = define command / instruction type */
typedef enum cmd 
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
	COMMENT,			/* comment line or white chars line -  the assmbler will ignore */
	UNKNOWN_CMD = 99	
} Cmd;

/* addressing_method - addresing method (0-4) */
typedef enum addressing_method
{	
	IMMEDIATE,
	DIRECT,
	INDEX,
	DOUBLE_INDEX,
	REGISTER,
	UNKNOWN
} AddressingMethod;

/* linker info enum */
enum linker_enum
{	
	ABSOLUTE,
	RELOCATABLE,
	EXTERNAL,
	NONE
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

/* Command - define an assembly command binary code and legitimate addressing methods (0-4) */
typedef struct
{
	Cmd cmd_type;
	char name[8];
} CommandStruct;

