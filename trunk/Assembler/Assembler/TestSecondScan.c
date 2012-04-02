#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "pre_compiled.h"
#include "data_structs.h"
#include "second_scan.h"
#include "symbols.h"

void Test_second_scan(CuTest *tc)
{

	char *filename = "testerObj";
	char *label = "MAIN";
	CompilerNode *cn_list;
	CompilerNode *stmt = (CompilerNode *)malloc(sizeof(CompilerNode));
	
	stmt->address = 100;
	stmt->cmd_type = JSR;
	strcpy(stmt->label,"MAIN");
	stmt->line_number = 111;
	stmt->sourceAddressing = DIRECT;
	stmt->source_operand = "#3";
	stmt->targetAddressing = DIRECT;
	stmt->target_operand = "var1"; 

	/*set cn_list*/
	add_compiler_node(stmt);
	cn_list = get_compiler_nodes_list_head();
	strcpy(cn_list->binary_machine_code,"1011111010111110");
	cn_list->linker_flag = ABSOLUTE;
	cn_list->second_scan_type = SKIP; //TODO: Yuval please verify me
	cn_list->sourceAddressing = DIRECT;
	cn_list->source_operand = "#3";
	cn_list->targetAddressing = DIRECT;
	cn_list->target_operand = "var1";

	//====== node 2 ======//
	stmt->address = 101;
	stmt->cmd_type = JSR;
	strcpy(stmt->label,"");
	stmt->line_number = 111;
	stmt->sourceAddressing = DIRECT;
	stmt->source_operand = "#3";
	stmt->targetAddressing = DIRECT;
	stmt->target_operand = "var1";

	add_compiler_node(stmt);
	cn_list = cn_list->next;
	strcpy(cn_list->binary_machine_code,"LABEL1");
	cn_list->second_scan_type != SKIP; //TODO: Yuval please verify me
	cn_list->linker_flag = NONE;
	cn_list->sourceAddressing = DIRECT;
	cn_list->source_operand = "#3";
	cn_list->targetAddressing = DIRECT;
	cn_list->target_operand = "var1";

	//====== node 2 ======//
	stmt->address = 102;
	stmt->cmd_type = LEA;
	strcpy(stmt->label,"");
	stmt->line_number = 111;
	stmt->sourceAddressing = DIRECT;
	stmt->source_operand = "#3";
	stmt->targetAddressing = DIRECT;
	stmt->target_operand = "var1";

	add_compiler_node(stmt);
	cn_list = cn_list->next;
	strcpy(cn_list->binary_machine_code,"EXT1");
	cn_list->second_scan_type != SKIP; //TODO: Yuval please verify me
	cn_list->linker_flag = NONE;
	cn_list->sourceAddressing = DIRECT;
	cn_list->source_operand = "#3";
	cn_list->targetAddressing = DIRECT;
	cn_list->target_operand = "var1";


	//=================================================
			/*set data / string*/
	add_data_symbol("MAIN",20);
	add_data_symbol("LABEL1",21);
	add_data_symbol("LABEL2",22);

		/*set entries*/
	add_entries_symbol("MAIN",20);
	add_entries_symbol("ADD",101);
	add_entries_symbol("ENTRY",102);

		/*set externals*/
	//add_external_symbol("EXT1",23);
	add_external_symbol("EXT2",57);
	add_external_symbol("EXT3",2261);

	second_scan();
	//CuAssertStrEquals(tc, expected, actual);
}
//void second_scan(CompilerNode *compiler_node_header)

CuSuite* SecondScanGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, Test_second_scan);
	return suite;
}
