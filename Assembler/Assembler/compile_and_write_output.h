#include "pre_compiled.h"
#include "symbols.h"


/* linker info enum */
enum fileStream_enum
{	
	OK,
	CREATE_FILE_ERR,
	CLOSE_FILE_ERR
};

int compile_and_write_output(char *filename);


int create_file_ob(char *filename, CompilerNode *cn_list);
int create_file_ent(char *filename, Symbol *entries_symbols_list);
int create_file_ext(char *filename, Symbol *entries_symbols_list);