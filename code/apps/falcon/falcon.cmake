cmake_minimum_required(VERSION 3.0)
project(falcon_config)

# CMake include guard
if(falcon_config_included)
  return()
endif(falcon_config_included)
set(falcon_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../mzn_config.cmake)
include(${DIR_MZN}/commands/commands.cmake)

message("\n###### config(falcon) #######")

set(DIR_falcon ${DIR_MZN}/apps/falcon)
include_directories(${DIR_falcon}/inc)

set(SOURCES_falcon ${SOURCES_core}
                   ${DIR_falcon}/src/falcon.cpp)
