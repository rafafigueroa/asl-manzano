cmake_minimum_required(VERSION 3.4)
project(manzano)

include(manzano.cmake)

set(k_manzano_VERSION_MAJOR 1)
set(k_manzano_VERSION_MINOR 0)

configure_file("${DIR_manzano}/inc/manzano_cmake_config.h.in"
               "${DIR_manzano}/inc/manzano_cmake_config.h")

message("\n###### manzano #######")

add_library(manzano STATIC ${SOURCES_manzano})

install(TARGETS manzano
        DESTINATION /usr/local/lib/)

# ---------------------------------------------------------------- #
message("\n ***** Include directories: ***** ")

get_property(manzano_include_dirs
             TARGET manzano
             PROPERTY INCLUDE_DIRECTORIES)

foreach(dir ${manzano_include_dirs})

    message(STATUS "dir='${dir}'")
    file(GLOB manzano_include_files ${dir}/*.h)
    message("......................")
    message(${manzano_include_files})
    install(FILES ${manzano_include_files}
            DESTINATION /usr/local/include/manzano)

endforeach()

message("\n ******************************** ")

