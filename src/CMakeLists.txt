add_subdirectory(common)
add_library(pvm-lib-settings INTERFACE)
target_link_libraries(pvm-lib-settings INTERFACE pvm-common)

set(SUBDIRLIST interpreter decoder executor memory)

foreach(DIR ${SUBDIRLIST})
  add_subdirectory(${DIR})
endforeach()

