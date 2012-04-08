#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_functions.h"
#include "second_scan.h"
#include "symbols.h"
#include "error.h"
#include "pre_compiled.h"
#include "global_constants.h"

#define MACHINE_CODE_WORD_BITLENGTH 16
#define EMPTY_ADDRESS 0
#define MINIMAL_DATA_ADDRESS 1

#define OK 0
#define ERROR -1

int set_binary_machine_code(enum boolean_ex is_external, SymbolPtr current_symbol, CompilerNode *cn);
enum boolean_ex get_sym_by_name_and_set_external(SymbolPtr current_symbol, char *symbol_name);
void add_data_entries_to_output_list();
int set_entry(CompilerNode *cn);
void set_entry_with_data_address(char * current_entry_name, CompilerNode *cn);
void set_entry_with_code_address(char *current_entry_name, CompilerNode *cn);
int check_entries();

/**
* perform second scan on compile node list
* set binary machine code by symbols list if needed
* set a flag for the linker program
* set lists for entries & external files
*/
void second_scan()
{

	/* set local variabels*/
	CompilerNode *h = get_code_list_head();

	boolean_ex is_external = NO;
	SymbolPtr current_symbol = create_symbol(); /*need to be memory allocated for copying data to it*/

	while(h != NULL) 
	{
		if(h->second_scan_type == DATA_NODE) /*this is a data node - skip all*/
		{
				/*skip to next node*/
				h = h->next;
				continue;
		}

		if(h->second_scan_type == SKIP) /*this is a command node - set entry if needed*/
		{

			h->linker_flag = ABSOLUTE;

			if(!h->label[0])
			{
				/*skip to next node*/
				h = h->next;
				continue;
			}

			if(set_entry(h) != OK)
			{
				/*skip to next node*/
				h = h->next;
				continue;
			}

			
		}
		else /* second scan required*/ 
		{
			if(is_binary_Str(h->binary_machine_code) != TRUE) /*not a binary string*/
			{
				/* 1 - find the correct label if exist */
				is_external = get_sym_by_name_and_set_external(current_symbol, h->label);

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
				{
					if(h->second_scan_type == LABEL_OFFSET)
						h->linker_flag = ABSOLUTE;
					else
						h->linker_flag = RELOCATABLE;
				}
			}

			/*4 - if external - add to externals file*/
			if (is_external == YES)
				add_externalFile_row(h->label, h->address, h->line_number);

			/*5 - if entry - set with data address*/
			if (get_entry_symbol_by_name(current_symbol->name) != NULL)
				set_entry_with_data_address(current_symbol->name, h);
		}

		/*final check - binary machine code is really a binary*/	
		if(is_binary_Str(h->binary_machine_code) != TRUE) /*not a binary string*/
			/*add_error*/
			add_error(h->line_number, UNKNOWN_ERROR);

		/*go to next node*/
		h = h->next;
	}

	/*check that entries are ok*/
	check_entries();

	/*free memory allocations*/
	free(current_symbol);
}

/**
* set a CompilerNode binary machine code value. return an int number reperesent OK or ERROR
* is_external - is current symbol is external
* current_symbol - current symbol
* CompilerNode - current compiler node to be modified
* return(int) - OK if successful, ERROR if not (OK, ERROR defined in global_constants.h);
*/

int set_binary_machine_code(enum boolean_ex is_external, SymbolPtr current_symbol, CompilerNode *cn)
{
	/* set local variabels*/
	int label_address = 0;
	int machine_code_integer = 0;

	/* set local variabels - data address boundries*/
	int low, high;
	low = MINIMAL_DATA_ADDRESS;
	high = get_data_counter();

	if (is_external == YES)
	{
		/*external - set row of 0's*/
		dec2bin(EMPTY_ADDRESS, cn->binary_machine_code, MACHINE_CODE_WORD_BITLENGTH);
		return OK;
	}
	else
		label_address = current_symbol->address;					/* get the label address from sybol*/
			
	if (cn->second_scan_type == LABEL_OFFSET) /* label offset is required.label is: %LABEL"*/
	{
		machine_code_integer = MINIMAL_DATA_ADDRESS + label_address; /*binary machine code will get the offset value*/
	}
	else /*actual label required*/
	{
		if(get_data_symbol_by_name(current_symbol->name) != NULL) /*if is data symbol*/
			machine_code_integer = label_address + MINIMAL_DATA_ADDRESS + get_instruction_counter(); /*binary machine code will get the address value*/
		else
			machine_code_integer = label_address;

		/* set data boundries for relocatable*/
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
enum boolean_ex get_sym_by_name_and_set_external(SymbolPtr current_symbol, char *symbol_name)
{
	
	boolean_ex returnVal = NEITHER;
	SymbolPtr found_symbols = get_data_symbol_by_name(symbol_name); /* try to get data symbol by binary_machine_code temporary string*/

	if(found_symbols != NULL) /* label is a data symbol*/
		returnVal = NO; /*mark symbol as NOT external*/
	else
		found_symbols = get_code_symbol_by_name(symbol_name);
	
	if (found_symbols != NULL) /* label is a code symbol*/
		returnVal = NO; /*mark symbol as NOT external*/
	else
	{
		found_symbols = get_external_symbol_by_name(symbol_name); /* try to get external symbol by binary_machine_code temporary string*/
		if(found_symbols != NULL) /*lable found -  as external symbol*/
			returnVal =  YES; /*mark symbol as external*/
		else 
			returnVal = NEITHER; /*no symbol - return NULL_VALUE*/
	}

	if(found_symbols != NULL)
		copy_symbol_contant(found_symbols, current_symbol);

	return returnVal;
}

/**
* set entry 
*/
int set_entry(CompilerNode *cn)
{
	SymbolPtr current_symbol = NULL;
	SymbolPtr temp_symbol = NULL;

	/*1- add to symbol_outpuFile_list*/
	if ( get_external_symbol_by_name(cn->label)  != NULL)
	{
		/*add error - label should be defined as entry*/
		add_error(cn->line_number, ENTRY_LABEL_UNDEFINED);

		return ERROR;
	}

	if ( (current_symbol = get_entry_symbol_by_name(cn->label) ) == NULL)
		return OK; /*may be a useless label(still valid)*/

	if (current_symbol->address == UNDEFINED_ADDRESS)
	{	/*set entry address*/
				
		if( (temp_symbol = get_code_symbol_by_name(cn->label))   != NULL)
			current_symbol->address = temp_symbol->address; /*set with code address*/

		else if( (temp_symbol = get_data_symbol_by_name(cn->label) ) != NULL)
			current_symbol->address = MINIMAL_DATA_ADDRESS + temp_symbol->address + get_instruction_counter(); /*set with data address*/
				
		else
		{
			/*add error  - entry does not found in data & code symbol lists (shouldn't be possible)*/
			add_error(cn->line_number, UNKNOWN_ERROR);
			
			return ERROR;
		}
	}
	return OK;
}

/**
* set entry symbol with data address
*/
void set_entry_with_data_address(char * current_entry_name, CompilerNode *cn)
{
	SymbolPtr current_entry = get_entry_symbol_by_name(current_entry_name);
	SymbolPtr temp_symbol = NULL;
	
	if (current_entry->address <= get_instruction_counter())
	{
		if( (temp_symbol = get_data_symbol_by_name(current_entry->name) ) != NULL)
				current_entry->address =  temp_symbol->address + get_instruction_counter(); /*set data address*/
			
		else
			/*add error  - entry  is defined but not been used*/
			add_error(cn->line_number, ENTRY_LABEL_NOT_SET);
	}
}

/**
* set entry symbol with code address
*/
void set_entry_with_code_address(char *current_entry_name, CompilerNode *cn)
{
	SymbolPtr current_entry = get_entry_symbol_by_name(current_entry_name);
	SymbolPtr temp_symbol = NULL;

	if (current_entry->address == UNDEFINED_ADDRESS)
	{
		if( (temp_symbol = get_code_symbol_by_name(current_entry->name) ) != NULL)
				current_entry->address = temp_symbol->address; /*set code address*/
			
		else
			/*add error  - entry  is defined but not been used*/
			add_error(cn->line_number, ENTRY_LABEL_NOT_SET);
	}
}

/**
* make sure all entries have adresses
*/
int check_entries()
{
	SymbolPtr entries_head = get_entries_symbols_list();

	while(entries_head)
	{
		if (entries_head->address == UNDEFINED_ADDRESS)
		{
			/*add error  - entry  is defined but not been used*/
			add_error(UNDEFINED_ADDRESS, ENTRY_LABEL_NOT_SET);
			
			return ERROR;
		}
		entries_head = entries_head->next;
	}
	return OK;
}
