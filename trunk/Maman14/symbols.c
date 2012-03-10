#include "symbols.h"

void add_symbole(char *name, int address)
{
	Symbole temp_symbole;
	temp_symbole.name = name;
	temp_symbole.address = address;
	temp_symbole.next = symboles_list;
	symboles_list = &temp_symbole;
}