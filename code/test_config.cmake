cmake_minimum_required(VERSION 3.0)
project(mzn_test_config)

# CMake include guard
if(mzn_test_config_included)
  return()
endif(mzn_test_config_included)
set(mzn_test_config_included true)

enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# This effectively redefines the location of the configuration file
# instead of using a configuration of a real digitizer, it points to a
# directory that has configuration information for a dummy digitizer
# basically a port on the same computer where the test is run
# a test then can use a udp server listening to this port to simulate
# a digitizer
set(k_mzn_runtime_config_DIR "${DIR_MZN}/../test_config")
configure_file("${DIR_MZN}/mzn_cmake_config.h.in"
               "${DIR_MZN}/mzn_cmake_config.h")
message("\n###### config(google_test) #######")
