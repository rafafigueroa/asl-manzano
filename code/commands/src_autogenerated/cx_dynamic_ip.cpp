#include "cx_dynamic_ip.h"

namespace mzn {
CxDynamicIp::CxDynamicIp():
    Command(0, 16),
    serial_1_ip_address(),
    serial_2_ip_address(),
    reserved(),
    ethernet_ip_address() { }

uint16_t CxDynamicIp::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDynamicIp",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_1_ip_address.msg_to_data(msg, mf_begin);
    mf_begin = serial_2_ip_address.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ethernet_ip_address.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxDynamicIp::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDynamicIp",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_1_ip_address.data_to_msg(msg, mf_begin);
    mf_begin = serial_2_ip_address.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ethernet_ip_address.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxDynamicIp::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_DYNAMIC_IP ---  \n";

    cmd_os << "\nserial_1_ip_address: "; cmd_os << serial_1_ip_address;

    cmd_os << "\nserial_2_ip_address: "; cmd_os << serial_2_ip_address;

    cmd_os << "\nethernet_ip_address: "; cmd_os << ethernet_ip_address;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace