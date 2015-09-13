cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(yaarg)

set(yaarg_MAJOR_VERSION 1)
set(yaarg_MINOR_VERSION 0)
set(yaarg_PATCH_VERSION 0)

set(yaarg_VERSION ${yaarg_MAJOR_VERSION}.${yaarg_MINOR_VERSION}.${yaarg_PATCH_VERSION})
set(yaarg_COMPATIBILITY_VERSION ${yaarg_MAJOR_VERSION}.${yaarg_MINOR_VERSION}.0)
set(yaarg_SOVERSION ${yaarg_MAJOR_VERSION}.${yaarg_MINOR_VERSION}.0)

set(yaarg_LIBRARY_NAME yaarg)

set(yaarg_source
    config-parser.cc
    config-parser-argv.cc
    config-parser-options.cc
)
source_group("yaarg" FILES ${yaarg_source})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(yaarg STATIC ${yaarg_source})

if(UNIX)
	set_target_properties(yaarg PROPERTIES
		VERSION ${yaarg_VERSION}
		SOVERSION ${yaarg_SOVERSION}
	)
endif()

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

if (WINDOWS)
else () # unix
     set_source_files_properties(${yaarg_source} PROPERTIES COMPILE_FLAGS "")
     set(CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -Wformat-nonliteral -Wextra -Wstrict-overflow=5 -Wfloat-equal -Wconversion -Wlogical-op -fstack-protector-all -Wno-unused-parameter -fno-exceptions -std=c++98")
endif ()

