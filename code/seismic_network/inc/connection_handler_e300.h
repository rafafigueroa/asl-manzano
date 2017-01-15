// ** ConnectionHandlerE300 Object ** //
// Manzano software

#ifndef _MZN_CONNECTION_HANDLER_E300_H
#define _MZN_CONNECTION_HANDLER_E300_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <map>
#include <thread>
#include <future>

#include "connection_handler.h"

namespace mzn {

//! Holds configuration and alive of e300 port
/*!
    In addition, has a unique udp connection
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class ConnectionHandlerE300 : public ConnectionHandler {

public:
    ConnectionHandlerE300(std::string const ip_remote,
                          int const port_remote,
                          unsigned long const auth_code,
                          int const port_host) :

            ConnectionHandler(ip_remote,
                              port_remote,
                              auth_code,
                              port_host),

            cancel_keep_alive_(false) {}

    ~ConnectionHandlerE300() {

        try {
            wait_keep_alive();
        } catch(std::exception const & e) {
            std::cerr << e.what();
        }

    }

    //! TODO: use chap
    void check(std::string const & msg, std::string const expected) const;
    std::string send_recv(std::string const msg);

    //! send msgs to e300 server running on dp
    void cmd_alive();
    void cmd_status_cal();
    void cmd_status_ext_cal();
    void cmd_main();
    void cmd_enable();
    void cmd_calibrate();
    void cmd_ext_cal_connect();

    void reg();
    void cal_connect();

    using Seconds = Time::Seconds<>;

    std::future<Seconds> keep_alive_ftr;

    //! the delay allow for returning (cancelling) before first sleep period
    void keep_alive(Seconds const keep_alive_duration,
                    Seconds const keep_alive_delay = Seconds(0) );

    //! waits for future to end (if valid)
    void wait_keep_alive();

    //! checked on keep alive before every sleep period
    std::atomic<bool> cancel_keep_alive_;

    //! sets cancel flag and waits
    void cancel_keep_alive();

    //! called as an async thread from keep_alive
    std::chrono::seconds
    cmd_status_for(Seconds const keep_alive_duration,
                   Seconds const keep_alive_delay);

    ConnectionHandlerE300(ConnectionHandlerE300 && rhs) noexcept :
            ConnectionHandler( std::move(rhs) ),
            keep_alive_ftr( std::move(rhs.keep_alive_ftr) ) {}

private:

    std::mutex registration_mutex_;

};

} // end mzn
#endif // _MZN_CONNECTION_HANDLER_E300_H_
