#include <stdlib.h>
#include <string.h>
#include "pre_compiled.h"

/* Functions prototypes */
void set_binary_machine_code_to(CompilerNodePtr compiler_node);
void add_node_to(CompilerNodePtr node, CompilerNodePtr *list_head, CompilerNodePtr *list_tail);

/* Head of code nodes list */
CompilerNodePtr code_list_head = NULL;
/* Tail of code nodes list */
CompilerNodePtr code_list_tail = NULL;
/* Head of data nodes list */
CompilerNodePtr data_list_head = NULL;
/*Tail of data nodes list */
CompilerNodePtr data_list_tail = NULL;


/*
* creat and initialize new compiler node pointer (include memory allocation)
*/
CompilerNodePtr create_compiler_node()
{
	CompilerNodePtr node = (CompilerNodePtr)malloc(sizeof * node);
	/* init node values */
	if(node)
	{
		node->address = 0;
		node->binary_machine_code[0] = '\0';
		node->cmd_type = UNKNOWN_CMD;
		node->second_scan_type = SKIP;
		node->label[0] = '\0';
		node->line_number = 0;
		node->linker_flag = NONE;
		node->next = NULL;
		node->sourceAddressing = UNKNOWN;
		node->source_operand = NULL;
		node->source_register = -1;
		node->targetAddressing = UNKNOWN;
		node->target_operand = NULL;
		node->target_register = -1;
	}
	return node;
}

/*
* free compiler node list memory allocations
*/
void free_compiler_node_list()
{
	CompilerNodePtr current_cn = code_list_head;
	
	while(code_list_head != NULL)
	{
		code_list_head = code_list_head->next;		/*advance head pointer*/
		free(current_cn);							/*free current node*/
		current_cn = code_list_head;				/*point current node to head pointer*/
	}
}

/*add a compiler node to the code list */
void add_code_node(CompilerNodePtr stmt)
{
	add_node_to(stmt,&code_list_head,&code_list_tail);
}

/*add a compiler node to the data list */
void add_data_node(CompilerNodePtr stmt)
{
	add_node_to(stmt,&data_list_head,&data_list_tail);
}

/*get compiler node list*/
CompilerNodePtr get_code_list_head()
{
	return code_list_head;
}

/* Add a new struct to the end of the list */
void add_node_to(CompilerNodePtr node, CompilerNodePtr *list_head, CompilerNodePtr *list_tail)
{
	/*set linked list nodes*/
	if(*list_tail) /*code_list_tail already defined*/
	{
		(*list_tail)->next = node;
		*list_tail = (*list_tail)->next;
	}
	else
		*list_tail = node;
	
	if(!*list_head)/*code_list_head not defined - first node*/
		*list_head = *list_tail;
}

/* This function connect the data list to the end of the code list and increment the data counter addresses */
void connect_data_list_to_code_list()
{
	int last_code_address = 0;
	CompilerNodePtr current = NULL;

	if(code_list_tail != NULL)
	{
		last_code_address = code_list_tail->address;
		current = data_list_head;
		while(current)
		{
			current->address += last_code_address +1;
			current = current->next;
		}
		code_list_tail->next = data_list_head;
	}
}
