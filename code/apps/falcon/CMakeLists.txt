cmake_minimum_required(VERSION 3.0)
project(falcon)

include(falcon.cmake)

message("\n###### falcon #######")

add_executable(falcon ${SOURCES_falcon})
install(TARGETS falcon DESTINATION /usr/bin/)
