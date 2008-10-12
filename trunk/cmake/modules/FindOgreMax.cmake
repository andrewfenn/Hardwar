SET( OgreMax_DEFINITIONS "" )
 
##
# Includes
##
SET( OgreMax_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OgreMax )
 
IF( EXISTS ${OgreMax_INCLUDE_DIR} )
    SET( OgreMax_FOUND TRUE )
ENDIF( EXISTS ${OgreMax_INCLUDE_DIR} )
 
IF( NOT OgreMax_FOUND)
    IF( NOT OgreMax_INCLUDE_DIR )
        IF (CEGUIOGRE_FIND_REQUIRED)
	    	MESSAGE(FATAL_ERROR "Could not find OgreMax")
    	ENDIF (CEGUIOGRE_FIND_REQUIRED)
    ENDIF( NOT OgreMax_INCLUDE_DIR )
ENDIF( NOT OgreMax_FOUND)
 
 
# Finally, display informations if not in quiet mode
IF( NOT OgreMax_FIND_QUIETLY )
  MESSAGE( STATUS "OgreMax found " )
  MESSAGE( STATUS " includes : ${OgreMax_INCLUDE_DIR}" )
ENDIF( NOT OgreMax_FIND_QUIETLY )
 
MARK_AS_ADVANCED(
  OgreMax_INCLUDE_DIR
)
