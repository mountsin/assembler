#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "CuTest.h"
#include "first_scan.h"
#include "global_functions.h"
#include "global_constants.h"
#include "pre_compiled.h"
#include "error.h"

void TestGetFirstToken(CuTest *tc)
{
	char input[] = "		.entry		STRADD";
	char *actual = get_first_token(input);
	char *expected =  ".entry";
	CuAssertStrEquals(tc, expected, actual);
}

void TestGetNextToken(CuTest *tc)
{
	char input[] = "		.entry		STRADD";
	char *actual;
	char *expected =  "STRADD";
	get_first_token(input);
	actual = get_next_token();
	
	CuAssertStrEquals(tc, expected, actual);
}

void TestIsRegister(CuTest *tc)
{
	char input[] = "r1";
	CuAssertTrue(tc,is_register(input));
}
void TestIsRegister2(CuTest *tc)
{
	char input[] = "r9";
	CuAssertTrue(tc,!is_register(input));
}

void TestIsLiteral(CuTest *tc)
{
	char input[] = "#-1";
	CuAssertTrue(tc, is_literal(input));
}

void TestIsLiteral2(CuTest *tc)
{
	char input[] = "1";
	CuAssertTrue(tc, !is_literal(input));
}

void TestIsIndex(CuTest *tc)
{
	char input[] = "y[%j]";
	CuAssertTrue(tc,is_index(input));
}

void TestIsDoubleIndex(CuTest *tc)
{
	char input[] = "[j]y[r5]";
	CuAssertTrue(tc,is_double_index(input));
}

void TestStrTok(CuTest *tc)
{
	char input[] = "		.entry		STRADD";
	char *delimiters = " ,\t\n\r";
	char *result = strtok(input,delimiters);
	CuAssertStrEquals(tc,".entry",result);
}

void TestSetSymbol(CuTest *tc)
{
	char result[10] = "";
	extract_symbol("STR[%LEN]",result);
	CuAssertStrEquals(tc,"STR",result);
}
void TestSetSymbol2(CuTest *tc)
{
	char result[10] = "";
	extract_symbol("[K]LASTCHAR[R3]",result);
	CuAssertStrEquals(tc,"LASTCHAR",result);
}

void TestSetIndex(CuTest *tc)
{
	char result[10] = "";
	SecondScanType type;
	extract_index("STR[%LEN]",result,&type);
	CuAssertStrEquals(tc,"LEN",result);
}
void TestSetIndex2(CuTest *tc)
{
	char result[10] = "";
	SecondScanType type;
	extract_index("[K]LASTCHAR[R3]",result, &type);
	CuAssertStrEquals(tc,"K",result);
}

void TestCreateCompilerNodePtr(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	CuAssertPtrNotNull(tc,p);
}

void TestReadLineCmdType(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "		.entry		STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertIntEquals(tc,ENTRY,p->cmd_type);
}

void TestReadLineLabel(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "		.entry		STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertStrEquals(tc,"",p->label);
}

void TestReadLineCmdTypeLea(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "MAIN:		lea		STR[%LEN], STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertIntEquals(tc,LEA,p->cmd_type);
}

void TestReadLineLabelLea(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "MAIN:		lea		STR[%LEN], STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertStrEquals(tc,"MAIN",p->label);
}

void TestReadLineIsSecondNeededLea(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "MAIN:		lea		STR[%LEN], STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertIntEquals(tc,SKIP,p->second_scan_type);
}

void TestReadLineBinaryLea(CuTest *tc)
{
	CompilerNodePtr p = create_compiler_node();
	char line[] = "MAIN:		lea		STR[%LEN], STRADD";
	read_line_and_set_compiler_node(line,p);
	CuAssertStrEquals(tc,"0110010000001000",p->binary_machine_code);
}

void TestDec2Bin(CuTest *tc)
{
	char binary_machine_code[17];
	char bin_code[5];
	binary_machine_code[0] = '\0';
	dec2bin(LEA,bin_code,4);
	bin_code[4] = '\0';
	strcat(binary_machine_code,bin_code);
	CuAssertStrEquals(tc,"0110",binary_machine_code);
}

void TestAddExtSymbol(CuTest *tc)
{
	char line[] = "		.extern		PRTSTR";
	CompilerNodePtr p = create_compiler_node();
	read_line_and_set_compiler_node(line,p);
	CuAssertPtrEquals(tc,NULL,get_errors_list());

}

void TestIsValidLabel(CuTest *tc)
{
	char *token = "MAIN:";
	char *line = " MAIN:";
	CompilerNodePtr p = create_compiler_node();
	CuAssertTrue(tc,!is_valid_label(token,p,line));
}

void TestIsValidLabel2(CuTest *tc)
{
	char *token = "MAIN:";
	char *line = "MAIN:";
	CompilerNodePtr p = create_compiler_node();
	CuAssertTrue(tc,is_valid_label(token,p,line));
}

void TestIsValidLabel3(CuTest *tc)
{
	char *token = "MAIN:";
	char *line = "\tMAIN:";
	CompilerNodePtr p = create_compiler_node();
	CuAssertTrue(tc,!is_valid_label(token,p,line));
}

void TestAtoi(CuTest *tc)
{
	char *str = "-5";
	int i = atoi(str);
	CuAssertTrue(tc,-5 == i);
}

void TestIsNumber(CuTest *tc)
{
	int n;
	CuAssertTrue(tc,try_parse_number("+5",&n));
}

void TestIsNumber2(CuTest *tc)
{
	int n;
	CuAssertTrue(tc,!try_parse_number("+",&n));
}

void TestString(CuTest *tc)
{
	CuAssertIntEquals(tc,TRUE,is_valid_string("\"abcde\""));
}

void TestString2(CuTest *tc)
{
	CuAssertIntEquals(tc,FALSE,is_valid_string("abcde"));
}

void TestDec2bin(CuTest *tc)
{
	int x = 5;
	char binary[17];
	x = -x;
	CuAssertIntEquals(tc,-5,x);
	dec2bin(x,binary,16);
	CuAssertStrEquals(tc,"1111111111111011",binary);
}

void TestExtractSecondIndex(CuTest *tc)
{
	char result[10] = "";
	extract_second_index("[K]LASTCHAR[R3]",result);
	CuAssertStrEquals(tc,"R3",result);
}

CuSuite* FirstScanGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestGetFirstToken);
	SUITE_ADD_TEST(suite, TestGetNextToken);
	SUITE_ADD_TEST(suite, TestIsRegister);
	SUITE_ADD_TEST(suite, TestIsRegister2);
	SUITE_ADD_TEST(suite, TestIsLiteral);
	SUITE_ADD_TEST(suite, TestIsLiteral2);
	SUITE_ADD_TEST(suite, TestIsIndex);
	SUITE_ADD_TEST(suite, TestIsDoubleIndex);
	SUITE_ADD_TEST(suite, TestStrTok);
	SUITE_ADD_TEST(suite, TestSetSymbol);
	SUITE_ADD_TEST(suite, TestSetSymbol2);
	SUITE_ADD_TEST(suite, TestSetIndex);
	SUITE_ADD_TEST(suite, TestSetIndex2);
	SUITE_ADD_TEST(suite,TestCreateCompilerNodePtr);
	SUITE_ADD_TEST(suite,TestReadLineCmdType);
	SUITE_ADD_TEST(suite,TestReadLineLabel);
	SUITE_ADD_TEST(suite,TestReadLineCmdTypeLea);
	SUITE_ADD_TEST(suite,TestReadLineLabelLea);
	SUITE_ADD_TEST(suite,TestReadLineIsSecondNeededLea);
	SUITE_ADD_TEST(suite,TestAddExtSymbol);
	SUITE_ADD_TEST(suite,TestIsValidLabel);
	SUITE_ADD_TEST(suite,TestIsValidLabel2);
	SUITE_ADD_TEST(suite,TestIsValidLabel3);
	SUITE_ADD_TEST(suite,TestAtoi);
	SUITE_ADD_TEST(suite,TestIsNumber);
	SUITE_ADD_TEST(suite,TestIsNumber2);
	SUITE_ADD_TEST(suite,TestString);
	SUITE_ADD_TEST(suite,TestString2);
	SUITE_ADD_TEST(suite,TestDec2bin);
	SUITE_ADD_TEST(suite, TestExtractSecondIndex);
	return suite;
}


