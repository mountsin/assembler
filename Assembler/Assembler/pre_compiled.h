#include <stdio.h>

/**
* compiler_Node (linked list of structs) table of nodes
* each represent a pre-compiled line
*/
typedef struct compiler_node
{
	struct compiler_node *next;  
	char label[31];
	int address;
	enum cmd cmd_type;							/* type of command */
	enum addressing_method sourceAddressing;
	enum addressing_method targetAddressing;
	int source_register;
	int target_register;
	char *source_operand;						/* first parameter (nullable) */
	char *target_operand;						/* second parameter (nullable) */
	char binary_machine_code[17];					/* "101010110" "STR" "&L STR" */
	enum boolean is_second_scan_needed;			/* is second scan processing needed */
	enum linker_enum linker_flag;
	int line_number;
} CompilerNode;



void add_compiler_node(CompilerNode *stmt);

int get_instruction_counter();
int get_data_counter();
CompilerNode *get_compiler_nodes_list_head();


