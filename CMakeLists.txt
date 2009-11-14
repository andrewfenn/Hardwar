cmake_minimum_required(VERSION 2.6)
project(Hardwar)
cmake_policy(SET CMP0003 NEW)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 4 )

SET( CMAKE_MODULE_PATH   "${CMAKE_MODULE_PATH}"
                         "${Hardwar_SOURCE_DIR}/cmake/modules"
                         "${Hardwar_SOURCE_DIR}/cmake/scripts"
 )

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#Declare any external dependencies that your project may have here.
SET(Required_Packages
   OIS
   OGRE
   MyGUI
   SQLITE
   ENET
   TinyXML
   OgreMax
)

# searches for all .cpp and .h files in the current directory and add them 
# to the current project

FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/code/*.cpp)
SOURCE_GROUP("Hardwar" FILES ${folder_source})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/code/shared/include/Config.h)

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-msse)
endif ()
if (MSVC)
  add_definitions(/fp:fast)
endif ()

INCLUDE_DIRECTORIES(
         ${CMAKE_CURRENT_SOURCE_DIR}/code/client/core/include
         ${CMAKE_CURRENT_SOURCE_DIR}/code/client/editor/include
         ${CMAKE_CURRENT_SOURCE_DIR}/code/client/console/include
         ${CMAKE_CURRENT_SOURCE_DIR}/code/server/include
         ${CMAKE_CURRENT_SOURCE_DIR}/code/shared/include
)

ADD_DEFINITIONS("-Wall -Wunused -O2 -g3")

IF (MSVC_IDE)
	IF (EXISTS $ENV{OGRE_HOME})
		SET(OGRESDK $ENV{OGRE_HOME})
		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
		MESSAGE(STATUS "Adding Additional Link Directories")
		LINK_DIRECTORIES(${OGRESDK}/lib)
	ENDIF(EXISTS $ENV{OGRE_HOME})

	LINK_DIRECTORIES(C:/Program\ Files/boost/boost_1_34_0/lib)
	LINK_DIRECTORIES(C:/Program\ Files/boost/boost_1_35_0/lib)
	LINK_DIRECTORIES(C:/Program\ Files/boost/boost_1_36_0/lib)

	SET(PLATFORM_C_FLAGS "/W3 /MD /Od /D \"WIN32\" /D \"_WINDOWS\" /D \"TIXML_USE_STL\" /D \"ENET_DLL\" /GmCONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/code/shared/include/config.h) /Gy /fp:fast /ZI /EHsc /SUBSYSTEM:WINDOWS /ENTRY:\"main\"")
	SET(PLATFORM_C_FLAGS_DEBUG "/W3 /MDd /Od /Gm /Gy /fp:fast /ZI")

	SET(CMAKE_CXX_FLAGS "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${PLATFORM_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS_DEBUG "${PLATFORM_C_FLAGS_DEBUG}")
ENDIF (MSVC_IDE)


# create the project
ADD_EXECUTABLE(hardwar WIN32 ${folder_source})

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  FIND_PACKAGE(${Package} REQUIRED)
  IF (${Package}_FOUND)
      INCLUDE_DIRECTORIES(${${Package}_INCLUDE_DIR})
      IF (${Package}_LIBRARIES)
         TARGET_LINK_LIBRARIES(hardwar ${${Package}_LIBRARIES})
      ELSE (${Package}_LIBRARIES)
         LINK_DIRECTORIES(${${Package}_LIBRARY_DIRS})
      ENDIF (${Package}_LIBRARIES)
  ELSE (${Package}_FOUND)
      MESSAGE(STATUS "${Package} not found")
  ENDIF(${Package}_FOUND)
ENDFOREACH(Package)

FIND_PACKAGE(Boost COMPONENTS filesystem thread thread-mt REQUIRED)
TARGET_LINK_LIBRARIES(hardwar ${Boost_LIBRARIES})
