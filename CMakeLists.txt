cmake_minimum_required(VERSION 2.6)
project(Hardwar)
cmake_policy(SET CMP0003 NEW)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

SET( CMAKE_MODULE_PATH   "${CMAKE_MODULE_PATH}"
                         "${Hardwar_SOURCE_DIR}/cmake/modules"
                         "${Hardwar_SOURCE_DIR}/cmake/scripts"
 )

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Set compiler specific build flags
IF (UNIX)
   add_definitions(-msse -Wunused -g3 -O3 -Wall)
ELSE()
   add_definitions(/fp:fast)
ENDIF()

IF (MSVC_IDE)
	IF (EXISTS $ENV{OGRE_HOME})
		SET(OGRESDK $ENV{OGRE_HOME})
		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
		MESSAGE(STATUS "Adding Additional Link Directories")
		LINK_DIRECTORIES(${OGRESDK}/lib)
	ENDIF(EXISTS $ENV{OGRE_HOME})

	SET(PLATFORM_C_FLAGS "/W3 /MD /D \"WIN32\" /D \"_WINDOWS\" /D \"TIXML_USE_STL\" /D \"ENET_DLL\" /Gm /Gy /fp:fast /EHsc")
	SET(PLATFORM_C_FLAGS_DEBUG "/W3 /MDd /Gm /Gy /fp:fast")

	SET(CMAKE_CXX_FLAGS "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_DEBUG "${PLATFORM_C_FLAGS_DEBUG}")
ENDIF (MSVC_IDE)

set(deps_built True)

FIND_PACKAGE(ENET)
if (NOT ENET_FOUND)
   message("Enet not found. Using dependencies.")
   add_subdirectory(dependencies/enet)
   set(deps_built False)
endif ()

FIND_PACKAGE(OgreMax)
if (NOT OgreMax_FOUND)
   message("OgreMax not found. Using dependencies.")
   add_subdirectory(dependencies/OgreMax)
   set(deps_built False)
endif ()

FIND_PACKAGE(SQLITE)
if (NOT SQLITE_FOUND)
   message("SQLite not found. Using dependencies.")
   add_subdirectory(dependencies/sqlite3)
   set(deps_built False)
endif ()

if (NOT deps_built)
   message("-------------------------")
   message("\tThe make files have been generated.\n\tFirst build then re-run cmake to build the game.")
   message("-------------------------")
else ()

   set(Boost_USE_MULTITHREADED True)
   if (WIN32)
      get_filename_component(windows_extras ${CMAKE_SOURCE_DIR}/../hardwar-deps/ ABSOLUTE)
      set (BOOST_ROOT "${windows_extras}/boost_1_42")

      if (WIN32 OR APPLE)
         set(Boost_USE_STATIC_LIBS TRUE)
      endif ()

      set(Boost_ADDITIONAL_VERSIONS "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
   endif ()

   add_subdirectory(code/client)
   add_subdirectory(code/server)
endif ()
