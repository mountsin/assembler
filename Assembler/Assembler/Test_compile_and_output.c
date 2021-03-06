#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "compile_and_write_output.h"

void Test_create_file_ob(CuTest *tc)
{
	char *filename = "testerObj";
	char *label = "MAIN";
	char *machine_code = "1011111010111110";
	CompilerNode *cn_list;
	CompilerNode *stmt = (CompilerNode *)malloc(sizeof(CompilerNode));
	
	stmt->address = 122;
	stmt->cmd_type = MOV;
	stmt->second_scan_type = SKIP;
	strcpy(stmt->label,"LABEL1");
	stmt->line_number = 111;
	stmt->linker_flag = ABSOLUTE;
	stmt->sourceAddressing = DIRECT;
	stmt->source_operand = "#3";
	stmt->targetAddressing = DIRECT;
	stmt->target_operand = "var1"; 



	/*set cn_list*/
	add_code_node(stmt);
	cn_list = get_code_list_head();
	strcpy(cn_list->binary_machine_code, machine_code);
	cn_list->linker_flag = ABSOLUTE;
	cn_list->sourceAddressing = DIRECT;
	cn_list->source_operand = "#3";
	cn_list->targetAddressing = DIRECT;
	cn_list->target_operand = "var1"; 
	
	create_file_ob(filename, cn_list);
}

void Test_create_file_ent(CuTest *tc)
{
	char *filename = "testerEnt";
	Symbol *ent_sym_list;
	
	/*set cn_list*/
	add_entries_symbol("MAIN",101,0);
	add_entries_symbol("ADD",150,0);
	add_entries_symbol("ENTRY",2251,0);

	ent_sym_list = get_entries_symbols_list();
	
	create_file_ent(filename, ent_sym_list);
}

void Test_create_file_ext(CuTest *tc)
{
	char *filename = "testerEnt";
	Symbol *ext_sym_list;
	
	/*set cn_list*/
	add_external_symbol("EXT1",23,0);
	add_external_symbol("EXT2",57,0);
	add_external_symbol("EXT3",2261,0);

	ext_sym_list = get_external_symbols_list();
	
	create_file_ext(filename, ext_sym_list);
}

void Test_runFileCreators_lite(CuTest *tc)
{
	create_file_ob("tester", get_code_list_head());
	create_file_ent("tester", get_entries_symbols_list());
	create_file_ext("tester", get_external_symbols_list());
}

void Test_compile_and_write_output(CuTest *tc)
{
	compile_and_write_output("tester");
}

CuSuite* Compile_and_outputGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	//SUITE_ADD_TEST(suite, Test_runFileCreators_lite);

	SUITE_ADD_TEST(suite, Test_compile_and_write_output);

	/*Test_runFileCreators_liteTest_create_file_ob);
	SUITE_ADD_TEST(suite, Test_create_file_ent);
	SUITE_ADD_TEST(suite, Test_create_file_ext);
	*/
	return suite;
}

