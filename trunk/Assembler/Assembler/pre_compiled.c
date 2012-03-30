#include <stdlib.h>
#include <string.h>
#include "pre_compiled.h"
#include "data_structs.h"

/* Functions prototypes */
void set_binary_machine_code_to(CompilerNode *compiler_node);

/* Head of compiler nodes list */
CompilerNode *compiler_nodes_list_head = NULL;
/* Tail of compiler nodes list */
CompilerNode *compiler_nodes_list_tail = NULL;

CompilerNodePtr create_compiler_node()
{
	CompilerNodePtr node = (CompilerNodePtr)malloc(sizeof * node);
	if(node)
	{
		node->address = 0;
		node->binary_machine_code[0] = '\0';
		node->cmd_type = UNKNOWN_CMD;
		node->is_second_scan_needed = FALSE;
		node->label[0] = '\0';
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
void add_compiler_node(CompilerNode *stmt)
{
	CompilerNode *tmp = (CompilerNode *)malloc(sizeof(CompilerNode));
	strcpy(tmp->label,stmt->label);
	tmp->address = stmt->address;
	tmp->cmd_type = stmt->cmd_type;
	tmp->sourceAddressing = stmt->sourceAddressing;
	tmp->targetAddressing = stmt->targetAddressing;
	tmp->source_operand = stmt->source_operand;
	tmp->target_operand = stmt->target_operand;
	tmp->line_number = stmt->line_number;

	tmp->next = NULL;

	/*set linked list nodes*/
	if(compiler_nodes_list_tail) /*compiler_nodes_list_tail already defined*/
	{
		compiler_nodes_list_tail->next = tmp;
		compiler_nodes_list_tail = compiler_nodes_list_tail->next;
	}
	else
		compiler_nodes_list_tail = tmp;
	
	if(!compiler_nodes_list_head)/*compiler_nodes_list_head not defined - first node*/
		compiler_nodes_list_head = compiler_nodes_list_tail;

}

CompilerNode *get_compiler_nodes_list_head()
{
	return compiler_nodes_list_head;
}

