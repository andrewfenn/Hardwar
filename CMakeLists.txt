cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(HARDWAR CXX)

set( HARDWAR_MAJOR_VERSION 0 )
set( HARDWAR_MINOR_VERSION 1 )
set( HARDWAR_PATCH_LEVEL 0 )

set(HARDWAR_VERSION_STRING "${HARDWAR_MAJOR_VERSION}.${HARDWAR_MINOR_VERSION}.${HARDWAR_PATCH_LEVEL}")
message(STATUS "Building Hardwar version ${HARDWAR_VERSION_STRING}")
set(abi_version ${HARDWAR_VERSION_STRING})

set(HARDWAR_LIBPATH  "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HARDWAR_LIBPATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HARDWAR_LIBPATH}")
#set(DEPENDENCIES_LIBPATH "${HARDWAR_LIBPATH}")
set(LIBRARY_OUTPUT_PATH "${HARDWAR_LIBPATH}")

#set(CMAKE_MACOSX_RPATH "${CMAKE_MACOSX_RPATH};${HARDWAR_LIBPATH}")
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${HARDWAR_LIBPATH}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set( CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${HARDWAR_SOURCE_DIR}/cmake/modules"
    "${HARDWAR_SOURCE_DIR}/cmake/scripts"
    "${HARDWAR_SOURCE_DIR}/dependencies/ogre/CMake/Packages/"
)

include(Build)
set(CMAKE_DISABLE_MESSAGE TRUE)

include(SourceFiles)
include(Windows)

if(APPLE)
    include(OSX)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC ON)
set(Boost_USE_MULTITHREAD ON)
set(BOOST_INCLUDEDIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*/")
find_package(Boost REQUIRED COMPONENTS thread filesystem system)

set(OIS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ois/includes")
set(OIS_LIBRARIES "OIS")
set(OIS_BUILD_DEMOS False)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ois")

# Surpress compiler warnings from dependencies
set(ORIGINAL_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include")
set(FREETYPE_FT2BUILD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include")
set(FREETYPE_LIBRARIES "freetype")
set(FREETYPE_FOUND True)
set(BUILD_SHARED_LIBS False)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype")

set(TINYXML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML")
set(TINYXML_LIBRARIES "${DEPENDENCIES_LIBPATH}")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML")

set(OGRE_STATIC TRUE CACHE BOOL NULL FORCE)
set(OGRE_USE_BOOST TRUE CACHE BOOL NULL FORCE)
set(OGRE_BUILD_TEST FALSE CACHE BOOL NULL FORCE)
set(OGRE_BUILD_SAMPLES FALSE CACHE BOOL NULL FORCE)
set(OGRE_BUILD_TOOLS FALSE CACHE BOOL NULL FORCE)
set(OGRE_UNITY_BUILD TRUE CACHE BOOL NULL FORCE)
set(OGRE_CONFIG_ENABLE_ZIP TRUE CACHE BOOL NULL FORCE)
set(OGRE_BUILD_PLUGIN_CG FALSE CACHE BOOL NULL FORCE)
set(OGRE_INSTALL_PDB FALSE CACHE BOOL NULL FORCE)
set(OGRE_INSTALL_SAMPLES FALSE CACHE BOOL NULL FORCE)
set(OGRE_INSTALL_TOOLS FALSE CACHE BOOL NULL FORCE)
set(OGRE_INSTALL_DOCS FALSE CACHE BOOL NULL FORCE)
set(OGRE_INSTALL_SAMPLES_SOURCE FALSE CACHE BOOL NULL FORCE)
set(OGRE_UNITY_BUILD TRUE CACHE BOOL NULL FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ogre")
set(CMAKE_CXX_FLAGS "${ORIGINAL_CMAKE_CXX_FLAGS} -w")
set(OGRE_LIBRARIES OgreMain RenderSystem_GL Plugin_OctreeZone OgreProperty Plugin_OctreeSceneManager Plugin_BSPSceneManager Plugin_ParticleFX Plugin_PCZSceneManager OgrePaging OgreTerrain OgreRTShaderSystem)
set_target_properties(${OGRE_LIBRARIES} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${HARDWAR_LIBPATH}" ARCHIVE_OUTPUT_DIRECTORY "${HARDWAR_LIBPATH}")


set(HARDWAR_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/code/client/include")
set(DEPENDENCIES_INCLUDES
    "${CMAKE_CURRENT_BINARY_DIR}/dependencies/ogre/include"
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

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/unittest")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/amop")

set(BUILD_PYTHON_BINDINGS FALSE CACHE BOOL NULL FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libRocket/Build")

# Turn off messages from external librarie's cmake scripts
set(CMAKE_DISABLE_MESSAGE FALSE)

# Re-enable compiler warnings for hardwar code
set(CMAKE_CXX_FLAGS "${ORIGINAL_CMAKE_CXX_FLAGS}")

# client unit tests
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/client")

# client
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/code/client")

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/server")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/code/server")

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
