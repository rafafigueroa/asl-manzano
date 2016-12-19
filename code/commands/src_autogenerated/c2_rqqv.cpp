#include "c2_rqqv.h"

namespace mzn {
C2Rqqv::C2Rqqv():
    Command(0x5F, 6),
    lowest_sequence_number(),
    channel_map() { }

uint16_t C2Rqqv::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Rqqv",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = lowest_sequence_number.msg_to_data(msg, mf_begin);
    mf_begin = channel_map.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C2Rqqv::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Rqqv",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = lowest_sequence_number.data_to_msg(msg, mf_begin);
    mf_begin = channel_map.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C2Rqqv::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_RQQV ---  \n";

    cmd_os << "\nlowest_sequence_number: "; cmd_os << lowest_sequence_number;

    cmd_os << "\nchannel_map: "; cmd_os << channel_map;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace