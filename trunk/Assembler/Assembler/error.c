#include <stdio.h>
#include <stdlib.h>
#include "error.h"

Error *errors_list;
//TODO: reverse error list (like in pre - compiled nodes)
//add an option to insert the error in the middle of the list (by line number) OR - 
// add function to order the list by line numbers (so error list output will be by order)
// by the way - maybe we should add an optional error string to struct "Error"? (like - name of label which is missing and so on)
void add_error(int line_number, enum error_type error)
{
	Error *tmp = (Error *)malloc(sizeof(Error));
	tmp->type = error;
	tmp->line_number = line_number;
	tmp->next = errors_list;
	errors_list = tmp;

	printf("Error");
}

Error *get_errors_list()
{
	return errors_list;
}