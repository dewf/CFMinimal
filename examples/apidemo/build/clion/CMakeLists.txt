cmake_minimum_required(VERSION 3.8)
project(apidemo)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        "../../source/apidemo.cpp")

add_executable(apidemo ${SOURCE_FILES})

add_subdirectory("../../../../build/clion/" "${CMAKE_CURRENT_BINARY_DIR}/cfminimal")

target_link_libraries(apidemo cfminimal)
