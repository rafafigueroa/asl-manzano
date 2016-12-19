#include "c1_pollsn.h"

namespace mzn {
C1Pollsn::C1Pollsn():
    Command(0x14, 4),
    serial_number_mask(),
    serial_number_match() { }

uint16_t C1Pollsn::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Pollsn",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number_mask.msg_to_data(msg, mf_begin);
    mf_begin = serial_number_match.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Pollsn::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Pollsn",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number_mask.data_to_msg(msg, mf_begin);
    mf_begin = serial_number_match.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Pollsn::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_POLLSN ---  \n";

    cmd_os << "\nserial_number_mask: "; cmd_os << serial_number_mask;

    cmd_os << "\nserial_number_match: "; cmd_os << serial_number_match;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace