list_dirs(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(TOOLLIST ${DIRS})

foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
  message(STATUS "Included subdirectory: ${DIR}")
endforeach()

foreach(TOOL ${TOOLLIST})
  set_target_properties(${TOOL} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                           "${CMAKE_BINARY_DIR}/bin/")
endforeach()

set(TOOLLIST
    "${TOOLLIST}"
    PARENT_SCOPE)
