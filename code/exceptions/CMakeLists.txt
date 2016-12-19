cmake_minimum_required(VERSION 3.0)
project(exceptions)

include(exceptions.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### exceptions #######")

add_library(exceptions ${SOURCES_exceptions})

set_target_properties(exceptions
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_exceptions}/bin)

# ---- TEST ----- #
add_executable(test_exceptions ${DIR_exceptions}/test/test_exceptions.cpp)
target_link_libraries(test_exceptions ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_exceptions exceptions)
