
aux_source_directory(. SRC)
add_library(httpclient ${SRC})
target_link_libraries(httpclient libcurl.a)
target_include_directories(httpclient INTERFACE .)
