cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(TinyXML)

set(TinyXML_MAJOR_VERSION 2)
set(TinyXML_MINOR_VERSION 5)
set(TinyXML_PATCH_VERSION 3)

set(TinyXML_VERSION ${TinyXML_MAJOR_VERSION}.${TinyXML_MINOR_VERSION}.${TinyXML_PATCH_VERSION})
set(TinyXML_COMPATIBILITY_VERSION ${TinyXML_MAJOR_VERSION}.${TinyXML_MINOR_VERSION}.0)
set(TinyXML_SOVERSION ${TinyXML_MAJOR_VERSION}.${TinyXML_MINOR_VERSION}.0)

set(TinyXML_source
    tinystr.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

set(TinyXML_LIBRARY_NAME TinyXML)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

source_group("TinyXML" FILES ${TinyXML_source})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(TinyXML STATIC ${TinyXML_source})

IF(UNIX)
	set_target_properties(TinyXML PROPERTIES
		VERSION ${TinyXML_VERSION}
		SOVERSION ${TinyXML_SOVERSION}
	)

    set(CMAKE_CXX_FLAGS -fPIC)
ENDIF(UNIX)

set_target_properties(TinyXML PROPERTIES SOLUTION_FOLDER Dependencies)

