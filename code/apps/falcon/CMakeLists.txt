cmake_minimum_required(VERSION 3.0)
project(falcon)

include(falcon.cmake)

message("\n###### falcon #######")

add_executable(falcon ${SOURCES_falcon})

# depending on final use, update directory passing logic
# install(TARGETS falcon DESTINATION /usr/local/bin/)
# install(PROGRAMS get_falcon_data.bash DESTINATION /usr/local/bin/)
