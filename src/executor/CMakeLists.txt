add_library(pvm-executor STATIC)
target_sources(pvm-executor PRIVATE executor.cpp)
target_link_libraries(pvm-executor PRIVATE pvm-lib-settings)
