#include "c2_amass.h"

namespace mzn {
C2Amass::C2Amass():
    Command(0xC4, 32),
    tolerance_1a(),
    tolerance_1b(),
    tolerance_1c(),
    maximum_tries_1(),
    normal_interval_1(),
    squelch_interval_1(),
    sensor_control_bitmap_1(),
    pulse_duration_1(),
    tolerance_2a(),
    tolerance_2b(),
    tolerance_2c(),
    maximum_tries_2(),
    normal_interval_2(),
    squelch_interval_2(),
    sensor_control_bitmap_2(),
    pulse_duration_2() { }

uint16_t C2Amass::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Amass",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = tolerance_1a.msg_to_data(msg, mf_begin);
    mf_begin = tolerance_1b.msg_to_data(msg, mf_begin);
    mf_begin = tolerance_1c.msg_to_data(msg, mf_begin);
    mf_begin = maximum_tries_1.msg_to_data(msg, mf_begin);
    mf_begin = normal_interval_1.msg_to_data(msg, mf_begin);
    mf_begin = squelch_interval_1.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_bitmap_1.msg_to_data(msg, mf_begin);
    mf_begin = pulse_duration_1.msg_to_data(msg, mf_begin);
    mf_begin = tolerance_2a.msg_to_data(msg, mf_begin);
    mf_begin = tolerance_2b.msg_to_data(msg, mf_begin);
    mf_begin = tolerance_2c.msg_to_data(msg, mf_begin);
    mf_begin = maximum_tries_2.msg_to_data(msg, mf_begin);
    mf_begin = normal_interval_2.msg_to_data(msg, mf_begin);
    mf_begin = squelch_interval_2.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_bitmap_2.msg_to_data(msg, mf_begin);
    mf_begin = pulse_duration_2.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C2Amass::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Amass",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = tolerance_1a.data_to_msg(msg, mf_begin);
    mf_begin = tolerance_1b.data_to_msg(msg, mf_begin);
    mf_begin = tolerance_1c.data_to_msg(msg, mf_begin);
    mf_begin = maximum_tries_1.data_to_msg(msg, mf_begin);
    mf_begin = normal_interval_1.data_to_msg(msg, mf_begin);
    mf_begin = squelch_interval_1.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_bitmap_1.data_to_msg(msg, mf_begin);
    mf_begin = pulse_duration_1.data_to_msg(msg, mf_begin);
    mf_begin = tolerance_2a.data_to_msg(msg, mf_begin);
    mf_begin = tolerance_2b.data_to_msg(msg, mf_begin);
    mf_begin = tolerance_2c.data_to_msg(msg, mf_begin);
    mf_begin = maximum_tries_2.data_to_msg(msg, mf_begin);
    mf_begin = normal_interval_2.data_to_msg(msg, mf_begin);
    mf_begin = squelch_interval_2.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_bitmap_2.data_to_msg(msg, mf_begin);
    mf_begin = pulse_duration_2.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C2Amass::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_AMASS ---  \n";

    cmd_os << "\ntolerance_1a: "; cmd_os << tolerance_1a;

    cmd_os << "\ntolerance_1b: "; cmd_os << tolerance_1b;

    cmd_os << "\ntolerance_1c: "; cmd_os << tolerance_1c;

    cmd_os << "\nmaximum_tries_1: "; cmd_os << maximum_tries_1;

    cmd_os << "\nnormal_interval_1: "; cmd_os << normal_interval_1;

    cmd_os << "\nsquelch_interval_1: "; cmd_os << squelch_interval_1;

    cmd_os << "\nsensor_control_bitmap_1: "; cmd_os << sensor_control_bitmap_1;

    cmd_os << "\npulse_duration_1: "; cmd_os << pulse_duration_1;

    cmd_os << "\ntolerance_2a: "; cmd_os << tolerance_2a;

    cmd_os << "\ntolerance_2b: "; cmd_os << tolerance_2b;

    cmd_os << "\ntolerance_2c: "; cmd_os << tolerance_2c;

    cmd_os << "\nmaximum_tries_2: "; cmd_os << maximum_tries_2;

    cmd_os << "\nnormal_interval_2: "; cmd_os << normal_interval_2;

    cmd_os << "\nsquelch_interval_2: "; cmd_os << squelch_interval_2;

    cmd_os << "\nsensor_control_bitmap_2: "; cmd_os << sensor_control_bitmap_2;

    cmd_os << "\npulse_duration_2: "; cmd_os << pulse_duration_2;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace