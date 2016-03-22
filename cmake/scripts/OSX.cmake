
if (NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 1)
endif()

set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.hardwar.hardwar")
set(CMAKE_VERBOSE_MAKEFILE true)

add_definitions(-x objective-c++)
