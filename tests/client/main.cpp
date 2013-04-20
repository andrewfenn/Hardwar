#include <UnitTest++.h>
#include <TestReporterStdout.h>

int main(int argc, char** argv)
{
    printf("Running hardwar client unit tests..\n");
    return UnitTest::RunAllTests();
}
