set(TEMPLATE_FILE ${CMAKE_SOURCE_DIR}/include/common/instruction.hpp.j2)
set(GENERATED_FILE ${CMAKE_BINARY_DIR}/include/generated/instruction.hpp)
pvm_add_generated(pvm-instruction-generated ${TEMPLATE_FILE} ${GENERATED_FILE})

add_library(pvm-common INTERFACE)
add_dependencies(pvm-common pvm-instruction-generated)

target_link_libraries(pvm-common INTERFACE pvm-settings)
