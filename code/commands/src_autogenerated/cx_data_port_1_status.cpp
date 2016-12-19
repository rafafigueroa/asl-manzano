#include "cx_data_port_1_status.h"

namespace mzn {
CxDataPort1Status::CxDataPort1Status():
    Command(0, 28),
    total_data_packets_sent(),
    total_packets_resent(),
    total_fill_packets_sent(),
    receive_sequence_errors(),
    bytes_of_packet_currently_used("bytes"),
    physical_interface_number(),
    data_port_number(),
    retransmission_timout(),
    dataport_flags() { }

uint16_t CxDataPort1Status::msg_to_data(std::vector<uint8_t> const & msg,
                                        uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDataPort1Status",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_data_packets_sent.msg_to_data(msg, mf_begin);
    mf_begin = total_packets_resent.msg_to_data(msg, mf_begin);
    mf_begin = total_fill_packets_sent.msg_to_data(msg, mf_begin);
    mf_begin = receive_sequence_errors.msg_to_data(msg, mf_begin);
    mf_begin = bytes_of_packet_currently_used.msg_to_data(msg, mf_begin);
    mf_begin = physical_interface_number.msg_to_data(msg, mf_begin);
    mf_begin = data_port_number.msg_to_data(msg, mf_begin);
    mf_begin = retransmission_timout.msg_to_data(msg, mf_begin);
    mf_begin = dataport_flags.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxDataPort1Status::data_to_msg(std::vector<uint8_t> & msg,
                                        uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxDataPort1Status",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_data_packets_sent.data_to_msg(msg, mf_begin);
    mf_begin = total_packets_resent.data_to_msg(msg, mf_begin);
    mf_begin = total_fill_packets_sent.data_to_msg(msg, mf_begin);
    mf_begin = receive_sequence_errors.data_to_msg(msg, mf_begin);
    mf_begin = bytes_of_packet_currently_used.data_to_msg(msg, mf_begin);
    mf_begin = physical_interface_number.data_to_msg(msg, mf_begin);
    mf_begin = data_port_number.data_to_msg(msg, mf_begin);
    mf_begin = retransmission_timout.data_to_msg(msg, mf_begin);
    mf_begin = dataport_flags.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxDataPort1Status::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_DATA_PORT_1_STATUS ---  \n";

    cmd_os << "\ntotal_data_packets_sent: "; cmd_os << total_data_packets_sent;

    cmd_os << "\ntotal_packets_resent: "; cmd_os << total_packets_resent;

    cmd_os << "\ntotal_fill_packets_sent: "; cmd_os << total_fill_packets_sent;

    cmd_os << "\nreceive_sequence_errors: "; cmd_os << receive_sequence_errors;

    cmd_os << "\nbytes_of_packet_currently_used: "; cmd_os << bytes_of_packet_currently_used;

    cmd_os << "\nphysical_interface_number: "; cmd_os << physical_interface_number;

    cmd_os << "\ndata_port_number: "; cmd_os << data_port_number;

    cmd_os << "\nretransmission_timout: "; cmd_os << retransmission_timout;

    cmd_os << "\ndataport_flags: "; cmd_os << dataport_flags;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace