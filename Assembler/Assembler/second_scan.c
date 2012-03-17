#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "global_functions.h"
#include "second_scan.h"
#include "symbols.h"
#include "error.h"
#include "pre_compiled.h"

#define  MACHINE_CODE_WORD_BITLENGTH 16


void second_scan(CompilerNode *compiler_node_header, Symbol *symbols_list_header)
{

	int label_address, machine_code_integer;

	CompilerNode *h = compiler_node_header;
	Symbol *current_symbol = symbols_list_header;

	while(h != NULL) 
	{
		if(h->is_second_scan_needed) /* second scan required*/
		{
			/* 1 - set binary machine code */
			current_symbol = get_symbol_by_name(h->binary_machine_code);/* get symbol by binary_machine_code temporary string*/
			
			//TODO: ADD ERROR
			//if (current_symbol == NULL)	/* error - label is not in the symbols table*/
				//add_error
			
			label_address = current_symbol->address;					/* get the label address from sybol*/
			
			if (h->linker_flag == ABSOLUTE) /* linker flag is absolute*/
				machine_code_integer = label_address - get_end_of_code_address(); /*binary machine code will get the offset value*/
			else
				machine_code_integer = label_address; /*binary machine code will get the address value*/

			/* set binary machine code*/
			 dec2bin(machine_code_integer, h->binary_machine_code, MACHINE_CODE_WORD_BITLENGTH);
		
		
			/* 2 - set linker flag */
			if (h->linker_flag == NONE)  /*linker flag has not been set - can be external or relocatable*/
			{
				if (1)//current_symbol->type == EXTERNAL)
					h->linker_flag = EXTERNAL;
				else
					h->linker_flag = RELOCATABLE;
			}
			/*
			function get_symbol_by_name
			function get_data_counter
			function - change name to: get instruction counter?...

			add to symbol struct: is_entry flag
			add to symbol struct: type (data / string / external)

			main should hold headers: pre_compiled, symbols, errors

			add line number to error struct
			*/
		
		}


	}
	
}

//TODO: remove if not needed
int is_binary_Str(char *str)
{
	int i;
	
	for(i = 0; str[i]; i++)
		if ( (str[i] != '0') && (str[i] != '1') ) /* not a binary string*/
			return 0;

	return 1;
}

/**
* get label name and return 1 if offset (absolute number) or an adress (relocatable)
* input - label name (label / %label)
* output - real label name (name with not & sign)
* return - 1 if offset (absolute number), 0 if address(relocatable)
*/
//TODO: remove if not needed
int get_second_scan_definitions(char *input, char *output)
{
	int i,j,is_offset_flag;
	
	if(input[0] == '%')
		is_offset_flag == 1;  /* operand use % - get relative(offset) data label location(a)*/
	else
		is_offset_flag = 0;	/* operand use % - get data label address(r)*/

	j = 0;

	for(i = 0; input[i]; i++)
	{
		if (input[i] != '%')
		{
			output[j] = input[i];
			j++;
		}
	}
}