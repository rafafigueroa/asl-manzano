cmake_minimum_required(VERSION 3.0)
project(cmd_field)

include(cmd_field.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### cmd_field #######")

add_library(cmd_field ${SOURCES_CMD_FIELD})

set_target_properties(cmd_field
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_CMD_FIELD}/lib)

# ---- TEST ----- #
if(TRUE)
    add_executable(test_cmd_field ${DIR_CMD_FIELD}/test/test_cmd_field.cpp)
    target_link_libraries(test_cmd_field ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(test_cmd_field cmd_field)
endif()
