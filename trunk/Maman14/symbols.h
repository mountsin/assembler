typedef struct symbol
{
	char *name;
	int address;
	struct symbol *next;
	struct symbol *prev;

} Symbol;

Symbol *symbols_list;

void add_symbol(char *name, int address);