include_directories(../)

# file(GLOB TEST_SOURCES "test*.cpp")

# foreach(TEST_SOURCE ${TEST_SOURCES})
#     get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
#     add_executable(${TEST_NAME} ${TEST_SOURCE})
# endforeach(TEST_SOURCE)

add_executable(test_any "test_any.cpp")
add_executable(test_iterator "test_iterator.cpp")
add_executable(test_number "test_number.cpp")
add_executable(test_pointer "test_pointer.cpp")
add_executable(test_self "test_self.cpp")
add_executable(test_set "test_set.cpp")
add_executable(test_map "test_map.cpp")
add_executable(test_time "test_time.cpp")
