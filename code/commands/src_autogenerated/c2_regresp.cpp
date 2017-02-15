#include "c2_regresp.h"

namespace mzn {
C2Regresp::C2Regresp():
    Command(0xC9, 4),
    registered() { }

uint16_t C2Regresp::msg_to_data(std::vector<uint8_t> const & msg,
                                uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Regresp",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = registered.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C2Regresp::data_to_msg(std::vector<uint8_t> & msg,
                                uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Regresp",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = registered.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C2Regresp::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_REGRESP ---  \n";

    cmd_os << "\nregistered: "; cmd_os << registered;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace