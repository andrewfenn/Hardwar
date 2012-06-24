cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(ClientTest)

set(ClientTest_source
    ${CMAKE_SOURCE_DIR}/code/client/core/src/GameRoot.cpp
    ${CMAKE_SOURCE_DIR}/code/client/core/src/GameManager.cpp
    ${CMAKE_SOURCE_DIR}/code/client/core/src/RootGameState.cpp
    ${CMAKE_SOURCE_DIR}/code/client/core/src/GameState.cpp
    ${CMAKE_SOURCE_DIR}/code/client/tasks/src/InputTask.cpp
    core/TestGameRoot.cpp

    core/main.cpp
)
source_group("ClientTest" FILES ${ClientTest_source})

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/dependencies/UnitTest++/src")
include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})
include_directories(SYSTEM ${OIS_INCLUDE_DIRS})
include_directories(SYSTEM ${DEPENDENCIES_INCLUDES})

include_directories("${CMAKE_SOURCE_DIR}/code/client/core/include")
include_directories(
   ${CMAKE_SOURCE_DIR}/code/client/core/include
   ${CMAKE_SOURCE_DIR}/code/client/states/include
   ${CMAKE_SOURCE_DIR}/code/client/states/menu/include
   ${CMAKE_SOURCE_DIR}/code/client/helpers/include
   ${CMAKE_SOURCE_DIR}/code/client/tasks/include
   ${CMAKE_SOURCE_DIR}/code/shared/include
)


link_libraries(UnitTest)

if (WIN32)
   set(exe "WIN32")
endif ()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests/)
add_executable(ClientTest ${exe} ${ClientTest_source})

add_dependencies(ClientTest UnitTest)
add_dependencies(ClientTest Enet)

target_link_libraries(ClientTest
    ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARY} ${OGRE_Paging_LIBRARY}
    ${Boost_LIBRARIES}
    ${OIS_LIBRARIES}
    )

if(UNIX)
    include(GetC11CompilerFlag)
    get_c11_flag()
    add_definitions(-Wall -pedantic -Wextra -Wwrite-strings ${c11flag})
endif()

set_target_properties(ClientTest PROPERTIES SOLUTION_FOLDER UnitTests)

add_custom_command(TARGET ClientTest POST_BUILD COMMAND ${CMAKE_BINARY_DIR}/bin/tests/ClientTest)
