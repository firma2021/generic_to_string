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
add_executable(test_tuple "test_tuple.cpp")
add_executable(test_variant "test_variant.cpp")
add_executable(test_enum "test_enum.cpp")
add_executable(test_container_adaptor "test_container_adaptor.cpp")
add_executable(test_istream "test_istream.cpp")
add_executable(test_optional "test_optional.cpp")
add_executable(test_costom "test_costom.cpp")
add_executable(test_container "test_container.cpp")
add_executable(test_multithread "test_multithread.cpp")
add_executable(test_sout "test_sout.cpp")
add_executable(test_smartpointer "test_smartpointer.cpp")