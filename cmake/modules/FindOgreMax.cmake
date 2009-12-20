SET( OgreMax_DEFINITIONS "" )
 
##
# You're going to have to edit this file if you intend to use it for your own project
##
SET( OgreMax_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/OgreMax )
 
IF( EXISTS ${OgreMax_INCLUDE_DIR} )
    SET( OgreMax_FOUND TRUE )
    FILE(GLOB OgreMax_source ${OgreMax_INCLUDE_DIR}/*.cpp)
    ADD_LIBRARY(OgreMax ${OgreMax_source})

    SET( OgreMax_LIBRARIES OgreMax TinyXML)
ENDIF( EXISTS ${OgreMax_INCLUDE_DIR} )
 
IF( NOT OgreMax_FOUND)
    IF( NOT OgreMax_INCLUDE_DIR )
        IF (OgreMax_FIND_REQUIRED)
	    	MESSAGE(FATAL_ERROR "Could not find OgreMax")
    	ENDIF (OgreMax_FIND_REQUIRED)
    ENDIF( NOT OgreMax_INCLUDE_DIR )
ELSE ( NOT OgreMax_FOUND)
    INCLUDE_DIRECTORIES("${OgreMax_INCLUDE_DIR}")
ENDIF( NOT OgreMax_FOUND)
 
 
# Finally, display informations if not in quiet mode
IF( NOT OgreMax_FIND_QUIETLY )
  MESSAGE( STATUS "OgreMax found " )
  MESSAGE( STATUS " libraries : ${OgreMax_LIBRARIES}" )
  MESSAGE( STATUS " includes : ${OgreMax_INCLUDE_DIR}" )
ENDIF( NOT OgreMax_FIND_QUIETLY )
 
MARK_AS_ADVANCED(
  OgreMax_INCLUDE_DIR
  OgreMax_LIBRARIES
)
