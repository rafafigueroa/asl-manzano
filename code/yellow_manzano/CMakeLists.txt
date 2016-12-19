cmake_minimum_required(VERSION 3.0)
project(yellow_manzano)

include(yellow_manzano.cmake)

set(k_yellow_manzano_VERSION_MAJOR 0)
set(k_yellow_manzano_VERSION_MINOR 1)

configure_file("${DIR_yellow_manzano}/inc/yellow_manzano_cmake_config.h.in"
               "${DIR_yellow_manzano}/inc/yellow_manzano_cmake_config.h")

message("\n###### yellow_manzano #######")

add_executable(yellow_manzano ${SOURCES_yellow_manzano})
install(TARGETS yellow_manzano DESTINATION /usr/bin/)

if(UNIX AND NOT APPLE)
    ## serial is built as a library in config.cmake
    add_dependencies(yellow_manzano serial_library)
    target_link_libraries(yellow_manzano serial_library)

endif()


#set_target_properties(yellow_manzano
#                      PROPERTIES
#                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_yellow_manzano}/bin)

# ---- TEST ----- #
if(FALSE)

include(${DIR_MZN}/test_config.cmake)

set(SOURCES_test_serial ${SOURCES_core}
                                  ${DIR_yellow_manzano}/src/serial.cpp
                                  ${DIR_yellow_manzano}/test/test_serial.cpp)


add_executable(test_serial EXCLUDE_FROM_ALL ${SOURCES_test_serial})
target_link_libraries(test_serial ${GTEST_BOTH_LIBRARIES})

endif()
# to create a deb package:
#set(CPACK_GENERATOR "DEB")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "rfigueroa@usgs.gov")
#set(CPACK_PACKAGE_FILE_NAME "yellow_manzano")
#include(CPack)
