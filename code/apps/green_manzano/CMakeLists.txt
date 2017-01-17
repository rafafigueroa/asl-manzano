cmake_minimum_required(VERSION 3.0)
project(green_manzano)

include(green_manzano.cmake)

set(k_green_manzano_VERSION_MAJOR 0)
set(k_green_manzano_VERSION_MINOR 9)

configure_file("${DIR_green_manzano}/inc/green_manzano_cmake_config.h.in"
               "${DIR_green_manzano}/inc/green_manzano_cmake_config.h")

message("\n###### green_manzano #######")

# ---------------------------------------------------------------- #
message("\n ***** config files: ***** ")
message("path: " ${DIR_install_config_files})

install(FILES ${runtime_config_files}
        DESTINATION ${DIR_install_config_files})

# ---------------------------------------------------------------- #
message("\n ***** executable: ***** ")
message("path: " ${DIR_install_bin})

add_executable(green_manzano ${SOURCES_green_manzano})

#execute_process(COMMAND test -w ${DIR_install_bin}
#                RESULT_VARIABLE install_dir_no_permission)
#if (${install_dir_no_permission})

install(TARGETS green_manzano DESTINATION ${DIR_install_bin}/)

# GOOGLE TEST
# ---------------------------------------------------------------- #
if(FALSE)

    include(${DIR_MZN}/test_config.cmake)

    set(SOURCES_test_user_interpreter
            ${SOURCES_core}
            ${DIR_green_manzano}/src/user_interpreter.cpp
            ${DIR_green_manzano}/test/test_user_interpreter.cpp)

    add_executable(test_user_interpreter EXCLUDE_FROM_ALL
                   ${SOURCES_test_user_interpreter})

    target_link_libraries(test_user_interpreter ${GTEST_BOTH_LIBRARIES})

endif()

# to create a deb package:
#set(CPACK_GENERATOR "DEB")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "rfigueroa@usgs.gov")
#set(CPACK_PACKAGE_FILE_NAME "green_manzano")
#include(CPack)
