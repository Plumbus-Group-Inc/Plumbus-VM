add_library(pvm-interpreter STATIC)
target_sources(pvm-interpreter PRIVATE interpreter.cpp)
target_link_libraries(pvm-interpreter PRIVATE pvm-lib-settings)
target_link_libraries(pvm-interpreter PUBLIC pvm-decoder pvm-executor pvm-memory)
