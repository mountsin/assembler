#include "symbols.h"

void add_symbol(char *name, int address)
{
	Symbol temp_symbol;
	temp_symbol.name = name;
	temp_symbol.address = address;
	temp_symbol.next = symbols_list;
	symbols_list = &temp_symbol;
}