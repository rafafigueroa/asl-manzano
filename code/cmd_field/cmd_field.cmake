cmake_minimum_required(VERSION 3.0)
project(cmd_field_config)

# CMake include guard
if(cmd_field_config_included)
  return()
endif(cmd_field_config_included)
set(cmd_field_config_included true)

# this is hard coded
include(${CMAKE_CURRENT_SOURCE_DIR}/../mzn_config.cmake)

message("\n###### config(cmd_field) #######")

set(DIR_CMD_FIELD ${DIR_MZN}/cmd_field)

include_directories(${DIR_CMD_FIELD}/inc/)

set(SOURCES_CMD_FIELD
                      ${DIR_exceptions}/src/mzn_except.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_array.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_array_byte.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_array_char.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_array_ubyte.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_bitmap.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_duration.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_hex.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_ignore.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_cal_amplitude.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_pstring.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_ip.cpp
                      ${DIR_CMD_FIELD}/src/cmd_field_time.cpp
)
