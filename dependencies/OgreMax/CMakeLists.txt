cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(OgreMax)

set(OgreMax_MAJOR_VERSION 2)
set(OgreMax_MINOR_VERSION 1)
set(OgreMax_PATCH_VERSION 2)

set(OgreMax_VERSION ${OgreMax_MAJOR_VERSION}.${OgreMax_MINOR_VERSION}.${OgreMax_PATCH_VERSION})
set(OgreMax_COMPATIBILITY_VERSION ${OgreMax_MAJOR_VERSION}.${OgreMax_MINOR_VERSION}.0)
set(OgreMax_SOVERSION ${OgreMax_MAJOR_VERSION}.${OgreMax_MINOR_VERSION}.0)

set(OgreMax_source
    OgreMaxModel.cpp
    OgreMaxScene.cpp
    OgreMaxUtilities.cpp
    ProgressCalculator.cpp
    Version.cpp
)

set(OgreMax_LIBRARY_NAME OgreMax)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
set(LIBRARY_OUTPUT_PATH  ../../lib)

source_group("OgreMax" FILES ${OgreMax_source})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(../TinyXML/)
include_directories(${OGRE_INCLUDE_DIRS})

link_libraries(TinyXML)

add_library(OgreMax STATIC ${OgreMax_source})

add_dependencies(OgreMax TinyXML)
add_dependencies(OgreMax OGRE)
add_dependencies(OgreMax Boost)

IF(UNIX)
	SET_TARGET_PROPERTIES(OgreMax PROPERTIES
		VERSION ${OgreMax_VERSION}
		SOVERSION ${OgreMax_SOVERSION}
	)

    set(CMAKE_CXX_FLAGS -fPIC)

ENDIF(UNIX)

set_target_properties(OgreMax PROPERTIES SOLUTION_FOLDER Dependencies)
target_link_libraries(OgreMax ${OGRE_LIBRARIES} ${Boost_LIBRARIES} ${TinyXML_LIBRARIES})


