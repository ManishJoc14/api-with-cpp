cmake_minimum_required(VERSION 3.0)
project(curllib_Test)

find_package(CURL REQUIRED)

add_executable(a main.cpp)

target_link_libraries(a CURL::libcurl_shared)
