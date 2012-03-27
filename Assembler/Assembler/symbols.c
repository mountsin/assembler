#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "global_constants.h"

/*symbol list head pointers*/
Symbol *data_symbols_list = NULL;
Symbol *entries_symbols_list = NULL;
Symbol *external_symbols_list = NULL;

/*symbol list tail pointers*/
Symbol *data_symbols_list_tail = NULL;
Symbol *entries_symbols_list_tail = NULL;
Symbol *external_symbols_list_tail = NULL;


//TODO: reverse symbols lists (like in pre - compiled nodes)
//TODO: when adding a symbol, check that is not exist on the other list

Symbol *get_data_symbols_list()
{
	return data_symbols_list;
}

Symbol *get_entries_symbols_list()
{
	return entries_symbols_list;
}

Symbol *get_external_symbols_list()
{
	return external_symbols_list;
}

int add_data_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = NULL;
	
	if(get_data_symbol_by_name(name) != NULL) /*symbols already exist in the list*/
		return ERROR;

	if(get_external_symbol_by_name(name) != NULL) /*symbols already exist in a different list*/
		return ERROR;

	
	/*set linked list nodes*/
	if(data_symbols_list_tail) /*tail already defined*/
	{
		data_symbols_list_tail->next = tmp;						/*add new node to tail*/
		data_symbols_list_tail = data_symbols_list_tail->next;
	}
	else
		data_symbols_list = data_symbols_list_tail = tmp;		/*first node - set head and tail*/

	return OK;
}

int add_external_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = NULL;
	
	if(get_external_symbol_by_name(name) != NULL) /*symbols already exist in the list*/
		return ERROR;

	if(get_data_symbol_by_name(name) != NULL) /*symbols already exist in a different list*/
		return ERROR;

	/*set linked list nodes*/
	if(external_symbols_list_tail) /*tail already defined*/
	{
		external_symbols_list_tail->next = tmp;						/*add new node to tail*/
		external_symbols_list_tail = external_symbols_list_tail->next;
	}
	else
		external_symbols_list = external_symbols_list_tail = tmp;		/*first node - set head and tail*/

	return OK;
}

int add_entries_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = NULL;
	
	/*set linked list nodes*/
	if(entries_symbols_list_tail) /*tail already defined*/
	{
		entries_symbols_list_tail->next = tmp;						/*add new node to tail*/
		entries_symbols_list_tail = entries_symbols_list_tail->next;
	}
	else
		entries_symbols_list = entries_symbols_list_tail = tmp;		/*first node - set head and tail*/

	return OK;
}


Symbol *get_data_symbol_by_name(char *name_to_find)
{
	Symbol *datasym_pointer = data_symbols_list;
	while(datasym_pointer)
	{
		if (strcmp(datasym_pointer->name, name_to_find) == 0) /* name has found*/
			return datasym_pointer;

		datasym_pointer = datasym_pointer->next;
	}

	return NULL; /* symbol not found*/
}

Symbol *get_external_symbol_by_name(char *name_to_find)
{
	Symbol *extsym_pointer = external_symbols_list;
	while(extsym_pointer)
	{
		if (strcmp(extsym_pointer->name, name_to_find) == 0) /* name has found*/
			return extsym_pointer;

		extsym_pointer = extsym_pointer->next;
	}

	return NULL;
}

int dispose()
{
	//TODO: implement - return value by "global_constants.h"
}