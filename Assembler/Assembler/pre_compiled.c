#include "pre_compiled.h"

CompilerNode *compiler_nodes_list = NULL;

void add_compiler_node(char *label, int adress, enum cmd cmd_type, 
						enum addressing_method source_addressing,
						enum addressing_method target_addressing,
						char *source_operand,
						char *target_operand,
						char *binary_machine_code)
{
	CompilerNode tmp;
	tmp.label = label;
	tmp.address = adress;
	tmp.cmd_type = cmd_type;
	tmp.sourceAddressing = source_addressing;
	tmp.targetAddressing = target_addressing;
	tmp.source_operand = source_operand;
	tmp.target_operand = target_operand;
	tmp.binary_machine_code = binary_machine_code;
	tmp.next = compiler_nodes_list;
	compiler_nodes_list = &tmp;
}