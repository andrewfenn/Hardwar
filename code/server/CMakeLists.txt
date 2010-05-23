project(HardwarServer)
cmake_policy(SET CMP0003 NEW)

# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(shared_source
${Hardwar_SOURCE_DIR}/code/shared/src/md5.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/db.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/Building.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/GameSettings.cpp
)

SOURCE_GROUP("Server" FILES ${folder_source})

INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${Hardwar_SOURCE_DIR}/code/shared/include
)

INCLUDE(addLibraryHelper)

MACRO_FIND_LIBRARY(OGRE)
MACRO_FIND_LIBRARY(ENET)
MACRO_FIND_LIBRARY(SQLITE)
MACRO_FIND_LIBRARIES(Boost "filesystem;thread;")

if (MSVC)
	MESSAGE(STATUS "Using GetText in dependencies")
	INCLUDE_DIRECTORIES(${Hardwar_SOURCE_DIR}/dependencies/gettext/include)
	LINK_DIRECTORIES(${Hardwar_SOURCE_DIR}/dependencies/gettext/lib)
endif ()

if (WIN32)
   set(exe "WIN32")
elseif (WIN64)
   set(exe "WIN64")
endif ()

# create the project
ADD_EXECUTABLE(server ${exe} ${folder_source} ${shared_source})

MACRO_ADD_LIBRARY(OGRE server)
MACRO_ADD_LIBRARY(ENET server)
MACRO_ADD_LIBRARY(SQLITE server)
MACRO_ADD_LIBRARY(Boost server)

