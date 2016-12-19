#include "cx_dev.h"

namespace mzn {
CxDev::CxDev():
    Command(0, 20),
    port_number(),
    unit_id(),
    version(),
    options(),
    serial_number(),
    device_static_storage(),
    seconds_since_heard() { }

uint16_t CxDev::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDev",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = port_number.msg_to_data(msg, mf_begin);
    mf_begin = unit_id.msg_to_data(msg, mf_begin);
    mf_begin = version.msg_to_data(msg, mf_begin);
    mf_begin = options.msg_to_data(msg, mf_begin);
    mf_begin = serial_number.msg_to_data(msg, mf_begin);
    mf_begin = device_static_storage.msg_to_data(msg, mf_begin);
    mf_begin = seconds_since_heard.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxDev::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDev",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = port_number.data_to_msg(msg, mf_begin);
    mf_begin = unit_id.data_to_msg(msg, mf_begin);
    mf_begin = version.data_to_msg(msg, mf_begin);
    mf_begin = options.data_to_msg(msg, mf_begin);
    mf_begin = serial_number.data_to_msg(msg, mf_begin);
    mf_begin = device_static_storage.data_to_msg(msg, mf_begin);
    mf_begin = seconds_since_heard.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxDev::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_DEV ---  \n";

    cmd_os << "\nport_number: "; cmd_os << port_number;

    cmd_os << "\nunit_id: "; cmd_os << unit_id;

    cmd_os << "\nversion: "; cmd_os << version;

    cmd_os << "\noptions: "; cmd_os << options;

    cmd_os << "\nserial_number: "; cmd_os << serial_number;

    cmd_os << "\ndevice_static_storage: "; cmd_os << device_static_storage;

    cmd_os << "\nseconds_since_heard: "; cmd_os << seconds_since_heard;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace