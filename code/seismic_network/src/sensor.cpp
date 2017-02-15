#include "sensor.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void Sensor::stream_config(std::ostream & os) const {

    os << "\n    " << "input    : " << config.input
       << "\n    " << "model    : " << config.model
       << std::boolalpha
       << "\n    " << "has_e300 : " << config.has_e300
       << std::noboolalpha;

    if (config.has_e300) os << port_e300_const_ref();
}

// -------------------------------------------------------------------------- //
void Sensor::stream_status(std::ostream & os) const {

    if (config.has_e300) {
        os << "E300:";
        os << std::boolalpha
           << "\n    " << "connected  : " << port_e300_const_ref().connected()
           << "\n    " << "registered : " << port_e300_const_ref().registered;
    }
}

} // <- mzn
