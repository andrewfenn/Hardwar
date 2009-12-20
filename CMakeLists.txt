cmake_minimum_required(VERSION 2.6)
project(Hardwar)
cmake_policy(SET CMP0003 NEW)

SET(Required_Packages
   ENET
   OIS
   OGRE
   MyGUI
   SQLITE
   TinyXML
   OgreMax
)

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
if (CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-msse -Wunused -g3)
endif ()
if (MSVC)
  add_definitions(/fp:fast)
endif ()

ADD_DEFINITIONS("-Wall -O0")

IF (MSVC_IDE)
	IF (EXISTS $ENV{OGRE_HOME})
		SET(OGRESDK $ENV{OGRE_HOME})
		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
		MESSAGE(STATUS "Adding Additional Link Directories")
		LINK_DIRECTORIES(${OGRESDK}/lib)
	ENDIF(EXISTS $ENV{OGRE_HOME})

	SET(PLATFORM_C_FLAGS "/W3 /MD /Od /D \"WIN32\" /D \"_WINDOWS\" /D \"TIXML_USE_STL\" /D \"ENET_DLL\" /Gm /Gy /fp:fast /ZI /EHsc /SUBSYSTEM:WINDOWS /ENTRY:\"main\"")
	SET(PLATFORM_C_FLAGS_DEBUG "/W3 /MDd /Od /Gm /Gy /fp:fast /ZI")

	SET(CMAKE_CXX_FLAGS "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_DEBUG "${PLATFORM_C_FLAGS_DEBUG}")
ENDIF (MSVC_IDE)

#this foreach loads all of the packages that you specified as required.
FOREACH(Package ${Required_Packages})
  FIND_PACKAGE(${Package} REQUIRED)
  IF (${Package}_FOUND)
      INCLUDE_DIRECTORIES(${${Package}_INCLUDE_DIR})
  ELSE (${Package}_FOUND)
      MESSAGE(STATUS "${Package} not found")
  ENDIF(${Package}_FOUND)
ENDFOREACH(Package)

FIND_PACKAGE(Boost COMPONENTS filesystem thread thread-mt REQUIRED)

add_subdirectory(code/client)
add_subdirectory(code/server)
