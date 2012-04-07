typedef struct symbol *SymbolPtr;

SymbolPtr create_symbol();
void destroy_symbol(SymbolPtr symbol);
void add_data_symbol(char *name, int address, int line_number);
void add_code_symbol(char *name, int address, int line_number);
void add_entries_symbol(char *name, int address, int line_number);
void add_external_symbol(char *name, int address, int line_number);

SymbolPtr get_entries_symbols_list();
SymbolPtr get_external_symbols_list();
/* Search for the symbole name in the data list and in the externals list and returns the symbol struct or NULL if not found */
SymbolPtr get_data_symbol_by_name(char *name_to_find);
SymbolPtr get_code_symbol_by_name(char *name_to_find);
SymbolPtr get_external_symbol_by_name(char *name_to_find);
SymbolPtr get_entry_symbol_by_name(char *name_to_find);

/*symbols output Files lists*/
//void add_entriesFile_row(char *name, int address, int line_number);
void add_externalFile_row(char *name, int address, int line_number);

SymbolPtr get_externalFile_head();

/*free symbols list memory allocations*/
void free_all_symbols_lists();
void free_symbols_list(SymbolPtr symbols_list_head);

/*copy fields of source symbol to taget symbol*/
void copy_symbol_contant(SymbolPtr source, SymbolPtr target);

typedef struct symbol
{
	char name[31];
	int address;
	struct symbol *next;
} Symbol;