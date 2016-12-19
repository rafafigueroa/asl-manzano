cmake_minimum_required(VERSION 3.0)
project(config_manzano)

include(config_manzano.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### config_manzano #######")

add_executable(config_manzano ${SOURCES_config_manzano})

install(TARGETS config_manzano DESTINATION /usr/bin/)

#set_target_properties(config_manzano
#                      PROPERTIES
#                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_config_manzano}/bin)

# ---- TEST ----- #
if(TRUE)

include(${DIR_MZN}/test_config.cmake)

set(SOURCES_test_builder_loop ${SOURCES_core}
                                  ${DIR_config_manzano}/src/builder_loop.cpp
                                  ${DIR_config_manzano}/test/test_builder_loop.cpp)

set(SOURCES_test_network_loop ${SOURCES_core}
                                  ${DIR_config_manzano}/src/builder_loop.cpp
                                  ${DIR_config_manzano}/test/test_network_loop.cpp)                                 


add_executable(test_builder_loop EXCLUDE_FROM_ALL ${SOURCES_test_builder_loop})
target_link_libraries(test_builder_loop ${GTEST_BOTH_LIBRARIES})

add_executable(test_network_loop EXCLUDE_FROM_ALL ${SOURCES_test_network_loop})
target_link_libraries(test_network_loop ${GTEST_BOTH_LIBRARIES})

endif()
# to create a deb package:
#set(CPACK_GENERATOR "DEB")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "akearns@usgs.gov")
#set(CPACK_PACKAGE_FILE_NAME "config_manzano")
#include(CPack)
