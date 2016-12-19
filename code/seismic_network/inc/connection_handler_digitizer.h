// ** ConnectionHandlerDigitizer Object ** //
// Manzano software

#ifndef _MZN_CONNECTION_HANDLER_DIGITIZER_H
#define _MZN_CONNECTION_HANDLER_DIGITIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>

#include "connection_handler.h"

namespace mzn {

//! Holds configuration and status of q_port
/*!
    handles the basic errors when communicating with the digitizer
    manages the creation of the qpd header, including the crc calculation
    handles timed_send messages which the autocal_scheduler uses
    called md or md_ in the classes that use it

    q : short for digitizer of q330 family

    In addition, has a unique udp connection
    and the sensor remotes
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
    /*
class ConnectionHandlerDigitizer : public ConnectionHandler {

public:
    ConnectionHandlerDigitizer(std::string const ip_remote,
          int const port_remote,
          unsigned long const auth_code,
          int const port_host,
          unsigned long const serial_number) :

            ConnectionHandler(ip_remote,
                       port_remote,
                       auth_code,
                       port_host),

            serial_number(serial_number) {}

    ~ConnectionHandlerDigitizer() = default;

    //! use the default constructed mutex on each one, no move for mutex
    // ---------------------------------------------------------------------- //
    ConnectionHandlerDigitizer(ConnectionHandlerDigitizer && rhs) noexcept :
            ConnectionHandler( rhs.ip_remote,
                        rhs.port_remote,
                        rhs.auth_code,
                        rhs.uc.port_host() ),

            serial_number(rhs.serial_number) {}

    // ---------------------------------------------------------------------- //
    //! copy from digitizer in constructor
    unsigned long const serial_number;
};

*/
} // end mzn
#endif // _MZN_CONNECTION_HANDLER_DIGITIZER_H_
