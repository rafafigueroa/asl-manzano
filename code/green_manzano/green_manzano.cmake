cmake_minimum_required(VERSION 3.0)
project(green_manzano_config)

# CMake include guard
if(green_manzano_config_included)
  return()
endif(green_manzano_config_included)
set(green_manzano_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../mzn_config.cmake)

include(${DIR_MZN}/core/core.cmake)

message("\n###### config(green_manzano) #######")

set(DIR_green_manzano ${DIR_MZN}/green_manzano)
include_directories(${DIR_green_manzano}/inc)

set(SOURCES_green_manzano ${SOURCES_core}
                          ${DIR_green_manzano}/src/user_interpreter.cpp
                          ${DIR_green_manzano}/src/green_manzano.cpp)
