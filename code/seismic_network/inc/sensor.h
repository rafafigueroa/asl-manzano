// ** Sensor Object ** //
// Manzano Software

#ifndef _MZN_SENSOR_H_
#define _MZN_SENSOR_H_

#include <string>
#include <vector>
#include <memory>

#include "connection_handler_e300.h"

namespace mzn {

//! Sensor representing a single device like the STS2 or electronics
//! and mechanical sensors package like e300/STS1
/*!
    Notes from E300/STS1 manual:
    Axes labeled ENZ by convention. E300 could have 3 vertical (Z) axes anyway.
    Communication with e300: async serial 9600 baud, 8 data bits, 1 stop bit, np
    A good msg to send to keep alive is "status"
    An external reset can be sent from the digitizer (p12)
    If the E300 does not receive a command in 3600 seconds (1 hour) it returns
    to SAFE mode. Where calibrations can not be performed. Single calibrations
    don't require a keep_alive msg (p17) except some calibrations take 4 hours.
    A 500
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Sensor {

public:
    enum class Input {a, b};

    struct Config {

        friend class Sensor;

        Config(Input const input,
               std::string const model,
               std::string const cals,
               bool const has_e300) :
            input(input),
            model(model),
            cals(cals),
            has_e300(has_e300) {}

        ~Config() = default;

        Input const input;
        std::string const model;
        std::string const cals;
        bool const has_e300;
    };

    struct Status {
        friend class Sensor;
        std::string last_cal = "";
    };

    // TODO: sensors without e300 should not have uc
    Config config;
    Status status;
    // called port for consistency with config_port, data_port names
    std::unique_ptr<ConnectionHandlerE300> port_e300_ptr_;

    //! internal access to e300 connection handler when there is one
    ConnectionHandlerE300 & port_e300_ref() {
        if (config.has_e300) {
            return *( port_e300_ptr_.get() );
        } else {
            throw std::logic_error{"Sensor::port_e300"};
        }
    }

    //! internal access to e300 connection handler when there is one
    //! TODO: test this
    ConnectionHandlerE300 const & port_e300_const_ref() const {
        if (config.has_e300) {
            return *( port_e300_ptr_.get() );
        } else {
            throw std::logic_error{"Sensor::port_e300"};
        }
    }

    Sensor(std::string const input,
           std::string const model,
           std::string const cals,
           std::unique_ptr<ConnectionHandlerE300> port_e300_ptr = nullptr) :

            config(string_to_sp(input),
                   model,
                   cals,
                   // has_e300?
                   (port_e300_ptr != nullptr) ) {

        if (config.has_e300) port_e300_ptr_ = std::move(port_e300_ptr);
    }

    ~Sensor() = default;

    // move constructor
    // ---------------------------------------------------------------------- //
    Sensor(Sensor && rhs) noexcept :
            config(rhs.config),
            status(rhs.status),
            port_e300_ptr_( std::move(rhs.port_e300_ptr_) ) {}

private:
    Input string_to_sp (std::string const & _input) {
        if (_input != "A" and _input != "B" ) {
            std::cerr << "\nSensor input not A or B";
        }
        return ( (_input == "A") ? Input::a : Input::b );
    }

friend std::ostream & operator<<(std::ostream & os,
                                 Config const & c);

friend std::ostream & operator<<(std::ostream & os,
                                 Status const & c);
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & sn_os,
                          Sensor::Input const & sp) {

    if (sp == Sensor::Input::a)      sn_os << "A";
    else if (sp == Sensor::Input::b) sn_os << "B";
    return sn_os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & sn_os,
                          Sensor const & s) {

    sn_os << "sensor(" << s.config.input
          << ", " << s.config.model
          <<  ")";
    return sn_os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          Sensor::Config const & config) {

    os << "\n    " << "input    : " << config.input
       << "\n    " << "model    : " << config.model
       << std::boolalpha
       << "\n    " << "has_e300 : " << config.has_e300
       << std::noboolalpha;

    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          Sensor::Status const & status) {

    os << "\n    " << "last_cal : " << status.last_cal;
    return os;
}


} // << mzn
#endif // _MZN_SENSOR_H_
