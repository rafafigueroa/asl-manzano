// Manzano software
#ifndef MZN_E300_SERVER_H_
#define MZN_E300_SERVER_H_

#include <string>
#include <iostream>
#include <vector>

#include "serial_connection.h"
#include "udp_connection.h"
#include "mzn_time.h"

namespace mzn {

//! Starts a listening udp server and serial connection
/*!
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class E300Server {

public:

    explicit
    E300Server() : sc( "/dev/ttyS1", 9600, std::chrono::milliseconds(250) ),
                   uc(4300) {}

    ~E300Server() = default;

    SerialConnection sc;
    UdpConnection uc;

    void run();

};

} // <- mzn

#endif
