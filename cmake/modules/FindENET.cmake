# Find ENET includes and library
#
# This module defines
#  ENET_INCLUDE_DIR
#  ENET_LIB_DIR, the location of the libraries
#  ENET_FOUND, If false, do not try to use ENET
#
# Copyright © 2009-2010, Andrew Fenn
#

FIND_PATH(ENET_INCLUDE_DIR enet/enet.h
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  ${CMAKE_SOURCE_DIR}/dependencies/enet/
)

FIND_LIBRARY(ENET_LIBRARY 
  NAMES enet libenet libEnet.a Enet.lib
  HINTS
  $ENV{ENETDIR}
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

set(ENET_FOUND "NO")
if (ENET_LIBRARY AND ENET_INCLUDE_DIR)
  set(ENET_FOUND "YES")
endif (ENET_LIBRARY AND ENET_INCLUDE_DIR)
