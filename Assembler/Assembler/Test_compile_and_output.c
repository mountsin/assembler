#include <stdio.h>
#include "CuTest.h"
#include "compile_and_write_output.h"
#include "data_structs.h"
//#include "symbols.h"

void Test_create_file_ob(CuTest *tc)
{
	char *filename = "testerObj";
	char *label = "MAIN";
	CompilerNode *cn_list;
	
	/*set cn_list*/
	add_compiler_node(label,500,MOV,IMMEDIATE,IMMEDIATE,NULL,NULL,4);
	cn_list = get_compiler_nodes_list();
	cn_list->linker_flag = ABSOLUTE;
	cn_list->binary_machine_code = "1011111010111110";
	
	create_file_ob(filename, cn_list);
}

void Test_create_file_ent(CuTest *tc)
{
	char *filename = "testerEnt";
	Symbol *ent_sym_list;
	
	/*set cn_list*/
	add_entries_symbol("MAIN",101);
	add_entries_symbol("ADD",150);
	add_entries_symbol("ENTRY",2251);

	ent_sym_list = get_entries_symbols_list();
	
	create_file_ent(filename, ent_sym_list);
}

void Test_create_file_ext(CuTest *tc)
{
	char *filename = "testerEnt";
	Symbol *ext_sym_list;
	
	/*set cn_list*/
	add_external_symbol("EXT1",23);
	add_external_symbol("EXT2",57);
	add_external_symbol("EXT3",2261);

	ext_sym_list = get_external_symbols_list();
	
	create_file_ext(filename, ext_sym_list);
}
/*
void TestGetNextToken(CuTest *tc)
{
	char input[] = "		.entry		STRADD";
	char *actual;
	char *expected =  "STRADD";
	get_first_token(input);
	actual = get_next_token();
	
	CuAssertStrEquals(tc, expected, actual);
}
*/


CuSuite* Compile_and_outputGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, Test_create_file_ob);
	SUITE_ADD_TEST(suite, Test_create_file_ent);
	SUITE_ADD_TEST(suite, Test_create_file_ext);
	
	return suite;
}

