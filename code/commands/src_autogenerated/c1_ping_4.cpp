#include "c1_ping_4.h"

namespace mzn {
C1Ping4::C1Ping4():
    Command(0x38, 4),
    ping_type(),
    ignore_0() { }

uint16_t C1Ping4::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping4",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type

    return mf_begin;
}

uint16_t C1Ping4::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping4",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type

    return mf_begin;
}

std::ostream & C1Ping4::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_PING_4 ---  \n";

    cmd_os << "\nping_type: "; cmd_os << ping_type;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace