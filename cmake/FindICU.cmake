# Module for locating ICU.
#
# Cutomizable variables:
#   ICU_ROOT_DIR
#     This variable points to the ICU root directory. On Windows the
#     library location typically will have to be provided explicitly using the
#     -D command-line option. Alternatively, an environment variable can be set.
#
# Read-Only variables:
#   ICU_FOUND
#     Indicates whether the library has been found.
#
#   ICU_INCLUDE_DIRS
#     Points to the ICU include directory.
#
#   ICU_LIBRARIES
#     Points to the ICU libraries that should be passed to
#     target_link_libararies.
#
#   ICU_C_FLAGS, ICU_C_FLAGS_SHARED
#     Required C compile flags when building against ICU.
#
#   ICU_CXX_FLAGS, ICU_CXX_FLAGS_SHARED
#     Required C++ compile flags when building against ICU.
#
#   ICU_CPP_FLAGS
#     Preprocessor flags when building against ICU.
#
#   ICU_LINK_FLAGS
#     Linker flags when building linking against ICU. Defaults to "common" (libicuuc) and "i18n" (libicui18n) libraries, as well as the data library. Take care, this is a list.
#
#   ICU_LDFLAGS
#     Stringified version of ICU_LINK_FLAGS
#
#   ICU_LINK_LAYOUT_FLAGS
#     Linker flags for the ICU layout library.
#
#   ICU_LINK_IO_FLAGS
#     Linker flags for the ICU I/O library.
#
#   ICU_<COMPONENT>_FOUND
#     Indicates whether the specified <COMPONENT> has been found.
#
#   ICU_<PROGRAM>_EXECUTABLE
#     The path to the specified ICU <PROGRAM>.
#
# Building resource bundles:
#
#   CREATE_ICU_RESOURCE_BUNDLE (TARGET target NAME name
#     FILES file1 [file2 ...] [COMMON | STATIC | SHARED] [VERBOSE]
#     [C_FLAGS VAR] [CXX_FLAGS VAR] [CPP_FLAGS VAR] [LINK_FLAGS VAR] [LINK_LAYOUT_FLAGS VAR] [LINK_IO_FLAGS VAR] [GENRB_ARGS arg1 [arg2 ...]
#     [PKGDATA_ARGS arg1 [arg2 ...]] [RESOURCE_LOCATION VAR] [REVISION rev])
#
#   This function wraps the functionality provided by ICU's genrb and pkgdata
#   tools. These tools allow to create a resource bundle from a set of text
#   files. The module adds the executables as a custom command to the specified
#   `target'. The name of the resource bundle is determined by the `name'
#   argument, the input files by `file1', `file2' etc. Resource bundles can be
#   created either as an endian-specific but portable common data file or as a
#   static/shared library. Unless explicitly specified, a common data resource
#   bundle will be created. When creating a static or a shared resource bundle,
#   an imported library target will be added, which can be referenced by
#   bundle's `name'. In some situations it may be necessary to specify
#   additional genrb or pkgdata command-line arguments. This can be achieved by
#   setting the optional GENRB_ARGS or PKGDATA_ARGS argument respectively. The
#   standard output of both tools is surpressed by default. To prevent this
#   behavior, the VERBOSE option can be used. The location of the resource can
#   be determined using the RESOURCE_LOCATION option together with a variable
#   name VAR that will store that path. When creating a shared library bundle on
#   Unix-like system, library's revision has to be set. This can be done by
#   supplying the appropriate version to the REVISION option. If the option is
#   omitted, the revision will be set to 1.0.
#
#
# Copyright (c) 2011 Sergiu Dotenco
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# 2014 Jean-Damien Durand
# Adaptation for my setup

INCLUDE (CMakeParseArguments)
INCLUDE (FindPackageHandleStandardArgs)

SET (_PF86 "ProgramFiles(x86)")
SET (_ICU_POSSIBLE_DIRS
  ${ICU_ROOT_DIR}
  "$ENV{ICU_ROOT_DIR}"
  "C:/icu"
  "$ENV{PROGRAMFILES}/icu"
  "$ENV{${_PF86}}/icu")

SET (_ICU_POSSIBLE_INCLUDE_SUFFIXES include)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_ICU_POSSIBLE_LIB_SUFFIXES lib64)
  SET (_ICU_POSSIBLE_BIN_SUFFIXES bin64)

  IF (NOT WIN32)
    LIST (APPEND _ICU_POSSIBLE_LIB_SUFFIXES lib)
    LIST (APPEND _ICU_POSSIBLE_BIN_SUFFIXES bin)
  ENDIF (NOT WIN32)
ELSE (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET (_ICU_POSSIBLE_LIB_SUFFIXES lib)
  SET (_ICU_POSSIBLE_BIN_SUFFIXES bin)
ENDIF (CMAKE_SIZEOF_VOID_P EQUAL 8)

FIND_PATH (ICU_ROOT_DIR
  NAMES include/unicode/utypes.h
  PATHS ${_ICU_POSSIBLE_DIRS}
  DOC "ICU root directory")

IF (ICU_ROOT_DIR)
  # Re-use the previous path:
  FIND_PATH (ICU_INCLUDE_DIR
    NAMES unicode/utypes.h
    PATHS ${ICU_ROOT_DIR}
    PATH_SUFFIXES ${_ICU_POSSIBLE_INCLUDE_SUFFIXES}
    DOC "ICU include directory"
    NO_DEFAULT_PATH)
ELSE (ICU_ROOT_DIR)
  # Use default path search
  FIND_PATH (ICU_INCLUDE_DIR
    NAMES unicode/utypes.h
    DOC "ICU include directory"
    )
ENDIF (ICU_ROOT_DIR)

IF (NOT DEFINED ICU_BINARY_DIR)
  # Binary directory hasn't been located yet: extract directory from icuinfo's
  # path after invoking find_program
  SET (_ICU_FIND_BINARY_DIR TRUE)
ELSE (NOT DEFINED ICU_BINARY_DIR)
  SET (_ICU_FIND_BINARY_DIR FALSE)
ENDIF (NOT DEFINED ICU_BINARY_DIR)

IF (ICU_ROOT_DIR)
  FIND_PROGRAM (ICU_BINARY_DIR
    NAMES icuinfo
    PATHS ${ICU_ROOT_DIR}
    PATH_SUFFIXES ${_ICU_POSSIBLE_BIN_SUFFIXES}
    NO_DEFAULT_PATH)
ELSE (ICU_ROOT_DIR)
  FIND_PROGRAM (ICU_BINARY_DIR
    NAMES icuinfo
    PATH_SUFFIXES ${_ICU_POSSIBLE_BIN_SUFFIXES}
    )
ENDIF (ICU_ROOT_DIR)

IF (_ICU_FIND_BINARY_DIR)
  GET_FILENAME_COMPONENT (ICU_BINARY_DIR ${ICU_BINARY_DIR} PATH)
  SET (ICU_BINARY_DIR ${ICU_BINARY_DIR} CACHE PATH "ICU binary directory" FORCE)
ENDIF (_ICU_FIND_BINARY_DIR)

SET (_ICU_EXECUTABLES derb genbrk genccode gencfu gencmn gencnval genctd
  gennorm2 genrb gensprep icuinfo icupkg makeconv pkgdata uconv)

FOREACH (_ICU_EXECUTABLE ${_ICU_EXECUTABLES})
  STRING (TOUPPER ${_ICU_EXECUTABLE} _ICU_EXECUTABLE_UPPER)

  FIND_PROGRAM (ICU_${_ICU_EXECUTABLE_UPPER}_EXECUTABLE
    NAMES ${_ICU_EXECUTABLE}
    PATHS ${ICU_BINARY_DIR}
    DOC "ICU ${_ICU_EXECUTABLE} path")

  MARK_AS_ADVANCED (ICU_${_ICU_EXECUTABLE_UPPER}_EXECUTABLE)
ENDFOREACH (_ICU_EXECUTABLE ${_ICU_EXECUTABLES})

# Use at least the data library
IF (NOT ICU_FIND_COMPONENTS)
  SET (ICU_FIND_COMPONENTS icudt icuin icuio icule iculx icutu icuuc)
ENDIF (NOT ICU_FIND_COMPONENTS)

IF (NOT DEFINED ICU_CONFIG_EXECUTABLE)
  FIND_PROGRAM (ICU_CONFIG_EXECUTABLE icu-config PATH ${ICU_ROOT_DIR})

  IF (ICU_CONFIG_EXECUTABLE)

    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --cflags OUTPUT_VARIABLE
      _ICU_C_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --cxxflags OUTPUT_VARIABLE
      _ICU_CXX_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --cppflags OUTPUT_VARIABLE
      _ICU_CPP_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --ldflags OUTPUT_VARIABLE
      _ICU_LINK_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --ldflags-layout OUTPUT_VARIABLE
      _ICU_LINK_LAYOUT_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --ldflags-icuio OUTPUT_VARIABLE
      _ICU_LINK_IO_FLAGS)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --cflags-dynamic
      OUTPUT_VARIABLE _ICU_C_FLAGS_SHARED)
    EXEC_PROGRAM (${ICU_CONFIG_EXECUTABLE} ARGS --cxxflags-dynamic
      OUTPUT_VARIABLE _ICU_CXX_FLAGS_SHARED)

    IF (APPLE)
      # Both are set to -dynamic which results in compilation errors: set to
      # empty
      SET (_ICU_C_FLAGS_SHARED "")
      SET (_ICU_CXX_FLAGS_SHARED "")
    ENDIF (APPLE)
  ENDIF (ICU_CONFIG_EXECUTABLE)
ENDIF (NOT DEFINED ICU_CONFIG_EXECUTABLE)

IF (ICU_INCLUDE_DIR)
  # ICU 4 version numbers header
  SET (_ICU_VERSION_HEADER ${ICU_INCLUDE_DIR}/unicode/uvernum.h)

  IF (NOT EXISTS ${_ICU_VERSION_HEADER})
    # ICU 3 stores the version numbers in uversion.h
    SET (_ICU_VERSION_HEADER ${ICU_INCLUDE_DIR}/unicode/uversion.h)
  ENDIF (NOT EXISTS ${_ICU_VERSION_HEADER})

  IF (EXISTS ${_ICU_VERSION_HEADER})
    FILE (STRINGS ${_ICU_VERSION_HEADER}
      _ICU_VERSION_TMP REGEX "^#define U_ICU_VERSION_MAJOR_NUM[ \t]+[0-9]+$")

    STRING (REGEX REPLACE
      "^#define U_ICU_VERSION_MAJOR_NUM[ \t]+([0-9]+)" "\\1" ICU_VERSION_MAJOR
      ${_ICU_VERSION_TMP})

    FILE (STRINGS ${_ICU_VERSION_HEADER}
      _ICU_VERSION_TMP REGEX "^#define U_ICU_VERSION_MINOR_NUM[ \t]+[0-9]+$")

    STRING (REGEX REPLACE
      "^#define U_ICU_VERSION_MINOR_NUM[ \t]+([0-9]+)" "\\1" ICU_VERSION_MINOR
      ${_ICU_VERSION_TMP})

    FILE (STRINGS ${_ICU_VERSION_HEADER} _ICU_VERSION_TMP REGEX
      "^#define U_ICU_VERSION_PATCHLEVEL_NUM[ \t]+[0-9]+$")

    STRING (REGEX REPLACE
      "^#define U_ICU_VERSION_PATCHLEVEL_NUM[ \t]+([0-9]+)" "\\1"
      ICU_VERSION_PATCH ${_ICU_VERSION_TMP})

    SET (ICU_VERSION_COUNT 3)
    SET (_ICU_VERSION
      ${ICU_VERSION_MAJOR}.${ICU_VERSION_MINOR}.${ICU_VERSION_PATCH})
  ENDIF (EXISTS ${_ICU_VERSION_HEADER})

  SET (_ICU_VERSION_PATTERN "[0-9]+\\.[0-9]+\\.[0-9]+")

  # Version sanity check
  IF ("${_ICU_VERSION}" MATCHES "${_ICU_VERSION_PATTERN}")
    SET (ICU_VERSION ${_ICU_VERSION})
  ELSE ("${_ICU_VERSION}" MATCHES "${_ICU_VERSION_PATTERN}")
    MESSAGE (WARNING "Cannot determine ICU's version")
  ENDIF ("${_ICU_VERSION}" MATCHES "${_ICU_VERSION_PATTERN}")

  SET (_ICU_DETECTED_SUFFIX ${ICU_VERSION_MAJOR}${ICU_VERSION_MINOR})

  # Loop over each components
  FOREACH (_ICU_COMPONENT ${ICU_FIND_COMPONENTS})
    SET (_ICU_COMPONENT_BASE ${_ICU_COMPONENT})

    SET (_ICU_COMPONENT_POSSIBLE_DEBUG_NAMES
         ${_ICU_COMPONENT_BASE}d
         ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d
         ${_ICU_COMPONENT_BASE}d
         ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d)
    SET (_ICU_COMPONENT_POSSIBLE_RELEASE_NAMES
         ${_ICU_COMPONENT_BASE}
         ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}
         ${_ICU_COMPONENT_BASE}
         ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX})

    # On Unix-like systems the `icudt' component is called `icudata' and `icuin'
    # `icui18n': map the names accordingly and search for both variants
    IF (${_ICU_COMPONENT} STREQUAL "icudt")
        SET (_ICU_COMPONENT_BASE icudata)
        LIST (APPEND _ICU_COMPONENT_POSSIBLE_DEBUG_NAMES
             ${_ICU_COMPONENT_BASE}d
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d
             ${_ICU_COMPONENT_BASE}d
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d)
        LIST (APPEND _ICU_COMPONENT_POSSIBLE_RELEASE_NAMES
             ${_ICU_COMPONENT_BASE}
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}
             ${_ICU_COMPONENT_BASE}
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX})
    ELSEIF (${_ICU_COMPONENT} STREQUAL "icuin")
        SET (_ICU_COMPONENT_BASE icui18n)
        LIST (APPEND _ICU_COMPONENT_POSSIBLE_DEBUG_NAMES
             ${_ICU_COMPONENT_BASE}d
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d
             ${_ICU_COMPONENT_BASE}d
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}d)
        LIST (APPEND _ICU_COMPONENT_POSSIBLE_RELEASE_NAMES
             ${_ICU_COMPONENT_BASE}
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX}
             ${_ICU_COMPONENT_BASE}
             ${_ICU_COMPONENT_BASE}${_ICU_DETECTED_SUFFIX})
    ENDIF (${_ICU_COMPONENT} STREQUAL "icudt")

    STRING (TOUPPER ${_ICU_COMPONENT} _ICU_COMPONENT_UPPER)
    SET (_ICU_LIBRARY_BASE ICU_${_ICU_COMPONENT_UPPER}_LIBRARY)

    IF (ICU_ROOT_DIR)
      FIND_LIBRARY (${_ICU_LIBRARY_BASE}_DEBUG
	NAMES ${_ICU_COMPONENT_POSSIBLE_DEBUG_NAMES}
	PATHS ${ICU_ROOT_DIR} PATH_SUFFIXES ${_ICU_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
    ELSE (ICU_ROOT_DIR)
      FIND_LIBRARY (${_ICU_LIBRARY_BASE}_DEBUG
	NAMES ${_ICU_COMPONENT_POSSIBLE_DEBUG_NAMES}
	PATH_SUFFIXES ${_ICU_POSSIBLE_LIB_SUFFIXES}
	)
    ENDIF (ICU_ROOT_DIR)

    IF (ICU_ROOT_DIR)
      FIND_LIBRARY (${_ICU_LIBRARY_BASE}_RELEASE
	NAMES ${_ICU_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATHS ${ICU_ROOT_DIR} PATH_SUFFIXES ${_ICU_POSSIBLE_LIB_SUFFIXES}
	NO_DEFAULT_PATH)
    ELSE (ICU_ROOT_DIR)
      FIND_LIBRARY (${_ICU_LIBRARY_BASE}_RELEASE
	NAMES ${_ICU_COMPONENT_POSSIBLE_RELEASE_NAMES}
	PATH_SUFFIXES ${_ICU_POSSIBLE_LIB_SUFFIXES}
	)
    ENDIF (ICU_ROOT_DIR)

    SET (ICU_${_ICU_COMPONENT_UPPER}_FOUND TRUE)

    # Debug and release
    IF (${_ICU_LIBRARY_BASE}_DEBUG AND ${_ICU_LIBRARY_BASE}_RELEASE)
      SET (${_ICU_LIBRARY_BASE}
        debug ${${_ICU_LIBRARY_BASE}_DEBUG}
        optimized ${${_ICU_LIBRARY_BASE}_RELEASE})
      # Debug only
    ELSEIF (${_ICU_LIBRARY_BASE}_DEBUG)
      SET (${_ICU_LIBRARY_BASE} ${${_ICU_LIBRARY_BASE}_DEBUG})
      # Release only
    ELSEIF (${_ICU_LIBRARY_BASE}_RELEASE)
      SET (${_ICU_LIBRARY_BASE} ${${_ICU_LIBRARY_BASE}_RELEASE})
    ELSE (${_ICU_LIBRARY_BASE}_DEBUG AND ${_ICU_LIBRARY_BASE}_RELEASE)
      # Component missing: record it for a later report
      # Starting from ICU 58.2 layout has been removed
      SET (_ICU_MISSING_COMPONENT_BUT_OK FALSE)
      IF ((${ICU_VERSION_MAJOR} GREATER 58) OR (${ICU_VERSION_MAJOR} EQUAL 58))
        IF ((${ICU_VERSION_MINOR} GREATER 2) OR (${ICU_VERSION_MINOR} EQUAL 2))
          IF ("${_ICU_COMPONENT}" STREQUAL "icule")
            SET (_ICU_MISSING_COMPONENT_BUT_OK TRUE)
            SET (_ICU_MISSING_COMPONENT_BUT_OK_REASON "Layout was removed starting from ICU 58.2")
          ENDIF ()
          IF ("${_ICU_COMPONENT}" STREQUAL "iculx")
            SET (_ICU_MISSING_COMPONENT_BUT_OK TRUE)
            SET (_ICU_MISSING_COMPONENT_BUT_OK_REASON "Layout was removed starting from ICU 58.2")
          ENDIF ()
        ENDIF ()
      ENDIF ()
      IF (${_ICU_MISSING_COMPONENT_BUT_OK})
        MESSAGE (STATUS "ICU component ${_ICU_COMPONENT} is missing but this is ok: ${_ICU_MISSING_COMPONENT_BUT_OK_REASON}")
      ELSE ()
        LIST (APPEND _ICU_MISSING_COMPONENTS ${_ICU_COMPONENT})
      ENDIF ()
      SET (ICU_${_ICU_COMPONENT_UPPER}_FOUND FALSE)
    ENDIF (${_ICU_LIBRARY_BASE}_DEBUG AND ${_ICU_LIBRARY_BASE}_RELEASE)

    MARK_AS_ADVANCED (${_ICU_LIBRARY_BASE}_DEBUG ${_ICU_LIBRARY_BASE}_RELEASE)

    # Make sure only libraries that have been actually found are registered
    IF (${_ICU_LIBRARY_BASE})
      LIST (APPEND _ICU_LIBRARIES ${${_ICU_LIBRARY_BASE}})
    ENDIF (${_ICU_LIBRARY_BASE})
  ENDFOREACH (_ICU_COMPONENT)

  IF (NOT DEFINED _ICU_MISSING_COMPONENTS)
    # Success: all components were found
    SET (ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIR})
    SET (ICU_LIBRARIES ${_ICU_LIBRARIES})
  ELSE (NOT DEFINED _ICU_MISSING_COMPONENTS)
    MESSAGE (STATUS "Several ICU components were not found:")

    # Display missing components indented, each on a separate line
    FOREACH (_ICU_MISSING_COMPONENT ${_ICU_MISSING_COMPONENTS})
      MESSAGE (STATUS "  " ${_ICU_MISSING_COMPONENT})
    ENDFOREACH (_ICU_MISSING_COMPONENT ${_ICU_MISSING_COMPONENTS})
  ENDIF (NOT DEFINED _ICU_MISSING_COMPONENTS)

  #
  # Simulate hopefully correct flags based on heuristic discovery
  #
  IF (NOT _ICU_C_FLAGS)
    SET (_ICU_C_FLAGS "${CMAKE_INCLUDE_FLAG_C}${CMAKE_INCLUDE_FLAG_SEP_C}${ICU_INCLUDE_DIR}")
  ENDIF ()

  IF (NOT _ICU_C_FLAGS_SHARED)
    SET (_ICU_C_FLAGS_SHARED "${CMAKE_SHARED_LIBRARY_C_FLAGS}")
  ENDIF ()

  IF (NOT _ICU_CXX_FLAGS)
    SET (_ICU_CXX_FLAGS "${CMAKE_INCLUDE_FLAG_CXX}${CMAKE_INCLUDE_FLAG_SEP_CXX}${ICU_INCLUDE_DIR}")
  ENDIF ()

  IF (NOT _ICU_CXX_FLAGS_SHARED)
    SET (_ICU_CXX_FLAGS_SHARED "${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
  ENDIF ()

  IF (NOT _ICU_CPP_FLAGS)
    TRY_COMPILE (HAVE_ATOMIC ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CheckAtomic.cpp
      OUTPUT_VARIABLE OUTPUT)
    IF (HAVE_ATOMIC)
      IF (WIN32)
	SET (_ICU_CPP_FLAGS_U_HAVE_ATOMIC "/DU_HAVE_ATOMIC=1")
      ELSE (WIN32)
	SET (_ICU_CPP_FLAGS_U_HAVE_ATOMIC "-DU_HAVE_ATOMIC=1")
      ENDIF (WIN32)
    ENDIF (HAVE_ATOMIC)
    TRY_COMPILE (HAVE_ELF ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CheckElf.c
      OUTPUT_VARIABLE OUTPUT)
    IF (HAVE_ELF)
      IF (WIN32)
	SET (_ICU_CPP_FLAGS_U_HAVE_ATOMIC "/DU_HAVE_ELF=1")
      ELSE (WIN32)
	SET (_ICU_CPP_FLAGS_U_HAVE_ATOMIC "-DU_HAVE_ELF=1")
      ENDIF (WIN32)
    ENDIF (HAVE_ELF)
    IF (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
      SET (_ICU_CPP_FLAGS__FORTIFY_SOURCE "-D_FORTIFY_SOURCE=2")
      SET (_ICU_CPP_FLAGS__REENTRANT "-D_REENTRANT=1")
    ENDIF (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    SET (_ICU_CPP_FLAGS "${_ICU_CPP_FLAGS__FORTIFY_SOURCE} ${_ICU_CPP_FLAGS__REENTRANT} ${_ICU_CPP_FLAGS_U_HAVE_ELF} ${_ICU_CPP_FLAGS_U_HAVE_ATOMIC}")
  ENDIF ()

  IF (NOT _ICU_LINK_FLAGS)
    SET (_ICU_LINK_FLAGS ${ICU_LIBRARIES})
  ENDIF ()

ENDIF (ICU_INCLUDE_DIR)

SET (_ICU_LDFLAGS " ")
FOREACH (_icu_link_flag ${_ICU_LINK_FLAGS})
  SET (_ICU_LDFLAGS "${_ICU_LDFLAGS} ${_icu_link_flag}")
ENDFOREACH ()

MARK_AS_ADVANCED (ICU_ROOT_DIR ICU_INCLUDE_DIR ICU_BINARY_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS (ICU REQUIRED_VARS ICU_INCLUDE_DIR
  ICU_LIBRARIES VERSION_VAR ICU_VERSION)

IF (_ICU_LINK_FLAGS)
  SET (ICU_FOUND 1 CACHE STRING "ICU is found")
ENDIF ()

SET (ICU_C_FLAGS "${_ICU_C_FLAGS}" CACHE STRING
  "Required C compile flags when building against ICU")
SET (ICU_CXX_FLAGS "${_ICU_CXX_FLAGS}" CACHE STRING
  "Required C++ compile flags when building against ICU")
SET (ICU_CPP_FLAGS "${_ICU_CPP_FLAGS}" CACHE STRING
  "Preprocessor flags when building against ICU")
SET (ICU_LINK_FLAGS "${_ICU_LINK_FLAGS}" CACHE STRING
  "Linker flags when building linking against ICU. Defaults to \"common\" (libicuuc) and \"i18n\" (libicui18n) libraries, as well as the data library.")
SET (ICU_LDFLAGS "${_ICU_LDFLAGS}" CACHE STRING
  "Stringified version of ICU_LINK_FLAGS")
SET (ICU_LINK_LAYOUT_FLAGS "${_ICU_LINK_LAYOUT_FLAGS}" CACHE STRING
  "Linker flags for the ICU layout library")
SET (ICU_LINK_IO_FLAGS "${_ICU_LINK_IO_FLAGS}" CACHE STRING
  "Linker flags for the ICU I/O library")
SET (ICU_C_FLAGS_SHARED "${_ICU_C_FLAGS_SHARED}" CACHE STRING
  "Required dynamic C compile flags when building against ICU")
SET (ICU_CXX_FLAGS_SHARED "${_ICU_CXX_FLAGS_SHARED}" CACHE STRING
  "Required dynamic C++ compile flags when building against ICU")

IF(ICU_FOUND)
  MESSAGE(STATUS "-----------------------------------------")
  MESSAGE(STATUS "Setup ICU:")
  MESSAGE(STATUS "")
  MESSAGE(STATUS "           ROOT_DIR: ${ICU_ROOT_DIR}")
  MESSAGE(STATUS "        INCLUDE_DIR: ${ICU_INCLUDE_DIR}")
  MESSAGE(STATUS "  CONFIG_EXECUTABLE: ${ICU_CONFIG_EXECUTABLE}")
  MESSAGE(STATUS "          CPP_FLAGS: ${ICU_CPP_FLAGS}")
  MESSAGE(STATUS "            C_FLAGS: ${ICU_C_FLAGS}")
  MESSAGE(STATUS "     C_FLAGS_SHARED: ${ICU_C_FLAGS_SHARED}")
  MESSAGE(STATUS "          CXX_FLAGS: ${ICU_CXX_FLAGS}")
  MESSAGE(STATUS "   CXX_FLAGS_SHARED: ${ICU_CXX_FLAGS_SHARED}")
  MESSAGE(STATUS "          LIBRARIES: ${ICU_LIBRARIES}")
  MESSAGE(STATUS "         BINARY_DIR: ${ICU_BINARY_DIR}")
  MESSAGE(STATUS "         LINK_FLAGS: ${ICU_LINK_FLAGS}")
  MESSAGE(STATUS "  LINK_LAYOUT_FLAGS: ${ICU_LINK_LAYOUT_FLAGS}")
  MESSAGE(STATUS "      LINK_IO_FLAGS: ${ICU_LINK_IO_FLAGS}")
  MESSAGE(STATUS "            LDFLAGS: ${ICU_LDFLAGS}")
  MESSAGE(STATUS "-----------------------------------------")
ENDIF()

MARK_AS_ADVANCED (
  ICU_FOUND
  ICU_CONFIG_EXECUTABLE
  ICU_C_FLAGS   ICU_C_FLAGS_SHARED
  ICU_CXX_FLAGS ICU_CXX_FLAGS_SHARED
  ICU_CPP_FLAGS
  ICU_LINK_FLAGS
  ICU_LDFLAGS
  ICU_LINK_LAYOUT_FLAGS
  ICU_LINK_IO_FLAGS)

# Functions

FUNCTION (CREATE_ICU_RESOURCE_BUNDLE ARGS)
  IF (NOT ICU_GENRB_EXECUTABLE)
    MESSAGE (FATAL_ERROR
      "CREATE_ICU_RESOURCE_BUNDLE requires genrb which is missing")
  ENDIF (NOT ICU_GENRB_EXECUTABLE)

  IF (NOT ICU_PKGDATA_EXECUTABLE)
    MESSAGE (FATAL_ERROR
      "CREATE_ICU_RESOURCE_BUNDLE requires pkgdata which is missing")
  ENDIF (NOT ICU_PKGDATA_EXECUTABLE)

  SET (_ICU_CRB_OPTIONS COMMON STATIC SHARED VERBOSE)
  SET (_ICU_CRB_SINGLE TARGET NAME RESOURCE_LOCATION REVISION C_FLAGS CXX_FLAGS CPP_FLAGS LINK_FLAGS LINK_LAYOUT_FLAGS LINK_IO_FLAGS)
  SET (_ICU_CRB_MULTI FILES GENRB_ARGS PKGDATA_ARGS)

  CMAKE_PARSE_ARGUMENTS (_ICU_CRB_ARGS "${_ICU_CRB_OPTIONS}"
    "${_ICU_CRB_SINGLE}" "${_ICU_CRB_MULTI}" ${ARGS} ${ARGN})

  # Error checking

  IF ("${_ICU_CRB_ARGS_TARGET}" STREQUAL "")
    MESSAGE (FATAL_ERROR "Missing resource bundle TARGET name")
  ENDIF ("${_ICU_CRB_ARGS_TARGET}" STREQUAL "")

  IF (NOT TARGET ${_ICU_CRB_ARGS_TARGET})
    MESSAGE (FATAL_ERROR "Resource bundle cannot be added to a non-exisiting "
      "target ${_ICU_CRB_ARGS_TARGET}")
  ENDIF (NOT TARGET ${_ICU_CRB_ARGS_TARGET})

  IF ("${_ICU_CRB_ARGS_NAME}" STREQUAL "")
    MESSAGE (FATAL_ERROR "Missing resource bundle NAME")
  ENDIF ("${_ICU_CRB_ARGS_NAME}" STREQUAL "")

  LIST (LENGTH _ICU_CRB_ARGS_FILES _ICU_CRB_FILE_COUNT)

  IF (${_ICU_CRB_FILE_COUNT} LESS 1)
    MESSAGE (FATAL_ERROR "At least one resource file has to be specified")
  ENDIF (${_ICU_CRB_FILE_COUNT} LESS 1)

  # Parameter construction

  SET (_ICU_CRB_BUNDLE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/icudata/${CMAKE_CFG_INTDIR})
  # Strip the . after the slash
  GET_FILENAME_COMPONENT (_ICU_CRB_BUNDLE_DIR ${_ICU_CRB_BUNDLE_DIR} ABSOLUTE)
  SET (_ICU_CRB_BUNDLE_NAME ${_ICU_CRB_ARGS_NAME})

  SET (_ICU_CRB_BUNDLE_TYPE common)
  SET (_ICU_CRB_BUNDLE_EXT "dat")

  IF (${_ICU_CRB_ARGS_STATIC})
    SET (_ICU_CRB_BUNDLE_TYPE static)
    SET (_ICU_CRB_LIB_TYPE STATIC)

    IF (WIN32)
      SET (_ICU_CRB_BUNDLE_EXT "lib")
    ELSE (WIN32)
      SET (_ICU_CRB_BUNDLE_EXT "a")
    ENDIF (WIN32)
  ELSEIF (${_ICU_CRB_ARGS_SHARED})
    SET (_ICU_CRB_BUNDLE_TYPE dll)
    SET (_ICU_CRB_LIB_TYPE SHARED)

    IF (WIN32)
      SET (_ICU_CRB_BUNDLE_EXT "lib")
    ELSEIF (APPLE)
      SET (_ICU_CRB_BUNDLE_EXT "dylib")
    ELSE (WIN32)
      SET (_ICU_CRB_BUNDLE_EXT "so")
    ENDIF (WIN32)

    IF (NOT "${_ICU_CRB_ARGS_C_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_C_FLAGS} ${ICU_C_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_C_FLAGS}" STREQUAL "")

    IF (NOT "${_ICU_CRB_ARGS_CXX_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_CXX_FLAGS} ${ICU_CXX_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_CXX_FLAGS}" STREQUAL "")

    IF (NOT "${_ICU_CRB_ARGS_CPP_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_CPP_FLAGS} ${ICU_CPP_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_CPP_FLAGS}" STREQUAL "")

    IF (NOT "${_ICU_CRB_ARGS_LINK_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_LINK_FLAGS} ${ICU_LINK_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_LINK_FLAGS}" STREQUAL "")

    IF (NOT "${_ICU_CRB_ARGS_LINK_LAYOUT_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_LINK_LAYOUT_FLAGS} ${ICU_LINK_LAYOUT_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_LINK_LAYOUT_FLAGS}" STREQUAL "")

    IF (NOT "${_ICU_CRB_ARGS_LINK_IO_FLAGS}" STREQUAL "")
      SET (${_ICU_CRB_ARGS_LINK_IO_FLAGS} ${ICU_LINK_IO_FLAGS_SHARED} PARENT_SCOPE)
    ENDIF (NOT "${_ICU_CRB_ARGS_LINK_IO_FLAGS}" STREQUAL "")
  ENDIF (${_ICU_CRB_ARGS_STATIC})

  IF (${_ICU_CRB_BUNDLE_TYPE} STREQUAL "common")
    SET (_ICU_CRB_CREATE_LIBRARY FALSE)
  ELSE (${_ICU_CRB_BUNDLE_TYPE} STREQUAL "common")
    SET (_ICU_CRB_CREATE_LIBRARY TRUE)
  ENDIF (${_ICU_CRB_BUNDLE_TYPE} STREQUAL "common")

  # The file containg the list of all *.res files
  SET (_ICU_RES_LIST "list.txt")
  # The absolute path to the previous file
  SET (_ICU_RES_PATH ${_ICU_CRB_BUNDLE_DIR}/${_ICU_RES_LIST})

  IF (WIN32)
    SET (_ICU_CRB_CREATE_LIST dir /b "*.res" > "${_ICU_RES_PATH}")
  ELSE (WIN32)
    SET (_ICU_CRB_CREATE_LIST find . -maxdepth 1 -name '*.res' |
      xargs -I {} basename {} > "${_ICU_RES_PATH}")
  ENDIF (WIN32)

  SET (_ICU_CRB_LIB_NAME ${_ICU_CRB_BUNDLE_NAME})

  IF (_ICU_CRB_CREATE_LIBRARY AND NOT WIN32)
    SET (_ICU_CRB_LIB_NAME "lib${_ICU_CRB_BUNDLE_NAME}")
  ENDIF (_ICU_CRB_CREATE_LIBRARY AND NOT WIN32)

  SET (_ICU_CRB_BUNDLE_LOCATION
    ${_ICU_CRB_BUNDLE_DIR}/${_ICU_CRB_LIB_NAME}.${_ICU_CRB_BUNDLE_EXT})

  IF (_ICU_CRB_CREATE_LIBRARY)
    IF (${CMAKE_GENERATOR} MATCHES ".*Makefiles.*")
      # HACK: Create a dummy library, otherwise make will complain about a
      # missing rule
      EXEC_PROGRAM (${CMAKE_COMMAND} . ARGS -E make_directory
        ${_ICU_CRB_BUNDLE_DIR} OUTPUT_VARIABLE _TMP)
      EXEC_PROGRAM (${CMAKE_COMMAND} . ARGS -E touch ${_ICU_CRB_BUNDLE_LOCATION}
        OUTPUT_VARIABLE _TMP)
    ENDIF (${CMAKE_GENERATOR} MATCHES ".*Makefiles.*")
  ENDIF (_ICU_CRB_CREATE_LIBRARY)

  IF (NOT ${_ICU_CRB_ARGS_RESOURCE_LOCATION} STREQUAL "")
    SET (${_ICU_CRB_ARGS_RESOURCE_LOCATION} ${_ICU_CRB_BUNDLE_LOCATION}
      PARENT_SCOPE)
  ENDIF (NOT ${_ICU_CRB_ARGS_RESOURCE_LOCATION} STREQUAL "")

  ADD_CUSTOM_COMMAND (TARGET ${_ICU_CRB_ARGS_TARGET} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_ICU_CRB_BUNDLE_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${_ICU_CRB_BUNDLE_LOCATION})

  SET (_ICU_CRB_BUNDLE_TMP_DIR ${_ICU_CRB_BUNDLE_DIR})
  SET (_ICU_CRB_BUNDLE_TMP_ARGS)

  IF (NOT _ICU_CRB_CREATE_LIBRARY)
    # Output .dat to a temporary directory, otherwise pkgdata will complain
    # about not being able to move the file because the output location and the
    # destination are the same
    SET (_ICU_CRB_BUNDLE_TMP_DIR "${_ICU_CRB_BUNDLE_TMP_DIR}/tmp")
    SET (_ICU_CRB_BUNDLE_TMP_ARGS -T ${_ICU_CRB_BUNDLE_TMP_DIR})
  ENDIF (NOT _ICU_CRB_CREATE_LIBRARY)

  SET (_ICU_CRB_BUNDLE_REVISION_ARG)

  IF (${_ICU_CRB_ARGS_SHARED} AND NOT WIN32)
    IF ("${_ICU_CRB_ARGS_REVISION}" STREQUAL "")
      SET (_ICU_CRB_ARGS_REVISION 1.0)
    ELSEIF (NOT "${_ICU_CRB_ARGS_REVISION}" MATCHES
        "[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?")
      MESSAGE (FATAL_ERROR "Invalid revision format. "
        "The revision has to match the major.minor.patchlevel pattern.")
    ENDIF ("${_ICU_CRB_ARGS_REVISION}" STREQUAL "")

    SET (_ICU_CRB_BUNDLE_REVISION_ARG -r ${_ICU_CRB_ARGS_REVISION})
  ENDIF (${_ICU_CRB_ARGS_SHARED} AND NOT WIN32)

  IF (NOT ${_ICU_CRB_ARGS_VERBOSE})
    IF (WIN32)
      SET (_ICU_CRB_STDOUT_REDIRECT > NUL)
    ELSE (WIN32)
      SET (_ICU_CRB_STDOUT_REDIRECT > /dev/null)
    ENDIF (WIN32)
  ELSE (NOT ${_ICU_CRB_ARGS_VERBOSE})
    SET (_ICU_CRB_STDOUT_REDIRECT)
  ENDIF (NOT ${_ICU_CRB_ARGS_VERBOSE})

  IF (WIN32)
    SET (_ICU_CRB_REMOVE_RES del /q *.res > NUL)
  ELSE (WIN32)
    SET (_ICU_CRB_REMOVE_RES rm -f *.res)
  ENDIF (WIN32)

  ADD_CUSTOM_COMMAND (TARGET ${_ICU_CRB_ARGS_TARGET} PRE_LINK
    COMMAND ${_ICU_CRB_REMOVE_RES}
    COMMAND ${ICU_GENRB_EXECUTABLE} ${_ICU_CRB_ARGS_FILES} -d
            ${_ICU_CRB_BUNDLE_DIR} ${_ICU_CRB_ARGS_GENRB_ARGS}
            ${_ICU_CRB_STDOUT_REDIRECT}
    COMMAND ${_ICU_CRB_CREATE_LIST}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_ICU_CRB_BUNDLE_TMP_DIR}
    COMMAND ${ICU_PKGDATA_EXECUTABLE} -m ${_ICU_CRB_BUNDLE_TYPE} -p
            ${_ICU_CRB_BUNDLE_NAME} "${_ICU_RES_PATH}"
            -s ${_ICU_CRB_BUNDLE_DIR} ${_ICU_CRB_BUNDLE_REVISION_ARG}
            -d ${_ICU_CRB_BUNDLE_DIR} ${_ICU_CRB_BUNDLE_TMP_ARGS}
            ${_ICU_CRB_ARGS_PKGDATA_ARGS} ${_ICU_CRB_STDOUT_REDIRECT}
    WORKING_DIRECTORY ${_ICU_CRB_BUNDLE_DIR}
    COMMENT "Creating resource bundle ${_ICU_CRB_BUNDLE_NAME}")

  IF (_ICU_CRB_CREATE_LIBRARY)
    ADD_LIBRARY (${_ICU_CRB_BUNDLE_NAME} ${_ICU_CRB_LIB_TYPE} IMPORTED)
    SET_TARGET_PROPERTIES (${_ICU_CRB_BUNDLE_NAME} PROPERTIES
      IMPORTED_LOCATION ${_ICU_CRB_BUNDLE_LOCATION}
      IMPORTED_IMPLIB ${_ICU_CRB_BUNDLE_LOCATION})
  ENDIF (_ICU_CRB_CREATE_LIBRARY)
ENDFUNCTION (CREATE_ICU_RESOURCE_BUNDLE)
