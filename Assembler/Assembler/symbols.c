#include "symbols.h"

void add_symbol(char *name, int address, enum symbol_type type)
{
	Symbol tmp;
	tmp.name = name;
	tmp.type = type;
	tmp.address = address;
	tmp.next = symbols_list;
	symbols_list = &tmp;
}

void add_external_symbol(char *name)
{
	ExternalSymbol tmp;
	tmp.name = name;
	tmp.next = external_symbols_list;
	external_symbols_list = &tmp;
}