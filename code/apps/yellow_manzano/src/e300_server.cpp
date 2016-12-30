#include "e300_server.h"

namespace mzn {

// -------------------------------------------------------------------------- //
std::ostream & bold(std::ostream & os) {
    return os << "\e[1m";
}

// -------------------------------------------------------------------------- //
std::ostream & nobold(std::ostream & os) {
    return os << "\e[0m";
}

// -------------------------------------------------------------------------- //
void E300Server::run() {

    // recv/send to another manzano program local or remote
    // no timeout
    uc.setup_socket();
    // send/recv to the e300 using a serial connection
    sc.setup_connection();

    using Message = std::string;
    auto constexpr ret_size = 548;

    try {

        while (true) {

            Message uc_msg_recv;
            uc_msg_recv.resize(ret_size);

            std::cout << std::endl << "** listening at: "
                      << Time::sys_year_month_day() << " "
                      << Time::sys_time_of_day() << " **\n";

            uc.recv(uc_msg_recv);

            std::cout << std::endl << "<" << uc_msg_recv << ">\n";

            Message sc_msg_send = uc_msg_recv + std::string("\r");

            Message sc_msg_recv = "";

            sc.send_recv(sc_msg_send, sc_msg_recv);

            std::cout << std::endl << "[" << sc_msg_recv << "]\n";

            // msgs are sent to the last ip/port from were msgs were received
            Message uc_msg_send = sc_msg_recv;

            uc.send(uc_msg_send);
        }

    } catch(std::exception const & e) {
        std::cerr << std::endl << "caught error @E300Server::run\n";
        throw WarningException("E300Server", "run", e.what() );
    }
}

} // <- mzn
