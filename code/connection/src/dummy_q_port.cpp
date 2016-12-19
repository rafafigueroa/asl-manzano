#include "dummy_q_port.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void DummyQPort::server() {

    using Message = std::string;

    auto constexpr ret_size = 548;

    // wait forever when receiving, no timeout
    mzn::UdpConnection uc(port_host_);

    try {

        uc.setup_socket(connection_timeout_);

        while (true) {

            Message msg_recv;
            msg_recv.resize(ret_size);

            std::cout << std::endl << "**** dummy q_port listening ****\n";

            uc.recv(msg_recv);

            if (msg_recv.find("quit") != std::string::npos) {
                std::cout << std::endl << "**** udpServerBye!" << std::endl;
                break;
            }

            // msgs are sent to the last ip/port from were msgs were received
            uc.send(cack_msg_);
        }

    } catch(WarningException const & e) {
        std::cerr << std::endl
                  << "!!! DummyQPort timed out, no \"quit\" message received\n";
        return;

    } catch(std::exception const & e) {
        std::cerr << "caught error @DummyQPort::server";
        throw e;
    }
}

// -------------------------------------------------------------------------- //
DummyQPort::~DummyQPort() {

    // catch everything
    try {
        auto const wait_duration = connection_timeout_
                                   + std::chrono::milliseconds(500);

        auto future_wait_result = server_future_.wait_for(wait_duration);

        if (future_wait_result == std::future_status::timeout) {

            std::cout << std::endl << "**** "
                      << "DummyQPort still listening, send \"quit\" to stop,\n"
                      << "or it will expire when nothing have been received"
                      << " for "
                      << std::chrono::seconds(connection_timeout_).count()
                      << "s ****\n";

            server_future_.wait();

        } else if (future_wait_result == std::future_status::ready) {
            std::cout << std::endl
                      << "Thanks for using the dummy q_port, bye!\n";
        }

    } catch (std::exception const & e) {

        std::cerr << std::endl << "Caught @DummyQPort destructor";
        std::cerr << std::endl << e.what();
        // destructors shall not throw
    }
}


} // <- mzn
