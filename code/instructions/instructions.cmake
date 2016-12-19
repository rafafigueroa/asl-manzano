cmake_minimum_required(VERSION 3.0)
project(instructions_config)

# CMake include guard
if(instructions_config_included)
  return()
endif(instructions_config_included)
set(instructions_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

message("\n###### config(instructions) #######")

set(DIR_instructions ${DIR_MZN}/instructions)
include_directories(${DIR_instructions}/inc)

set(SOURCES_instructions
    ${DIR_instructions}/src/instruction_map.cpp
    ${DIR_instructions}/src/target_address.cpp
)

