#include <stdio.h>
#include "symbols.h"

Symbol *symbols_list = NULL;
Symbol *entries_symbols_list = NULL;
Symbol *external_symbols_list = NULL;

void add_symbol(char *name, int address)
{
	Symbol tmp;
	tmp.name = name;
	tmp.address = address;
	tmp.next = symbols_list;
	symbols_list = &tmp;
}

void add_external_symbol(char *name)
{
	Symbol tmp;
	tmp.name = name;
	tmp.next = external_symbols_list;
	external_symbols_list = &tmp;
}

Symbol *get_symbol_by_name(char *name)
{
	return NULL;
}