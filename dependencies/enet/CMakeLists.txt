cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(Enet)

SET(Enet_MAJOR_VERSION 1)
SET(Enet_MINOR_VERSION 3)
SET(Enet_PATCH_VERSION 3)

SET(Enet_VERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.${Enet_PATCH_VERSION})
SET(Enet_COMPATIBILITY_VERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.0)
SET(Enet_SOVERSION ${Enet_MAJOR_VERSION}.${Enet_MINOR_VERSION}.0)

SET(Enet_LIBRARY_NAME Enet)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)

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
SOURCE_GROUP("Enet" FILES ${enet_source})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
ADD_LIBRARY(Enet STATIC ${enet_source})

IF(UNIX)
	SET_TARGET_PROPERTIES(Enet PROPERTIES
		VERSION ${Enet_VERSION}
		SOVERSION ${Enet_SOVERSION}
	)
ENDIF(UNIX)

set(LIBRARY_OUTPUT_PATH  ../../lib)

IF (WINDOWS)
     IF (WIN32)
        TARGET_LINK_LIBRARIES(Enet "ws2_32.lib")
     ELSEIF (WIN64)
        TARGET_LINK_LIBRARIES(Enet "ws2_64.lib")
     ENDIF ()
ELSE (WINDOWS) # unix
     INCLUDE(CheckCSourceCompiles)
     INCLUDE(CheckCXXSourceCompiles)
     INCLUDE(CheckStructMember)
     INCLUDE(CheckType)
     INCLUDE(CheckFunctionExists) 
     INCLUDE(CheckIncludeFiles) 
          
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
     SET(_CHECKS HAS_GETHOSTBYADDR_R; HAS_GETHOSTBYNAME_R; HAS_POLL; HAS_FCNTL; HAS_INET_PTON; HAS_INET_NTOP; HAS_SOCKET_INCLUDE; HAS_TYPES_INCLUDE; HAS_MSGHDR_FLAGS; HAS_SOCKLEN_T;)
     FOREACH (it ${_CHECKS})
         IF (${_${it}})
             SET(Enet_DEFINITIONS "${Enet_DEFINITIONS}-D${it}=1 ")
         ENDIF (${_${it}})
     ENDFOREACH (it)

     SET_SOURCE_FILES_PROPERTIES(${enet_source} PROPERTIES COMPILE_FLAGS ${Enet_DEFINITIONS})
ENDIF (WINDOWS)

if(UNIX)
        set(CMAKE_CXX_FLAGS -fPIC)
endif(UNIX)

