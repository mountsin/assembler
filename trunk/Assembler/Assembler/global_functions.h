void dec2bin(long decimal, char *binary, int binLength);
void dec2bin_withBoundries(long decimal, char *binary, int binLength, int low, int high);

void convert_negative_binary_2complement(char *binary);
char *get_first_token(char *text);
char *get_next_token(void);
int get_Longest_length(char *str1 , char *str2);
enum boolean is_binary_Str(char *str);
void dispose(void *p, ...);