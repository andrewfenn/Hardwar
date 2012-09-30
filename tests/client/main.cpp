#include <UnitTest++.h>
#include <TestReporterStdout.h>

int main()
{
    printf("Running hardwar client unit tests..\n");
    return UnitTest::RunAllTests();
}
