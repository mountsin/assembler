#include <stdio.h>
#include <string.h>
#include "CuTest.h"
#include "first_scan.h"
#include "data_structs.h"
#include "pre_compiled.h"

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
	set_symbol("STR[%LEN]",result);
	CuAssertStrEquals(tc,"STR",result);
}
void TestSetSymbol2(CuTest *tc)
{
	char result[10] = "";
	set_symbol("[K]LASTCHAR[R3]",result);
	CuAssertStrEquals(tc,"LASTCHAR",result);
}

void TestSetIndex(CuTest *tc)
{
	char result[10] = "";
	set_index("STR[%LEN]",result);
	CuAssertStrEquals(tc,"%LEN",result);
}
void TestSetIndex2(CuTest *tc)
{
	char result[10] = "";
	set_index("[K]LASTCHAR[R3]",result);
	CuAssertStrEquals(tc,"K",result);
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
	return suite;
}
