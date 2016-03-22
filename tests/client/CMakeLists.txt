cmake_minimum_required(VERSION 2.8)
project(clientTest)

add_subdirectory("${CMAKE_SOURCE_DIR}/code/client/core/" "${CMAKE_CURRENT_BINARY_DIR}/client/core/")
add_subdirectory("${CMAKE_SOURCE_DIR}/code/client/tasks/" "${CMAKE_CURRENT_BINARY_DIR}/client/tasks/")
add_subdirectory("${CMAKE_SOURCE_DIR}/code/client/states/" "${CMAKE_CURRENT_BINARY_DIR}/client/states/")
add_subdirectory("${CMAKE_SOURCE_DIR}/code/client/helpers/" "${CMAKE_CURRENT_BINARY_DIR}/client/helpers/")

set(clientTest_source
    ${client_source}
    ${CMAKE_SOURCE_DIR}/code/shared/src/Building.cpp
    ${CMAKE_SOURCE_DIR}/code/shared/src/GameSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/TestGameRoot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/TestGameManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/TestGameState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/TestGameTask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/TestNetworkTask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

source_group("clientTest" FILES ${clientTest_source})

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/dependencies/unittest/UnitTest++")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/dependencies/amop/include")
include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})
include_directories(SYSTEM ${CEGUI_INCLUDE_DIR})
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

link_libraries(UnitTest++ Enet)

if (WIN32)
   set(exe "WIN32")
endif ()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests/)
add_executable(clientTest ${exe} ${clientTest_source})

add_dependencies(clientTest yaarg)
add_dependencies(clientTest UnitTest++)
add_dependencies(clientTest Enet)
add_dependencies(clientTest RocketCore)
add_dependencies(clientTest RocketControls)
add_dependencies(clientTest RocketDebugger)
add_dependencies(clientTest OIS)

target_link_libraries(clientTest
    ${OGRE_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OIS_LIBRARIES}
    RocketCore RocketControls RocketDebugger
    yaarg
)

set(CMAKE_CXX_FLAGS "${ORIGINAL_CMAKE_CXX_FLAGS} -Wall -pthread -pedantic -Wextra -Wwrite-strings -std=c++11")

set_target_properties(clientTest PROPERTIES SOLUTION_FOLDER UnitTests)

add_custom_command(TARGET clientTest POST_BUILD COMMAND ${CMAKE_BINARY_DIR}/bin/tests/clientTest)
