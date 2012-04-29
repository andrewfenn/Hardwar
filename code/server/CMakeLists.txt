cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(HardwarServer)

SET(HardwarServer_MAJOR_VERSION ${HARDWAR_MAJOR_VERSION})
SET(HardwarServer_MINOR_VERSION ${HARDWAR_MINOR_VERSION})
SET(HardwarServer_PATCH_VERSION ${HARDWAR_PATCH_VERSION})


# searches for all .cpp and .h files
FILE(GLOB_RECURSE folder_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(shared_source
    ${HARDWAR_SOURCE_DIR}/code/shared/src/md5.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/db.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/Building.cpp
    ${HARDWAR_SOURCE_DIR}/code/shared/src/GameSettings.cpp
)

SOURCE_GROUP("Server" FILES ${folder_source})

include_directories (
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${HARDWAR_SOURCE_DIR}/code/shared/include
)

include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})
include_directories(SYSTEM ${DEPENDENCIES_INCLUDES})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

if (MSVC)
   message(STATUS "Using GetText in dependencies")
   get_filename_component(windows_extras ${HARDWAR_SOURCE_DIR}/hardwar-deps/ ABSOLUTE)
   include_directories(${windows_extras}/gettext/include)
   link_directories(${windows_extras}/gettext/lib)
endif ()

if (WIN32)
   set(exe "WIN32")
endif ()

link_libraries(Enet SQLite)

add_executable(server ${exe} ${folder_source} ${shared_source})

target_link_libraries(server
    ${OGRE_LIBRARIES}
    ${Boost_LIBRARIES}
    )

if (UNIX)
#    add_definitions(-Wall -pedantic -Wextra -Wwrite-strings)
endif()

