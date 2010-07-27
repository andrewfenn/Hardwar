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

set(paths
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

  set(prefixes lib64 lib libs64 libs libs/Win32 libs/Win64)

  set(release_libs MyGUI libMyGUI libMyGUI.OgrePlatform.a)
  set(debug_libs MyGUI_d libMyGUI libMyGUI.OgrePlatform.a)
  set(release_engine_libs MyGUIEngine libMyGUIEngine.so)
  set(debug_engine_libs  MyGUIEngine_d libMyGUIEngine.so)

FIND_PATH(MYGUI_INCLUDE_DIR MyGUI.h
  HINTS
  $ENV{MYGUIDIR}
  PATH_SUFFIXES include/MyGUI include/MYGUI include
  PATHS ${paths} 
)

FIND_LIBRARY(MYGUI_ENGINE_REL NAMES ${release_engine_libs} HINTS $ENV{MYGUIDIR} PATH_SUFFIXES ${prefixes} PATHS ${paths})
FIND_LIBRARY(MYGUI_ENGINE_DBG NAMES ${debug_engine_libs}   HINTS $ENV{MYGUIDIR} PATH_SUFFIXES ${prefixes} PATHS ${paths})
FIND_LIBRARY(MYGUI_LIBRARY_REL NAMES ${release_libs} HINTS $ENV{MYGUIDIR} PATH_SUFFIXES ${prefixes} PATHS ${paths})
FIND_LIBRARY(MYGUI_LIBRARY_DBG NAMES ${debug_libs}   HINTS $ENV{MYGUIDIR} PATH_SUFFIXES ${prefixes} PATHS ${paths})
set(MYGUI_LIBRARY optimized "${MYGUI_LIBRARY_REL};${MYGUI_ENGINE_REL}" debug "${MYGUI_LIBRARY_DBG};${MYGUI_ENGINE_DBG};")

set(MYGUI_FOUND False)
if (MYGUI_LIBRARY_REL AND MYGUI_INCLUDE_DIR)
	  set(MYGUI_FOUND True)
     get_filename_component(MYGUI_LIBRARY_DIR_REL "${MYGUI_LIBRARY_REL}" PATH)
     set(MYGUI_LIBRARY_DIRS ${MYGUI_LIBRARY_DIR_REL})
endif ()
