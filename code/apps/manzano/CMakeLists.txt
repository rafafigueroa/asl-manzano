cmake_minimum_required(VERSION 3.4)
project(manzano)

include(manzano.cmake)

set(k_manzano_VERSION_MAJOR 1)
set(k_manzano_VERSION_MINOR 0)

configure_file("${DIR_manzano}/inc/manzano_cmake_config.h.in"
               "${DIR_manzano}/inc/manzano_cmake_config.h")

message("\n###### manzano #######")

# ---------------------------------------------------------------- #
message("\n ***** library: ***** ")
message("path: " ${DIR_install_lib})

add_library(manzano STATIC ${SOURCES_manzano})

install(TARGETS manzano DESTINATION ${DIR_install_lib}/)

# ---------------------------------------------------------------- #
message("\n ***** header files: ***** ")
message("path: " ${DIR_install_headers})

get_property(manzano_include_dirs
             TARGET manzano
             PROPERTY INCLUDE_DIRECTORIES)

foreach(dir ${manzano_include_dirs})

    message(STATUS "dir='${dir}'")
    file(GLOB manzano_include_files ${dir}/*.h)
    # message("......................")
    # message(${manzano_include_files})
    install(FILES ${manzano_include_files}
            DESTINATION ${DIR_install_lib})

endforeach()

# ---------------------------------------------------------------- #
message("\n ***** config files: ***** ")
message("path: " ${DIR_install_config_files})

install(FILES ${runtime_files}
        DESTINATION ${DIR_install_config_files})

message("\n ******************************** ")
# ---------------------------------------------------------------- #
