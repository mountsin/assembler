#include <stdio.h>
#include "CuTest.h"
#include "AllTests.h"

CuSuite* FirstScanGetSuite();
CuSuite* SecondScanGetSuite();
CuSuite* Compile_and_outputGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, FirstScanGetSuite());
	//CuSuiteAddSuite(suite, SecondScanGetSuite());
	//CuSuiteAddSuite(suite, Compile_and_outputGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}