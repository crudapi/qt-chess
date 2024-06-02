cmake_minimum_required(VERSION 3.3)
project(cpplog)

option(TEST_CPPLOG off)

add_library(cpplog Log.cpp)

if(TEST_CPPLOG)
    add_executable(test_cpplog test_main.cpp)
    target_link_libraries(test_cpplog cpplog)
endif()
