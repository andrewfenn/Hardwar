# Find OgreMax includes and library
#
# This module defines
#  OgreMax_INCLUDE_DIR
#  OgreMax_LIB_DIR, the location of the libraries
#  OgreMax_FOUND, If false, do not try to use OgreMax
#
# Copyright © 2009-2010, Andrew Fenn
#

FIND_PATH(OgreMax_INCLUDE_DIR OgreMaxScene.hpp
  HINTS
  $ENV{OgreMaxDIR}
  PATH_SUFFIXES OgreMax
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  ${CMAKE_SOURCE_DIR}/dependencies/
)

FIND_LIBRARY(OgreMax_LIBRARY 
  NAMES libOgreMax libOgreMax.a OgreMax.lib
  HINTS
  $ENV{OgreMaxDIR}
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


SET(OgreMax_FOUND "NO")
IF(OgreMax_LIBRARY AND OgreMax_INCLUDE_DIR)
  SET(OgreMax_FOUND "YES")
ENDIF(OgreMax_LIBRARY AND OgreMax_INCLUDE_DIR)
