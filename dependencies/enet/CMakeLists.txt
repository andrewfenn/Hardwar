cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(Enet)

set(Enet_MAJOR_VERSION 1)
set(Enet_MINOR_VERSION 3)
set(Enet_PATCH_VERSION 3)

set(Enet_VERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.${Enet_PATCH_VERSION})
set(Enet_COMPATIBILITY_VERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.0)
set(Enet_SOVERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.0)

set(Enet_LIBRARY_NAME Enet)

set(enet_source 
    callbacks.c
    compress.c
    host.c
    list.c
    packet.c
    peer.c
    protocol.c
    unix.c
    win32.c        
)
source_group("Enet" FILES ${enet_source})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Enet STATIC ${enet_source})

if(UNIX)
	set_target_properties(Enet PROPERTIES
		VERSION ${Enet_VERSION}
		SOVERSION ${Enet_SOVERSION}
	)
endif()

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

if (WINDOWS)
     if (WIN32)
        target_link_libraries(Enet "ws2_32.lib")
     elseif (WIN64)
        target_link_libraries(Enet "ws2_64.lib")
     endif()
else () # unix
     include(CheckCSourceCompiles)
     include(CheckCXXSourceCompiles)
     include(CheckStructMember)
     include(CheckType)
     include(CheckFunctionExists)
     include(CheckIncludeFiles)
          
     CHECK_FUNCTION_EXISTS("gethostbyaddr_r"   _HAS_GETHOSTBYADDR_R)
     CHECK_FUNCTION_EXISTS("gethostbyname_r"   _HAS_GETHOSTBYNAME_R)
     CHECK_FUNCTION_EXISTS("poll"              _HAS_POLL)
     CHECK_FUNCTION_EXISTS("fcntl"             _HAS_FCNTL)
     CHECK_FUNCTION_EXISTS("inet_pton"         _HAS_INET_PTON)
     CHECK_FUNCTION_EXISTS("inet_ntop"         _HAS_INET_NTOP)

     CHECK_INCLUDE_FILES ("sys/socket.h"       _HAS_SOCKET_INCLUDE) 
     CHECK_INCLUDE_FILES ("sys/types.h"        _HAS_TYPES_INCLUDE) 

     CHECK_STRUCT_MEMBER("msghdr" "msg_flags" "sys/socket.h"  _HAS_MSGHDR_FLAGS)
     CHECK_TYPE(socklen_t "sys/types.h;sys/socket.h" _HAS_SOCKLEN_T)

     # list all from above here and add them to the definitions
     set(_CHECKS HAS_GETHOSTBYADDR_R; HAS_GETHOSTBYNAME_R; HAS_POLL; HAS_FCNTL; HAS_INET_PTON; HAS_INET_NTOP; HAS_SOCKET_INCLUDE; HAS_TYPES_INCLUDE; HAS_MSGHDR_FLAGS; HAS_SOCKLEN_T;)
     foreach (it ${_CHECKS})
         if (${_${it}})
             set(Enet_DEFINITIONS "${Enet_DEFINITIONS}-D${it}=1 ")
         endif ()
     endforeach (it)

     set_source_files_properties(${enet_source} PROPERTIES COMPILE_FLAGS ${Enet_DEFINITIONS})
     set(CMAKE_CXX_FLAGS -fPIC)
endif ()

