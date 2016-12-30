cmake_minimum_required(VERSION 3.0)
project(falcon)

message("\n###### falcon #######")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wno-missing-braces -Wmissing-field-initializers")

set(SOURCES_falcon falcon.cpp)
add_executable(falcon ${SOURCES_falcon})
include_directories(${CMAKE_CURRENT_LIST_DIR})

# target_link_libraries(falcon stdc++fs)

install(TARGETS falcon DESTINATION /usr/bin/)
