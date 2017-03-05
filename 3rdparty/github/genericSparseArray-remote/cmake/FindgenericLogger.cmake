# Module for locating genericLogger, based on ICU module.
#
# Cutomizable variables:
#   GENERICLOGGER_ROOT_DIR
#     This variable points to the genericLogger root directory. On Windows the
#     library location typically will have to be provided explicitly using the
#     -D command-line option. Alternatively, an environment variable can be set.
#
# Read-Only variables:
#   GENERICLOGGER_FOUND
#     Indicates whether the library has been found.
#
#   GENERICLOGGER_INCLUDE_DIRS
#     Points to the genericLogger include directory.
#
#   GENERICLOGGER_LIBRARIES
#     Points to the genericLogger libraries that should be passed to
#     target_link_libraries in dynamic mode.
#
#   GENERICLOGGER_LIBRARIES_STATIC
#     Points to the genericLogger libraries static that should be passed to
#     target_link_libraries in static mode.
#
#   GENERICLOGGER_LINK_FLAGS
#     Linker flags when building linking against genericLogger. Defaults to "libgenericLogger". Take care, this is a list.
#
#   GENERICLOGGER_STATIC_LINK_FLAGS
#     Linker flags when building linking against static genericLogger. Defaults to "libgenericLogger_static". Take care, this is a list.
#
#   GENERICLOGGER_LDFLAGS
#     Stringified version of GENERICLOGGER_LINK_FLAGS
#
#   GENERICLOGGER_STATIC_LDFLAGS
#     Stringified version of GENERICLOGGER_STATIC_LINK_FLAGS
#
INCLUDE (CMakeParseArguments)
INCLUDE (FindPackageHandleStandardArgs)

SET (_PF86 "ProgramFiles(x86)")
SET (_GENERICLOGGER_POSSIBLE_DIRS
  ${GENERICLOGGER_ROOT_DIR}
  "$ENV{GENERICLOGGER_ROOT_DIR}"
  "C:/genericLogger"
  "$ENV{PROGRAMFILES}/genericLogger"
  "$ENV{${_PF86}}/genericLogger")

SET (_GENERICLOGGER_POSSIBLE_INCLUDE_SUFFIXES include)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES lib64)
  SET (_GENERICLOGGER_POSSIBLE_BIN_SUFFIXES bin64)

  IF (NOT WIN32)
    LIST (APPEND _GENERICLOGGER_POSSIBLE_LIB_SUFFIXES lib)
    LIST (APPEND _GENERICLOGGER_POSSIBLE_BIN_SUFFIXES bin)
  ENDIF (NOT WIN32)
ELSE (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES lib)
  SET (_GENERICLOGGER_POSSIBLE_BIN_SUFFIXES bin)
ENDIF (CMAKE_SIZEOF_VOID_P EQUAL 8)

FIND_PATH (GENERICLOGGER_ROOT_DIR
  NAMES include/genericLogger.h
  PATHS ${_GENERICLOGGER_POSSIBLE_DIRS}
  DOC "genericLogger root directory")

IF (GENERICLOGGER_ROOT_DIR)
  # Re-use the previous path:
  FIND_PATH (GENERICLOGGER_INCLUDE_DIR
    NAMES genericLogger.h
    PATHS ${GENERICLOGGER_ROOT_DIR}
    PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_INCLUDE_SUFFIXES}
    DOC "genericLogger include directory"
    NO_DEFAULT_PATH)
ELSE (GENERICLOGGER_ROOT_DIR)
  # Use default path search
  FIND_PATH (GENERICLOGGER_INCLUDE_DIR
    NAMES genericLogger.h
    DOC "genericLogger include directory"
    )
ENDIF (GENERICLOGGER_ROOT_DIR)

IF (NOT GENERICLOGGER_FIND_COMPONENTS)
  SET (GENERICLOGGER_FIND_COMPONENTS genericLogger)
ENDIF (NOT GENERICLOGGER_FIND_COMPONENTS)

IF (GENERICLOGGER_INCLUDE_DIR)
  # Loop over each components
  FOREACH (_GENERICLOGGER_COMPONENT ${GENERICLOGGER_FIND_COMPONENTS})
    SET (_GENERICLOGGER_COMPONENT_BASE ${_GENERICLOGGER_COMPONENT})
    SET (_GENERICLOGGER_COMPONENT_POSSIBLE_DEBUG_NAMES ${_GENERICLOGGER_COMPONENT_BASE}d)
    SET (_GENERICLOGGER_COMPONENT_POSSIBLE_RELEASE_NAMES ${_GENERICLOGGER_COMPONENT_BASE})

    SET (_GENERICLOGGER_STATIC_COMPONENT_BASE "${_GENERICLOGGER_COMPONENT}_static")
    SET (_GENERICLOGGER_STATIC_COMPONENT_POSSIBLE_DEBUG_NAMES ${_GENERICLOGGER_STATIC_COMPONENT_BASE}d)
    SET (_GENERICLOGGER_STATIC_COMPONENT_POSSIBLE_RELEASE_NAMES ${_GENERICLOGGER_STATIC_COMPONENT_BASE})

    STRING (TOUPPER ${_GENERICLOGGER_COMPONENT} _GENERICLOGGER_COMPONENT_UPPER)
    SET (_GENERICLOGGER_LIBRARY_BASE GENERICLOGGER_${_GENERICLOGGER_COMPONENT_UPPER}_LIBRARY)
    SET (_GENERICLOGGER_STATIC_LIBRARY_BASE GENERICLOGGER_STATIC_${_GENERICLOGGER_COMPONENT_UPPER}_LIBRARY)

    IF (GENERICLOGGER_ROOT_DIR)
      FIND_LIBRARY (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG
	NAMES ${_GENERICLOGGER_COMPONENT_POSSIBLE_DEBUG_NAMES}
	PATHS ${GENERICLOGGER_ROOT_DIR} PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
      FIND_LIBRARY (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG
	NAMES ${_GENERICLOGGER_STATIC_COMPONENT_POSSIBLE_DEBUG_NAMES}
	PATHS ${GENERICLOGGER_ROOT_DIR} PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
    ELSE (GENERICLOGGER_ROOT_DIR)
      FIND_LIBRARY (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG
	NAMES ${_GENERICLOGGER_COMPONENT_POSSIBLE_DEBUG_NAMES}
	PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	)
      FIND_LIBRARY (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG
	NAMES ${_GENERICLOGGER_COMPONENT_STATIC_POSSIBLE_DEBUG_NAMES}
	PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	)
    ENDIF (GENERICLOGGER_ROOT_DIR)

    IF (GENERICLOGGER_ROOT_DIR)
      FIND_LIBRARY (${_GENERICLOGGER_LIBRARY_BASE}_RELEASE
	NAMES ${_GENERICLOGGER_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATHS ${GENERICLOGGER_ROOT_DIR} PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
      FIND_LIBRARY (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE
	NAMES ${_GENERICLOGGER_STATIC_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATHS ${GENERICLOGGER_ROOT_DIR} PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
    ELSE (ICU_ROOT_DIR)
      FIND_LIBRARY (${_GENERICLOGGER_LIBRARY_BASE}_RELEASE
	NAMES ${_GENERICLOGGER_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	)
      FIND_LIBRARY (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE
	NAMES ${_GENERICLOGGER_STATIC_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATH_SUFFIXES ${_GENERICLOGGER_POSSIBLE_LIB_SUFFIXES}
	)
    ENDIF (GENERICLOGGER_ROOT_DIR)

    SET (GENERICLOGGER_${_GENERICLOGGER_COMPONENT_UPPER}_FOUND TRUE)

    # Debug and release
    IF (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_LIBRARY_BASE}_RELEASE)
      SET (${_GENERICLOGGER_LIBRARY_BASE}
        debug ${${_GENERICLOGGER_LIBRARY_BASE}_DEBUG}
        optimized ${${_GENERICLOGGER_LIBRARY_BASE}_RELEASE})
      # Debug only
    ELSEIF (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG)
      SET (${_GENERICLOGGER_LIBRARY_BASE} ${${_GENERICLOGGER_LIBRARY_BASE}_DEBUG})
      # Release only
    ELSEIF (${_GENERICLOGGER_LIBRARY_BASE}_RELEASE)
      SET (${_GENERICLOGGER_LIBRARY_BASE} ${${_GENERICLOGGER_LIBRARY_BASE}_RELEASE})
    ELSE (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_LIBRARY_BASE}_RELEASE)
      # Component missing: record it for a later report
      LIST (APPEND _GENERICLOGGER_MISSING_COMPONENTS ${_GENERICLOGGER_COMPONENT})
      SET (GENERICLOGGER_${__COMPONENT_UPPER}_FOUND FALSE)
    ENDIF (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_LIBRARY_BASE}_RELEASE)

    IF (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE)
      SET (${_GENERICLOGGER_STATIC_LIBRARY_BASE}
        debug ${${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG}
        optimized ${${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE})
      # Debug only
    ELSEIF (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG)
      SET (${_GENERICLOGGER_STATIC_LIBRARY_BASE} ${${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG})
      # Release only
    ELSEIF (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE)
      SET (${_GENERICLOGGER_STATIC_LIBRARY_BASE} ${${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE})
    ELSE (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE)
      # Component missing: record it for a later report
      LIST (APPEND _GENERICLOGGER_STATIC_MISSING_COMPONENTS ${_GENERICLOGGER_COMPONENT})
      SET (GENERICLOGGER_STATIC_${__COMPONENT_UPPER}_FOUND FALSE)
    ENDIF (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG AND ${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE)

    MARK_AS_ADVANCED (${_GENERICLOGGER_LIBRARY_BASE}_DEBUG ${_GENERICLOGGER_LIBRARY_BASE}_RELEASE)
    MARK_AS_ADVANCED (${_GENERICLOGGER_STATIC_LIBRARY_BASE}_DEBUG ${_GENERICLOGGER_STATIC_LIBRARY_BASE}_RELEASE)

    # Make sure only libraries that have been actually found are registered
    IF (${_GENERICLOGGER_LIBRARY_BASE})
      LIST (APPEND _GENERICLOGGER_LIBRARIES ${${_GENERICLOGGER_LIBRARY_BASE}})
    ENDIF (${_GENERICLOGGER_LIBRARY_BASE})

    IF (${_GENERICLOGGER_STATIC_LIBRARY_BASE})
      LIST (APPEND _GENERICLOGGER_STATIC_LIBRARIES ${${_GENERICLOGGER_STATIC_LIBRARY_BASE}})
    ENDIF (${_GENERICLOGGER_STATIC_LIBRARY_BASE})

  ENDFOREACH (_GENERICLOGGER_COMPONENT)

  IF (NOT DEFINED _GENERICLOGGER_MISSING_COMPONENTS AND NOT DEFINED _GENERICLOGGER_STATIC_MISSING_COMPONENTS)
    # Success: all components were found
    SET (GENERICLOGGER_INCLUDE_DIRS "${GENERICLOGGER_INCLUDE_DIR}")
    SET (GENERICLOGGER_LIBRARIES ${_GENERICLOGGER_LIBRARIES})
    SET (GENERICLOGGER_STATIC_LIBRARIES ${_GENERICLOGGER_STATIC_LIBRARIES})
  ELSE ()
    MESSAGE (STATUS "Several GENERICLOGGER components were not found:")

    # Display missing components indented, each on a separate line
    FOREACH (_GENERICLOGGER_MISSING_COMPONENT ${_GENERICLOGGER_MISSING_COMPONENTS})
      MESSAGE (STATUS "  " ${_GENERICLOGGER_MISSING_COMPONENT})
    ENDFOREACH ()
    FOREACH (_GENERICLOGGER_STATIC_MISSING_COMPONENT ${_GENERICLOGGER_STATIC_MISSING_COMPONENTS})
      MESSAGE (STATUS "  " ${_GENERICLOGGER_STATIC_MISSING_COMPONENT})
    ENDFOREACH ()
  ENDIF ()

  #
  # Simulate hopefully correct flags based on heuristic discovery
  #
  IF (NOT _GENERICLOGGER_C_FLAGS_SHARED)
    SET (_GENERICLOGGER_C_FLAGS_SHARED "${CMAKE_SHARED_LIBRARY_C_FLAGS}")
  ENDIF ()

  IF (NOT _GENERICLOGGER_CXX_FLAGS_SHARED)
    SET (_GENERICLOGGER_CXX_FLAGS_SHARED "${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
  ENDIF ()

  SET (_GENERICLOGGER_LINK_FLAGS)
  SET (_GENERICLOGGER_STATIC_LINK_FLAGS)

ENDIF (GENERICLOGGER_INCLUDE_DIR)

SET (_GENERICLOGGER_LDFLAGS " ")
FOREACH (_genericlogger_link_flag ${_GENERICLOGGER_LINK_FLAGS})
  SET (_GENERICLOGGER_LDFLAGS "${_GENERICLOGGER_LDFLAGS} ${_genericlogger_link_flag}")
ENDFOREACH ()

SET (_GENERICLOGGER_STATIC_LDFLAGS " ")
FOREACH (_genericlogger_static_link_flag ${_GENERICLOGGER_STATIC_LINK_FLAGS})
  SET (_GENERICLOGGER_STATIC_LDFLAGS "${_GENERICLOGGER_STATIC_LDFLAGS} ${_genericlogger_static_link_flag}")
ENDFOREACH ()

MARK_AS_ADVANCED (GENERICLOGGER_ROOT_DIR GENERICLOGGER_INCLUDE_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS (GENERICLOGGER
  REQUIRED_VARS
  GENERICLOGGER_INCLUDE_DIR
  GENERICLOGGER_LIBRARIES)

IF (_GENERICLOGGER_LINK_FLAGS AND _GENERICLOGGER_STATIC_LINK_FLAGS)
  SET (GENERICLOGGER_FOUND 1 CACHE STRING "GENERICLOGGER is found")
ENDIF ()

SET (GENERICLOGGER_C_FLAGS_SHARED    "${_GENERICLOGGER_C_FLAGS_SHARED}"    CACHE STRING "C flags when compiling against shared GENERICLOGGER")
SET (GENERICLOGGER_CXX_FLAGS_SHARED  "${_GENERICLOGGER_CXX_FLAGS_SHARED}"  CACHE STRING "C flags when compiling against shared GENERICLOGGER")
SET (GENERICLOGGER_LINK_FLAGS        "${_GENERICLOGGER_LINK_FLAGS}"        CACHE STRING "Linker flags when linking against GENERICLOGGER")
SET (GENERICLOGGER_LDFLAGS           "${_GENERICLOGGER_LDFLAGS}"           CACHE STRING "Stringified version of GENERICLOGGER_LINK_FLAGS")
SET (GENERICLOGGER_STATIC_LINK_FLAGS "${_GENERICLOGGER_STATIC_LINK_FLAGS}" CACHE STRING "Linker flags when linking against static GENERICLOGGER")
SET (GENERICLOGGER_STATIC_LDFLAGS    "${_GENERICLOGGER_STATIC_LDFLAGS}"    CACHE STRING "Stringified version of GENERICLOGGER_STATIC_LINK_FLAGS")

IF(GENERICLOGGER_FOUND)
  MESSAGE(STATUS "-----------------------------------------")
  MESSAGE(STATUS "Setup GENERICLOGGER:")
  MESSAGE(STATUS "")
  MESSAGE(STATUS "           ROOT_DIR: ${GENERICLOGGER_ROOT_DIR}")
  MESSAGE(STATUS "     C_FLAGS_SHARED: ${GENERICLOGGER_C_FLAGS_SHARED}")
  MESSAGE(STATUS "   CXX_FLAGS_SHARED: ${GENERICLOGGER_CXX_FLAGS_SHARED}")
  MESSAGE(STATUS "        INCLUDE_DIR: ${GENERICLOGGER_INCLUDE_DIR}")
  MESSAGE(STATUS "          LIBRARIES: ${GENERICLOGGER_LIBRARIES}")
  MESSAGE(STATUS "   STATIC LIBRARIES: ${GENERICLOGGER_STATIC_LIBRARIES}")
  MESSAGE(STATUS "         LINK_FLAGS: ${GENERICLOGGER_LINK_FLAGS}")
  MESSAGE(STATUS "  STATIC LINK_FLAGS: ${GENERICLOGGER_STATIC_LINK_FLAGS}")
  MESSAGE(STATUS "            LDFLAGS: ${GENERICLOGGER_LDFLAGS}")
  MESSAGE(STATUS "     STATIC LDFLAGS: ${GENERICLOGGER_STATIC_LDFLAGS}")
  MESSAGE(STATUS "-----------------------------------------")
ENDIF()

MARK_AS_ADVANCED (
  GENERICLOGGER_FOUND
  GENERICLOGGER_C_FLAGS_SHARED
  GENERICLOGGER_CXX_FLAGS_SHARED
  GENERICLOGGER_LINK_FLAGS
  GENERICLOGGER_LDFLAGS
  GENERICLOGGER_STATIC_LINK_FLAGS
  GENERICLOGGER_STATIC_LDFLAGS
  )
