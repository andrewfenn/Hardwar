project(HardwarClient)
cmake_policy(SET CMP0003 NEW)

#Declare any external dependencies
SET(Required_Packages
   ENET
   OIS
   OGRE
   MyGUI
   TinyXML
   OgreMax
   SQLITE
)

# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB_RECURSE shared_source ${Hardwar_SOURCE_DIR}/code/shared/src/*.cpp)
SOURCE_GROUP("Client" FILES ${folder_source})

INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/core/include
   ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
   ${CMAKE_CURRENT_SOURCE_DIR}/console/include
   ${Hardwar_SOURCE_DIR}/code/shared/include
)

ADD_EXECUTABLE(client WIN32 ${folder_source} ${shared_source})

#this foreach loads all of the packages that you specified as required.
FOREACH(Package ${Required_Packages})
   IF (${Package}_LIBRARIES)
      TARGET_LINK_LIBRARIES(client ${${Package}_LIBRARIES})
   ELSE (${Package}_LIBRARIES)
      LINK_DIRECTORIES(${${Package}_LIBRARY_DIRS})
   ENDIF (${Package}_LIBRARIES)
ENDFOREACH(Package)

TARGET_LINK_LIBRARIES(client ${Boost_LIBRARIES})

IF (MSVC)
	MESSAGE(STATUS "Using GetText in dependencies")
	INCLUDE_DIRECTORIES(${Hardwar_SOURCE_DIR}/dependencies/gettext/include)
	LINK_DIRECTORIES(${Hardwar_SOURCE_DIR}/dependencies/gettext/lib)
ENDIF(MSVC)
