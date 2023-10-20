add_library(pvm-decoder STATIC)
target_sources(pvm-decoder PRIVATE decoder.cpp)
target_link_libraries(pvm-decoder PRIVATE pvm-lib-settings)
