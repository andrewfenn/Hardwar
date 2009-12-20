# - Try to find the SQLITE library
# Once done this will define
#
#  SQLITE_FOUND - system has sqlite
#  SQLITE_INCLUDE_DIR - the sqlite include directory
#  SQLITE_LIBRARIES - Link these to use sqlite
#  SQLITE_DEFINITIONS - Compiler switches required for using sqlite
#

IF (SQLITE_LIBRARIES AND SQLITE_INCLUDE_DIR)
	SET(SQLITE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (SQLITE_LIBRARIES AND SQLITE_INCLUDE_DIR)

IF (WIN32) #Windows
	MESSAGE(STATUS "Using SQLite3 in dependencies")
	SET(SQLITE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/sqlite3)
   ADD_LIBRARY(sqlite3_${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/dependencies/sqlite3/sqlite3.c)
	SET( SQLITE_LIBRARIES sqlite3_${PROJECT_NAME})
	INCLUDE_DIRECTORIES("${SQLITE_INCLUDE_DIR}") 
ELSE (WIN32) #Unix
	CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
	FIND_PACKAGE(PkgConfig)
	PKG_SEARCH_MODULE(SQLITE sqlite3)
	SET(SQLITE_INCLUDE_DIR ${SQLITE_INCLUDE_DIRS})
	SET(SQLITE_LIB_DIR ${SQLITE_LIBDIR})
	SET(SQLITE_LIBRARIES ${SQLITE_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(SQLITE_INCLUDE_DIR)
SEPARATE_ARGUMENTS(SQLITE_LIBRARIES)

SET(SQLITE_INCLUDE_DIR ${SQLITE_INCLUDE_DIR} CACHE PATH "")
SET(SQLITE_LIBRARIES ${SQLITE_LIBRARIES} CACHE STRING "")
SET(SQLITE_LIB_DIR ${SQLITE_LIB_DIR} CACHE PATH "")

IF (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARIES)
	SET(SQLITE_FOUND TRUE)
ENDIF (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARIES)

IF (SQLITE_FOUND)
	IF (NOT SQLITE_FIND_QUIETLY)
      MESSAGE(STATUS "SQLite Found")
		MESSAGE(STATUS "  libraries : ${SQLITE_LIBRARIES} from ${SQLITE_LIB_DIR}")
		MESSAGE(STATUS "  includes  : ${SQLITE_INCLUDE_DIR}")
	ENDIF (NOT SQLITE_FIND_QUIETLY)
ELSE (SQLITE_FOUND)
	IF (SQLITE_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find SQLite3")
	ENDIF (SQLITE_FIND_REQUIRED)
ENDIF (SQLITE_FOUND)
