#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "global_functions.h"
#include "first_scan.h"
#include "second_scan.h"
#include "symbols.h"
#include "error.h"
#include "pre_compiled.h"

#define MACHINE_CODE_WORD_BITLENGTH 16
#define EMPTY_ADDRESS 0

void second_scan(CompilerNode *compiler_node_header)
{

	/* set local variabels*/
	int label_address, machine_code_integer;

	CompilerNode *h = compiler_node_header;
	//Error *error_collector = get_errors_list();

	enum boolean is_external;
	Symbol *current_symbol;


	/*
	Symbol *get_symbols_list();
Symbol *get_entries_symbols_list();
Symbol *get_external_symbols_list();
*/

	while(h != NULL) 
	{
		if(h->is_second_scan_needed) /* second scan required*/
		{
			/* 1 - find the correct label if exist */
			current_symbol = get_data_symbol_by_name(h->binary_machine_code); /* try to get data symbol by binary_machine_code temporary string*/
			
			if(current_symbol != NULL) /* label is a data symbol*/
				is_external = FALSE; /*mark symbol as NOT external*/
			else
			{
				get_external_symbol_by_name(h->binary_machine_code); /* try to get external symbol by binary_machine_code temporary string*/
				if(current_symbol != NULL) /*lable found -  as external symbol*/
					is_external = TRUE; /*mark symbol as external*/
			}
			

			if (current_symbol == NULL)	/* error - label is not in the symbols lists*/
			{
				/*add_error*/
				add_error(h->line_number, LABEL_NOT_DEFINED);
				/*skip to next node*/
				continue;
			}
				

			/*2 - set binary machine code*/
			if (is_external == TRUE)
				label_address = EMPTY_ADDRESS;
			else
				label_address = current_symbol->address;					/* get the label address from sybol*/
			
			if (h->linker_flag == ABSOLUTE) /* linker flag is absolute*/
				machine_code_integer = label_address - get_instruction_counter(); /*binary machine code will get the offset value*/
			else
				machine_code_integer = label_address; /*binary machine code will get the address value*/

			
			dec2bin(machine_code_integer, h->binary_machine_code, MACHINE_CODE_WORD_BITLENGTH);
		
			/* 3 - set linker flag */
			if (h->linker_flag == NONE)  /*linker flag has not been set - can be external or relocatable*/
			{
				if (is_external == TRUE)//current_symbol->type == EXTERNAL)
					h->linker_flag = EXTERNAL;
				else
					h->linker_flag = RELOCATABLE;
			}

			/*final check - binary machine code is really a binary*/	
			if(is_binary_Str(h->binary_machine_code) != TRUE) /*not a binary string*/
				/*add_error*/
				add_error(h->line_number, UNKNOWN_ERROR);
		}


	}
	
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