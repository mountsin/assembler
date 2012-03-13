#define EMPTY -1
#define EMPTY_ARRAY {-1,-1,-1,-1,-1}

//basic data types


//===== enums ==================

//cmd = define command / instruction type
enum cmd {
				mov = 0,
				cmp = 1,
				add = 2,
				sub = 3,
				not = 4,
				clr = 5,
				lea = 6,
				inc = 7,
				dec = 8,
				jmp = 9,
				bne = 10, 
				red = 11,
				prn = 12,
				jsr = 13,
				rts = 14,
				stop = 15,
				data = 16,		//.data
				string = 17,	//.string
				entry  = 18,	//.entry
				external = 19	//.extern
};

//status = define current command status (can be extended)
enum status {
				ok = 0,						//OK
				unknownCmd = 1,				//assembly command is not recognized
				labelAlreadyUsed = 2,		//trying to set a value to the same label already defined 
				tooMuchParams = 3,			//number of parameters exceeding assembly command rules
				missingParams = 4,			//number of parameters is not enough for assembly command rules
				labelNotDefined = 5,		//trying to use a label which not defined in the file
				unknownError = 99			//other error
};

//paramType = define assembly command param type
enum paramTypeEnum {Int = 0,
					Char = 1, 
					String = 2,
					assemblyLabel = 3
};


//=========== Complex Elements ==================================
//cmdParam - represent a parameter of assembly command
typedef struct
{
	paramTypeEnum paramType;
	char *paramData;
}
cmdParam;

//asmCommand - represent an assembly command
typedef struct
{
	char *label;				//string or null
	cmd command;				//cmd enum
	status (*covert2MachineCode)(char *, cmd ,cmdParam,cmdParam);	//HERE IS WHEN YOU PUT YOU FUNCTION
	cmdParam param1;			//first parameter (nullable)
	cmdParam param2;			//second parameter (nullable)
} 
asmCommand;


// commandRule - define an assembly command binary code and legitimate addressing methods (0-4)
typedef struct
{
	cmd  cmdType;
	char binary_code[5];
	int sourceAddressing[5]; //legal source addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
	int destAddressing[5];	 //legal destination addressing typr for that command (cell can be 0-4 or EMPTY if not needed)
} commandRule;


//===================  Tables & Collectors ==========================

//commandRules - table(array) contains each assembly command and its rules
commandRule commandRules[] = {
						mov, "0000", {0,1,2,3,4}, {1,2,3,4,EMPTY},
						cmp, "0001", {0,1,2,3,4}, {0,1,2,3,4},
						add, "0010", {0,1,2,3,4}, {1,2,3,4,EMPTY},
						sub, "0011", {0,1,2,3,4}, {1,2,3,4,EMPTY},
						not, "0100", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						clr, "0101", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						lea, "0110", {1,2,3,EMPTY,EMPTY},{1,2,3,4,EMPTY},
						inc, "0111", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						dec, "1000", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						jmp, "1001", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						bne, "1010", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						red, "1011", EMPTY_ARRAY, {1,2,3,4,EMPTY},
						prn, "1100", EMPTY_ARRAY, {0,1,2,3,4},
						jsr, "1101", EMPTY_ARRAY, {1,EMPTY,EMPTY,EMPTY,EMPTY},
						rts, "1110", EMPTY_ARRAY, EMPTY_ARRAY,
						stop, "1111",EMPTY_ARRAY, EMPTY_ARRAY
};

//Table (linked list of structs) of all assembly Labels
typedef struct labelNode
{
	labelNode *next;  
	cmd type;			//can get enum values 16 - 19 (data, string...)
	int decimalAddr;
}
labelNode;

//errorCollector (linked list of structs) of all errors
typedef struct error
{
	error *next;  
	status errType;			//type of error
	cmd cmdType;			//type of command caused the error
	int lineNumber;			//assembly (input) file line number
}
errorCollector;