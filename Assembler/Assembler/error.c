#include <stdio.h>
#include <stdlib.h>
#include "error.h"

Error *errors_list = NULL;			/*error list head*/

void add_errorNode_by_line_number(Error *new_node);

void add_error(int line_number, enum error_type error)
{
	Error *tmp = (Error *)malloc(sizeof(Error));
	tmp->type = error;
	tmp->line_number = line_number;
	tmp->next = NULL;
	
	/*set linked list nodes*/
	if(errors_list != NULL) /*tail already defined*/
	{
		add_errorNode_by_line_number(tmp);
	}
	else
		errors_list = tmp;		/*first node - set head and tail*/
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

void free_errors_collector()
{
	Error *error_node = errors_list;
	
	while(errors_list != NULL)
	{
		errors_list = errors_list->next;		/*advance head pointer*/
		free(error_node);						/*free current node*/
		error_node = errors_list;				/*point current node to head pointer*/
	}
}