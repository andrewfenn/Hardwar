if (WINDOWS)

    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "Debug Postfitx for libraries")
    endif(WIN32)

    if(MSVC80 OR MSVC90 OR MSVC10)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_definitions(-D_SCL_SECURE_NO_WARNINGS)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_definitions(-DNOMINMAX)
    endif(MSVC80 OR MSVC90 OR MSVC10)

endif()
