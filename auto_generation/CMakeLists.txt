cmake_minimum_required(VERSION 3.0)
project(mzn_autogeneration)

# ----- COMPILER FLAGS ----- #
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ----- MAIN DIR ----- #
set(DIR_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/../)

# ----- RAPIDJSON ----- #
include_directories(${DIR_MAIN}/code/libs/json)

# ----- AUTOGENERATION ----- #
set(DIR_AG ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${DIR_AG}/inc)

set(SOURCES_AG_CMDS ${DIR_AG}/src/main_create_cmds.cpp)
add_executable(create_cmds ${SOURCES_AG_CMDS})
set(SOURCES_AG_BMS ${DIR_AG}/src/main_create_bms.cpp)
add_executable(create_bms ${SOURCES_AG_BMS})
set(SOURCES_AG_TESTING_BMS ${DIR_AG}/src/main_create_testing_bms.cpp)
add_executable(create_testing_bms ${SOURCES_AG_TESTING_BMS})
set(SOURCES_AG_TCRC ${DIR_AG}/src/main_create_tcrc.cpp)
add_executable(create_tcrc ${SOURCES_AG_TCRC})
set(SOURCES_AG_CALS ${DIR_AG}/src/main_create_cals.cpp)
add_executable(create_cals ${SOURCES_AG_CALS})

set_target_properties(create_cmds create_bms create_tcrc create_cals create_testing_bms
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${DIR_AG}/bin)

