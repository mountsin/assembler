#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_functions.h"
//TODO: REMOVE COMMENTS IN FILE #include "first_scan.h"
#include "second_scan.h"
#include "symbols.h"
#include "error.h"
#include "pre_compiled.h"
#include "global_constants.h"
#include "data_structs.h"

#define MACHINE_CODE_WORD_BITLENGTH 16
#define EMPTY_ADDRESS 0
#define FIRST_DATA_OFFSET 1

#define OK 0
#define ERROR -1

int set_binary_machine_code(enum boolean is_external, Symbol *current_symbol, CompilerNode *cn);
enum boolean_ex get_sym_by_name_and_set_external(Symbol *current_symbol, char *symbol_name);

/**
* perform second scan on compile node list
* set binary machine code by symbols list if needed
* set a flag for the linker program
*/
void second_scan()
{

	/* set local variabels*/
	CompilerNode *h = get_compiler_nodes_list_head();

	enum boolean_ex is_external = NO; //TODO: Yuval - it was not initialized, please verify me
	Symbol *current_symbol = (Symbol *)malloc(sizeof(Symbol));



	while(h != NULL) 
	{

		if(h->second_scan_type != SKIP) /* second scan required*/ //TODO: please verify me
		{
			if(is_binary_Str(h->binary_machine_code) != TRUE) /*not a binary string*/
			{
				/* 1 - find the correct label if exist */
				is_external = get_sym_by_name_and_set_external(current_symbol, h->binary_machine_code);

			

				if (is_external == NEITHER)	/* error - label is not in the symbols lists*/
				{
					/*add_error*/
					add_error(h->line_number, LABEL_NOT_DEFINED);
					/*skip to next node*/
					 h = h->next;
					continue;
				}
				

				/*2 - set binary machine code*/
				if (set_binary_machine_code(is_external, current_symbol, h) != OK ) /* check for errors*/
				{
					/*add_error*/
					add_error(h->line_number, ILLEGAL_DATA_ADDRESS);

					/*skip to next node*/
					h = h->next;

					continue;
				}
			}
		
			/* 3 - set linker flag */
			if (h->linker_flag == NONE)  /*linker flag has not been set - can be external or relocatable*/
			{
				if (is_external == YES)
					h->linker_flag = EXTERNAL;
				else
					h->linker_flag = RELOCATABLE;
			}


		}
		//TODO: else /*second scan not needed - just set entries and external symbols*/
		//{
					
		//Symbol = get_external_symbol_by_name(symbol_name);
		//if(h->)

		/*final check - binary machine code is really a binary*/	
		if(is_binary_Str(h->binary_machine_code) != TRUE) /*not a binary string*/
			/*add_error*/
			add_error(h->line_number, UNKNOWN_ERROR);

		/*go to next node*/
		h = h->next;
	}
}

/**
* set a CompilerNode binary machine code value. return an int number reperesent OK or ERROR
* is_external - is current symbol is external
* current_symbol - current symbol
* CompilerNode - current compiler node to be modified
* return(int) - OK if successful, ERROR if not (OK, ERROR defined in global_constants.h);
*/

int set_binary_machine_code(enum boolean_ex is_external, Symbol *current_symbol, CompilerNode *cn)
{
	/* set local variabels*/
	int label_address, machine_code_integer;

	/* set local variabels - data address boundries*/
	int low, high;
	low = FIRST_DATA_OFFSET;
	high = get_data_counter();

	if (is_external == YES)
		label_address = EMPTY_ADDRESS;
	else
		label_address = current_symbol->address;					/* get the label address from sybol*/
			
	if (cn->linker_flag == ABSOLUTE) /* linker flag is absolute*/
	{
		machine_code_integer = label_address - get_instruction_counter(); /*binary machine code will get the offset value*/
	}
	else
	{
		machine_code_integer = label_address; /*binary machine code will get the address value*/

		/* set data boundries*/
		low += get_instruction_counter();
		high += get_instruction_counter();
	}
			

	/* check if offset is out of data boundries*/
	if( (machine_code_integer < low) || (high < machine_code_integer) )
		return ERROR;

	dec2bin(machine_code_integer, cn->binary_machine_code, MACHINE_CODE_WORD_BITLENGTH);

	return OK;
}

/**
* find a symbol in the symbols_list by name
* point current_symbol to the symbol found in the symbol list.
* return:	YES if symbol found  - data symbol (not external)
*			NO if symbol found  - external
*			in case symbol did not found  - current_symbol get a Null value,return NEITHER.
*/
enum boolean_ex get_sym_by_name_and_set_external(Symbol *current_symbol, char *symbol_name)
{
	
	current_symbol = get_data_symbol_by_name(symbol_name); /* try to get data symbol by binary_machine_code temporary string*/
			
	if(current_symbol != NULL) /* label is a data symbol*/
		return NO; /*mark symbol as NOT external*/
	else
	{
		current_symbol = get_external_symbol_by_name(symbol_name); /* try to get external symbol by binary_machine_code temporary string*/
		if(current_symbol != NULL) /*lable found -  as external symbol*/
			return  YES; /*mark symbol as external*/
		else return NEITHER; /*no symbol - return NULL_VALUE*/
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