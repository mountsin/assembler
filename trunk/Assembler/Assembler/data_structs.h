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



