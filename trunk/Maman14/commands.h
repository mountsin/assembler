typedef struct
{
	char name[5];
	char binary_code[5];
} Command;

Command commands[] = {
						{"mov", "0000"},
						{"cmp", "0001"},
						{"add", "0010"},
						{"sub", "0011"},
						{"not", "0100"},
						{"clr", "0101"},
						{"lea", "0110"},
						{"inc", "0111"},
						{"dec", "1000"},
						{"jmp", "1001"},
						{"bne", "1010"}, 
						{"red", "1011"},
						{"prn", "1100"},
						{"jsr", "1101"},
						{"rts", "1110"},
						{"stop", "1111"}};
							