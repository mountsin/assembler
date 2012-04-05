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

void TestGet_data_symbol_by_name(CuTest *tc)
{
	SymbolPtr h1,h2,h3;
	add_data_symbol("STR", 11, 14);
	add_data_symbol("STRADD", 12, 14);
	
	h1 = get_data_symbol_by_name("STRADD");
	h2 = get_data_symbol_by_name("STR");
	h3 = get_data_symbol_by_name("NULL");
	if (h3 != NULL)
		strcpy("error",h1->name);


	CuAssertStrEquals(tc,"STRADD",h1->name);
	CuAssertStrEquals(tc,"STR",h2->name);
}


CuSuite* SymbolsGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSymbol1);
	SUITE_ADD_TEST(suite, TestGet_data_symbol_by_name);
	return suite;
}

