add_executable(pvm main.cpp)
target_link_libraries(pvm PRIVATE pvm-tool-settings pvm-common pvm-interpreter)
