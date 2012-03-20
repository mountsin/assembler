

typedef struct symbol
{
	char *name;
	int address;
	struct symbol *next;
} Symbol;


void add_symbol(char *name, int address);
void add_entry_symbol(char *name, int address);
void add_external_symbol(char *name, int address);
void dispose();

Symbol *get_next_symbol();
Symbol *get_next_entry_symbol();
Symbol *get_next_external_symbol();
/* Search for the symbole name in the data list and in the externals list and returns the symbol struct or NULL if not found */
Symbol *get_symbol_by_name(char *symbole_name);