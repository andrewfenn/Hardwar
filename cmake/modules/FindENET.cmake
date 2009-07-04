SET( ENET_DEFINITIONS "")

##
# You're going to have to edit this file if you intend to use it for your own project
##
SET( ENET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/enet )
 
IF( EXISTS ${ENET_INCLUDE_DIR} )
    SET( ENET_FOUND TRUE )
    FILE(GLOB ENET_source ${ENET_INCLUDE_DIR}/*.c)
    ADD_LIBRARY(Enet ${ENET_source})
    SET(ENET_LIBRARIES Enet)
    SET(ENET_INCLUDE_DIR ${ENET_INCLUDE_DIR}/include)
    IF (WINDOWS)
        IF (WIN32)
    	    TARGET_LINK_LIBRARIES(Enet "ws2_32.lib")
        ELSE (WIN32)
            TARGET_LINK_LIBRARIES(Enet "ws2_64.lib")
        ENDIF (WIN32)
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
                SET(ENET_DEFINITIONS "${ENET_DEFINITIONS}-D${it}=1 ")
            ENDIF (${_${it}})
        ENDFOREACH (it)

        SET_SOURCE_FILES_PROPERTIES(${ENET_source} PROPERTIES COMPILE_FLAGS ${ENET_DEFINITIONS})
    ENDIF (WINDOWS)
ENDIF( EXISTS ${ENET_INCLUDE_DIR} )
 
IF( NOT ENET_FOUND)
    IF( NOT ENET_INCLUDE_DIR )
        IF (ENET_FIND_REQUIRED)
	    	MESSAGE(FATAL_ERROR "Could not find Enet")
    	ENDIF (ENET_FIND_REQUIRED)
    ENDIF( NOT ENET_INCLUDE_DIR )
ENDIF( NOT ENET_FOUND)
 
 
# Finally, display informations if not in quiet mode
IF( NOT ENET_FIND_QUIETLY )
  MESSAGE( STATUS "Enet found " )
  MESSAGE( STATUS " libraries : ${ENET_LIBRARIES}" )
  MESSAGE( STATUS " includes : ${ENET_INCLUDE_DIR}" )
ENDIF( NOT ENET_FIND_QUIETLY )
 
MARK_AS_ADVANCED(
  ENET_INCLUDE_DIR
  ENET_LIBRARIES
)

