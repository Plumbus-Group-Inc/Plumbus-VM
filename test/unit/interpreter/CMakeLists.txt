pvm_add_test(test-example example.cpp)

pvm_add_test(test-square-eq square-eq.cpp)
target_link_libraries(test-square-eq PRIVATE pvm-interpreter)
target_include_directories(test-square-eq PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

pvm_add_test(test-cycle cycle.cpp)
target_link_libraries(test-cycle PRIVATE pvm-interpreter)

pvm_add_test(test-handlers handlers.cpp)
target_link_libraries(test-handlers PRIVATE pvm-interpreter)
target_include_directories(test-handlers PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

pvm_add_test(test-call call.cpp)
target_link_libraries(test-call PRIVATE pvm-interpreter)
