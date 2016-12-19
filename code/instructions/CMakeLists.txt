cmake_minimum_required(VERSION 3.0)
project(instructions)

include(instructions.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### instructions #######")

add_library(instructions ${SOURCES_instructions})

set_target_properties(instructions
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_instructions}/bin)

# ---- TEST ----- #
add_executable(test_instructions ${DIR_instructions}/test/test_instructions.cpp)
target_link_libraries(test_instructions ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_instructions instructions)

