set(TEMPLATE_FILE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/run.cpp.j2)
set(GENERATED_FILE_CPP ${CMAKE_CURRENT_BINARY_DIR}/run.cpp)
pvm_add_generated(pvm-interpreter-generated ${TEMPLATE_FILE_CPP} ${GENERATED_FILE_CPP})

set(TEMPLATE_FILE_HPP ${CMAKE_SOURCE_DIR}/include/interpreter/handlers.hpp.j2)
set(GENERATED_FILE_HPP ${CMAKE_BINARY_DIR}/include/generated/handlers.hpp)
pvm_add_generated(pvm-handlers-generated ${TEMPLATE_FILE_HPP} ${GENERATED_FILE_HPP})

add_library(pvm-interpreter STATIC)
target_sources(pvm-interpreter PRIVATE interpreter.cpp handlers.cpp ${GENERATED_FILE_CPP})
add_dependencies(pvm-interpreter pvm-interpreter-generated pvm-handlers-generated)

target_link_libraries(pvm-interpreter PRIVATE pvm-lib-settings)
target_link_libraries(pvm-interpreter PUBLIC pvm-decoder pvm-memory)
target_compile_options(pvm-interpreter
  PUBLIC
    -fno-inline-functions-called-once
    -foptimize-sibling-calls
    -O2
)
