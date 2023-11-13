find_package( Python3 3.10 REQUIRED)
execute_process(
  COMMAND ${Python3_EXECUTABLE} -m pip show pyyaml jinja2
  RESULT_VARIABLE EXIT_CODE
  OUTPUT_QUIET
)

if(NOT ${EXIT_CODE} EQUAL 0)
  message(FATAL_ERROR "Install python packages: pyyaml, jinja2")
endif()

add_library(pvm-lib-settings INTERFACE)
target_link_libraries(pvm-lib-settings INTERFACE pvm-common)

set(SUBDIRLIST common decoder executor interpreter memory)
foreach(DIR ${SUBDIRLIST})
  add_subdirectory(${DIR})
endforeach()
