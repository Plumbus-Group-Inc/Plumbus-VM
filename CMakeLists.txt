cmake_minimum_required(VERSION 3.22.1)

if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(
    FATAL_ERROR
      "In-source builds are disabled.
  Please create a subfolder and use `cmake ..` inside it.
  NOTE: cmake will now create CMakeCache.txt and CMakeFiles/*.
  You must delete them, or cmake will refuse to work.")
endif()

project(Plumbus-VM
    LANGUAGES ASM C CXX
    VERSION 1.0.0
    DESCRIPTION "Pet Virtual Machine"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/list-dirs.cmake)
include(cmake/upd-tar-list.cmake)
include(cmake/comp-flags.cmake)

# indicate the tests build
option(BUILD_TESTS "Build tests" ON)
# add -Werror option
option(ENABLE_WERROR "Enable -Werror option (CI)" OFF)
# Test running stuff
if(BUILD_TESTS)
  enable_testing()
endif()

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)
# add_subdirectory(thirdparty)

message("Collected libs: ${LIBLIST}")
message("Collected tools: ${TOOLLIST}")

set(TARGETS)
list(APPEND TARGETS ${LIBLIST} ${TOOLLIST})

foreach(TARGET ${TARGETS})
  target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_compile_features(${TARGET} PRIVATE cxx_std_20)
  apply_compiler_flags(${TARGET} PUBLIC)
  if(ENABLE_WERROR)
    target_compile_options(${TARGET} PRIVATE -Werror)
  endif()
endforeach()

