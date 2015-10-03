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

function(message)
    list(GET ARGV 0 MessageType)
    if(NOT CMAKE_DISABLE_MESSAGE)
        if(MessageType STREQUAL FATAL_ERROR OR
         MessageType STREQUAL SEND_ERROR OR
         MessageType STREQUAL WARNING OR
         MessageType STREQUAL AUTHOR_WARNING)
        list(REMOVE_AT ARGV 0)
        _message(${MessageType} "${ARGV}")
        endif()
    endif()
endfunction()
