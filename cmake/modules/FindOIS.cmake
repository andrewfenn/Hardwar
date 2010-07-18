# Find OIS includes and library
#
# This module defines
#  OIS_INCLUDE_DIR
#  OIS_LIB_DIR, the location of the libraries
#  OIS_FOUND, If false, do not try to use OIS
#
# Copyright © 2009-2010, Andrew Fenn
#

get_filename_component(windows_extras ${CMAKE_SOURCE_DIR}/../hardwar-deps/Ogre ABSOLUTE)

FIND_PATH(OIS_INCLUDE_DIR OIS.h
  HINTS
  $ENV{OISDIR}
  PATH_SUFFIXES include/OIS include/OIS include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  C:/OgreSDK
  C:/Ogre
  C:/OGRE
  ${windows_extras}
)

FIND_LIBRARY(OIS_LIBRARY 
  NAMES OIS libOIS OIS.lib
  HINTS
  $ENV{OISDIR}
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 lib/release
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
   C:/OgreSDK
   C:/Ogre
   C:/OGRE
   ${windows_extras}
)


SET(OIS_FOUND "NO")
IF(OIS_LIBRARY AND OIS_INCLUDE_DIR)
  SET(OIS_FOUND "YES")
ENDIF(OIS_LIBRARY AND OIS_INCLUDE_DIR)
