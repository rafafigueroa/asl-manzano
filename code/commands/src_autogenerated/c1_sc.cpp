#include "c1_sc.h"

namespace mzn {
C1Sc::C1Sc():
    Command(0xAF, 32),
    sensor_output_1_definition(),
    sensor_output_2_definition(),
    sensor_output_3_definition(),
    sensor_output_4_definition(),
    sensor_output_5_definition(),
    sensor_output_6_definition(),
    sensor_output_7_definition(),
    sensor_output_8_definition() { }

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

    mf_begin = sensor_output_1_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_2_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_3_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_4_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_5_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_6_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_7_definition.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_8_definition.msg_to_data(msg, mf_begin);

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

    mf_begin = sensor_output_1_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_2_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_3_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_4_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_5_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_6_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_7_definition.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_8_definition.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Sc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SC ---  \n";

    cmd_os << "\nsensor_output_1_definition: "; cmd_os << sensor_output_1_definition;

    cmd_os << "\nsensor_output_2_definition: "; cmd_os << sensor_output_2_definition;

    cmd_os << "\nsensor_output_3_definition: "; cmd_os << sensor_output_3_definition;

    cmd_os << "\nsensor_output_4_definition: "; cmd_os << sensor_output_4_definition;

    cmd_os << "\nsensor_output_5_definition: "; cmd_os << sensor_output_5_definition;

    cmd_os << "\nsensor_output_6_definition: "; cmd_os << sensor_output_6_definition;

    cmd_os << "\nsensor_output_7_definition: "; cmd_os << sensor_output_7_definition;

    cmd_os << "\nsensor_output_8_definition: "; cmd_os << sensor_output_8_definition;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace