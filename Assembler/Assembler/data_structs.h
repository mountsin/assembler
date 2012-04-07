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
