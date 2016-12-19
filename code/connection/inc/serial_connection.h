// ** SerialConnection Object ** //
// Manzano software

#ifndef _MZN_SERIAL_CONNECTION_H
#define _MZN_SERIAL_CONNECTION_H

// Utility
#include <cstring>
#include <chrono>
#include <memory>
#include <thread>

#include "message.h"
#include "mzn_except.h"
#include "serial.h"

namespace mzn {

//! Takes a msg and send/recv using an serial connection
/*!

    @tparam M message data countainer type.
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class SerialConnection {

public:

    using Serial = serial::Serial;
    using Timeout = serial::Timeout;

    //! timeout_ always in milliseconds, but timeout can be in any rep/period
    //! that does not truncate
    template <typename Rep, typename Period>
    SerialConnection(std::string const port,
                     int const baud_rate,
                     std::chrono::duration<Rep, Period> const timeout) :
            port_(port),
            baud_rate_(baud_rate),
            timeout_(timeout),
            serial_("",
                    baud_rate_,
                    Timeout::simpleTimeout( timeout_.count() ) ) {}

    ~SerialConnection() = default;

    //! called separetely from object creation, requires admin priviliges
    void setup_connection();

    //! calls low level close() and shutdown() on the connection
    void close_connection();
    void shutdown_connection();

    //! utility to print array/vector type objects, if using std::string
    //! it would be better to just use their operator<<
    template< typename M = std::vector<uint8_t> >
    static
    void print_msg(M const & msg);

    //! @throws InfoException
    template< typename M = std::vector<uint8_t> >
    void send(M const & msg);

    //! @throws InfoException
    //! calls send and then recv
    template< typename SendType = std::vector<uint8_t> ,
              typename RecvType = std::vector<uint8_t> >
    void send_recv(SendType const & msg_send, RecvType & msg_recv);

    //! @throws InfoException
    template< typename M = std::vector<uint8_t> >
    void recv(M & msg_recv);

    std::string port() const {return port_;}

    void enumerate_ports() const;

private:

    //! information about connection
    std::string const port_;
    int const baud_rate_;
    std::chrono::milliseconds timeout_;

    //!
    Serial serial_;

    //! print debug messages
    bool debug_ = false;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<< (std::ostream & sc_os,
                           SerialConnection const & sc) {

    sc_os << "sc[" << sc.port() << "]";
    return sc_os;
};

// -------------------------------------------------------------------------- //
template<typename M>
void SerialConnection::print_msg(M const & msg) {
    std::cout << std::uppercase <<std::showbase << std::hex << "\n{";
    for (auto const & b : msg) std::cout << static_cast<const int>(b) << ", ";
    std::cout << "}\n" << std::noshowbase << std::uppercase << std::dec;
}

// -------------------------------------------------------------------------- //
template<typename SendType, typename RecvType>
void SerialConnection::send_recv(SendType const & msg_send,
                                 RecvType & msg_recv) {

    serial_.flushInput();
    serial_.flush();

    send(msg_send);

    serial_.flushInput();
    serial_.flush();

    std::this_thread::sleep_for( std::chrono::milliseconds(100) );

    recv(msg_recv);
}

// -------------------------------------------------------------------------- //
template<typename M>
void SerialConnection::send(M const & msg) {

    // send message
    if (debug_) std::cout << "\nsending message\n";

    // try can throw SerialException, derived from std::exception
    auto send_result = serial_.write(msg);

    // Success!
    if (debug_) {
        std::cout << "\nsent bytes: " << send_result << "\n";
        print_msg(msg);
    }
}

// -------------------------------------------------------------------------- //
template<typename M>
void SerialConnection::recv(M & msg_recv) {

    msg_recv = serial_.read(1000);
    // if (debug_) { }
}

} // end mzn
#endif // _MZN_SERIAL_CONNECTION_H_
