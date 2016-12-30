cmake_minimum_required(VERSION 3.0)
project(serial_config)

# CMake include guard
if(serial_config_included)
  return()
endif(serial_config_included)
set(serial_config_included true)

# IMPORTANT: include mzn_config.cmake when you are building this
# it provides the variable DIR_MZN

message("\n###### config(serial) #######")

# ----- SERIAL WJWWOOD C++ LIBRARY ----- #
if(UNIX AND NOT APPLE)

    set(DIR_serial ${DIR_MZN}/libs/serial)
    include_directories(${DIR_serial}/include/serial)
    include_directories(${DIR_serial}/include/serial/impl)

    set(SOURCES_serial ${DIR_serial}/src/serial.cc
                       ${DIR_serial}/src/impl/serial_unix.cc
                       ${DIR_serial}/src/impl/list_ports/list_ports_linux.cc
                       ${DIR_serial}/src/impl/list_ports/list_ports_osx.cc)

    add_library(serial_library STATIC ${SOURCES_serial})
    target_link_libraries(serial_library rt pthread)

endif()
