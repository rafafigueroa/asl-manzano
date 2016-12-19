cmake_minimum_required(VERSION 3.0)
project(yellow_manzano_config)

# CMake include guard
if(yellow_manzano_config_included)
  return()
endif(yellow_manzano_config_included)
set(yellow_manzano_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

include(${DIR_MZN}/core/core.cmake)

message("\n###### config(yellow_manzano) #######")

set(DIR_yellow_manzano ${DIR_MZN}/yellow_manzano)
include_directories(${DIR_yellow_manzano}/inc)

set(SOURCES_yellow_manzano ${SOURCES_core}
                           ${DIR_yellow_manzano}/src/e300_server.cpp
                           ${DIR_yellow_manzano}/src/yellow_manzano.cpp)

