# - Find OIS includes and library
#
# This module defines
#  OIS_INCLUDE_DIR
#  OIS_LIBRARIES, the libraries to link against to use OIS.
#  OIS_LIB_DIR, the location of the libraries
#  OIS_FOUND, If false, do not try to use OIS
#
# Copyright © 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (OIS_LIBRARIES AND OIS_INCLUDE_DIR)
	SET(OIS_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (OIS_LIBRARIES AND OIS_INCLUDE_DIR)

IF (WIN32) #Windows
	MESSAGE(STATUS "Looking for OIS")
	
	SET(OGRESDK $ENV{OGRE_HOME})
	SET(OGRESOURCE $ENV{OGRE_SRC})
	IF (OGRESDK)
		MESSAGE(STATUS "Using OIS in OGRE SDK")
		SET(OGRESDK $ENV{OGRE_HOME})
		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
		SET(OIS_INCLUDE_DIR ${OGRESDK}/include/OIS)
		SET(OIS_LIB_DIR ${OGRESDK}/lib)
		SET(OIS_LIBRARIES debug OIS_d optimized OIS)
	ENDIF (OGRESDK)
	IF (OGRESOURCE)
		MESSAGE(STATUS "Using OIS in OGRE dependencies")
            SET(OIS_INCLUDE_DIR C:/ogre/Dependencies/include C:/ogre/Dependencies/include/OIS)
		SET(OIS_LIB_DIR C:/ogre/Dependencies/lib/Release C:/ogre/Dependencies/lib/Debug)
		SET(OIS_LIBRARIES debug OIS_d optimized OIS)
	ENDIF (OGRESOURCE)
ELSE (WIN32) #Unix
	CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
	FIND_PACKAGE(PkgConfig REQUIRED)
	PKG_SEARCH_MODULE(OIS OIS)
	SET(OIS_INCLUDE_DIR ${OIS_INCLUDE_DIRS})
	SET(OIS_LIB_DIR ${OIS_LIBDIR})
	SET(OIS_LIBRARIES ${OIS_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(OIS_INCLUDE_DIR)
SEPARATE_ARGUMENTS(OIS_LIBRARIES)

SET(OIS_INCLUDE_DIR ${OIS_INCLUDE_DIR} CACHE PATH "")
SET(OIS_LIBRARIES ${OIS_LIBRARIES} CACHE STRING "")
SET(OIS_LIB_DIR ${OIS_LIB_DIR} CACHE PATH "")

IF (OIS_INCLUDE_DIR AND OIS_LIBRARIES)
	SET(OIS_FOUND TRUE)
ENDIF (OIS_INCLUDE_DIR AND OIS_LIBRARIES)

IF (OIS_FOUND)
	IF (NOT OIS_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${OIS_LIBRARIES} from ${OIS_LIB_DIR}")
		MESSAGE(STATUS "  includes  : ${OIS_INCLUDE_DIR}")
	ENDIF (NOT OIS_FIND_QUIETLY)
ELSE (OIS_FOUND)
	IF (OIS_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find OIS")
	ENDIF (OIS_FIND_REQUIRED)
ENDIF (OIS_FOUND)
