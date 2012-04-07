#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "global_functions.h"
#include "first_scan.h"
#include "global_constants.h"
#include "symbols.h"


 


char *get_token(char *text);
 
/**
*  accepts a decimal integer and set a binary coded string into char pointer "binary
*/
void dec2bin(long decimal, char *binary, int binLength)
{
	int  i = 0, n = 0;
	int  neg_flag = 0; //get 1 if number is negative
	int  remain;
	char temp[80];
 
	// take care of negative input
	if (decimal < 0)
	{      
		decimal = -decimal;
		neg_flag = 1;
	}
	do 
	{
		remain    = decimal % 2;
		// whittle down the decimal number
		decimal   = decimal / 2;

		// converts digit 0 or 1 to character '0' or '1'
		temp[i++] = remain + '0';
	} while (decimal > 0);
 

	//add extra zeroes
	while (i < binLength)
	temp[i++] = '0';
   
	// reverse the spelling
	while (i >= 0)
	binary[n++] = temp[--i];

	// end with NULL
	binary[n-1] = 0; 

	if (neg_flag)//if number is negative - convert in 2complement method
		convert_negative_binary_2complement(binary);  
}

/*
* convert to binary using dec2bin.
* if decimal is out of bouds send it with decimal 0
*/
void dec2bin_withBoundries(long decimal, char *binary, int binLength, int low, int high)
{
	if( (decimal < low) || (high < decimal) )
		dec2bin(0, binary, binLength);
	else
		dec2bin(decimal, binary, binLength);
}

/**
* convert a binary string to it's negative equivalent in the Two's complement method
*/
void convert_negative_binary_2complement(char *binary)
{

	int i = 0; 

	while (binary[i]) /*turn bits (1->0, 0->1)*/
	{
		if (binary[i] == '0')
			binary[i++] = '1';
		else
			binary[i++] = '0';
	}

	i--;
	while (i > 0) /*add 1 to binary number - go from right to left*/
	{
		if (binary[i] == '1')
			binary[i--] = '0';
		else
		{
			binary[i--] = '1'; /* end loop */
			break;
		}
	}
	
	binary[0] = '1'; /*mark number as negative*/
}

/* check if string is a binary string containing only 0 or 1*/
enum boolean is_binary_Str(char *str)
{
	int i;

	if( (!str) || (strlen(str) == 0) || (str[0] == NULL) ) /*null / empty string*/
		return FALSE;

	for(i = 0; str[i]; i++)
		if ( (str[i] != '0') && (str[i] != '1') ) /* not a binary string*/
			return FALSE;

	return TRUE;
}


int get_instruction_counter()
{
	return get_IC();
}

int get_data_counter()
{
	return get_DC();
}

/* Utility function that encapsulates the strtok library function */
char *get_first_token(char *text)
{
	return get_token(text);
}

/* Utility function that encapsulates the strtok library function */
char *get_next_token(void)
{
	return get_token(NULL);
}

/* Utility function that encapsulates the strtok library function */
char *get_token(char *text)
{
	char *delimiters = " ,\t\n\r";
	char *temp;
	temp = strtok(text, delimiters);
	return temp;
}

/*check the length of two string and retun the bigger value*/
int get_Longest_length(char *str1 , char *str2)
{
	int length1, length2;
		
	length1 = strlen(str1);
	length2 = strlen(str2);

	if (length1>length2) 
		return length1;
	else
		return length2;
}