cmake_minimum_required(VERSION 3.0)
project(connection)

include(connection.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### connection #######")

add_library(connection ${SOURCES_connection})

set_target_properties(connection
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_connection}/bin)

# ---- TEST ----- #

add_executable(test_udp_connection ${DIR_connection}/test/test_udp_connection.cpp)
target_link_libraries(test_udp_connection ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_udp_connection connection)

if(UNIX AND NOT APPLE)

    add_executable(test_serial ${DIR_connection}/test/test_serial.cpp)
    target_link_libraries(test_serial ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(test_serial connection)

    ## serial is built as a library in config.cmake
    add_dependencies(test_serial serial_library)
    target_link_libraries(test_serial serial_library)

endif()

# ---- EXECUTABLES ----- #
add_executable(server_udp ${DIR_connection}/test/server_udp.cpp)
add_executable(client_udp ${DIR_connection}/test/client_udp.cpp)
target_link_libraries(server_udp connection)
target_link_libraries(client_udp connection)


