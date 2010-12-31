project(OgreMax)
cmake_policy(SET CMP0003 NEW)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)

FILE(GLOB_RECURSE ogremax_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
SOURCE_GROUP("OgreMax" FILES ${ogremax_source})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(TinyXML REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${TinyXML_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_LIBRARY(OgreMax STATIC ${ogremax_source})

TARGET_LINK_LIBRARIES(OgreMax ${OGRE_LIBRARIES})
TARGET_LINK_LIBRARIES(OgreMax ${TinyXML_LIBRARIES})
TARGET_LINK_LIBRARIES(OgreMax ${Boost_LIBRARIES})