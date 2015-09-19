cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(HardwarClient)

SET(HardwarClient_MAJOR_VERSION ${HARDWAR_MAJOR_VERSION})
SET(HardwarClient_MINOR_VERSION ${HARDWAR_MINOR_VERSION})
SET(HardwarClient_PATCH_VERSION ${HARDWAR_PATCH_VERSION})

add_subdirectory("core/")
add_subdirectory("tasks/")
add_subdirectory("states/")
add_subdirectory("helpers/")

set(client_source ${client_source} core/src/main.cpp)

source_group("Client" FILES ${client_source})

set(shared_source
    ${CMAKE_SOURCE_DIR}/code/shared/src/md5.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/db.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/Building.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/GameSettings.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/DynamicLines.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/DynamicRenderable.cpp
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/core/include
   ${CMAKE_CURRENT_SOURCE_DIR}/states/include
   ${CMAKE_CURRENT_SOURCE_DIR}/states/menu/include
   ${CMAKE_CURRENT_SOURCE_DIR}/helpers/include
   ${CMAKE_CURRENT_SOURCE_DIR}/tasks/include
   ${CMAKE_SOURCE_DIR}/code/shared/include
)

include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})
include_directories(SYSTEM ${OGRE_Terrain_INCLUDE_DIR})
include_directories(SYSTEM ${OGRE_Paging_INCLUDE_DIR})
include_directories(SYSTEM ${DEPENDENCIES_INCLUDES})
include_directories(SYSTEM ${OIS_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

if (MSVC)
   MESSAGE(STATUS "Using GetText in dependencies")
   get_filename_component(windows_extras ${HARDWAR_SOURCE_DIR}/hardwar-deps/ ABSOLUTE)
   include_directories(${windows_extras}/gettext/include)
   link_directories(${windows_extras}/gettext/lib)
endif ()

if (WIN32)
   set(exe "WIN32")
endif ()

link_libraries(Enet SQLite yaarg)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
add_executable(client ${exe} ${client_source} ${shared_source})

add_dependencies(client SQLite)
add_dependencies(client Enet)
add_dependencies(client yaarg)
add_dependencies(client RocketCore)
add_dependencies(client RocketControls)
add_dependencies(client RocketDebugger)

add_custom_command(TARGET client PRE_BUILD COMMAND ${CMAKE_BINARY_DIR}/bin/tests/clientTest)

if (MSVC)
	target_link_libraries(client ${windows_extras}/gettext/lib/intl.lib)
endif()

set_target_properties(client PROPERTIES SOLUTION_FOLDER "Hardwar Client")
target_link_libraries(client
    ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARY} ${OGRE_Paging_LIBRARY}
    ${OIS_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    RocketCore RocketControls RocketDebugger
)

if (UNIX)
    include(GetC11CompilerFlag)
    get_c11_flag()

    add_definitions(-g -o2 -Wall -pedantic -Wextra -Wwrite-strings ${c11flag})
endif()

