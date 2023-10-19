add_library(pvm-executor executor.cpp regfile.cpp)
target_link_libraries(pvm-executor PRIVATE pvm-lib-settings)
target_link_libraries(pvm-executor PUBLIC pvm-memory)
