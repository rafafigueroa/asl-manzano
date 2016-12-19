#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <chrono>
#include <thread>

#include "udp_connection.h"

int main(int argc, char **argv) {

    using Message = std::string;
    Message msg_send;

    // SERVER (mxe)
    auto constexpr host_port = 4500;

    mzn::UdpConnection uc(host_port);

    uc.setup_socket();

    // ----------------------------------------------------------------- //
    while (true) {

        Message msg_recv;
        msg_recv.resize(50);
        uc.recv(msg_recv);

        // std::cout << std::endl << msg_recv << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds(30) );

        // std::cout << std::endl << "sending answer" << std::endl;

        msg_send = std::string("ms") + std::to_string(0);

        // std::cout << std::endl << "sending: " << msg_send;

        uc.send(msg_send);
    }

    uc.close_socket();

    return 0;
}


