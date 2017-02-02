#include "c2_regchk.h"

namespace mzn {
C2Regchk::C2Regchk():
    Command(0x5D, 4),
    ip_address() { }

uint16_t C2Regchk::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Regchk",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ip_address.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C2Regchk::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Regchk",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ip_address.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C2Regchk::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_REGCHK ---  \n";

    cmd_os << "\nip_address: "; cmd_os << ip_address;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace