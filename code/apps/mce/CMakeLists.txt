cmake_minimum_required(VERSION 3.0)
project(mce)

include(mce.cmake)

set(k_mce_VERSION_MAJOR 0)
set(k_mce_VERSION_MINOR 9)

configure_file("${DIR_mce}/inc/mce_cmake_config.h.in"
               "${DIR_mce}/inc/mce_cmake_config.h")

message("\n###### mce #######")

# ---------------------------------------------------------------- #
message("\n ***** config files: ***** ")
message("path: " ${DIR_install_config_files})

file(MAKE_DIRECTORY ${DIR_install_config_files})

# this works at install time but still with root permissions
# with sudo make install
# install(DIRECTORY
#        DESTINATION ${DIR_install_config_files} USE_SOURCE_PERMISSIONS)

install(FILES ${runtime_config_files}
        DESTINATION ${DIR_install_config_files})

# ---------------------------------------------------------------- #
message("\n ***** executable: ***** ")
message("path: " ${DIR_install_bin})

add_executable(mce ${SOURCES_mce})

#execute_process(COMMAND test -w ${DIR_install_bin}
#                RESULT_VARIABLE install_dir_no_permission)
#if (${install_dir_no_permission})

install(TARGETS mce
        DESTINATION ${DIR_install_bin}/)

# GOOGLE TEST
# ---------------------------------------------------------------- #
if(FALSE)

    include(${DIR_MZN}/test_config.cmake)

    set(SOURCES_test_mce
            ${SOURCES_core}
            ${DIR_mce}/src/mce.cpp
            ${DIR_mce}/src/mce_cli.cpp
            ${DIR_green_manzano}/src/user_interpreter.cpp
            ${DIR_mce}/test/test_mce.cpp)

    add_executable(test_mce EXCLUDE_FROM_ALL
                   ${SOURCES_test_mce})

    target_link_libraries(test_mce ${GTEST_BOTH_LIBRARIES})

endif()

# to create a deb package:
#set(CPACK_GENERATOR "DEB")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "rfigueroa@usgs.gov")
#set(CPACK_PACKAGE_FILE_NAME "mce")
#include(CPack)
