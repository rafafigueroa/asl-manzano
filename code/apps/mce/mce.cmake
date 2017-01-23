cmake_minimum_required(VERSION 3.0)
project(mce_config)

# CMake include guard
if(mce_config_included)
  return()
endif(mce_config_included)
set(mce_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../mzn_config.cmake)

include(${DIR_MZN}/apps/green_manzano/green_manzano.cmake)

message("\n###### config(mce) #######")

set(DIR_mce ${DIR_MZN}/apps/mce)
include_directories(${DIR_mce}/inc)

set(SOURCES_mce ${SOURCES_core}
                ${DIR_green_manzano}/src/user_interpreter.cpp
                ${DIR_mce}/src/mce.cpp)
