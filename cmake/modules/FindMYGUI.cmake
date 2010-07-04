# Find MYGUI includes and library
#
# This module defines
#  MYGUI_INCLUDE_DIR
#  MYGUI_LIB_DIR, the location of the libraries
#  MYGUI_FOUND, If false, do not try to use MYGUI
#
# Copyright © 2009-2010, Andrew Fenn
#

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
)

FIND_LIBRARY(MYGUI_LIBRARY 
  NAMES MyGUI MyGUIEngine libMyGUI libmygui.so
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
)


SET(MYGUI_FOUND False)
IF(MYGUI_LIBRARY AND MYGUI_INCLUDE_DIR)
  SET(MYGUI_FOUND True)
ENDIF(MYGUI_LIBRARY AND MYGUI_INCLUDE_DIR)

