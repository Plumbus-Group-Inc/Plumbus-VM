add_library(pvm-memory STATIC)
target_sources(pvm-memory PRIVATE memory.cpp)
target_link_libraries(pvm-memory PRIVATE pvm-lib-settings)
