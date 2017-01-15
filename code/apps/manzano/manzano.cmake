cmake_minimum_required(VERSION 3.0)
project(manzano_config)

# CMake include guard
if(manzano_config_included)
  return()
endif(manzano_config_included)
set(manzano_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../mzn_config.cmake)

include(${DIR_MZN}/core/core.cmake)

message("\n###### config(manzano) #######")

set(DIR_manzano ${DIR_MZN}/apps/manzano)
include_directories(${DIR_manzano}/inc)

set(SOURCES_manzano ${SOURCES_core}
                    ${DIR_manzano}/src/manzano.cpp)
