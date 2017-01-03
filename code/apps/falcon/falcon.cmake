cmake_minimum_required(VERSION 3.0)
project(falcon_config)

# CMake include guard
if(falcon_config_included)
  return()
endif(falcon_config_included)
set(falcon_config_included true)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../mzn_config.cmake)
include(${DIR_MZN}/commands/commands.cmake)

message("\n###### config(falcon) #######")

set(DIR_falcon ${DIR_MZN}/apps/falcon)
include_directories(${DIR_falcon}/inc)

set(SOURCES_falcon ${SOURCES_falcon_cmds}
                   ${DIR_falcon}/src/csv_file.cpp
                   ${DIR_falcon}/src/falcon_data_packet.cpp
                   ${DIR_falcon}/src/falcon.cpp)

# sets the projects with full paths, in my computer:
# dir_website_data=/home/rafa/asl-manzano/code/apps/falcon/website_data
# dir_falcon_config=/home/rafa/asl-manzano/code/../runtime_config
set(DIR_website_data "${DIR_falcon}/website_data")
set(DIR_falcon_config "${DIR_MZN}/../runtime_config")

# copies file x.in to file x, creates file x if is not there
# replacing @VAR@ with the value of VAR set here
configure_file("${DIR_falcon}/falcon_dirs.conf.in"
               "${DIR_falcon}/falcon_dirs.conf")
