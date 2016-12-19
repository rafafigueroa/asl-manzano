#include "c1_ctrl.h"

namespace mzn {
C1Ctrl::C1Ctrl():
    Command(0x19, 2),
    ctrl_flags() { }

uint16_t C1Ctrl::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ctrl",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ctrl_flags.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Ctrl::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ctrl",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ctrl_flags.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Ctrl::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_CTRL ---  \n";

    cmd_os << "\nctrl_flags: "; cmd_os << ctrl_flags;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace