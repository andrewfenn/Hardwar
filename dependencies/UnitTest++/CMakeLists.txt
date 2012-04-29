cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(UnitTest)

set(UnitTest_MAJOR_VERSION 1)
set(UnitTest_MINOR_VERSION 4)
set(UnitTest_PATCH_VERSION 207)

set(UnitTest_VERSION ${UnitTest_MAJOR_VERSION}.${UnitTest_MINOR_VERSION}.${UnitTest_PATCH_VERSION})
set(UnitTest_COMPATIBILITY_VERSION ${UnitTest_MAJOR_VERSION}.${UnitTest_MINOR_VERSION}.0)
set(UnitTest_SOVERSION ${UnitTest_MAJOR_VERSION}.${UnitTest_MINOR_VERSION}.0)

set(UnitTest_source
    src/AssertException.cpp
    src/Checks.cpp
    src/CurrentTest.cpp
    src/DeferredTestReporter.cpp
    src/DeferredTestResult.cpp
    src/MemoryOutStream.cpp
    src/ReportAssert.cpp
    src/Test.cpp
    src/TestDetails.cpp
    src/TestList.cpp
    src/TestReporter.cpp
    src/TestReporterStdout.cpp
    src/TestResults.cpp
    src/TestRunner.cpp
    src/TimeConstraint.cpp
    src/XmlTestReporter.cpp
)

if(WINDOWS)
    set(UnitTest_extras_source
        src/Win32/TimeHelpers.cpp
    )
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Win32)
else()
    set(UnitTest_extras_source
        src/Posix/SignalTranslator.cpp
        src/Posix/TimeHelpers.cpp
    )
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Posix)
endif()

set(UnitTest_LIBRARY_NAME UnitTest)
set(LIBRARY_OUTPUT_PATH  ../../lib)

source_group("UnitTest" FILES ${UnitTest_source} ${UnitTest_extras_source})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(UnitTest SHARED ${UnitTest_source} ${UnitTest_extras_source})

if(UNIX)
	set_target_properties(UnitTest PROPERTIES
		VERSION ${UnitTest_VERSION}
		SOVERSION ${UnitTest_SOVERSION}
	)

    set(UnitTest_DEFINITIONS "${UnitTest_DEFINITIONS} -g -Wall -W -Winline -ansi")
    set(CMAKE_CXX_FLAGS -fPIC)
endif()

set_target_properties(UnitTest PROPERTIES SOLUTION_FOLDER Dependencies)

#########################################
# UnitTest's Unit Tests
#########################################

project(UnitTestTest)

set(UnitTestTest_source
    src/tests/Main.cpp
    src/tests/TestAssertHandler.cpp
    src/tests/TestCheckMacros.cpp
    src/tests/TestChecks.cpp
    src/tests/TestCurrentTest.cpp
    src/tests/TestDeferredTestReporter.cpp
    src/tests/TestMemoryOutStream.cpp
    src/tests/TestTest.cpp
    src/tests/TestTestList.cpp
    src/tests/TestTestMacros.cpp
    src/tests/TestTestResults.cpp
    src/tests/TestTestRunner.cpp
    src/tests/TestTestSuite.cpp
    src/tests/TestTimeConstraint.cpp
    src/tests/TestTimeConstraintMacro.cpp
    src/tests/TestUnitTest++.cpp
    src/tests/TestXmlTestReporter.cpp
)
source_group("UnitTestTest" FILES ${UnitTestTest_source})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/tests/)
include_directories(SYSTEM ${UnitTest_INCLUDE_DIR})

link_libraries(UnitTest)

set(EXECUTABLE_OUTPUT_PATH ${HARDWAR_BINARY_DIR}/tests/)
add_executable(UnitTestTest ${exe} ${UnitTestTest_source})

add_dependencies(UnitTestTest UnitTest)

if(UNIX)
	set_target_properties(UnitTestTest PROPERTIES
		VERSION ${UnitTest_VERSION}
		SOVERSION ${UnitTest_SOVERSION}
	)

    set(UnitTestTest_DEFINITIONS "${UnitTestTest_DEFINITIONS} -g")
endif()

Add_Custom_Command (TARGET UnitTestTest POST_BUILD COMMAND ${HARDWAR_BINARY_DIR}/tests/UnitTestTest)
