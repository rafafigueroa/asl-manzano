#include "c1_ping_2.h"

namespace mzn {
C1Ping2::C1Ping2():
    Command(0x38, 8),
    ping_type(),
    user_message_number(),
    status_request_bitmap() { }

uint16_t C1Ping2::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping2",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.msg_to_data(msg, mf_begin);
    mf_begin = user_message_number.msg_to_data(msg, mf_begin);
    mf_begin = status_request_bitmap.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Ping2::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping2",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.data_to_msg(msg, mf_begin);
    mf_begin = user_message_number.data_to_msg(msg, mf_begin);
    mf_begin = status_request_bitmap.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Ping2::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_PING_2 ---  \n";

    cmd_os << "\nping_type: "; cmd_os << ping_type;

    cmd_os << "\nuser_message_number: "; cmd_os << user_message_number;

    cmd_os << "\nstatus_request_bitmap: "; cmd_os << status_request_bitmap;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace