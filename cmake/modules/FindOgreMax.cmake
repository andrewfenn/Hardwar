# Find OgreMax includes and library
#
# This module defines
#  OgreMax_INCLUDE_DIR
#  OgreMax_LIB_DIR, the location of the libraries
#  OgreMax_FOUND, If false, do not try to use OgreMax
#
# Copyright © 2009-2010, Andrew Fenn
#

set(paths
 ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  ${CMAKE_SOURCE_DIR}
  )

set(suffixes lib64 lib libs64 libs libs/Win32 libs/Win64)

FIND_PATH(OgreMax_INCLUDE_DIR OgreMaxScene.hpp
  HINTS
  $ENV{OgreMaxDIR}
  PATH_SUFFIXES OgreMax
  PATHS
  ${paths}
  ${CMAKE_SOURCE_DIR}/dependencies/
)

FIND_LIBRARY(OgreMax_LIBRARY_REL
  NAMES libOgreMax libOgreMax.a OgreMax.lib 
  HINTS $ENV{OgreMaxDIR} PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 lib/Release 
  PATHS ${paths}
)

FIND_LIBRARY(OgreMax_LIBRARY_DBG
  NAMES libOgreMax_d libOgreMax.a OgreMax.lib 
  HINTS $ENV{OgreMaxDIR} PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 lib/Debug
  PATHS ${paths}
)

set(OgreMax_LIBRARY optimized ${OgreMax_LIBRARY_REL} debug ${OgreMax_LIBRARY_DBG})

SET(OgreMax_FOUND False)
IF(OgreMax_LIBRARY AND OgreMax_INCLUDE_DIR)
   SET(OgreMax_FOUND True)
   if (NOT DEFINED OgreMax_LIBRARY_REL)
      SET(OgreMax_FOUND False)
	   message("Could not find a release copy of OgreMax. This is probably because you forgot to build it.")
   endif()
   if (NOT DEFINED OgreMax_LIBRARY_DBG)
      SET(OgreMax_FOUND False)
	   message("Could not find a debug copy of OgreMax. This is probably because you forgot to build it.")
   endif()
ENDIF(OgreMax_LIBRARY AND OgreMax_INCLUDE_DIR)
