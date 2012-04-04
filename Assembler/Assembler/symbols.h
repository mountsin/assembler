typedef struct symbol* SymbolPtr;

SymbolPtr create_symbol();
void destroy_symbol(SymbolPtr symbol);
void add_data_symbol(char *name, int address, int line_number);
void add_code_symbol(char *name, int address, int line_number);
void add_entries_symbol(char *name, int address);
void add_external_symbol(char *name, int address, int line_number);
SymbolPtr get_symbols_list();
SymbolPtr get_entries_symbols_list();
SymbolPtr get_external_symbols_list();
/* Search for the symbole name in the data list and in the externals list and returns the symbol struct or NULL if not found */
SymbolPtr get_data_symbol_by_name(char *name_to_find);
SymbolPtr get_code_symbol_by_name(char *name_to_find);
SymbolPtr get_external_symbol_by_name(char *name_to_find);

typedef struct symbol
{
	char name[31];
	int address;
	struct symbol *next;
} Symbol;