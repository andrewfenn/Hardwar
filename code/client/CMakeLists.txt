project(HardwarClient)
cmake_policy(SET CMP0003 NEW)

# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(shared_source
${Hardwar_SOURCE_DIR}/code/shared/src/md5.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/db.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/Building.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/GameSettings.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/DynamicLines.cpp
${Hardwar_SOURCE_DIR}/code/shared/src/DynamicRenderable.cpp
)
SOURCE_GROUP("Client" FILES ${folder_source})

INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/core/include
   ${CMAKE_CURRENT_SOURCE_DIR}/core/console/include
   ${CMAKE_CURRENT_SOURCE_DIR}/ingame/include
   ${CMAKE_CURRENT_SOURCE_DIR}/ingame/editor/include
   ${CMAKE_CURRENT_SOURCE_DIR}/loadscreen/include
   ${Hardwar_SOURCE_DIR}/code/shared/include
)

INCLUDE(addLibraryHelper)

MACRO_FIND_LIBRARY(ENET)
MACRO_FIND_LIBRARY(OIS)
MACRO_FIND_LIBRARY(OGRE)
MACRO_FIND_LIBRARY(MYGUI)
MACRO_FIND_LIBRARY(OgreMax)
MACRO_FIND_LIBRARY(SQLITE)
MACRO_FIND_LIBRARY(TinyXML)
MACRO_FIND_LIBRARIES(Boost "filesystem;thread;")

if (MSVC)
   MESSAGE(STATUS "Using GetText in dependencies")
   get_filename_component(windows_extras ${CMAKE_SOURCE_DIR}/../hardwar-deps/ ABSOLUTE)
   INCLUDE_DIRECTORIES(${windows_extras}/gettext/include)
   LINK_DIRECTORIES(${windows_extras}/gettext/lib)
endif ()

if (WIN32)
   set(exe "WIN32")
endif ()

ADD_EXECUTABLE(client ${exe} ${folder_source} ${shared_source})

if (MSVC)
	target_link_libraries(client ${windows_extras}/gettext/lib/intl.lib)
endif()

MACRO_ADD_LIBRARY(ENET client)
MACRO_ADD_LIBRARY(OIS client)
MACRO_ADD_LIBRARY(OGRE client)
MACRO_ADD_LIBRARY(MYGUI client)
MACRO_ADD_LIBRARY(OgreMax client)
MACRO_ADD_LIBRARY(SQLITE client)
MACRO_ADD_LIBRARY(TinyXML client)
MACRO_ADD_LIBRARY(Boost client)
