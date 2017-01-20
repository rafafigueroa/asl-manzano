cmake_minimum_required(VERSION 3.0)
project(commands)

include(commands.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### commands #######")

add_library(commands ${SOURCES_commands})

set_target_properties(commands
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_COMMANDS}/lib)

# ---- TEST ----- #
add_executable(test_commands ${DIR_COMMANDS}/test/test_commands.cpp)
target_link_libraries(test_commands ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_commands commands)
