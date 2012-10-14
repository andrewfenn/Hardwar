 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #
 #  Author:
 #    Reto Grieder
 #  Description:
 #    Extension of the standard module "FindPackageHandleStandardArgs".
 #    This function also checks the version requirements. Also regards the
 #    EXACT keyword if specified in FIND_PACKAGE(...).
 #

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(CompareVersionStrings)

FUNCTION(FIND_PACKAGE_HANDLE_ADVANCED_ARGS _name _fail_msg _version)

  # Modify the message to include version requirements
  IF("${_fail_msg}" STREQUAL "DEFAULT_MSG")
    SET(_fail_msg_adv "Could NOT find ${_name}")
  ELSE()
    SET(_fail_msg_adv "${_fail_msg}")
  ENDIF()
  IF(${_name}_FIND_VERSION_EXACT)
    SET(_fail_msg_adv "${_fail_msg_adv} (version requirements: exactly ${${_name}_FIND_VERSION})")
  ELSE()
    SET(_fail_msg_adv "${_fail_msg_adv} (version requirements: at least ${${_name}_FIND_VERSION})")
  ENDIF()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS("${_name}" "${_fail_msg_adv}" ${ARGN})
  STRING(TOUPPER ${_name} _NAME_UPPER)

  # Also check version requirements if given to FindPackage(...)
  IF(${_name}_FIND_VERSION)
    # Note: the last argument of the function tells it to cut the first
    #       version string instead of padding it with zeros if necessary
    COMPARE_VERSION_STRINGS("${_version}" "${${_name}_FIND_VERSION}" _compatible TRUE)
    IF(${_name}_FIND_VERSION_EXACT AND NOT _compatible EQUAL 0)
      MESSAGE(FATAL_ERROR "Exact ${_name} version required is ${${_name}_FIND_VERSION}\n"
                          "Your version is ${_version}")
      SET(${_NAME_UPPER}_FOUND FALSE)
    ELSEIF(_compatible LESS 0)
      MESSAGE(FATAL_ERROR "Minimum ${_name} version required is ${${_name}_FIND_VERSION}\n"
                          "Your version is ${_version}")
      SET(${_NAME_UPPER}_FOUND FALSE)
    ENDIF()
  ENDIF(${_name}_FIND_VERSION)

  # Raise scope (FindPackageHandleStandardArgs uses PARENT_SCOPE)
  SET(${_NAME_UPPER}_FOUND ${${_NAME_UPPER}_FOUND} PARENT_SCOPE)

ENDFUNCTION(FIND_PACKAGE_HANDLE_ADVANCED_ARGS)
