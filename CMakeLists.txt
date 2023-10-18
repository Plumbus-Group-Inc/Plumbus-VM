cmake_minimum_required(VERSION 3.16.3)

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

include(cmake/comp-flags.cmake)

# indicate the tests build
option(BUILD_TESTS "Build tests" ON)
# add -Werror option
option(ENABLE_WERROR "Enable -Werror option (CI)" OFF)
# Test running stuff
if(BUILD_TESTS)
  enable_testing()
endif()

add_library(pvm-settings INTERFACE)
target_include_directories(pvm-settings INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(pvm-settings INTERFACE cxx_std_20)
apply_compiler_flags(pvm-settings INTERFACE)
if(ENABLE_WERROR)
  target_compile_options(pvm-settings INTERFACE -Werror)
endif()

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)
add_subdirectory(thirdparty)
