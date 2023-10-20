pvm_add_test(test-example example.cpp)

pvm_add_test(test-square-eq square-eq.cpp)
target_link_libraries(test-square-eq PRIVATE pvm-interpreter)
