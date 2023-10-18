set(SUBDIRLIST interpreter decoder)

foreach(DIR ${SUBDIRLIST})
  add_subdirectory(${DIR})
endforeach()

