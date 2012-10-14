# Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIR
#  CEGUI_LIBRARY, the library to link against to use CEGUI.
#  CEGUILUA_LIBRARY, the library to link against to use the CEGUI script module.
#  CEGUI_TOLUA_LIBRARY, the library to link against to use Tolua++.
#  CEGUI_FOUND, If false, do not try to use CEGUI
#  CEGUI_VERSION, the version as string "x.y.z"
#
# Input:
#  ENV{CEGUIDIR}, CEGUI path, optional
#
# Created by Matt Williams to find OGRE libraries
# Copyright © 2007, Matt Williams
#
# Modified by Nicolas Schlumberger to find CEGUI libraries
# and make it run on the Tardis-Infrastucture of the ETH Zurich
# Copyright 2007, Nicolas Schlumberger
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli and Reto Grieder
# Version checking and CEGUILua finding by Reto Grieder
#                 > www.orxonox.net <
INCLUDE(CompareVersionStrings)
INCLUDE(DetermineVersion)
INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)
# Find CEGUI headers
FIND_PATH(CEGUI_INCLUDE_DIR CEGUI.h
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES include include/CEGUI
)
# Inspect CEGUIVersion.h for the version number
DETERMINE_VERSION(CEGUI ${CEGUI_INCLUDE_DIR}/CEGUIVersion.h)
# Find CEGUI library
FIND_LIBRARY(CEGUI_LIBRARY_OPTIMIZED
  NAMES CEGUIBase CEGUI
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUI_LIBRARY_DEBUG
  NAMES
    CEGUIBased CEGUIBase_d CEGUIBaseD CEGUIBase_D
    CEGUId CEGUI_d CEGUID CEGUI_D
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)
# Find CEGUILua headers
FIND_PATH(CEGUILUA_INCLUDE_DIR CEGUILua.h
  PATHS
    $ENV{CEGUIDIR}
    $ENV{CEGUILUADIR}
    ${CEGUI_INCLUDE_DIR}/ScriptingModules/LuaScriptModule
  PATH_SUFFIXES include include/CEGUI
)
# Find CEGUILua libraries
FIND_LIBRARY(CEGUILUA_LIBRARY_OPTIMIZED
  NAMES CEGUILua CEGUILuaScriptModule
  PATHS $ENV{CEGUIDIR} $ENV{CEGUILUADIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUILUA_LIBRARY_DEBUG
  NAMES CEGUILuad CEGUILua_d CEGUILuaScriptModuled CEGUILuaScriptModule_d
  PATHS $ENV{CEGUIDIR} $ENV{CEGUILUADIR}
  PATH_SUFFIXES lib bin
)
# Find CEGUI Tolua++ include file
# We only need to add this path since we use tolua++ like a normal
# dependency but it is shipped with CEGUILua.
FIND_PATH(CEGUI_TOLUA_INCLUDE_DIR tolua++.h
  PATHS
    ${CEGUILUA_INCLUDE_DIR}
    # For newer CEGUI versions >= 0.7
    ${CEGUILUA_INCLUDE_DIR}/support/tolua++
    # For Mac OS X, tolua++ is a separate framework in the dependency package
    ${DEP_FRAMEWORK_DIR}
  NO_DEFAULT_PATH # Don't attempt to find tolua++ installed on the system
)
# Find CEGUI Tolua++ libraries
FIND_LIBRARY(CEGUI_TOLUA_LIBRARY_OPTIMIZED
  NAMES CEGUItoluapp tolua++ ceguitolua++ tolua++5.1
  PATHS $ENV{CEGUIDIR} ${CEGUITOLUADIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUI_TOLUA_LIBRARY_DEBUG
  NAMES CEGUItoluappd CEGUItoluapp_d tolua++d tolua++_d
  PATHS $ENV{CEGUIDIR} ${CEGUITOLUADIR}
  PATH_SUFFIXES lib bin
)
# Newer versions of CEGUI have the renderer for OGRE shipped with them
COMPARE_VERSION_STRINGS("${CEGUI_VERSION}" "0.7" _version_compare TRUE)
IF(_version_compare GREATER -1)
  # Find CEGUI OGRE Renderer headers
  FIND_PATH(CEGUI_OGRE_RENDERER_INCLUDE_DIR CEGUIOgreRenderer.h
    PATHS
      $ENV{CEGUIDIR}
      $ENV{CEGUIOGRERENDERERDIR}
      ${CEGUI_INCLUDE_DIR}/RendererModules/Ogre
    PATH_SUFFIXES include include/CEGUI
  )
  # Find CEGUI OGRE Renderer libraries
  FIND_LIBRARY(CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED
    NAMES CEGUIOgreRenderer
    PATHS $ENV{CEGUIDIR} $ENV{CEGUIOGRERENDERERDIR}
    PATH_SUFFIXES lib bin
  )
  FIND_LIBRARY(CEGUI_OGRE_RENDERER_LIBRARY_DEBUG
    NAMES CEGUIOgreRendererd CEGUIOgreRenderer_d
    PATHS $ENV{CEGUIDIR} $ENV{CEGUIOGRERENDERERDIR}
    PATH_SUFFIXES lib bin
  )
  SET(CEGUI_OGRE_RENDERER_REQUIRED_VARIABLES
    CEGUI_OGRE_RENDERER_INCLUDE_DIR
    CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED
  )
ELSE()
  SET(CEGUI_OLD_VERSION TRUE)
  SET(CEGUI_OGRE_RENDERER_BUILD_REQUIRED TRUE)
ENDIF()
# Handle the REQUIRED argument and set CEGUI_FOUND
# Also checks the version requirements if given
FIND_PACKAGE_HANDLE_ADVANCED_ARGS(CEGUI DEFAULT_MSG "${CEGUI_VERSION}"
  CEGUI_INCLUDE_DIR
  CEGUI_LIBRARY_OPTIMIZED
  ${CEGUI_OGRE_RENDERER_REQUIRED_VARIABLES}
)
# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(CEGUI)
HANDLE_LIBRARY_TYPES(CEGUILUA)
HANDLE_LIBRARY_TYPES(CEGUI_TOLUA)
IF(NOT CEGUI_OGRE_RENDERER_BUILD_REQUIRED)
  HANDLE_LIBRARY_TYPES(CEGUI_OGRE_RENDERER)
ENDIF()
MARK_AS_ADVANCED(
  CEGUI_INCLUDE_DIR
  CEGUI_LIBRARY_OPTIMIZED
  CEGUI_LIBRARY_DEBUG
  CEGUI_OGRE_RENDERER_INCLUDE_DIR
  CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED
  CEGUI_OGRE_RENDERER_LIBRARY_DEBUG
)
