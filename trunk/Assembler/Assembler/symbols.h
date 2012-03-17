

typedef struct symbol
{
	char *name;
	int address;
	struct symbol *next;
} Symbol;


void add_symbol(char *name, int address);
void add_entry_symbol(char *name, int address);
void add_external_symbol(char *name, int address);

Symbol *get_next_symbol();
Symbol *get_next_entry_symbol();
Symbol *get_next_external_symbol();