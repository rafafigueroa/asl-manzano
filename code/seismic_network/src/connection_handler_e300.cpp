// Manzano software
#include "connection_handler_e300.h"

namespace mzn {

// -------------------------------------------------------------------------- //
std::string ConnectionHandlerE300::send_recv(std::string const msg_send) {

    std::cout << std::endl << "{" << msg_send << "}\n";
    std::string msg_recv = "";
    msg_recv.resize(1000);
    uc.send_recv(msg_send, msg_recv);

    return msg_recv;
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::check(std::string const & msg,
                     std::string const expected) const {

    if (msg.find(expected) == std::string::npos) {
        throw WarningException("ConnectionHandlerE300",
                               "check",
                               std::string("<") +
                               expected +
                               std::string("> not found") );
    }

    std::cout << "\ne300 <" << expected << "> found\n";
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_alive() {
    auto const msg_recv = send_recv("status");
    check(msg_recv, "Menu:");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_status_cal() {
    auto const msg_recv = send_recv("status");
    check(msg_recv, "Menu:CALIBRATE");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_status_ext_cal() {
    auto const msg_recv = send_recv("status");
    check(msg_recv, "Connected Directly");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_main() {
    auto const msg_recv = send_recv("return");
    check(msg_recv, "Menu:MAIN");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_enable() {

    std::stringstream ss;
    ss << std::hex << auth_code;
    std::string enable = std::string("enable") + ss.str();
    auto const msg_recv = send_recv(enable);
    check(msg_recv, "System Control Enabled");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_calibrate() {
    auto const msg_recv = send_recv("calibrate");
    check(msg_recv, "Menu:CALIBRATE");
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cmd_ext_cal_connect() {
    auto msg_recv = send_recv("extcalconnect");

    std::this_thread::sleep_for( std::chrono::milliseconds(2000) );

    msg_recv = send_recv("status");
    check(msg_recv, "Connected Directly to");
    std::cout << "\n<" << msg_recv << ">\n";
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::cal_connect() {

    if (not registered) {
        throw WarningException(
            "ConnectionHandlerE300",
            "cal_conect",
            "attempt to calibrate sensor without registration");
    }

    cmd_calibrate();
    cmd_status_cal();
    cmd_ext_cal_connect();
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::reg() {

    std::lock_guard<std::mutex> registration_lk(registration_mutex_);

    if (not registered) {
        connect();
        cmd_main();
        cmd_enable();
        registered = true;
        std::cout << std::endl << " ** e300 reg! **\n";
    }
}

// -------------------------------------------------------------------------- //
std::chrono::seconds
ConnectionHandlerE300::cmd_status_for(
        std::chrono::seconds const keep_alive_duration,
        std::chrono::seconds const keep_alive_delay) {

    // delay this process, keep_alive_delay defaults to zero
    std::this_thread::sleep_for(keep_alive_delay);
    // now really lets keep this thing alive
    auto constexpr alive_wait = std::chrono::seconds( std::chrono::minutes(45) );
    // how many times should this thread sleep t he alive_wait duration
    int const delay_times = keep_alive_duration.count()/alive_wait.count() + 1;

    std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++";
    std::cout << "\nkeep_alive_duration: " << keep_alive_duration << "\n";
    std::cout << "\nalive_wait: " << alive_wait << "\n";
    std::cout << "\ndelay times: " << delay_times << "\n";

    for (int i = 0; i < delay_times; i++) {
        std::this_thread::sleep_for(alive_wait);
        std::cout << std::endl << "keeping e300 alive\n";
        auto msg_recv = send_recv("status");
    }

    auto const alive_duration = keep_alive_duration * delay_times;

    return alive_duration;
}

// -------------------------------------------------------------------------- //
void ConnectionHandlerE300::keep_alive(std::chrono::seconds const keep_alive_duration,
                          std::chrono::seconds const keep_alive_delay) {

    keep_alive_ftr = std::async(std::launch::async,
                                &ConnectionHandlerE300::cmd_status_for,
                                this,
                                keep_alive_duration,
                                keep_alive_delay);
}

} // << mzn
