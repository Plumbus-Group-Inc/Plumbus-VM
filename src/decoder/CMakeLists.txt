set(TEMPLATE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/decoder.cpp.j2)
set(GENERATED_FILE ${CMAKE_CURRENT_BINARY_DIR}/decoder.cpp)
pvm_add_generated(pvm-decoder-generated ${TEMPLATE_FILE} ${GENERATED_FILE})

add_library(pvm-decoder ${GENERATED_FILE})
add_dependencies(pvm-decoder pvm-decoder-generated)

target_link_libraries(pvm-decoder PRIVATE pvm-lib-settings)
