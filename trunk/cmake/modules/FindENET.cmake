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
    IF (MSVC_IDE)
	TARGET_LINK_LIBRARIES(Enet "ws2_32.lib")
    ENDIF (MSVC_IDE)
ENDIF( EXISTS ${ENET_INCLUDE_DIR} )
 
IF( NOT ENET_FOUND)
    IF( NOT ENET_INCLUDE_DIR )
        IF (ENET_FIND_REQUIRED)
	    	MESSAGE(FATAL_ERROR "Could not find Enet")
    	ENDIF (ENET_FIND_REQUIRED)
    ENDIF( NOT ENET_INCLUDE_DIR )
ELSE ( NOT ENET_FOUND)
    INCLUDE_DIRECTORIES("${ENET_INCLUDE_DIR}/include")
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

