# Find MYGUI includes and library
#
# This module defines
#  MYGUI_INCLUDE_DIR
#  MYGUI_LIB_DIR, the location of the libraries
#  MYGUI_FOUND, If false, do not try to use MYGUI
#
# Copyright © 2009-2012, Andrew Fenn
#

include(FindPkgMacros)
findpkg_begin(MYGUI)

create_search_paths(MYGUI)
# redo search if prefix path changed
clear_if_changed(MYGUI_PREFIX_PATH
  MYGUI_LIBRARY_FWK
  MYGUI_LIBRARY_REL
  MYGUI_LIBRARY_DBG
  MYGUI_INCLUDE_DIR
)

set(MYGUI_LIBRARY_NAMES MYGUI)
get_debug_names(MYGUI_LIBRARY_NAMES)

use_pkgconfig(MYGUI MYGUI)

findpkg_framework(MYGUI)

find_path(MYGUI_INCLUDE_DIR NAMES MyGUI.h HINTS ${MYGUI_INC_SEARCH_PATH} ${MYGUI_PKGC_INCLUDE_DIRS} PATH_SUFFIXES MYGUI)
find_library(MYGUI_LIBRARY_REL NAMES ${MYGUI_LIBRARY_NAMES} HINTS ${MYGUI_LIB_SEARCH_PATH} ${MYGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(MYGUI_LIBRARY_DBG NAMES ${MYGUI_LIBRARY_NAMES_DBG} HINTS ${MYGUI_LIB_SEARCH_PATH} ${MYGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)
make_library_set(MYGUI_LIBRARY)

findpkg_finish(MYGUI)
add_parent_dir(MYGUI_INCLUDE_DIRS MYGUI_INCLUDE_DIR)

