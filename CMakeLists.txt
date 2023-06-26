cmake_minimum_required(VERSION 3.16)

project(TestProject)

set(CMAKE_CXX_STANDARD 20)
file(GLOB TEST_SOURCES "test*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endforeach(TEST_SOURCE)
