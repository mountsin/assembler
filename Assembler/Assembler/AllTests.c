#include <stdio.h>
#include "CuTest.h"
#include "AllTests.h"
//#include "Test_compile_and_output.h"

CuSuite* FirstScanGetSuite();
CuSuite* Compile_and_outputGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, FirstScanGetSuite());
	CuSuiteAddSuite(suite, Compile_and_outputGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}