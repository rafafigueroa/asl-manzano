// Manzano software

#include "connection_handler.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void ConnectionHandler::connect() {

    auto constexpr timeout_duration = std::chrono::seconds(2);

    std::lock_guard<std::mutex> lk(uc_mutex_);

    if (connected_ == false) {
        uc.setup_socket(timeout_duration, ip_remote, port_remote);
        connected_ = true;
    }
}

} // << mzn
