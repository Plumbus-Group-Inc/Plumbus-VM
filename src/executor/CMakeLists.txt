set(TEMPLATE_FILE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/executor-generated.cpp.j2)
set(GENERATED_FILE_CPP ${CMAKE_CURRENT_BINARY_DIR}/executor-generated.cpp)
pvm_add_generated(pvm-executor-generated ${TEMPLATE_FILE_CPP} ${GENERATED_FILE_CPP})

set(TEMPLATE_FILE_HPP ${CMAKE_SOURCE_DIR}/include/executor/handlers-generated.hpp.j2)
set(GENERATED_FILE_HPP ${CMAKE_BINARY_DIR}/include/generated/handlers-generated.hpp)
pvm_add_generated(pvm-handlers-generated ${TEMPLATE_FILE_HPP} ${GENERATED_FILE_HPP})

add_library(pvm-executor regfile.cpp executor.cpp ${GENERATED_FILE_CPP})
add_dependencies(pvm-executor pvm-executor-generated pvm-handlers-generated)

target_link_libraries(pvm-executor PRIVATE pvm-lib-settings)
target_link_libraries(pvm-executor PUBLIC pvm-memory pvm-decoder pvm-common)
target_compile_options(pvm-executor
  PUBLIC
    -fno-inline-functions-called-once
    -foptimize-sibling-calls
    -O2
)
