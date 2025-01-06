
add_executable(test_get test_get.cc)
target_link_libraries(test_get httpclient)

add_test(NAME test_get COMMAND test_get)
