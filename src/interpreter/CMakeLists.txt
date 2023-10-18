
add_library(pvm-interpreter STATIC)
target_sources(pvm-interpreter PRIVATE
    ./value.cpp
)

target_link_libraries(pvm-interpreter PRIVATE pvm-lib-settings)
