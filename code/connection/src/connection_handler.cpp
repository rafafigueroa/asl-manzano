// Manzano software

#include "connection_handler.h"

namespace mzn {

// -------------------------------------------------------------------------- //
std::chrono::seconds constexpr ConnectionHandler::timeout_duration;

// -------------------------------------------------------------------------- //
void ConnectionHandler::connect() {


    std::lock_guard<std::mutex> lk(uc_mutex_);

    if (connected_ == false) {

        uc.setup_socket(ConnectionHandler::timeout_duration,
                        ip_remote,
                        port_remote);

        connected_ = true;
    }
}

} // << mzn
