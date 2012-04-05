#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "global_constants.h"
#include "global_functions.h"
#include "error.h"

/*symbol list head pointers*/
SymbolPtr data_symbols_list = NULL;
SymbolPtr code_symbols_list = NULL;
SymbolPtr entries_symbols_list = NULL;
SymbolPtr external_symbols_list = NULL;
SymbolPtr entriesFile_rows = NULL;
SymbolPtr externalFile_rows = NULL;

/*symbol list tail pointers*/
SymbolPtr data_symbols_list_tail = NULL;
SymbolPtr code_symbols_list_tail = NULL;
SymbolPtr entries_symbols_list_tail = NULL;
SymbolPtr external_symbols_list_tail = NULL;
SymbolPtr entriesFile_rows_tail = NULL;
SymbolPtr externalFile_rows_tail = NULL;


SymbolPtr create_symbol()
{
	SymbolPtr symbol = (SymbolPtr)malloc(sizeof * symbol);
	if(symbol)
	{
		symbol->address = UNDEFINED_ADDRESS;
		symbol->name[0] = NULL;
		symbol->next = NULL;
	}
	return symbol;
}

SymbolPtr get_data_symbols_list()
{
	return data_symbols_list;
}

SymbolPtr get_entries_symbols_list()
{
	return entries_symbols_list;
}

SymbolPtr get_external_symbols_list()
{
	return external_symbols_list;
}

SymbolPtr get_entriesFile_head()
{
	return entriesFile_rows;
}

SymbolPtr get_externalFile_head()
{
	return externalFile_rows;
}

void add_data_symbol(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;
	
	if(get_data_symbol_by_name(name) != NULL)		/* symbol already exist in the list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}

	if(get_external_symbol_by_name(name) != NULL)	/* symbol already exist in a different list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}
	
	/*set linked list nodes*/
	if(data_symbols_list_tail) /*tail already defined*/
	{
		data_symbols_list_tail->next = tmp;						/*add new node to tail*/
		data_symbols_list_tail = data_symbols_list_tail->next;
	}
	else
		data_symbols_list = data_symbols_list_tail = tmp;		/*first node - set head and tail*/
}

void add_code_symbol(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;
	
	if(get_code_symbol_by_name(name) != NULL)		/* symbol already exist in the list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}

	if(get_external_symbol_by_name(name) != NULL)	/* symbol already exist in a different list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}
	
	/*set linked list nodes*/
	if(code_symbols_list_tail) /*tail already defined*/
	{
		code_symbols_list_tail->next = tmp;						/*add new node to tail*/
		code_symbols_list_tail = data_symbols_list_tail->next;
	}
	else
		code_symbols_list = data_symbols_list_tail = tmp;		/*first node - set head and tail*/
}

void add_external_symbol(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;
	
	if(get_external_symbol_by_name(name) != NULL)	/* symbol already exist in list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}
	if(get_data_symbol_by_name(name) != NULL)	/* symbol already exist in a different list */
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}


	/*set linked list nodes*/
	if(external_symbols_list_tail) /*tail already defined*/
	{
		external_symbols_list_tail->next = tmp;						/*add new node to tail*/
		external_symbols_list_tail = external_symbols_list_tail->next;
	}
	else
		external_symbols_list = external_symbols_list_tail = tmp;		/*first node - set head and tail*/
}

void add_entries_symbol(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;
	
	if(get_entry_symbol_by_name(name) != NULL) /* symbols already exist in a different list*/
	{
		add_error(line_number,SYMBOL_ALREADY_EXISTS);
		return;
	}

	/*set linked list nodes*/
	if(entries_symbols_list_tail) /*tail already defined*/
	{
		entries_symbols_list_tail->next = tmp;						/*add new node to tail*/
		entries_symbols_list_tail = entries_symbols_list_tail->next;
	}
	else
		entries_symbols_list = entries_symbols_list_tail = tmp;		/*first node - set head and tail*/
}

SymbolPtr get_data_symbol_by_name(char *name_to_find)
{
	int chars2compare;

	SymbolPtr datasym_pointer = data_symbols_list;

	while(datasym_pointer)
	{
		chars2compare = get_Longest_length(datasym_pointer->name , name_to_find);

		if (strncmp(datasym_pointer->name, name_to_find, chars2compare) == 0) /* name has found*/
			return datasym_pointer;
		datasym_pointer = datasym_pointer->next;
	}
	return NULL; /* symbol not found*/
}

SymbolPtr get_code_symbol_by_name(char *name_to_find)
{
	SymbolPtr datasym_pointer = code_symbols_list;
	while(datasym_pointer)
	{
		if (strncmp(datasym_pointer->name, name_to_find, MACHINE_WORD_BITLENGTH) == 0) /* name has found*/
			return datasym_pointer;
		datasym_pointer = datasym_pointer->next;
	}
	return NULL; /* symbol not found*/
}

SymbolPtr get_external_symbol_by_name(char *name_to_find)
{
	SymbolPtr extsym_pointer = external_symbols_list;
	while(extsym_pointer)
	{
		if (strncmp(extsym_pointer->name, name_to_find, MACHINE_WORD_BITLENGTH) == 0) /* name has found*/
			return extsym_pointer;
		extsym_pointer = extsym_pointer->next;
	}
	return NULL;
}

SymbolPtr get_entry_symbol_by_name(char *name_to_find)
{
	SymbolPtr entsym_pointer = entries_symbols_list;
	while(entsym_pointer)
	{
		if (strncmp(entsym_pointer->name, name_to_find, MACHINE_WORD_BITLENGTH) == 0) /* name has found*/
			return entsym_pointer;
		entsym_pointer = entsym_pointer->next;
	}
	return NULL;
}

/*private function for making sure entries are uniqe*/
SymbolPtr get_entryFile_row_by_name(char *name_to_find)
{
	SymbolPtr entrow_pointer = entriesFile_rows;
	while(entrow_pointer)
	{
		if (strncmp(entrow_pointer->name, name_to_find, MACHINE_WORD_BITLENGTH) == 0) /* name has found*/
			return entrow_pointer;
		entrow_pointer = entrow_pointer->next;
	}
	return NULL;
}


void add_entriesFile_row(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;

	if(get_entryFile_row_by_name(name) != NULL) /* entry row should have a uniqe label and cannot be used more then once*/
	{
		add_error(line_number,MULTIPLE_ENTRYLABEL_USE);
		return;
	}

	/*set linked list nodes*/
	if(entriesFile_rows_tail) /*at least one node (row) in list*/
	{
		entriesFile_rows_tail->next = tmp;						/*add new node as next node (row)*/
		entriesFile_rows_tail = entriesFile_rows_tail->next;	/*advacne tail*/
	}
	else
		entriesFile_rows_tail = entriesFile_rows = tmp;		/*first node - set head and tail*/
}

void add_externalFile_row(char *name, int address, int line_number)
{
	SymbolPtr tmp = create_symbol();
	strcpy(tmp->name, name);
	tmp->address = address;

	/*set linked list nodes*/
	if(externalFile_rows_tail)
	{
		externalFile_rows_tail->next = tmp;						/*add new node as next node (row)*/
		externalFile_rows_tail = externalFile_rows_tail->next;	/*advacne tail*/
	}
	else
		externalFile_rows_tail = externalFile_rows = tmp;		/*first node - set head and tail*/
}

/*copy fields of source symbol to taget symbol*/
void copy_symbol_contant(SymbolPtr source, SymbolPtr target)
{
	target->address = source->address;
	target->next = source->next;
	strcpy(target->name, source->name);	
}

void destroy_symbol(SymbolPtr symbol)
{
	free(symbol);
}