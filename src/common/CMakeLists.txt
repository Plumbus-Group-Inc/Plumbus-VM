add_library(pvm-common STATIC)
target_sources(pvm-common PRIVATE value.cpp)
target_link_libraries(pvm-common PUBLIC pvm-settings)
