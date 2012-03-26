#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "compile_and_write_output.h"
#include "data_structs.h"
//#include "symbols.h"

void Test_create_file_ob(CuTest *tc)
{
	char *filename = "testerObj";
	char *label = "MAIN";
	char *machine_code = "1011111010111110";
	CompilerNode *cn_list;
	CompilerNode *stmt = (CompilerNode *)malloc(sizeof(CompilerNode));
	
	stmt->address = 122;
	stmt->cmd_type = MOV;
	stmt->is_second_scan_needed = FALSE;
	strcpy(stmt->label,"LABEL1");
	stmt->line_number = 111;
	stmt->linker_flag = ABSOLUTE;
	stmt->sourceAddressing = DIRECT;
	stmt->source_operand = "#3";
	stmt->targetAddressing = DIRECT;
	stmt->target_operand = "var1"; 



	/*set cn_list*/
	add_compiler_node(stmt);
	cn_list = get_compiler_nodes_list_head();
	cn_list->binary_machine_code = machine_code;
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

void Test_runFileCreators_lite(CuTest *tc)
{
	create_file_ob("tester", get_compiler_nodes_list_head());
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

