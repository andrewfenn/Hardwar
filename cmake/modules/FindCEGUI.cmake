# Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIR
#  CEGUI_LIB_DIR, the location of the libraries
#  CEGUI_FOUND, If false, do not try to use CEGUI
#
# Copyright © 2009-2012, Andrew Fenn
#

include(FindPkgMacros)
findpkg_begin(CEGUI)

create_search_paths(CEGUI)
# redo search if prefix path changed
clear_if_changed(CEGUI_PREFIX_PATH
  CEGUI_LIBRARY_FWK
  CEGUI_LIBRARY_REL
  CEGUI_LIBRARY_DBG
  CEGUI_INCLUDE_DIR
)

set(CEGUI_LIBRARY_NAMES CEGUI)
get_debug_names(CEGUI_LIBRARY_NAMES)

use_pkgconfig(CEGUI CEGUI)

findpkg_framework(CEGUI)

find_path(CEGUI_INCLUDE_DIR NAMES CEGUI.h HINTS ${CEGUI_INC_SEARCH_PATH} ${CEGUI_PKGC_INCLUDE_DIRS} PATH_SUFFIXES CEGUI)
find_library(CEGUI_LIBRARY_REL NAMES ${CEGUI_LIBRARY_NAMES} HINTS ${CEGUI_LIB_SEARCH_PATH} ${CEGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(CEGUI_LIBRARY_DBG NAMES ${CEGUI_LIBRARY_NAMES_DBG} HINTS ${CEGUI_LIB_SEARCH_PATH} ${CEGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)
make_library_set(CEGUI_LIBRARY)

findpkg_finish(CEGUI)
add_parent_dir(CEGUI_INCLUDE_DIRS CEGUI_INCLUDE_DIR)

