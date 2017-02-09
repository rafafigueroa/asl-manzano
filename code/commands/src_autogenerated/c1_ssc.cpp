#include "c1_ssc.h"

namespace mzn {
C1Ssc::C1Ssc():
    Command(0x2E, 32),
    sensor_output_1_ignore(),
    sensor_output_1_active_high(),
    sensor_output_1(),
    sensor_output_2_ignore(),
    sensor_output_2_active_high(),
    sensor_output_2(),
    sensor_output_3_ignore(),
    sensor_output_3_active_high(),
    sensor_output_3(),
    sensor_output_4_ignore(),
    sensor_output_4_active_high(),
    sensor_output_4(),
    sensor_output_5_ignore(),
    sensor_output_5_active_high(),
    sensor_output_5(),
    sensor_output_6_ignore(),
    sensor_output_6_active_high(),
    sensor_output_6(),
    sensor_output_7_ignore(),
    sensor_output_7_active_high(),
    sensor_output_7(),
    sensor_output_8_ignore(),
    sensor_output_8_active_high(),
    sensor_output_8() { }

uint16_t C1Ssc::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ssc",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_1.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_2.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_3.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_4.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_5_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_5.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_6_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_6.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_7_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_7.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_8_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_output_8.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Ssc::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ssc",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_1_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_1.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_2_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_2.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_3_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_3.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_4_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_4.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_5_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_5.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_6_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_6.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_7_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_7.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sensor_output_8_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_output_8.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Ssc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_SSC ---  \n";

    cmd_os << "\nsensor_output_1_active_high: "; cmd_os << sensor_output_1_active_high;

    cmd_os << "\nsensor_output_1: "; cmd_os << sensor_output_1;

    cmd_os << "\nsensor_output_2_active_high: "; cmd_os << sensor_output_2_active_high;

    cmd_os << "\nsensor_output_2: "; cmd_os << sensor_output_2;

    cmd_os << "\nsensor_output_3_active_high: "; cmd_os << sensor_output_3_active_high;

    cmd_os << "\nsensor_output_3: "; cmd_os << sensor_output_3;

    cmd_os << "\nsensor_output_4_active_high: "; cmd_os << sensor_output_4_active_high;

    cmd_os << "\nsensor_output_4: "; cmd_os << sensor_output_4;

    cmd_os << "\nsensor_output_5_active_high: "; cmd_os << sensor_output_5_active_high;

    cmd_os << "\nsensor_output_5: "; cmd_os << sensor_output_5;

    cmd_os << "\nsensor_output_6_active_high: "; cmd_os << sensor_output_6_active_high;

    cmd_os << "\nsensor_output_6: "; cmd_os << sensor_output_6;

    cmd_os << "\nsensor_output_7_active_high: "; cmd_os << sensor_output_7_active_high;

    cmd_os << "\nsensor_output_7: "; cmd_os << sensor_output_7;

    cmd_os << "\nsensor_output_8_active_high: "; cmd_os << sensor_output_8_active_high;

    cmd_os << "\nsensor_output_8: "; cmd_os << sensor_output_8;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace