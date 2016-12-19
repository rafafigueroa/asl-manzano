#include "qdp_header.h"

namespace mzn {
QdpHeader::QdpHeader():
    Command(0x0, 8),
    command_number(),
    firmware_version(),
    length(),
    sequence_number(),
    acknowledge_number() { }

uint16_t QdpHeader::msg_to_data(std::vector<uint8_t> const & msg,
                                uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "QdpHeader",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = command_number.msg_to_data(msg, mf_begin);
    mf_begin = firmware_version.msg_to_data(msg, mf_begin);
    mf_begin = length.msg_to_data(msg, mf_begin);
    mf_begin = sequence_number.msg_to_data(msg, mf_begin);
    mf_begin = acknowledge_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t QdpHeader::data_to_msg(std::vector<uint8_t> & msg,
                                uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "QdpHeader",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = command_number.data_to_msg(msg, mf_begin);
    mf_begin = firmware_version.data_to_msg(msg, mf_begin);
    mf_begin = length.data_to_msg(msg, mf_begin);
    mf_begin = sequence_number.data_to_msg(msg, mf_begin);
    mf_begin = acknowledge_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & QdpHeader::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- QDP_HEADER ---  \n";

    cmd_os << "\ncommand_number: "; cmd_os << command_number;

    cmd_os << "\nfirmware_version: "; cmd_os << firmware_version;

    cmd_os << "\nlength: "; cmd_os << length;

    cmd_os << "\nsequence_number: "; cmd_os << sequence_number;

    cmd_os << "\nacknowledge_number: "; cmd_os << acknowledge_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace