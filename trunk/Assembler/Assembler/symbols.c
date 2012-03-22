#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

Symbol *symbols_list = NULL;
Symbol *entries_symbols_list = NULL;
Symbol *external_symbols_list = NULL;


//TODO: reverse symbols lists (like in pre - compiled nodes)

Symbol *get_symbols_list()
{
	return symbols_list;
}

Symbol *get_entries_symbols_list()
{
	return entries_symbols_list;
}

Symbol *get_external_symbols_list()
{
	return external_symbols_list;
}

void add_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = symbols_list;
	symbols_list = tmp;
}

void add_external_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = external_symbols_list;
	external_symbols_list = tmp;
}

void add_entries_symbol(char *name, int address)
{
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	tmp->name = name;
	tmp->address = address;
	tmp->next = entries_symbols_list;
	entries_symbols_list = tmp;
}


Symbol *get_data_symbol_by_name(char *name_to_find)
{
	Symbol *datasym_pointer = symbols_list;
	while(datasym_pointer)
	{
		if (strcmp(datasym_pointer->name, name_to_find) == 0) /* name has found*/
			return datasym_pointer;

		datasym_pointer = datasym_pointer->next;
	}

	return NULL;
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

void dispose()
{
	//TODO: implement
}