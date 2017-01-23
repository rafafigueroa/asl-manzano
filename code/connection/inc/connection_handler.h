// ** ConnectionHandler Object ** //
// Manzano software

#ifndef _MZN_CONNECTION_HANDLER_H
#define _MZN_CONNECTION_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>

#include "udp_connection.h"
#include "mzn_time.h"

namespace mzn {

//! Holds configuration and status of remote_port
/*!
    q : short for digitizer of q330 family

    In addition, has a unique udp connection
    and remotes
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class ConnectionHandler {

public:

    // ---------------------------------------------------------------------- //
    std::string const ip_remote;
    int const port_remote = 0;
    uint64_t const auth_code = 0;
    int const protocol_version = 0;

    // ---------------------------------------------------------------------- //
    ConnectionHandler(std::string const ip_remote,
               int const port_remote,
               uint64_t const auth_code,
               int const port_host,
               int const protocol_version = 0) :

            ip_remote(ip_remote),
            port_remote(port_remote),
            auth_code(auth_code),
            protocol_version(protocol_version),
            uc(port_host) {}

    ~ConnectionHandler() = default;

    //! use the default constructed mutex on each one, no move for mutex
    // ---------------------------------------------------------------------- //
    ConnectionHandler(ConnectionHandler && rhs) noexcept :
            ip_remote(rhs.ip_remote),
            port_remote(rhs.port_remote),
            auth_code(rhs.auth_code),
            protocol_version(rhs.protocol_version),
            uc( std::move(rhs.uc) ) {}

    //! udp connection
    UdpConnection uc;

    //! authorization has been completed
    bool registered = false;
    //! connects when it needs to
    void connect();

    //! should debug messages be printed to cout?
    //! prints the bytes of the messages sent and received
    bool debug_ = false;

private:


    //! modified inside a lock guard in remote_port
    bool connected_{false};

    int sequence_number_ = 0;
    int acknowledge_number_ = 0;

    //! state and mutex for uc (UdpConnection)
    std::mutex uc_mutex_;

public:

    //! connection timeout
    static
    std::chrono::seconds constexpr timeout_duration = std::chrono::seconds(2);

    int sequence_number() const {return sequence_number_; }
    int acknowledge_number() const {return acknowledge_number_;}

    void inc_sequence_number() {
        (sequence_number_ < 65536) ? (sequence_number_++) : (sequence_number_ = 1);
    }

    void set_sequence_number(int const sequence_number) {
        sequence_number_ = sequence_number;
    }

    void set_acknowledge_number(int const acknowledge_number) {
        acknowledge_number_ = acknowledge_number;
    }
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          ConnectionHandler const & ch) {
    //TODO
    return os;
}

} // end mzn
#endif // _MZN_CONNECTION_HANDLER_H_
