# Find MYGUI includes and library
#
# This module defines
#  MYGUI_INCLUDE_DIR
#  MYGUI_LIB_DIR, the location of the libraries
#  MYGUI_FOUND, If false, do not try to use MYGUI
#
# Copyright © 2009-2010, Andrew Fenn
#

get_filename_component(windows_extras ${CMAKE_SOURCE_DIR}/../hardwar-deps/MyGUI ABSOLUTE)

FIND_PATH(MYGUI_INCLUDE_DIR MyGUI.h
  HINTS
  $ENV{MYGUIDIR}
  PATH_SUFFIXES include/MyGUI include/MYGUI include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  ${windows_extras}
)

FIND_LIBRARY(MYGUI_LIBRARY 
  NAMES MyGUIEngine libMyGUIEngine 
  HINTS
  $ENV{MYGUIDIR}
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
  ${windows_extras}
)

FIND_LIBRARY(MYGUI_LIBRARY2 
  NAMES MyGUI libMyGUI libMyGUI.OgrePlatform.a
  HINTS
  $ENV{MYGUIDIR}
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
  ${windows_extras}
)

set(MYGUI_FOUND False)

if (MYGUI_LIBRARY AND MYGUI_LIBRARY2 AND MYGUI_INCLUDE_DIR)
	  set(MYGUI_FOUND True)
	  set(MYGUI_LIBRARIES
	  ${MYGUI_LIBRARY}
	  ${MYGUI_LIBRARY2}
	  )
	  set(MYGUI_LIBRARY "")
endif ()

