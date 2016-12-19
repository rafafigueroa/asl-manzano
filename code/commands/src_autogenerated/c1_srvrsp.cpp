#include "c1_srvrsp.h"

namespace mzn {
C1Srvrsp::C1Srvrsp():
    Command(0x11, 48),
    serial_number(),
    challenge_value(),
    server_ip_address(),
    server_udp_port(),
    registration_number(),
    server_random_value(),
    md5_result() { }

uint16_t C1Srvrsp::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Srvrsp",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.msg_to_data(msg, mf_begin);
    mf_begin = challenge_value.msg_to_data(msg, mf_begin);
    mf_begin = server_ip_address.msg_to_data(msg, mf_begin);
    mf_begin = server_udp_port.msg_to_data(msg, mf_begin);
    mf_begin = registration_number.msg_to_data(msg, mf_begin);
    mf_begin = server_random_value.msg_to_data(msg, mf_begin);
    mf_begin = md5_result.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Srvrsp::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Srvrsp",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.data_to_msg(msg, mf_begin);
    mf_begin = challenge_value.data_to_msg(msg, mf_begin);
    mf_begin = server_ip_address.data_to_msg(msg, mf_begin);
    mf_begin = server_udp_port.data_to_msg(msg, mf_begin);
    mf_begin = registration_number.data_to_msg(msg, mf_begin);
    mf_begin = server_random_value.data_to_msg(msg, mf_begin);
    mf_begin = md5_result.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Srvrsp::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SRVRSP ---  \n";

    cmd_os << "\nserial_number: "; cmd_os << serial_number;

    cmd_os << "\nchallenge_value: "; cmd_os << challenge_value;

    cmd_os << "\nserver_ip_address: "; cmd_os << server_ip_address;

    cmd_os << "\nserver_udp_port: "; cmd_os << server_udp_port;

    cmd_os << "\nregistration_number: "; cmd_os << registration_number;

    cmd_os << "\nserver_random_value: "; cmd_os << server_random_value;

    cmd_os << "\nmd5_result: "; cmd_os << md5_result;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace