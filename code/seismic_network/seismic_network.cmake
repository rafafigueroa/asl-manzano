cmake_minimum_required(VERSION 3.0)
project(seismic_network_config)

# CMake include guard
if(seismic_network_config_included)
  return()
endif(seismic_network_config_included)
set(seismic_network_config_included true)

include(${CMAKE_CURRENT_LIST_DIR}/../mzn_config.cmake)

include(${DIR_MZN}/instructions/instructions.cmake)
include(${DIR_MZN}/connection/connection.cmake)

message("\n###### config(seismic_network) #######")

set(DIR_seismic_network ${DIR_MZN}/seismic_network)

include_directories(${DIR_seismic_network}/inc/)

set(SOURCES_seismic_network
    ${SOURCES_instructions}
    ${SOURCES_connection}
    ${DIR_seismic_network}/src/data_processor.cpp
    ${DIR_seismic_network}/src/connection_handler_digitizer.cpp
    ${DIR_seismic_network}/src/connection_handler_e300.cpp
    ${DIR_seismic_network}/src/e300.cpp
    ${DIR_seismic_network}/src/seismic_network.cpp
)

