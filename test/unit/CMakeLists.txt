list_dirs(DIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
  message(STATUS "Included subdirectory: ${DIR}")
endforeach()

message(VERBOSE "Collected unit tests: ${UNIT}")

foreach(TEST ${UNIT})
  target_link_libraries(${TEST} gtest_main)
  add_test(NAME ${TEST} COMMAND ${TEST} --gtest_color=yes)
  set_property(TEST ${TEST} PROPERTY LABELS unit)
endforeach()
