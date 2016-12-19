// Manzano software
#ifndef _MZN_DUMMY_Q_PORT_H
#define _MZN_DUMMY_Q_PORT_H

#include <future>

#include "udp_connection.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class DummyQPort {

    void server();

    std::future<void> server_future_;

    int const port_host_;
    std::chrono::seconds const connection_timeout_{3};

    std::vector<uint8_t> const cack_msg_{0XEB, 0X8A, 0X1A, 0X80,
                                         0XA0, 0X02, 0X00, 0X00,
                                         0X00, 0X00, 0X00, 0X03};

public:
    template<typename Rep, typename Period>
    DummyQPort(int const port_host,
               std::chrono::duration<Rep, Period> const timeout);

    ~DummyQPort();
};

// -------------------------------------------------------------------------- //
template<typename Rep, typename Period>
DummyQPort::DummyQPort(int const port_host,
                       std::chrono::duration<Rep, Period> const timeout) :
        port_host_(port_host),
        connection_timeout_(timeout) {

    server_future_ = std::async(std::launch::async, &DummyQPort::server, this);
}

} // <- mzn

#endif // _MZN_DUMMY_Q_PORT_H_
