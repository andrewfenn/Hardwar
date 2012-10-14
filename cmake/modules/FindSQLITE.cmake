# - Try to find the SQLITE library
# Once done this will define
#
#  SQLITE_FOUND - system has sqlite
#  SQLITE_INCLUDE_DIR - the sqlite include directory
#  SQLITE_LIBRARIES - Link these to use sqlite
#  SQLITE_DEFINITIONS - Compiler switches required for using sqlite
#

if (UNIX)
	FIND_PACKAGE(PkgConfig)
	PKG_SEARCH_MODULE(SQLITE sqlite3)
	SET(SQLITE_INCLUDE_DIR ${SQLITE_INCLUDE_DIRS})
	SET(SQLITE_LIB_DIR ${SQLITE_LIBDIR})
	SET(SQLITE_LIBRARIES ${SQLITE_LIBRARIES} CACHE STRING "")

	SEPARATE_ARGUMENTS(SQLITE_INCLUDE_DIR)
	SEPARATE_ARGUMENTS(SQLITE_LIBRARIES)

	SET(SQLITE_INCLUDE_DIR ${SQLITE_INCLUDE_DIR} CACHE PATH "")
	SET(SQLITE_LIBRARIES ${SQLITE_LIBRARIES} CACHE STRING "")
	SET(SQLITE_LIB_DIR ${SQLITE_LIB_DIR} CACHE PATH "")
endif ()

if (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARIES)
	SET(SQLITE_FOUND True)
elseif (WIN32)
	FIND_PATH(SQLITE_INCLUDE_DIR sqlite3.h
		HINTS
		PATH_SUFFIXES sqlite3
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
		${CMAKE_SOURCE_DIR}/dependencies
	)

	FIND_LIBRARY(SQLITE_LIBRARY
		NAMES SQLite.lib
		HINTS
		PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 lib/Debug lib/Release
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw
		/opt/local
		/opt/csw
		/opt
		${CMAKE_SOURCE_DIR}
	)

	if (SQLITE_INCLUDE_DIR)
		message(" have include dir")
	endif ()

	if (SQLITE_LIBRARY)
		message(" have lib dir")
	endif ()

	if (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARY)
		SET(SQLITE_FOUND True)
	endif ()
endif ()
