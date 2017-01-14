#ifndef _MZN_SYSTEM_CALLS_H_
#define _MZN_SYSTEM_CALLS_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include "mzn_cmake_config.h"
#include "mzn_except.h"

namespace mzn {

// -------------------------------------------------------------------------- //
inline
std::string get_environmental_variable(std::string const & v) {

    char const * home_path = std::getenv( v.c_str() );

    if (home_path == nullptr) {
        auto const error_msg = std::string("No environmental variable: ") + v;
        throw std::runtime_error(error_msg);
    }

    return std::string(home_path);
}

// -------------------------------------------------------------------------- //
inline
std::string get_runtime_config_path() {

    auto const home_path = get_environmental_variable("HOME");

    auto const config_home_path = home_path + std::string("/.config/manzano");
    std::ifstream fs_home_check(config_home_path + "/config.json");
    if ( fs_home_check.good() ) return std::string(config_home_path);

    std::string const config_src_path{k_mzn_runtime_config_DIR};
    std::ifstream fs_src_check(config_src_path + "/config.json");
    if ( fs_src_check.good() ) return std::string(config_src_path);

    std::stringstream ss;
    ss << "Config path not found at: \n    "
       << config_home_path << " or " << config_src_path;

    throw FatalException( "Manzano",
                          "get_runtime_config_path",
                          ss.str() );
}

} // <- mzn

#endif
