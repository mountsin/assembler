#include <stdio.h>

/**
* compiler_Node (linked list of structs) table of nodes
* each represent a pre-compiled line
*/
typedef struct compiler_node
{
	struct compiler_node *next;  
	char *label;
	int address;
	enum cmd cmd_type;							/* type of command */
	enum addressing_method sourceAddressing;
	enum addressing_method targetAddressing;
	char *source_operand;						/* first parameter (nullable) */
	char *target_operand;						/* second parameter (nullable) */
	char *binary_machine_code;					/* "101010110" "STR" "&L STR" */
	int flag_is_linker_needed;					/* linker flag needed for non-data machine code lines */
	enum linker_enum linker_flag;
}	
CompilerNode;



void add_compiler_node(char *label, int adress, enum cmd cmd_type, 
						enum addressing_method source_addressing,
						enum addressing_method target_addressing,
						char *source_operand,
						char *target_operand);

