cmake_minimum_required(VERSION 2.8)
project(HARDWAR)
set( HARDWAR_MAJOR_VERSION 0 )
set( HARDWAR_MINOR_VERSION 1 )
set( HARDWAR_PATCH_LEVEL 0 )

set( CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${HARDWAR_SOURCE_DIR}/cmake/modules"
    "${HARDWAR_SOURCE_DIR}/cmake/scripts"
)

include(Build)
include(SourceFiles)
include(Windows)
include(OSX)

set(Boost_USE_STATIC_LIBS TRUE)
find_package(Boost COMPONENTS thread filesystem system)
find_package(OIS REQUIRED)

set(DEPENDENCIES_LIBPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML")

set(OGRE_STATIC TRUE)
set(OGRE_USE_BOOST TRUE)
set(OGRE_BUILD_SAMPLES FALSE)
set(OGRE_BUILD_TOOLS FALSE)
set(OGRE_UNITY_BUILD TRUE)
set(TINYXML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML")
set(TINYXML_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(FREETYPE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include")
set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ogre")


set(HARDWAR_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/code/client/include")
set(HARDWAR_LIBPATH  "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(DEPENDENCIES_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ogre/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ogre/OgreMain/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libyaarg/"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/enet/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/poshlib"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OgreMax"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libRocket/Include"
)



add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libyaarg")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/enet")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/poshlib")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/UnitTest++")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/amop")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libRocket")

# client unit tests
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/client")

# client
add_subdirectory(code/client)

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/server")
#add_subdirectory(code/server)

file(COPY
    ${CMAKE_SOURCE_DIR}/bin/media
    ${CMAKE_SOURCE_DIR}/bin/world

    ${CMAKE_SOURCE_DIR}/bin/gdb-client
    ${CMAKE_SOURCE_DIR}/bin/plugins.cfg
    ${CMAKE_SOURCE_DIR}/bin/resources.cfg
    ${CMAKE_SOURCE_DIR}/bin/server.cfg
    ${CMAKE_SOURCE_DIR}/bin/settings.cfg
    DESTINATION ${CMAKE_BINARY_DIR}/bin
)
