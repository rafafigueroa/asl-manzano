// Manzano software
#ifndef _MZN_DIGITIZER_H
#define _MZN_DIGITIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>

#include "sensor.h"
#include "mzn_except.h"

namespace mzn {

//! Holds configuration and status of digitizer
/*!
    In addition, has a unique udp connection
    and the sensor targets
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Digitizer {

private:
    struct Config {

        friend class Digitizer;

        Config(uint64_t const serial_number,
               std::string const model = "Q330") :

            serial_number(serial_number),
            model(model) {
        }

        ~Config() = default;

        uint64_t const serial_number;
        unsigned int kmi_tag = 0;
        std::string const model;
    };

    struct Status {
    };

public:
    explicit
    Digitizer(uint64_t const serial_number,
              ConnectionHandler port_config,
              ConnectionHandler port_control,
              ConnectionHandler port_data) :

    port_config( std::move(port_config) ),
    port_control( std::move(port_control) ),
    port_data( std::move(port_data) ),

    config(serial_number),
    status() {}

    ~Digitizer() = default;

public:
    //! digitizer ports, each one with an udp connection
    ConnectionHandler port_config;
    ConnectionHandler port_control;
    ConnectionHandler port_data;

    //! sensor targets
    std::vector<Sensor> s;
    //! configuration set at construction
    Config const config;
    //! current status
    Status status;

    // move constructor
    // ---------------------------------------------------------------------- //
    Digitizer(Digitizer && rhs) noexcept :
            port_config(  std::move(rhs.port_config) ),
            port_control( std::move(rhs.port_control) ),
            port_data(    std::move(rhs.port_data) ),
            s( std::move(rhs.s) ),
            config(rhs.config),
            status(rhs.status) {}

    friend
    std::ostream & operator<<(std::ostream & os, Config const & c);

    friend
    std::ostream & operator<<(std::ostream & os, Status const & c);
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & q_os, Digitizer const & q) {

    q_os << "digitizer("
         <<  std::hex << std::uppercase
         << q.config.serial_number
         <<  std::dec << std::nouppercase
         << ")";
    return q_os;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          Digitizer::Config const & c) {

    os << "\n    " << "serial  : " << c.serial_number
       << "\n    " << "model   : " << c.model;
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          Digitizer::Status const & s) {
    return os;
}

} // end mzn
#endif // _MZN_DIGITIZER_H_
