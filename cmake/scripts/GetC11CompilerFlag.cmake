
macro(get_c11_flag)
if (NOT DEFINED c11flag)
    if (CMAKE_COMPILER_IS_GNUCC)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                        OUTPUT_VARIABLE GCC_VERSION)
        string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
        list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
        list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

        if (${GCC_MINOR} LESS 7)
            set(c11flag "-std=c++0x")
        else()
            set(c11flag "-std=c++11")
        endif()
    endif()
endif()
endmacro()
