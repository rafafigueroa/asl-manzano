#include "digitizer.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void Digitizer::stream_config(std::ostream & os) const {

    os << "\n    " << "serial  : " << std::hex << std::uppercase
       << config.serial_number << std::dec << std::nouppercase
       << "\n    " << "model   : " << config.model
       << port_config;
}

// -------------------------------------------------------------------------- //
void Digitizer::stream_status(std::ostream & os) const {

    os << std::boolalpha
       << "\n    " << "connected  : " << port_config.connected()
       << "\n    " << "registered : " << port_config.registered;
}

} // <- mzn
