enum symbol_type
{
	INSTRUCTION,
	OPCODE
};

typedef struct symbol
{
	char *name;
	int address;
	enum symbol_type type;
	struct symbol *next;
} Symbol;

typedef struct external_symbol
{
	char *name;
	struct external_symbol *next;
} ExternalSymbol;



void add_symbol(char *name, int address, enum symbol_type type);
void add_external_symbol(char *name);