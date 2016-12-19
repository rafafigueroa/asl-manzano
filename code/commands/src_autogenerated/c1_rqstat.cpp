#include "c1_rqstat.h"

namespace mzn {
C1Rqstat::C1Rqstat():
    Command(0x1F, 4),
    request_bitmap() { }

uint16_t C1Rqstat::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Rqstat",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = request_bitmap.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Rqstat::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Rqstat",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = request_bitmap.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Rqstat::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQSTAT ---  \n";

    cmd_os << "\nrequest_bitmap: "; cmd_os << request_bitmap;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace