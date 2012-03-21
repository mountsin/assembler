#include <stdlib.h>
#include "pre_compiled.h"

/* Functions prototypes */
void set_binary_machine_code_to(CompilerNode *compiler_node);

/* Head of compiler nodes list */
CompilerNode *compiler_nodes_list = NULL;

void add_compiler_node(char *label, int adress, enum cmd cmd_type, 
						enum addressing_method source_addressing,
						enum addressing_method target_addressing,
						char *source_operand,
						char *target_operand,
						int line_number)
{
	CompilerNode *tmp = (CompilerNode *)malloc(sizeof(CompilerNode));
	tmp->label = label;
	tmp->address = adress;
	tmp->cmd_type = cmd_type;
	tmp->sourceAddressing = source_addressing;
	tmp->targetAddressing = target_addressing;
	tmp->source_operand = source_operand;
	tmp->target_operand = target_operand;
	tmp->line_number = line_number;
	set_binary_machine_code_to(tmp);
	tmp->next = compiler_nodes_list;
	compiler_nodes_list = tmp;

}

void set_binary_machine_code_to(CompilerNode *compiler_node)
{
	//TODO: implement
}

CompilerNode *get_compiler_nodes_list()
{
	return compiler_nodes_list;
}

