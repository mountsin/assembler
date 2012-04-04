#include <stdlib.h>
#include <string.h>
#include "pre_compiled.h"
#include "data_structs.h"

/* Functions prototypes */
void set_binary_machine_code_to(CompilerNodePtr compiler_node);

/* Head of compiler nodes list */
CompilerNodePtr compiler_nodes_list_head = NULL;
/* Tail of compiler nodes list */
CompilerNodePtr compiler_nodes_list_tail = NULL;

CompilerNodePtr create_compiler_node()
{
	CompilerNodePtr node = (CompilerNodePtr)malloc(sizeof * node);
	/* init node values */
	if(node)
	{
		node->address = 0;
		node->binary_machine_code[0] = NULL;
		node->cmd_type = UNKNOWN_CMD;
		node->second_scan_type = SKIP;
		node->label[0] = NULL;
		node->line_number = 0;
		node->linker_flag = NONE;
		node->next = NULL;
		node->sourceAddressing = UNKNOWN;
		node->source_operand = NULL;
		node->source_register = 0;
		node->targetAddressing = UNKNOWN;
		node->target_operand = NULL;
		node->target_register = 0;
	}
	return node;
}
	
void destroy_compiler_node(CompilerNodePtr node)
{
	free(node);
}

/* Add a new struct to the end of the list */
void add_compiler_node(CompilerNodePtr stmt)
{
	/*set linked list nodes*/
	if(compiler_nodes_list_tail) /*compiler_nodes_list_tail already defined*/
	{
		compiler_nodes_list_tail->next = stmt;
		compiler_nodes_list_tail = compiler_nodes_list_tail->next;
	}
	else
		compiler_nodes_list_tail = stmt;
	
	if(!compiler_nodes_list_head)/*compiler_nodes_list_head not defined - first node*/
		compiler_nodes_list_head = compiler_nodes_list_tail;
}

CompilerNodePtr get_compiler_nodes_list_head()
{
	return compiler_nodes_list_head;
}