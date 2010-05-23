# Find OIS includes and library
#
# This module defines
#  OIS_INCLUDE_DIR
#  OIS_LIB_DIR, the location of the libraries
#  OIS_FOUND, If false, do not try to use OIS
#
# Copyright © 2009-2010, Andrew Fenn
#

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
)

FIND_LIBRARY(OIS_LIBRARY 
  NAMES OIS libOIS
  HINTS
  $ENV{OISDIR}
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)


SET(OIS_FOUND "NO")
IF(OIS_LIBRARY AND OIS_INCLUDE_DIR)
  SET(OIS_FOUND "YES")
ENDIF(OIS_LIBRARY AND OIS_INCLUDE_DIR)
