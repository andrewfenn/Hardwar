project(HardwarServer)
cmake_policy(SET CMP0003 NEW)

# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB_RECURSE shared_source ${Hardwar_SOURCE_DIR}/code/shared/src/*.cpp)
SOURCE_GROUP("Server" FILES ${folder_source})

INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${Hardwar_SOURCE_DIR}/code/shared/include
)

# create the project
ADD_EXECUTABLE(server WIN32 ${folder_source} ${shared_source})
FIND_PACKAGE(OGRE REQUIRED)
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(server ${OGRE_LIBRARIES})
LINK_DIRECTORIES(${OGRE_LIBRARY_DIRS})

FIND_PACKAGE(ENET REQUIRED)
INCLUDE_DIRECTORIES(${ENET_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(server ${ENET_LIBRARIES})
LINK_DIRECTORIES(${ENET_LIBRARY_DIRS})

set(SQLITE_FIND_QUIETLY FALSE)
FIND_PACKAGE(SQLITE REQUIRED)
INCLUDE_DIRECTORIES(${SQLITE_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(server ${SQLITE_LIBRARIES})
LINK_DIRECTORIES(${SQLITE_LIB_DIR})

FIND_PACKAGE(Boost COMPONENTS filesystem thread REQUIRED)
TARGET_LINK_LIBRARIES(server ${Boost_LIBRARIES})