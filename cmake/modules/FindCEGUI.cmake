# Locate CEGUI (Made for CEGUI 7.5)
#
# This module defines
# CEGUI_FOUND, if false, do not try to link to CEGUI
# CEGUI_LIBRARY, where to find the librarys
# CEGUI_INCLUDE_DIR, where to find the headers
#
# $CEGUIDIR is an environment variable that would
# correspond to the ./configure --prefix=$CEGUIDIR
#
# There are several COMPONENTS that can be included:
# NULL, OPENGL, DIRECT3D9, DIRECT3D10, DIRECT3D11, DIRECTFB, OGRE, IRRLICHT
# Selecting no render as COMPONENT will create a error massage!
#
# 2011-07-21 Created by Frederik vom Hofe using the findSFML.cmake versions from David Guthrie with code from Robert Osfield.

set(CEGUI_FOUND "YES")
set(CEGUI_LIBRARY "")
set(CEGUI_INCLUDE_DIR "")

set( CEGUIDIR $ENV{CEGUIDIR} )
if ((WIN32 OR WIN64) AND NOT(CYGWIN))
   # Convert backslashes to slashes
   string(REGEX REPLACE "\\\\" "/" CEGUIDIR "${CEGUIDIR}")
endif ()

#To always have the right case sensitive name we use this list and a helper macro:
set(RENDER_NAME
   Null
   OpenGL
   Direct3D9
   Direct3D10
   Direct3D11
   DirectFB
   Ogre
   Irrlicht
)

macro(HELPER_GET_CASE_FROM_LIST SEARCHSTR LOOKUPLIST RESULTSTR)
   set(${RESULTSTR} ${SEARCHSTR}) #default return value if nothing is found
   foreach(LOOP_S IN LISTS ${LOOKUPLIST})
      string(TOLOWER ${LOOP_S} LOOP_S_LOWER)
      string(TOLOWER ${SEARCHSTR} LOOP_SEARCHSTR_LOWER)
      string(COMPARE EQUAL ${LOOP_S_LOWER} ${LOOP_SEARCHSTR_LOWER} LOOP_STR_COMPARE)
      if (LOOP_STR_COMPARE)
         set(${RESULTSTR} ${LOOP_S})
      endif ()
   endforeach()
endmacro()

set( CEGUI_INCLUDE_SEARCH_DIR
   ${CEGUIDIR}/include
   ${CEGUIDIR}/cegui/include
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local/include
   /usr/include
   /sw/include # Fink
   /opt/local/include # DarwinPorts
   /opt/csw/include # Blastwave
   /opt/include
   /usr/freeware/include
)

#helper
macro(FIND_PATH_HELPER FILENAME DIR SUFFIX)
   find_path(${FILENAME}_DIR ${FILENAME} PATHS ${${DIR}} PATH_SUFFIXES ${SUFFIX})
   if (NOT ${FILENAME}_DIR)
      message(STATUS "Could not locate ${FILENAME}")
      set(CEGUI_FOUND "NO")
   else()
      message(STATUS "${FILENAME} : ${${FILENAME}_DIR}")
      list(APPEND CEGUI_INCLUDE_DIR ${${FILENAME}_DIR})
   endif()
endmacro()

FIND_PATH_HELPER(CEGUI.h CEGUI_INCLUDE_SEARCH_DIR CEGUI)

if ("${CEGUI_FIND_COMPONENTS}" STREQUAL "")
   message("ERROR: No CEGUI renderer selected. \n\nSelect a renderer by including it's name in the component list:\n\ne.g. Find_Package(CEGUI REQUIRED COMPONENTS OPENGL)\n\nCEGUI renderers:")
   FOREACH(LOOP_S IN LISTS RENDER_NAME)
      message("${LOOP_S}")
   ENDFOREACH()
   message("\n")
   message(SEND_ERROR "Select at last one renderer!" )
endif()

foreach(COMPONENT ${CEGUI_FIND_COMPONENTS})
   HELPER_GET_CASE_FROM_LIST( ${COMPONENT} RENDER_NAME COMPONENT_CASE)
   FIND_PATH_HELPER( "CEGUI${COMPONENT_CASE}Renderer.h" "CEGUI_INCLUDE_SEARCH_DIR" "CEGUI/RendererModules/${COMPONENT_CASE}/;RendererModules/${COMPONENT_CASE}/" )
endforeach(COMPONENT)

if (APPLE)
   FIND_PATH(CEGUI_FRAMEWORK_DIR CEGUI.h
     PATHS
       ~/Library/Frameworks/CEGUI.framework/Headers
       /Library/Frameworks/CEGUI.framework/Headers
       ${DELTA3D_EXT_DIR}/Frameworks/CEGUI.framework/Headers
)
endif (APPLE)

if (CEGUI_FRAMEWORK_DIR)
   LIST(APPEND CEGUI_INCLUDE_DIR ${CEGUI_FRAMEWORK_DIR})
else()
   LIST(APPEND CEGUI_INCLUDE_DIR ${CEGUI_FRAMEWORK_DIR}/CEGUI)
endif()

set( CEGUI_LIBRARY_SEARCH_DIR
   ${CEGUIDIR}/lib
        ${CEGUIDIR}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;CEGUI_ROOT]/lib
        /usr/freeware/lib64
)

#helper
macro(FIND_LIBRARY_HELPER FILENAME DIR)
   find_library(${FILENAME}_DIR NAMES ${FILENAME} PATHS ${${DIR}})
   if (NOT ${FILENAME}_DIR)
      message("Could not located ${FILENAME}")
      set(CEGUI_FOUND "NO")
   else()
      message(STATUS "${FILENAME} : ${${FILENAME}_DIR}")
      list(APPEND CEGUI_LIBRARY ${${FILENAME}_DIR})
   endif()
endmacro()

FIND_LIBRARY_HELPER( CEGUIBase CEGUI_LIBRARY_SEARCH_DIR )

foreach(COMPONENT ${CEGUI_FIND_COMPONENTS})
   HELPER_GET_CASE_FROM_LIST( ${COMPONENT} RENDER_NAME COMPONENT_CASE)
   message(STATUS "Looking for lib: CEGUI${COMPONENT_CASE}Renderer")
   FIND_LIBRARY_HELPER( CEGUI${COMPONENT_CASE}Renderer "CEGUI_LIBRARY_SEARCH_DIR" CEGUI)
endforeach(COMPONENT)

if (NOT CEGUI_FOUND)
   message(SEND_ERROR "Error(s) during CEGUI detection!")
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEGUI DEFAULT_MSG CEGUI_LIBRARY CEGUI_INCLUDE_DIR)
