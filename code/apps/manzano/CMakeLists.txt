cmake_minimum_required(VERSION 3.0)
project(manzano)

include(manzano.cmake)

set(k_manzano_VERSION_MAJOR 1)
set(k_manzano_VERSION_MINOR 0)

configure_file("${DIR_manzano}/inc/manzano_cmake_config.h.in"
               "${DIR_manzano}/inc/manzano_cmake_config.h")

message("\n###### manzano #######")

add_library(manzano STATIC ${SOURCES_manzano})
install(TARGETS manzano DESTINATION /usr/local/lib/)

