
add_library(pvm-tool-settings INTERFACE)
target_link_libraries(pvm-tool-settings INTERFACE CLI11::CLI11)

list_dirs(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(TOOLLIST ${DIRS})

foreach(TOOL ${TOOLLIST})
  add_subdirectory(${TOOL})
  message(STATUS "Included subdirectory: ${DIR}")
  set_target_properties(${TOOL} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
endforeach()
