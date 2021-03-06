# Find ENET includes and library
#
# This module defines
#  ENET_INCLUDE_DIR
#  ENET_LIB_DIR, the location of the libraries
#  ENET_FOUND, If false, do not try to use ENET
#
# Copyright © 2009-2012, Andrew Fenn
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

set(suffixes
    lib64
    lib
    libs64
    libs
    libs/Win32
    libs/Win64
)


FIND_PATH(ENET_INCLUDE_DIR enet/enet.h
    PATH_SUFFIXES include
    PATHS
    ${paths}
    ${CMAKE_SOURCE_DIR}/dependencies/enet/
)

find_library(ENET_LIBRARY_REL NAMES enet libenet libEnet.a Enet.lib HINTS $ENV{ENETDIR} PATH_SUFFIXES ${suffixes}; lib/Release PATHS ${paths})
find_library(ENET_LIBRARY_DBG NAMES enet libenet libEnet.a Enet.lib HINTS $ENV{ENETDIR} PATH_SUFFIXES ${suffixes}; lib/Debug PATHS ${paths})

set(ENET_FOUND False)
set(ENET_LIBRARY optimized ${ENET_LIBRARY_REL} debug ${ENET_LIBRARY_DBG})

if (ENET_LIBRARY AND ENET_INCLUDE_DIR)
   set(ENET_FOUND True)

   if (NOT DEFINED ENET_LIBRARY_REL)
	   message("Could not find a release copy of Enet. This is probably because you forgot to build it.")
      set(ENET_FOUND False)
   endif()
   if (NOT DEFINED ENET_LIBRARY_DBG)
	   message("Could not find a debug copy of Enet. This is probably because you forgot to build it.")
      set(ENET_FOUND False)
   endif()
endif (ENET_LIBRARY AND ENET_INCLUDE_DIR)
