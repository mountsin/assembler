#include <stdio.h>
#include "CuTest.h"
#include "first_scan.h"

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

CuSuite* FirstScanGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestGetFirstToken);
	SUITE_ADD_TEST(suite, TestGetNextToken);
	SUITE_ADD_TEST(suite, TestIsRegister);
	SUITE_ADD_TEST(suite, TestIsRegister2);
	SUITE_ADD_TEST(suite, TestIsLiteral);
	SUITE_ADD_TEST(suite, TestIsLiteral2);
	return suite;
}
