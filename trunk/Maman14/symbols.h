typedef struct symbole
{
	char *name;
	int address;
	struct symbole *next;
	struct symbole *prev;

} Symbole;

Symbole *symboles_list;

void add_symbole(char *name, int address);