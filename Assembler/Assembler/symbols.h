enum symbol_type
{
	DATA,
	CODE
};


typedef struct symbol
{
	char *name;
	int address;
	enum symbol_type type;
	struct symbol *next;
} Symbol;

Symbol *symbols_list;

void add_symbol(char *name, int address, enum symbol_type type);