set(TEMPLATE_FILE ${CMAKE_SOURCE_DIR}/include/common/instruction-generated.hpp.j2)
set(GENERATED_FILE ${CMAKE_BINARY_DIR}/include/generated/instruction-generated.hpp)
pvm_add_generated(pvm-instruction-generated ${TEMPLATE_FILE} ${GENERATED_FILE})

add_library(pvm-common STATIC)
add_dependencies(pvm-common pvm-instruction-generated)

target_sources(pvm-common PRIVATE value.cpp ${GENERATED_FILE})
target_link_libraries(pvm-common PUBLIC pvm-settings)
