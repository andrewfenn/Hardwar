# Find MYGUI includes and library
#
# This module defines
#  MYGUI_INCLUDE_DIR
#  MYGUI_LIB_DIR, the location of the libraries
#  MYGUI_FOUND, If false, do not try to use MYGUI
#
# Copyright © 2009-2012, Andrew Fenn
#


findpkg_begin(MYGUI)

set(paths
 ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
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

set(MYGUI_LIBRARY_NAMES "MYGUI")
get_debug_names(MYGUI_LIBRARY_NAMES)
find_library(MYGUI_LIBRARY_REL NAMES ${MYGUI_LIBRARY_NAMES} HINTS ${MYGUI_LIBRARY_DIR_REL} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(MYGUI_LIBRARY_DBG NAMES ${MYGUI_LIBRARY_NAMES_DBG} HINTS ${MYGUI_LIBRARY_DIR_DBG} PATH_SUFFIXES "" "debug")
make_library_set(MYGUI_LIBRARY)
findpkg_finish(MYGUI)
if (MYGUI_FOUND)
    # find binaries
    if (NOT MYGUI_STATIC)
        if (WIN32)
            find_file(MYGUI_BINARY_REL NAMES "MyGUI.dll" HINTS ${paths} PATH_SUFFIXES "" bin bin/release bin/relwithdebinfo bin/minsizerel release)
            find_file(MYGUI_BINARY_DBG NAMES "MyGUI_d.dll" HINTS ${paths} PATH_SUFFIXES "" bin bin/debug debug)
        endif()
        mark_as_advanced(MYGUI_BINARY_REL MYGUI_BINARY_DBG)
    endif()
endif()

