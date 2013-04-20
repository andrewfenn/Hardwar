cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(Amop)

set(Amop_MAJOR_VERSION 0)
set(Amop_MINOR_VERSION 30)
set(Amop_PATCH_VERSION 1)

set(Amop_VERSION ${Amop_MAJOR_VERSION}.${Amop_MINOR_VERSION}.${Amop_PATCH_VERSION})
set(Amop_COMPATIBILITY_VERSION ${Amop_MAJOR_VERSION}.${Amop_MINOR_VERSION}.0)
set(Amop_SOVERSION ${Amop_MAJOR_VERSION}.${Amop_MINOR_VERSION}.0)

set(Amop_source
    source/CheckOffsetFunc.cpp
    source/MockObjectBase.cpp
    source/MockObject.cpp
)

set(Amop_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(Amop_LIBRARY_NAME Amop)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

source_group("Amop" FILES ${Amop_source})
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/dependencies/UnitTest++/src")
include_directories(${Amop_INCLUDE_DIR})

link_libraries(UnitTest)

add_library(Amop SHARED ${Amop_source})

add_dependencies(Amop UnitTest)

if(UNIX)
	set_target_properties(Amop PROPERTIES
		VERSION ${Amop_VERSION}
		SOVERSION ${Amop_SOVERSION}
	)

    set(Amop_DEFINITIONS "${Amop_DEFINITIONS} -g -Wall -W -Winline -ansi")
    set(CMAKE_CXX_FLAGS -fPIC)
endif()

set_target_properties(Amop PROPERTIES SOLUTION_FOLDER Dependencies)

#########################################
# Amop's Unit Tests
#########################################

project(AmopTest)

set(AmopTest_source
    source/test/Main.cpp
    source/test/TestMockObject.cpp
    source/test/TestTemplateHelppers.cpp
    source/test/TestVirtualFunctionRedirect.cpp
)
source_group("AmopTest" FILES ${AmopTest_source})

message(STATUS "amop: ${Amop_INCLUDE_DIR}")

include_directories(SYSTEM ${Amop_INCLUDE_DIR})

link_libraries(UnitTest Amop)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests/)
add_executable(AmopTest ${exe} ${AmopTest_source})

add_dependencies(AmopTest UnitTest)
add_dependencies(AmopTest Amop)

if(UNIX)
    set(AmopTest_DEFINITIONS "${AmopTest_DEFINITIONS} -g")
endif()

set_target_properties(AmopTest PROPERTIES SOLUTION_FOLDER UnitTests)

add_custom_command(TARGET AmopTest POST_BUILD COMMAND ${CMAKE_BINARY_DIR}/bin/tests/AmopTest)
