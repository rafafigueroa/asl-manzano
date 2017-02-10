#ifndef _MZN_SYSTEM_CALLS_H_
#define _MZN_SYSTEM_CALLS_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include <sys/ioctl.h>
// posix function
#include "poll.h"
#include <unistd.h>
#include "mzn_cmake_config.h"
#include "mzn_except.h"

namespace mzn {

namespace Utility {

// -------------------------------------------------------------------------- //
inline
std::string get_environmental_variable(std::string const & v) {

    char const * home_path = std::getenv( v.c_str() );

    if (home_path == nullptr) {

        auto const error_msg = std::string("No environmental variable: ") + v;

        throw FatalException( "Manzano",
                              "get_environmental_variable",
                              error_msg );
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
       << config_home_path << " or\n    " << config_src_path;

    throw FatalException( "Manzano",
                          "get_runtime_config_path",
                          ss.str() );
}

// -------------------------------------------------------------------------- //
inline
int get_terminal_cols() {
    winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
}

// -------------------------------------------------------------------------- //
template <typename Rep, typename Period>
inline
bool cin_cancel(std::chrono::duration<Rep, Period> timeout) {

    // poll file descriptor, can't be constant due to poll() signature
    pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;
    pfd.revents = 0;

    auto constexpr number_of_pfd = 1;

    auto const timeout_ms_count = std::chrono::milliseconds(timeout).count();

    // Poll one descriptor with a five second timeout
    auto const poll_result = poll(&pfd, number_of_pfd, timeout_ms_count);


    if (poll_result == 0) {
        // Timeout
        return false;

    } else if (poll_result > 0) {

        // enter to cancel, if needed cin could be read here
        // not interested on the response, just the user hitting enter
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        return true;

    } else {

        std::stringstream ss;
        ss << "Error from polling: " << strerror(errno) << std::endl;
        std::cin.clear();
        throw WarningException("Manzano", "cin_cancel", ss.str() );
    }
}

} // <- Utility
} // <- mzn

#endif
