cmake_minimum_required(VERSION 3.0)
project(core_config)

# CMake include guard
if(core_config_included)
  return()
endif(core_config_included)
set(core_config_included true)

include(${CMAKE_CURRENT_LIST_DIR}/../mzn_config.cmake)
include(${DIR_MZN}/commands/commands.cmake)
include(${DIR_MZN}/seismic_network/seismic_network.cmake)

message("\n###### config(core) #######")

set(DIR_CORE ${DIR_MZN}/core)

include_directories(${DIR_CORE}/inc/)

set(SOURCES_core ${SOURCES_md5}
                 ${SOURCES_commands}
                 ${SOURCES_seismic_network}
                 ${DIR_CORE}/src/cmd_file_reader.cpp
                 ${DIR_CORE}/src/comm.cpp
                 ${DIR_CORE}/src/stream_output.cpp
                 ${DIR_CORE}/src/stream_plotter.cpp
                 ${DIR_CORE}/src/ui_to_cmd_type.cpp
                 ${DIR_CORE}/src/input_store.cpp
                 ${DIR_CORE}/src/output_store.cpp
                 ${DIR_CORE}/src/instruction_interpreter.cpp
                 ${DIR_CORE}/src/msg_task.cpp
                 ${DIR_CORE}/src/msg_task_manager.cpp
                 ${DIR_CORE}/src/message_dispatch.cpp
                 )
