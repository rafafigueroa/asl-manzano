cmake_minimum_required(VERSION 3.0)
project(yellow_manzano)

include(yellow_manzano.cmake)

set(k_yellow_manzano_VERSION_MAJOR 0)
set(k_yellow_manzano_VERSION_MINOR 5)

configure_file("${DIR_yellow_manzano}/inc/yellow_manzano_cmake_config.h.in"
               "${DIR_yellow_manzano}/inc/yellow_manzano_cmake_config.h")

message("\n###### yellow_manzano #######")

add_executable(yellow_manzano ${SOURCES_yellow_manzano})
install(TARGETS yellow_manzano DESTINATION /usr/local/bin/)

## serial is built as a library in serial_config.cmake
add_dependencies(yellow_manzano serial_library)
target_link_libraries(yellow_manzano serial_library)
