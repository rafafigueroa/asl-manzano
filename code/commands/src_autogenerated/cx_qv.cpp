#include "cx_qv.h"

namespace mzn {
CxQv::CxQv():
    Command(0, 52),
    channel(),
    shift_count(),
    seconds_offset(),
    spare_0(),
    starting_value(),
    byte_difference() { }

uint16_t CxQv::msg_to_data(std::vector<uint8_t> const & msg,
                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxQv",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = channel.msg_to_data(msg, mf_begin);
    mf_begin = shift_count.msg_to_data(msg, mf_begin);
    mf_begin = seconds_offset.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = starting_value.msg_to_data(msg, mf_begin);
    mf_begin = byte_difference.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxQv::data_to_msg(std::vector<uint8_t> & msg,
                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxQv",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = channel.data_to_msg(msg, mf_begin);
    mf_begin = shift_count.data_to_msg(msg, mf_begin);
    mf_begin = seconds_offset.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = starting_value.data_to_msg(msg, mf_begin);
    mf_begin = byte_difference.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxQv::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_QV ---  \n";

    cmd_os << "\nchannel: "; cmd_os << channel;

    cmd_os << "\nshift_count: "; cmd_os << shift_count;

    cmd_os << "\nseconds_offset: "; cmd_os << seconds_offset;

    cmd_os << "\nstarting_value: "; cmd_os << starting_value;

    cmd_os << "\nbyte_difference: "; cmd_os << byte_difference;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace