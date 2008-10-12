#Change PROJECT_NAME to the name of your project
PROJECT(Hardwar)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 4 )

SET( CMAKE_MODULE_PATH    ${CMAKE_MODULE_PATH}
                          ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
   Boost
   OIS
   OGRE
   CEGUI
   CEGUIOGRE
   SQLITE
   ENET
   TinyXML
   OgreMax
)

# searches for all .cpp and .h files in the current directory and add them 
# to the current project

FILE(GLOB folder_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
SOURCE_GROUP("Hardwar" FILES ${folder_source})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_DEFINITIONS( "-Wall" )
  
# create the project
ADD_EXECUTABLE(hardwar ${folder_source})

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  FIND_PACKAGE(${Package} REQUIRED)
  IF (${Package}_FOUND)
      INCLUDE_DIRECTORIES(${${Package}_INCLUDE_DIR})
      IF (${Package}_LIBRARIES)
         TARGET_LINK_LIBRARIES(hardwar ${${Package}_LIBRARIES})
      ELSE (${Package}_LIBRARIES)
         LINK_DIRECTORIES(${${Package}_LIBRARY_DIRS})
      ENDIF (${Package}_LIBRARIES)
  ELSE (${Package}_FOUND)
      MESSAGE(STATUS "${Package} not found")
  ENDIF(${Package}_FOUND)
ENDFOREACH(Package)

FILE(GLOB TinyXML_source ${TinyXML_INCLUDE_DIR}/*.cpp)
ADD_LIBRARY(TinyXML ${TinyXML_source})

FILE(GLOB OgreMax_source ${OgreMax_INCLUDE_DIR}/*.cpp)
ADD_LIBRARY(OgreMax ${OgreMax_source})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)
TARGET_LINK_LIBRARIES(hardwar boost_filesystem)
TARGET_LINK_LIBRARIES(hardwar TinyXML)
TARGET_LINK_LIBRARIES(hardwar OgreMax)

