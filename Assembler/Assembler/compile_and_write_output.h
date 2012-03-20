#include "pre_compiled.h"


/* linker info enum */
enum fileStream_enum
{	
	OK,
	CREATE_FILE_ERR,
	CLOSE_FILE_ERR
};

int create_file_ob(char *filename, CompilerNode *cn_list);