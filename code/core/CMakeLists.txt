cmake_minimum_required(VERSION 3.0)
project(core)

include(core.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### core #######")

add_library(core ${SOURCES_core})

set_target_properties(core
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_CORE}/bin)

# ---- TEST ----- #
# set(SOURCES_test_core ${DIR_CORE}/test/test_core.cpp
#                      ${DIR_CORE}/test/test_instruction_interpreter.cpp)

add_executable(test_core ${DIR_CORE}/test/test_core.cpp)
target_link_libraries(test_core ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_core core)

add_executable(test_instruction_interpreter
                   ${DIR_CORE}/test/test_instruction_interpreter.cpp)

target_link_libraries(test_instruction_interpreter ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_instruction_interpreter core)
