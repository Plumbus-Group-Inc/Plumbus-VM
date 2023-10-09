cmake_minimum_required(VERSION 3.22.1)

project(Plumbus-VM
    LANGUAGES ASM C CXX
    VERSION 1.0.0
    DESCRIPTION "Pet Virtual Machine"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(plumbus-vm)
target_sources(plumbus-vm PRIVATE ./src/stub.cpp)

