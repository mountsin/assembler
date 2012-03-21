#include <stdio.h>
#include <stdlib.h>
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

//TODO:
Symbol *get_data_symbol_by_name(char *name)
{
	//TODO: implement	
	return NULL;
}

Symbol *get_external_symbol_by_name(char *symbole_name)
{
	//TODO: implement	
	return NULL;
}

void dispose()
{
	//TODO: implement
}