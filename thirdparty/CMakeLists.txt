list_dirs(DIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
  message(STATUS "Included subdirectory: ${DIR}")
endforeach()
