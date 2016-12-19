cmake_minimum_required(VERSION 3.0)
project(exceptions_config)

# CMake include guard
if(exceptions_config_included)
  return()
endif(exceptions_config_included)
set(exceptions_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

message("\n###### config(exceptions) #######")

set(DIR_exceptions ${DIR_MZN}/exceptions)

include_directories(${DIR_exceptions}/inc/)

set(SOURCES_exceptions ${DIR_exceptions}/src/mzn_except.cpp)

