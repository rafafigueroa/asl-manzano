#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "udp_connection.h"

int main(int argc, char **argv) {

    using Message = std::string;
    Message msg_send;
    Message msg_recv;

    // CLIENT (laptop)
    std::string const ip_remote = "136.177.121.80";
    auto constexpr port_remote = 4500;
    auto constexpr port_host = 4410;

    mzn::UdpConnection uc(port_host);

    mzn::TimeoutDuration timeout;
    timeout.tv_usec = 100'000;

    uc.setup_socket(timeout, ip_remote, port_remote);

    // --------------------------------------------------------------------- //
    msg_send = "\n---- hello from client ----\n";

    msg_recv.resize(30);
    uc.send_recv(msg_send, msg_recv);

    // --------------------------------------------------------------------- //
    uc.close_socket();

    return 0;
}


