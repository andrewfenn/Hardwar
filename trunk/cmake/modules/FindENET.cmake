IF (ENET_INCLUDE_DIR AND ENET_LIBRARY)
   SET(ENET_FIND_QUIETLY TRUE)
ENDIF (ENET_INCLUDE_DIR AND ENET_LIBRARY)

FIND_PATH(ENet_INCLUDE_DIR enet/enet.h
    /usr/include
    /usr/local/include
    /usr/pack/enet-2007-sd/include
    ../libs/enet-1.1/include
    C:/enet-1.2/include/
    C:/enet/include/
    )

FIND_LIBRARY(ENet_LIBRARY
    NAMES enet enet.lib
    PATHS /usr/lib /usr/local/lib /usr/pack/enet-2007-sd/i686-debian-linux3.1/lib/
    ../libs/enet-1.1
    C:/WINDOWS/System32/
    C:/enet-1.2/Debug
    C:/enet-1.2/Release
    C:/enet/Debug
    C:/enet/Release
    )

IF(WIN32)
  SET(WINDOWS_ENET_DEPENDENCIES "ws2_32;winmm;")
  SET(ENet_LIBRARY ${WINDOWS_ENET_DEPENDENCIES})
ENDIF(WIN32)

IF (ENet_INCLUDE_DIR AND ENet_LIBRARY)
    SET(ENET_FOUND TRUE)
    SET(ENET_INCLUDE_DIR ${ENet_INCLUDE_DIR})
    SET(ENET_LIBRARIES ${ENet_LIBRARY})
ELSE (ENet_INCLUDE_DIR AND ENet_LIBRARY)
    SET(ENET_FOUND FALSE)
ENDIF (ENet_INCLUDE_DIR AND ENet_LIBRARY)

IF (ENET_FOUND)
    IF (NOT ENET_FIND_QUIETLY)
        MESSAGE( STATUS "Found enet:")
	  MESSAGE( STATUS " libraries : ${ENET_LIBRARIES}" )
	  MESSAGE( STATUS " includes : ${ENET_INCLUDE_DIR}" )
    ENDIF (NOT ENET_FIND_QUIETLY)
ELSE (ENET_FOUND)
    IF (ENET_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could NOT find enet.")
	  IF(WIN32)
		MESSAGE(FATAL_ERROR "Did you put enet in C:/enet/ and build it?")
	  ENDIF(WIN32)
    ENDIF (ENET_FIND_REQUIRED)
ENDIF (ENET_FOUND)

MARK_AS_ADVANCED(
	ENET_INCLUDE_DIR 
	ENET_LIBRARIES
)
