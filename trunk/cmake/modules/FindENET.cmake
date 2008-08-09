# - Try to find enet
# Once done this will define
#
#  ENET_FOUND - system has enet
#  ENET_INCLUDE_DIR - the enet include directory
#  ENET_LIBRARIES - the libraries needed to use enet
#  ENET_DEFINITIONS - Compiler switches required for using enet
#
#  Borrowed from The Mana World
#  http://themanaworld.org/
#

IF (ENet_INCLUDE_DIR AND ENet_LIBRARY)
   SET(ENet_FIND_QUIETLY TRUE)
ENDIF (ENet_INCLUDE_DIR AND ENet_LIBRARY)

FIND_PATH(ENet_INCLUDE_DIR enet/enet.h
    /usr/include
    /usr/local/include
    /usr/pack/enet-2007-sd/include
    ../libs/enet-1.1/include
    )

FIND_LIBRARY(ENet_LIBRARY
    NAMES enet
    PATHS /usr/lib /usr/local/lib /usr/pack/enet-2007-sd/i686-debian-linux3.1/lib/
    ../libs/enet-1.1
    )

IF(WIN32)
  SET(WINDOWS_ENET_DEPENDENCIES "ws2_32;winmm")
  SET(ENet_LIBRARY ${ENet_LIBRARY} ${WINDOWS_ENET_DEPENDENCIES})
ENDIF(WIN32)

IF (ENet_INCLUDE_DIR AND ENet_LIBRARY)
    SET(ENET_FOUND TRUE)
    SET(ENET_INCLUDE_DIR ${ENet_INCLUDE_DIR})
    SET(ENET_LIBRARIES ${ENet_LIBRARY})
ELSE (ENet_INCLUDE_DIR AND ENet_LIBRARY)
    SET(ENET_FOUND FALSE)
ENDIF (ENet_INCLUDE_DIR AND ENet_LIBRARY)

IF (ENET_FOUND)
    IF (NOT ENet_FIND_QUIETLY)
        MESSAGE(STATUS "Found enet: ${ENet_LIBRARY}")
    ENDIF (NOT ENet_FIND_QUIETLY)
ELSE (ENET_FOUND)
    IF (ENet_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could NOT find enet")
    ENDIF (ENet_FIND_REQUIRED)
ENDIF (ENET_FOUND)

MARK_AS_ADVANCED(ENet_INCLUDE_DIR ENet_LIBRARY)
