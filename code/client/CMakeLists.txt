cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(HardwarClient)

SET(HardwarClient_MAJOR_VERSION ${HARDWAR_MAJOR_VERSION})
SET(HardwarClient_MINOR_VERSION ${HARDWAR_MINOR_VERSION})
SET(HardwarClient_PATCH_VERSION ${HARDWAR_PATCH_VERSION})

# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
SOURCE_GROUP("Client" FILES ${folder_source})

set(shared_source
    ${HARDWAR_SOURCE_DIR}/code/shared/src/md5.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/db.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/Building.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/GameSettings.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/DynamicLines.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/DynamicRenderable.cpp
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/core/include
   ${CMAKE_CURRENT_SOURCE_DIR}/states/include
   ${CMAKE_CURRENT_SOURCE_DIR}/states/menu/include
   ${CMAKE_CURRENT_SOURCE_DIR}/helpers/include
   ${CMAKE_CURRENT_SOURCE_DIR}/tasks/include
   ${HARDWAR_SOURCE_DIR}/code/shared/include
)

include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${DEPENDENCIES_INCLUDES})
include_directories(${OIS_INCLUDE_DIRS})
include_directories(${MYGUI_INCLUDE_DIRS})
include_directories(${OGRE_Terrain_INCLUDE_DIR})
include_directories(${OGRE_Paging_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})


#INCLUDE(addLibraryHelper)

#MACRO_FIND_LIBRARY(OIS)
#MACRO_FIND_LIBRARY(MYGUI)

if (MSVC)
   MESSAGE(STATUS "Using GetText in dependencies")
   get_filename_component(windows_extras ${HARDWAR_SOURCE_DIR}/hardwar-deps/ ABSOLUTE)
   include_directories(${windows_extras}/gettext/include)
   link_directories(${windows_extras}/gettext/lib)
endif ()

if (WIN32)
   set(exe "WIN32")
endif ()

link_libraries(Enet SQLite OgreMax)

add_executable(client ${exe} ${folder_source} ${shared_source})

add_dependencies(client SQLite)
add_dependencies(client Enet)
add_dependencies(client OgreMax)

if (MSVC)
	target_link_libraries(client ${windows_extras}/gettext/lib/intl.lib)
endif()

set_target_properties(client PROPERTIES SOLUTION_FOLDER "Hardwar Client")
target_link_libraries(client
    ${OGRE_LIBRARIES}
    ${OGRE_LIBRARY} ${OGRE_Terrain_LIBRARY} ${OGRE_Paging_LIBRARY}
    ${OIS_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${Boost_LIBRARIES}
    )
message(${Boost_LIBRARIES})

#MACRO_ADD_LIBRARY(OIS client)
#MACRO_ADD_LIBRARY(OGRE client)
#MACRO_ADD_LIBRARY(MYGUI client)

