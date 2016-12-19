// Manzano software

#include "udp_connection.h"

namespace mzn {

//! move to the type of message
// -------------------------------------------------------------------------- //
/*
template<typename M>
void UdpConnection::msg_resize(M & msg) {

    // cut message to size indicated in the qdp header
    // Important for variable length msgs
    // or unexpected msgs

    uint16_t const msg_length_high_byte = msg[6];
    uint16_t const msg_length_low_byte = msg[7];

    uint16_t ret_cmd_data_size = msg_length_high_byte;
    ret_cmd_data_size = (ret_cmd_data_size << 8 ) | msg_length_low_byte;

    uint16_t constexpr qdp_begin_cmd_data_ = 12;
    msg.resize(qdp_begin_cmd_data_ + ret_cmd_data_size);
}
*/

// -------------------------------------------------------------------------- //
void UdpConnection::setup_socket(TimeoutDuration const & timeout,
                                 std::string const & ip_remote,
                                 int port_remote) {
    try {

        auto aih_host = AddressInfoHandler(port_host_);

        // --------- Socket --------- //
        // skfd_ = socket file descriptor
        skfd_ = socket(aih_host.ai->ai_family,
                       aih_host.ai->ai_socktype,
                       aih_host.ai->ai_protocol);

        if (skfd_ < 0) {
            std::cerr << "\nsocket error: " << std::strerror(errno);
            throw std::runtime_error("socket error");
        }

        if (timeout.tv_sec != 0 or timeout.tv_usec != 0) {
            // if there is a timeout, set it in the socket options
            if ( setsockopt( skfd_,
                             SOL_SOCKET,
                             SO_RCVTIMEO,
                             &timeout,
                             sizeof(timeout) ) < 0 ) {

                std::cerr << "\nsocket setup options error";
                throw std::runtime_error("socket options error");
            }
        }

        // --------- Bind --------- //
        int bind_result = bind(skfd_,
                               aih_host.ai->ai_addr,
                               aih_host.ai->ai_addrlen);

        if (bind_result < 0) {
            throw std::runtime_error( std::strerror(errno) );
        }

    } catch (std::runtime_error const & e) {
        // connection was not successful
        std::cerr << std::endl << e.what() << std::endl;
        throw WarningException("UdpConnection",
                               "setup_socket",
                               "Host Socket Setup Failed");
    }

    // connection was successful
    try {

        if (port_remote != 0) {
            // use port_remote, ip_remote to setup sin_remote_
            auto aih_remote = AddressInfoHandler(port_remote, ip_remote);
            // copy the relevant bits to the class member
            // before ai_remote gets freed
            sin_remote_ = *(SockAddr *)(aih_remote.ai->ai_addr);

        } else {
            // update sin_remote_ with the remote of last received message
            should_update_sin_remote_ = true;
        }

    } catch (std::runtime_error const & e) {
        std::cerr << std::endl << e.what() << std::endl;
        throw WarningException("UdpConnection",
                               "setup_socket",
                               "Remote Socket Setup Failed");
    }
}

// -------------------------------------------------------------------------- //
void UdpConnection::close_socket() {
    // c function
    close(skfd_);
}

// -------------------------------------------------------------------------- //
void UdpConnection::shutdown_socket() {
    // another c networking function
    shutdown(skfd_, SHUT_WR);
}

//! explicitaly instatiated template
// -------------------------------------------------------------------------- //
//template class UdpConnection<Qdp::Message>;
//template class UdpConnection<std::string>;
} // << mzn
