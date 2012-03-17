#include <stdio.h>
#include <string.h>
#include "second_scan.h"

void second_scan(CompilerNode *compiler_node_header)
{
	/*
		search in all nodes:
		1	
			
		2	if machine have been set - skip to next step

			else - set code by labels table

		3   if linker flag is needed
			set linkerEnum

	*/
	char *current_label;
	char *current_label_addr;


	CompilerNode *h = compiler_node_header;

	while(h != NULL) 
	{

		if(!is_binary_Str(h->binary_machine_code)) /* not a binary string*/
		{
			//h->binary_machine_code = get_label_address()
	
		}
	}
	
}

int is_binary_Str(char *str)
{
	int i;
	
	for(i = 0; str[i]; i++)
		if ( (str[i] != '0') && (str[i] != '1') ) /* not a binary string*/
			return 0;

	return 1;
}