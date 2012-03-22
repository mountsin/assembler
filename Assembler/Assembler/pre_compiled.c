#include <stdlib.h>
#include "pre_compiled.h"

#define MACHINE_CODE_WORD_BITLENGTH 16

/* Functions prototypes */
void set_binary_machine_code_to(CompilerNode *compiler_node);

/* Head of compiler nodes list */
CompilerNode *compiler_nodes_list_head = NULL;
/* Head of compiler nodes list */
CompilerNode *compiler_nodes_list_tail = NULL;

/* Add a new struct to the end of the list */
void add_compiler_node(CompilerNode *stmt)
{
	CompilerNode *tmp = (CompilerNode *)malloc(sizeof(CompilerNode));
	tmp->label = stmt->label;
	tmp->address = stmt->address;
	tmp->cmd_type = stmt->cmd_type;
	tmp->sourceAddressing = stmt->sourceAddressing;
	tmp->targetAddressing = stmt->targetAddressing;
	tmp->source_operand = stmt->source_operand;
	tmp->target_operand = stmt->target_operand;
	tmp->line_number = stmt->line_number;

	/* memory allocations for futur data storage*/
	/*TODO: remove if not needed
	tmp->source_register = (int)malloc(sizeof(int));
	tmp->target_register = (int)malloc(sizeof(int));
	tmp->is_second_scan_needed = (enum Boolean)malloc(sizeof(enum boolean));
	tmp->linker_flag = (enum linker_enum)malloc(sizeof(enum linker_enum));
	*/
	tmp->binary_machine_code = (char *)calloc(MACHINE_CODE_WORD_BITLENGTH + 1, sizeof(char));
	tmp->next = NULL;

	if(compiler_nodes_list_tail)
		compiler_nodes_list_tail->next = tmp;
	compiler_nodes_list_tail = tmp;
	if(!compiler_nodes_list_head)
		compiler_nodes_list_head = compiler_nodes_list_tail;

}

CompilerNode *get_compiler_nodes_list_head()
{
	return compiler_nodes_list_head;
}

