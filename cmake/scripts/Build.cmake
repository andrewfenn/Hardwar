if(CMAKE_BUILD_TYPE STREQUAL "")
    # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
    # differentiation between debug and release builds.
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (UNIX)
       add_definitions(-O1)
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if (UNIX)
       add_definitions(-msse -O3)
    endif()
endif ()
message(STATUS "Build type set to: ${CMAKE_BUILD_TYPE}")