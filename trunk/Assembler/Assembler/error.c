#include <stdio.h>
#include <stdlib.h>
#include "error.h"


Error *errors_list = NULL;		/*error list head*/

void add_errorNode_by_line_number(Error *new_node);

//TODO: reverse error list (like in pre - compiled nodes)
//add an option to insert the error in the middle of the list (by line number) OR - 
// add function to order the list by line numbers (so error list output will be by order)
// by the way - maybe we should add an optional error string to struct "Error"? (like - name of label which is missing and so on)
void add_error(int line_number, enum error_type error)
{
	Error *tmp = (Error *)malloc(sizeof(Error));
	tmp->type = error;
	tmp->line_number = line_number;
	tmp->next = NULL;
	
	if (errors_list == NULL)/*adding first node*/
		errors_list = tmp;
	else
		add_errorNode_by_line_number(tmp);
	
	//TODO: REMOVE
	printf("Error - tester\n");
}

void add_errorNode_by_line_number(Error *new_node)
{
	Error *next_node, *this_node;
	this_node = errors_list;
	next_node = errors_list->next;

	/*1 - check first node*/
	if(new_node->line_number  < this_node->line_number) /*first node line exceeding new node line*/
	{	
		new_node->next = this_node; return; 
	}

	/*2 - check middle nodes*/
	while(next_node) /* while next node available and new node have not been set*/
	{
		if(new_node->line_number  < next_node->line_number) /*next node line exceeding new node line*/
		{
			new_node->next = next_node; /*set new node next as 'next node'*/ 
			this_node->next = new_node; /*set this node next as 'new node'*/ 
			return; 
		}

		/*advance error list pointers*/
		this_node = this_node->next;
		next_node = next_node->next;
	}

	/*3 - all nodes check - set as last node*/
	/* new node have not been set and next node is NULL (new node should be the last node in error lists)*/
	this_node->next = new_node;
	return;

}

Error *get_errors_list()
{
	return errors_list;
}