cmake_minimum_required(VERSION 3.0)
project(seismic_network)

include(seismic_network.cmake)
include(${DIR_MZN}/test_config.cmake)


message("\n###### seismic_network #######")

add_library(seismic_network ${SOURCES_seismic_network})

set_target_properties(seismic_network
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_seismic_network}/lib)

# ---- TEST ----- #
add_executable(test_seismic_network ${DIR_seismic_network}/test/test_seismic_network.cpp)
target_link_libraries(test_seismic_network ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_seismic_network seismic_network)



