cmake_minimum_required(VERSION 3.0)
project(connection)

include(connection.cmake)
include(${DIR_MZN}/test_config.cmake)

message("\n###### connection #######")


# ----- append serial files to connection library ---- #
if(UNIX AND NOT APPLE)

    include(${DIR_MZN}/serial_config.cmake)
    list(APPEND SOURCES_connection
            ${DIR_connection}/src/serial_connection.cpp)
endif()

# ---- normal connection library for test ---- #
add_library(connection ${SOURCES_connection})

set_target_properties(connection
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${DIR_connection}/bin)

# ----- add serial test linking with connection library ---- #
if(UNIX AND NOT APPLE)
    add_executable(test_serial ${DIR_connection}/test/test_serial.cpp)
    target_link_libraries(test_serial ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(test_serial connection)

    ## serial is built as a library in serial_config.cmake
    add_dependencies(test_serial serial_library)
    target_link_libraries(test_serial serial_library)

endif()

# ---- TEST ----- #
add_executable(test_udp_connection ${DIR_connection}/test/test_udp_connection.cpp)
target_link_libraries(test_udp_connection ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_udp_connection connection)

# ---- EXECUTABLES ----- #
add_executable(server_udp ${DIR_connection}/test/server_udp.cpp)
add_executable(client_udp ${DIR_connection}/test/client_udp.cpp)
target_link_libraries(server_udp connection)
target_link_libraries(client_udp connection)


