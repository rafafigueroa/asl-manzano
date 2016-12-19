// Manzano software
#include "serial_connection.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void SerialConnection::close_connection() {
}

// -------------------------------------------------------------------------- //
void SerialConnection::shutdown_connection() {
}

// -------------------------------------------------------------------------- //
void SerialConnection::setup_connection() {

    serial_.setPort(port_);
    serial_.open();

    if ( not serial_.isOpen() ) {

        throw WarningException("SerialConnection",
                               "setup_connection",
                               "could not open serial port");
    }

    serial_.flushInput();
    serial_.flush();
}

// -------------------------------------------------------------------------- //
void SerialConnection::enumerate_ports() const {

    std::vector<serial::PortInfo> devices_found = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    std::cout << std::endl << "Serial Ports: ";
    while( iter != devices_found.end() ) {

        serial::PortInfo device = *(iter++);

        std::cout << "\nport: " << device.port
                  << " --- description: " << device.description
                  << " --- hardware_id: " << device.hardware_id;
    }

    std::cout << std::endl;
}

} // << mzn
