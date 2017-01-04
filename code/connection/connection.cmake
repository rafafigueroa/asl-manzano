cmake_minimum_required(VERSION 3.0)
project(connection_config)

# CMake include guard
if(connection_config_included)
  return()
endif(connection_config_included)
set(connection_config_included true)

include(${CMAKE_CURRENT_LIST_DIR}/../mzn_config.cmake)

message("\n###### config(connection) #######")

set(DIR_connection ${DIR_MZN}/connection)

# mzn_time.h
include_directories(${DIR_MZN}/cmd_field/inc/)

include_directories(${DIR_connection}/inc/)

set(SOURCES_connection ${SOURCES_exceptions}
                       ${DIR_connection}/src/dummy_q_port.cpp
                       ${DIR_connection}/src/message.cpp
                       ${DIR_connection}/src/udp_connection.cpp
                       ${DIR_connection}/src/tcp_connection.cpp
                       ${DIR_connection}/src/address_info_handler.cpp
                       ${DIR_connection}/src/connection_handler.cpp
)
