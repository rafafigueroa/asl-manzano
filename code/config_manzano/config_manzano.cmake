cmake_minimum_required(VERSION 3.0)
project(config_manzano_config)

# CMake include guard
if(config_manzano_config_included)
  return()
endif(config_manzano_config_included)
set(config_manzano_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

include(${DIR_MZN}/core/core.cmake)

message("\n###### config(config_manzano) #######")

set(DIR_config_manzano ${DIR_MZN}/config_manzano)
include_directories(${DIR_config_manzano}/inc)

set(SOURCES_config_manzano ${SOURCES_core}
                           ${DIR_config_manzano}/src/config_manzano.cpp
                           ${DIR_config_manzano}/src/builder_loop.cpp
                           ${DIR_config_manzano}/src/component_loop.cpp
                           ${DIR_config_manzano}/src/network_loop.cpp)
                      

