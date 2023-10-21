add_library(pvm-executor executor.cpp regfile.cpp)
target_link_libraries(pvm-executor PRIVATE pvm-lib-settings)
target_link_libraries(pvm-executor PUBLIC pvm-memory pvm-decoder)
target_compile_options(pvm-executor
  PUBLIC
    -fno-inline-functions-called-once
    -foptimize-sibling-calls
    -O2
)
