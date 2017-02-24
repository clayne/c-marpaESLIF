MACRO (BuildDependency arg_name arg_srcdir arg_workdir arg_installprefix)
  MESSAGE (STATUS "... Processing dependency ${arg_name}")
  MESSAGE (STATUS "... Copying ${arg_name}")
  MESSAGE (STATUS "... ... ${CMAKE_COMMAND} -E copy_directory ${arg_srcdir} ${arg_workdir}")
  EXECUTE_PROCESS(
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${arg_srcdir}" "${arg_workdir}"
    )
  MESSAGE (STATUS "... Setting up ${arg_name}, generator ${CMAKE_GENERATOR}, build type ${CMAKE_BUILD_TYPE}, install prefix ${arg_installprefix}")
  EXECUTE_PROCESS(
    COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${arg_installprefix}
    WORKING_DIRECTORY "${arg_workdir}"
    )
  MESSAGE (STATUS "... Building and installing ${arg_name}")
  EXECUTE_PROCESS(
    COMMAND "${CMAKE_MAKE_PROGRAM}" install
    WORKING_DIRECTORY "${arg_workdir}"
    )
ENDMACRO ()
