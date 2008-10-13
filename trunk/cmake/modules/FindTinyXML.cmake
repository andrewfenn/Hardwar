SET( TinyXML_DEFINITIONS "-TIXML_USE_STL")
 
##
# You're going to have to edit this file if you intend to use it for your own project
##
SET( TinyXML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/TinyXML )
 
IF( EXISTS ${TinyXML_INCLUDE_DIR} )
    SET( TinyXML_FOUND TRUE )
    FILE(GLOB TinyXML_source ${TinyXML_INCLUDE_DIR}/*.cpp)
    ADD_LIBRARY(TinyXML ${TinyXML_source})

    SET( TinyXML_LIBRARIES TinyXML)    
ENDIF( EXISTS ${TinyXML_INCLUDE_DIR} )
 
IF( NOT TinyXML_FOUND)
    IF( NOT TinyXML_INCLUDE_DIR )
        IF (TinyXML_FIND_REQUIRED)
	    	MESSAGE(FATAL_ERROR "Could not find TinyXML")
    	ENDIF (TinyXML_FIND_REQUIRED)
    ENDIF( NOT TinyXML_INCLUDE_DIR )
ELSE ( NOT TinyXML_FOUND)
    INCLUDE_DIRECTORIES("${TinyXML_INCLUDE_DIR}")
ENDIF( NOT TinyXML_FOUND)
 
 
# Finally, display informations if not in quiet mode
IF( NOT TinyXML_FIND_QUIETLY )
  MESSAGE( STATUS "TinyXML found " )
  MESSAGE( STATUS " libraries : ${TinyXML_LIBRARIES}" )
  MESSAGE( STATUS " includes : ${TinyXML_INCLUDE_DIR}" )
ENDIF( NOT TinyXML_FIND_QUIETLY )
 
MARK_AS_ADVANCED(
  TinyXML_INCLUDE_DIR
  TinyXML_LIBRARIES
)
