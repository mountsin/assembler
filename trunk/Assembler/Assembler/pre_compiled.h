typedef struct compiler_node* CompilerNodePtr;

CompilerNodePtr create_compiler_node();
void destroy_compiler_node(CompilerNodePtr node);
void add_code_node(CompilerNodePtr stmt);
void add_data_node(CompilerNodePtr stmt);
CompilerNodePtr get_code_list_head();
CompilerNodePtr get_data_list_head();
int get_instruction_counter();
int get_data_counter();

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

/* Command - define an assembly command binary code and legitimate addressing methods (0-4) */
typedef struct
{
	Cmd cmd_type;
	char name[8];
} CommandStruct;

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

typedef enum second_scan_type
{
	SKIP,
	LABEL,
	LABEL_OFFSET,
	DATA_NODE
} SecondScanType;

/* linker info enum */
enum linker_enum
{
	ABSOLUTE,
	RELOCATABLE,
	EXTERNAL,
	NONE
};

/**
* compiler_Node (linked list of structs) table of nodes
* each represent a pre-compiled line
*/
typedef struct compiler_node
{
	struct compiler_node *next;  
	char label[31];								/* has two uses: 1. The first label of instruction line. 2. a temp label string that will be handled by the second scan */
	int address;
	enum cmd cmd_type;							/* type of command */
	enum addressing_method sourceAddressing;
	enum addressing_method targetAddressing;
	int source_register;
	int target_register;
	char *source_operand;						/* first parameter (nullable) */
	char *target_operand;						/* second parameter (nullable) */
	char binary_machine_code[17];					/* "101010110" "STR" "&L STR" */
	SecondScanType second_scan_type;			/* second scan processing type */
	enum linker_enum linker_flag;
	int line_number;
} CompilerNode;
