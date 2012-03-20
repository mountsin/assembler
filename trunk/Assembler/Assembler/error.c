#include <stdio.h>
#include "error.h"

Error *errors_list;

void add_error(int line_number, enum error_type error)
{
	Error tmp;
	tmp.type = error;
	tmp.line_number = line_number;
	tmp.next = errors_list;
	errors_list = &tmp;

	printf("Error");
}

Error *get_errors_list()
{
	return errors_list;
}