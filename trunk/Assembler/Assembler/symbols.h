

typedef struct symbol
{
	char *name;
	int address;
	struct symbol *next;
} Symbol;


void add_symbol(char *name, int address);
void add_entries_symbol(char *name, int address);
void add_external_symbol(char *name, int address);

void dispose();

Symbol *get_symbols_list();
Symbol *get_entries_symbols_list();
Symbol *get_external_symbols_list();
/* Search for the symbole name in the data list and in the externals list and returns the symbol struct or NULL if not found */
Symbol *get_data_symbol_by_name(char *symbole_name);
Symbol *get_external_symbol_by_name(char *symbole_name);