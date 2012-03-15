void first_scan(char *filename);
void read_file(char *filename);
void read_line_and_build_statement_struct(char *line);
enum cmd parse_command(char *command_name);
void debug_output(char *what);
char *get_next_token(char *delimiters);
void validate_label(char *token, char *delimiters, AssemblyStatement *stmt);
void process_statement(AssemblyStatement stmt);