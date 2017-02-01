#include "c1_pulse.h"

namespace mzn {
C1Pulse::C1Pulse():
    Command(0x22, 4),
    sensor_control_bitmap(),
    pulse_duration() { }

uint16_t C1Pulse::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Pulse",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = sensor_control_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = pulse_duration.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Pulse::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Pulse",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = sensor_control_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = pulse_duration.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Pulse::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_PULSE ---  \n";

    cmd_os << "\nsensor_control_bitmap: "; cmd_os << sensor_control_bitmap;

    cmd_os << "\npulse_duration: "; cmd_os << pulse_duration;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace