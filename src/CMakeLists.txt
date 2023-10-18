set(SUBDIRLIST interpreter decoder executor)

foreach(DIR ${SUBDIRLIST})
  add_subdirectory(${DIR})
endforeach()

