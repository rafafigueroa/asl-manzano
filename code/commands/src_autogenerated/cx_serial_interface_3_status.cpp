#include "cx_serial_interface_3_status.h"

namespace mzn {
CxSerialInterface3Status::CxSerialInterface3Status():
    Command(0, 36),
    receive_checksum_errors(),
    total_io_errors(),
    physical_interface_number(),
    spare_0(),
    destination_unreachable_icmp_packets_received(),
    source_quench_icmp_packets_received(),
    echo_request_icmp_packets_received(),
    redirect_icmp_packets_received(),
    character_overruns(),
    framing_errors() { }

uint16_t CxSerialInterface3Status::msg_to_data(std::vector<uint8_t> const & msg,
                                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSerialInterface3Status",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = receive_checksum_errors.msg_to_data(msg, mf_begin);
    mf_begin = total_io_errors.msg_to_data(msg, mf_begin);
    mf_begin = physical_interface_number.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = destination_unreachable_icmp_packets_received.msg_to_data(msg, mf_begin);
    mf_begin = source_quench_icmp_packets_received.msg_to_data(msg, mf_begin);
    mf_begin = echo_request_icmp_packets_received.msg_to_data(msg, mf_begin);
    mf_begin = redirect_icmp_packets_received.msg_to_data(msg, mf_begin);
    mf_begin = character_overruns.msg_to_data(msg, mf_begin);
    mf_begin = framing_errors.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxSerialInterface3Status::data_to_msg(std::vector<uint8_t> & msg,
                                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSerialInterface3Status",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = receive_checksum_errors.data_to_msg(msg, mf_begin);
    mf_begin = total_io_errors.data_to_msg(msg, mf_begin);
    mf_begin = physical_interface_number.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = destination_unreachable_icmp_packets_received.data_to_msg(msg, mf_begin);
    mf_begin = source_quench_icmp_packets_received.data_to_msg(msg, mf_begin);
    mf_begin = echo_request_icmp_packets_received.data_to_msg(msg, mf_begin);
    mf_begin = redirect_icmp_packets_received.data_to_msg(msg, mf_begin);
    mf_begin = character_overruns.data_to_msg(msg, mf_begin);
    mf_begin = framing_errors.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxSerialInterface3Status::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_SERIAL_INTERFACE_3_STATUS ---  \n";

    cmd_os << "\nreceive_checksum_errors: "; cmd_os << receive_checksum_errors;

    cmd_os << "\ntotal_io_errors: "; cmd_os << total_io_errors;

    cmd_os << "\nphysical_interface_number: "; cmd_os << physical_interface_number;

    cmd_os << "\ndestination_unreachable_icmp_packets_received: "; cmd_os << destination_unreachable_icmp_packets_received;

    cmd_os << "\nsource_quench_icmp_packets_received: "; cmd_os << source_quench_icmp_packets_received;

    cmd_os << "\necho_request_icmp_packets_received: "; cmd_os << echo_request_icmp_packets_received;

    cmd_os << "\nredirect_icmp_packets_received: "; cmd_os << redirect_icmp_packets_received;

    cmd_os << "\ncharacter_overruns: "; cmd_os << character_overruns;

    cmd_os << "\nframing_errors: "; cmd_os << framing_errors;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace