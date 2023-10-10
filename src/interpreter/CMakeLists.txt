
add_library(pvm-interpreter STATIC)
target_sources(pvm-interpreter PRIVATE
    ./value.cpp
)
target_include_directories(pvm-interpreter PRIVATE
    ${CMAKE_SOURCE_DIR}/include)
target_compile_features(pvm-interpreter PRIVATE cxx_std_20)

