#include <string.h>
#include "CuTest.h"
#include "symbols.h"

void TestSymbol1(CuTest *tc)
{
	SymbolPtr h;
	char result[10];
	add_external_symbol("Test1",0,0);
	add_external_symbol("Test2",0,0);
	h = get_external_symbols_list();
	strcpy(result,h->name);
	CuAssertStrEquals(tc,"Test1",result);
}


CuSuite* SymbolsGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSymbol1);
	return suite;
}

