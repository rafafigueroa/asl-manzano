#include "c1_sc.h"

namespace mzn {
C1Sc::C1Sc():
    Command(0xAF, 32),
    sensor_a_output_1_ignore(),
    sensor_a_output_1_active_high(),
    sensor_a_output_1_control_map(),
    sensor_a_output_2_ignore(),
    sensor_a_output_2_active_high(),
    sensor_a_output_2_control_map(),
    sensor_a_output_3_ignore(),
    sensor_a_output_3_active_high(),
    sensor_a_output_3_control_map(),
    sensor_a_output_4_ignore(),
    sensor_a_output_4_active_high(),
    sensor_a_output_4_control_map(),
    sensor_b_output_1_ignore(),
    sensor_b_output_1_active_high(),
    sensor_b_output_1_control_map(),
    sensor_b_output_2_ignore(),
    sensor_b_output_2_active_high(),
    sensor_b_output_2_control_map(),
    sensor_b_output_3_ignore(),
    sensor_b_output_3_active_high(),
    sensor_b_output_3_control_map(),
    sensor_b_output_4_ignore(),
    sensor_b_output_4_active_high(),
    sensor_b_output_4_control_map() { }

uint16_t C1Sc::msg_to_data(std::vector<uint8_t> const & msg,
                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Sc",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_1_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_a_output_1_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_2_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_a_output_2_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_3_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_a_output_3_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_4_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_a_output_4_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_1_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_b_output_1_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_2_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_b_output_2_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_3_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_b_output_3_control_map.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_4_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_b_output_4_control_map.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Sc::data_to_msg(std::vector<uint8_t> & msg,
                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Sc",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_1_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_a_output_1_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_2_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_a_output_2_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_3_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_a_output_3_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_a_output_4_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_a_output_4_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_1_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_b_output_1_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_2_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_b_output_2_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_3_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_b_output_3_control_map.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_b_output_4_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_b_output_4_control_map.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Sc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SC ---  \n";

    cmd_os << "\nsensor_a_output_1_active_high: "; cmd_os << sensor_a_output_1_active_high;

    cmd_os << "\nsensor_a_output_1_control_map: "; cmd_os << sensor_a_output_1_control_map;

    cmd_os << "\nsensor_a_output_2_active_high: "; cmd_os << sensor_a_output_2_active_high;

    cmd_os << "\nsensor_a_output_2_control_map: "; cmd_os << sensor_a_output_2_control_map;

    cmd_os << "\nsensor_a_output_3_active_high: "; cmd_os << sensor_a_output_3_active_high;

    cmd_os << "\nsensor_a_output_3_control_map: "; cmd_os << sensor_a_output_3_control_map;

    cmd_os << "\nsensor_a_output_4_active_high: "; cmd_os << sensor_a_output_4_active_high;

    cmd_os << "\nsensor_a_output_4_control_map: "; cmd_os << sensor_a_output_4_control_map;

    cmd_os << "\nsensor_b_output_1_active_high: "; cmd_os << sensor_b_output_1_active_high;

    cmd_os << "\nsensor_b_output_1_control_map: "; cmd_os << sensor_b_output_1_control_map;

    cmd_os << "\nsensor_b_output_2_active_high: "; cmd_os << sensor_b_output_2_active_high;

    cmd_os << "\nsensor_b_output_2_control_map: "; cmd_os << sensor_b_output_2_control_map;

    cmd_os << "\nsensor_b_output_3_active_high: "; cmd_os << sensor_b_output_3_active_high;

    cmd_os << "\nsensor_b_output_3_control_map: "; cmd_os << sensor_b_output_3_control_map;

    cmd_os << "\nsensor_b_output_4_active_high: "; cmd_os << sensor_b_output_4_active_high;

    cmd_os << "\nsensor_b_output_4_control_map: "; cmd_os << sensor_b_output_4_control_map;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace