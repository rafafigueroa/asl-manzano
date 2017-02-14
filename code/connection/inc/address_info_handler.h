// ** UdpConnection Object ** //
// Manzano software

#ifndef _MZN_ADDRESS_INFO_HANDLERH
#define _MZN_ADDRESS_INFO_HANDLERH

// Unix sockets imports
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Utility
#include <cstring>

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <stdexcept>

namespace mzn {

//! redefinitions of c structs for consistency

//! in_addr
// -------------------------------------------------------------------------- //
struct Addr {
    uint32_t s_addr = 0;
};

//! sockaddr_in
// -------------------------------------------------------------------------- //
struct SockAddr {
    int16_t  sin_family = 0;
    uint16_t sin_port = 0;
    Addr     sin_addr;
    char     sin_zero[8] = {0};
};

//! timeval, with std::chrono::duration converting constructor for timeout
//! the types (time_t, long int) are for compatibility with C sockets
// -------------------------------------------------------------------------- //
struct TimeoutDuration {

    TimeoutDuration() = default;
    ~TimeoutDuration() = default;

    using Seconds = std::chrono::seconds;
    using MicroSeconds = std::chrono::microseconds;

    // converting constructor
    template <typename Rep, typename Period>
    TimeoutDuration(std::chrono::duration<Rep, Period> const timeout) {

        // truncates towards zero. i.e. 3.4s to 3s
        auto const timeout_seconds =
            std::chrono::duration_cast<Seconds>(timeout);
        // what is left of the rounding
        auto const timeout_remainder = timeout - timeout_seconds;
        tv_sec = Seconds(timeout_seconds).count();
        tv_usec = MicroSeconds(timeout_remainder).count();
    }

    time_t tv_sec = 0;
    long int tv_usec = 0;
};

// -------------------------------------------------------------------------- //
inline
bool operator<(TimeoutDuration const & lhs, TimeoutDuration const & rhs) {

    auto const lhs_duration = TimeoutDuration::Seconds(lhs.tv_sec) +
                              TimeoutDuration::MicroSeconds(lhs.tv_usec);

    auto const rhs_duration = TimeoutDuration::Seconds(rhs.tv_sec) +
                              TimeoutDuration::MicroSeconds(rhs.tv_usec);

    return lhs_duration < rhs_duration;
}

// -------------------------------------------------------------------------- //
inline
bool operator> (TimeoutDuration const & lhs, TimeoutDuration const & rhs) {
    return rhs < lhs;
}

// -------------------------------------------------------------------------- //
inline
bool operator<=(TimeoutDuration const & lhs, TimeoutDuration const & rhs) {
    return not (lhs > rhs);
}

// -------------------------------------------------------------------------- //
inline
bool operator>=(TimeoutDuration const & lhs, TimeoutDuration const & rhs) {
    return not (lhs < rhs);
}

//! Handles the address info (addrinfo) resource
// -------------------------------------------------------------------------- //
/*! addrinfo (from linux man pages)
    holds internet address info
    struct addrinfo {
           int               ai_flags;
           int               ai_family;
           int               ai_socktype;
           int               ai_protocol;
           socklen_t         ai_addrlen;
           struct sockaddr * ai_addr;
           char            * ai_canonname;
           struct addrinfo * ai_next;
    };

    @author rfigueroa@usgs.gov
*/
class AddressInfoHandler {

    // the function getaddrinfo allocates memory for ai
    // the memory gets freed after use in setup_socket
    bool debug_ = false;

public:

    //! access to internal structure is needed for interfacing
    //! with linux sockets with the c functions
    addrinfo * ai = nullptr;

    //! must call freeaddrinfo after using the ai from this function
    AddressInfoHandler(int const port,
                       std::string const & ip = "",
                       bool const tcp = false);

    //! the "if" should not be necessary
    ~AddressInfoHandler() {if (ai != nullptr) freeaddrinfo(ai);};
};

} // end mzn
#endif // _MZN_ADDRESS_INFO_HANDLERH_
