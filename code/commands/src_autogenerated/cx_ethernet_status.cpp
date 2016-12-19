#include "cx_ethernet_status.h"

namespace mzn {
CxEthernetStatus::CxEthernetStatus():
    Command(0, 76),
    receive_checksum_errors(),
    total_io_errors(),
    physical_interface_number(),
    spare_1(),
    destination_unreachable_icmp_packets_received(),
    source_quench_icmp_packets_received(),
    echo_request_icmp_packets_received(),
    redirect_icmp_packets_received(),
    runt_frames(),
    crc_errors(),
    broadcast_frames(),
    unicast_frames(),
    total_good_frames(),
    jabber_errors(),
    out_of_window(),
    transmitted_ok(),
    received_packets_missed(),
    transmit_collisions(),
    current_link_status(),
    spare_2(),
    spare_3() { }

uint16_t CxEthernetStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                       uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxEthernetStatus",
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
    mf_begin = runt_frames.msg_to_data(msg, mf_begin);
    mf_begin = crc_errors.msg_to_data(msg, mf_begin);
    mf_begin = broadcast_frames.msg_to_data(msg, mf_begin);
    mf_begin = unicast_frames.msg_to_data(msg, mf_begin);
    mf_begin = total_good_frames.msg_to_data(msg, mf_begin);
    mf_begin = jabber_errors.msg_to_data(msg, mf_begin);
    mf_begin = out_of_window.msg_to_data(msg, mf_begin);
    mf_begin = transmitted_ok.msg_to_data(msg, mf_begin);
    mf_begin = received_packets_missed.msg_to_data(msg, mf_begin);
    mf_begin = transmit_collisions.msg_to_data(msg, mf_begin);
    mf_begin = current_link_status.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

uint16_t CxEthernetStatus::data_to_msg(std::vector<uint8_t> & msg,
                                       uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxEthernetStatus",
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
    mf_begin = runt_frames.data_to_msg(msg, mf_begin);
    mf_begin = crc_errors.data_to_msg(msg, mf_begin);
    mf_begin = broadcast_frames.data_to_msg(msg, mf_begin);
    mf_begin = unicast_frames.data_to_msg(msg, mf_begin);
    mf_begin = total_good_frames.data_to_msg(msg, mf_begin);
    mf_begin = jabber_errors.data_to_msg(msg, mf_begin);
    mf_begin = out_of_window.data_to_msg(msg, mf_begin);
    mf_begin = transmitted_ok.data_to_msg(msg, mf_begin);
    mf_begin = received_packets_missed.data_to_msg(msg, mf_begin);
    mf_begin = transmit_collisions.data_to_msg(msg, mf_begin);
    mf_begin = current_link_status.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

std::ostream & CxEthernetStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_ETHERNET_STATUS ---  \n";

    cmd_os << "\nreceive_checksum_errors: "; cmd_os << receive_checksum_errors;

    cmd_os << "\ntotal_io_errors: "; cmd_os << total_io_errors;

    cmd_os << "\nphysical_interface_number: "; cmd_os << physical_interface_number;

    cmd_os << "\ndestination_unreachable_icmp_packets_received: "; cmd_os << destination_unreachable_icmp_packets_received;

    cmd_os << "\nsource_quench_icmp_packets_received: "; cmd_os << source_quench_icmp_packets_received;

    cmd_os << "\necho_request_icmp_packets_received: "; cmd_os << echo_request_icmp_packets_received;

    cmd_os << "\nredirect_icmp_packets_received: "; cmd_os << redirect_icmp_packets_received;

    cmd_os << "\nrunt_frames: "; cmd_os << runt_frames;

    cmd_os << "\ncrc_errors: "; cmd_os << crc_errors;

    cmd_os << "\nbroadcast_frames: "; cmd_os << broadcast_frames;

    cmd_os << "\nunicast_frames: "; cmd_os << unicast_frames;

    cmd_os << "\ntotal_good_frames: "; cmd_os << total_good_frames;

    cmd_os << "\njabber_errors: "; cmd_os << jabber_errors;

    cmd_os << "\nout_of_window: "; cmd_os << out_of_window;

    cmd_os << "\ntransmitted_ok: "; cmd_os << transmitted_ok;

    cmd_os << "\nreceived_packets_missed: "; cmd_os << received_packets_missed;

    cmd_os << "\ntransmit_collisions: "; cmd_os << transmit_collisions;

    cmd_os << "\ncurrent_link_status: "; cmd_os << current_link_status;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace