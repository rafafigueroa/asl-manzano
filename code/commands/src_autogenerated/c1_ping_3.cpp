#include "c1_ping_3.h"

namespace mzn {
C1Ping3::C1Ping3():
    Command(0x38, 24),
    ping_type(),
    user_message_number(),
    drift_tolerance(),
    user_message_count(),
    time_of_last_reboot(),
    spare_0(),
    spare_1(),
    status_bitmap() { }

uint16_t C1Ping3::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping3",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.msg_to_data(msg, mf_begin);
    mf_begin = user_message_number.msg_to_data(msg, mf_begin);
    mf_begin = drift_tolerance.msg_to_data(msg, mf_begin);
    mf_begin = user_message_count.msg_to_data(msg, mf_begin);
    mf_begin = time_of_last_reboot.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = status_bitmap.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Ping3::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping3",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.data_to_msg(msg, mf_begin);
    mf_begin = user_message_number.data_to_msg(msg, mf_begin);
    mf_begin = drift_tolerance.data_to_msg(msg, mf_begin);
    mf_begin = user_message_count.data_to_msg(msg, mf_begin);
    mf_begin = time_of_last_reboot.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = status_bitmap.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Ping3::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_PING_3 ---  \n";

    cmd_os << "\nping_type: "; cmd_os << ping_type;

    cmd_os << "\nuser_message_number: "; cmd_os << user_message_number;

    cmd_os << "\ndrift_tolerance: "; cmd_os << drift_tolerance;

    cmd_os << "\nuser_message_count: "; cmd_os << user_message_count;

    cmd_os << "\ntime_of_last_reboot: "; cmd_os << time_of_last_reboot;

    cmd_os << "\nstatus_bitmap: "; cmd_os << status_bitmap;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace