// Manzano software

#include "address_info_handler.h"

namespace mzn {

// -------------------------------------------------------------------------- //
AddressInfoHandler::AddressInfoHandler(int const port,
                                       std::string const & ip,
                                       bool const tcp) {

    // setups ai or throws
    // getaddrinfo allocates memory for ai,
    // gets freed at destructor

    // holds ai_hints
    struct addrinfo ai_hints;

    // agnostic to ip4 or ip6
    ai_hints = {}; //set struct values to 0
    ai_hints.ai_family = AF_UNSPEC;

    if (tcp) {
        // TCP
        ai_hints.ai_socktype = SOCK_STREAM;
    } else {
        // UDP
        ai_hints.ai_socktype = SOCK_DGRAM;
    }

    auto port_str = std::to_string(port);

    // ------------------------
    int gai_result = 0;

    if ( ip.empty() ) {

        ai_hints.ai_flags = AI_PASSIVE;
        gai_result = getaddrinfo(0,
                                 port_str.c_str(),
                                 &ai_hints,
                                 &ai);

    } else {
        // remote
        gai_result = getaddrinfo(ip.c_str(),
                                 port_str.c_str(),
                                 &ai_hints,
                                 &ai);
    }

    // Error handling
    if (gai_result != 0) {
        std::cerr << std::endl;
        std::cerr << "digitizer get address info error: "
                  << gai_strerror(gai_result);

        throw std::runtime_error{"get address info error"};
    }

    if (ai == nullptr) throw std::runtime_error{"null address info"};

    if (debug_) {

        std::cout << "\n--------------------------------------"
                  << "\nai family: "         << ai->ai_family
                  << "\nai socktype: "       << ai->ai_socktype
                  << "\nai protocol: "       << ai->ai_protocol
                  << "\nai address length: " << ai->ai_addrlen;

        auto socket_info = (SockAddr *)(ai->ai_addr);

        std::cout << std::hex
                  << "\nai address: " << socket_info->sin_addr.s_addr
                  << "\nai port: " << socket_info->sin_port
                  << std::dec;
    }
}

} // << mzn
