cmake_minimum_required(VERSION 2.6)
project(HARDWAR)
cmake_policy(SET CMP0003 NEW)

# project version
SET( HARDWAR_MAJOR_VERSION 0 )
SET( HARDWAR_MINOR_VERSION 1 )
SET( HARDWAR_PATCH_LEVEL 0 )

SET( CMAKE_MODULE_PATH   "${CMAKE_MODULE_PATH}"
                         "${HARDWAR_SOURCE_DIR}/cmake/modules"
                         "${HARDWAR_SOURCE_DIR}/cmake/scripts"
 )

message(STATUS "Building Hardwar version ${HARDWAR_VERSION_STRING}")
set(abi_version ${HARDWAR_VERSION_STRING})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Set compiler specific build flags
IF (UNIX)
   add_definitions(-msse -O3)
ENDIF()

if(WIN32)
set(CMAKE_DEBUG_POSTFIX "_d")
endif(WIN32)

if(MSVC80 OR MSVC90 OR MSVC10)
        # Option is to enable the /MP switch for Visual Studio 2005 or later
        option(OGITOR_MSVC_ENABLE_MP
                "Set to ON to build Ogitor with the /MP option (Visual Studio 2005 and above)." ON)
        mark_as_advanced(OGITOR_MSVC_ENABLE_MP)
        if(OGITOR_MSVC_ENABLE_MP)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        endif(OGITOR_MSVC_ENABLE_MP)
        message(STATUS "Setting Visual Studio 2005+ option /MP to ${HARDWAR_MSVC_ENABLE_MP}")
        add_definitions(-D_SCL_SECURE_NO_WARNINGS)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_definitions(-DNOMINMAX)
endif(MSVC80 OR MSVC90 OR MSVC10)

find_package(OGRE REQUIRED)

if((NOT "${OGRE_VERSION_NAME}" STREQUAL "Byatis") AND (NOT "${OGRE_VERSION_NAME}" STREQUAL "Ghadamon"))
    message(SEND_ERROR "You need Ogre 1.8 Byatis or later to build this.")
endif()

if(APPLE)
    find_package(CG REQUIRED)
endif(APPLE)

if (NOT OGRE_BUILD_PLATFORM_IPHONE)
    if (WIN32 OR APPLE)
        set(Boost_USE_STATIC_LIBS TRUE)
    else ()
        # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
        set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
    endif ()
    set(Boost_ADDITIONAL_VERSIONS "1.38.0" "1.38" "1.39.0" "1.39" "1.40.0" "1.40" "1.44.0" "1.44" "1.45" "1.46" "1.47" "1.48" "1.49" "1.49.0" "1.50" "1.50.0")
    # Components that need linking (NB does not include header-only components like bind)
    set(OGRE_BOOST_COMPONENTS thread date_time regex)
    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    if (NOT Boost_FOUND)
        # Try again with the other type of libs
        set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    endif()
    if (Boost_FOUND AND WIN32)
        if(Boost_VERSION GREATER 104700)
            set(OGRE_BOOST_COMPONENTS thread filesystem system)
        endif(Boost_VERSION GREATER 104700)
        if(Boost_VERSION GREATER 104900)
            set(OGRE_BOOST_COMPONENTS thread filesystem system)
        endif(Boost_VERSION GREATER 104900)
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    endif(Boost_FOUND AND WIN32)
    find_package(Boost QUIET)
endif(NOT OGRE_BUILD_PLATFORM_IPHONE)

if(UNIX)
    get_filename_component(OGRE_PLUGIN_DIR_LINUX ${OGRE_Plugin_OctreeSceneManager_LIBRARY_REL} PATH)
    set (OGRE_PLUGIN_DIR_LINUX ${OGRE_PLUGIN_DIR_LINUX} CACHE STRING "Ogre plugin dir (release)")
endif(UNIX)

find_package(OIS REQUIRED)
find_package(MYGUI REQUIRED)

set(HARDWAR_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/code/client/include")
set(HARDWAR_LIBPATH  "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(DEPENDENCIES_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/enet/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/poshlib"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OgreMax"
)

set(DEPENDENCIES_LIBPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/enet)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/poshlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OgreMax)
add_subdirectory(code/client)
#add_subdirectory(code/server)
