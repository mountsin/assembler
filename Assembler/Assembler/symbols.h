
typedef struct symbol
{
	char *name;
	int address;
	struct symbol *next;
} Symbol;


int add_data_symbol(char *name, int address);
int add_code_symbol(char *name, int address);
int add_entries_symbol(char *name, int address);
void add_external_symbol(char *name, int address);

int dispose();

Symbol *get_symbols_list();
Symbol *get_entries_symbols_list();
Symbol *get_external_symbols_list();
/* Search for the symbole name in the data list and in the externals list and returns the symbol struct or NULL if not found */
Symbol *get_data_symbol_by_name(char *name_to_find);
Symbol *get_code_symbol_by_name(char *name_to_find);
Symbol *get_external_symbol_by_name(char *name_to_find);