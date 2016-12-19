#include "cx_boom_positions.h"

namespace mzn {
CxBoomPositions::CxBoomPositions():
    Command(0, 32),
    channel_1_boom(),
    channel_2_boom(),
    channel_3_boom(),
    channel_4_boom(),
    channel_5_boom(),
    channel_6_boom(),
    analog_positive_supply("x10 mV"),
    analog_negative_supply("x10 mV"),
    input_power("x150 mV"),
    system_temperature("Celcius"),
    main_current("mA"),
    gps_antenna_current("mA"),
    seismometer_1_temperature("Celcius"),
    seismometer_2_temperature("Celcius"),
    slave_processor_timeouts() { }

uint16_t CxBoomPositions::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxBoomPositions",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = channel_1_boom.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_boom.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_boom.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_boom.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_boom.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_boom.msg_to_data(msg, mf_begin);
    mf_begin = analog_positive_supply.msg_to_data(msg, mf_begin);
    mf_begin = analog_negative_supply.msg_to_data(msg, mf_begin);
    mf_begin = input_power.msg_to_data(msg, mf_begin);
    mf_begin = system_temperature.msg_to_data(msg, mf_begin);
    mf_begin = main_current.msg_to_data(msg, mf_begin);
    mf_begin = gps_antenna_current.msg_to_data(msg, mf_begin);
    mf_begin = seismometer_1_temperature.msg_to_data(msg, mf_begin);
    mf_begin = seismometer_2_temperature.msg_to_data(msg, mf_begin);
    mf_begin = slave_processor_timeouts.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxBoomPositions::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxBoomPositions",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = channel_1_boom.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_boom.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_boom.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_boom.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_boom.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_boom.data_to_msg(msg, mf_begin);
    mf_begin = analog_positive_supply.data_to_msg(msg, mf_begin);
    mf_begin = analog_negative_supply.data_to_msg(msg, mf_begin);
    mf_begin = input_power.data_to_msg(msg, mf_begin);
    mf_begin = system_temperature.data_to_msg(msg, mf_begin);
    mf_begin = main_current.data_to_msg(msg, mf_begin);
    mf_begin = gps_antenna_current.data_to_msg(msg, mf_begin);
    mf_begin = seismometer_1_temperature.data_to_msg(msg, mf_begin);
    mf_begin = seismometer_2_temperature.data_to_msg(msg, mf_begin);
    mf_begin = slave_processor_timeouts.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxBoomPositions::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_BOOM_POSITIONS ---  \n";

    cmd_os << "\nchannel_1_boom: "; cmd_os << channel_1_boom;

    cmd_os << "\nchannel_2_boom: "; cmd_os << channel_2_boom;

    cmd_os << "\nchannel_3_boom: "; cmd_os << channel_3_boom;

    cmd_os << "\nchannel_4_boom: "; cmd_os << channel_4_boom;

    cmd_os << "\nchannel_5_boom: "; cmd_os << channel_5_boom;

    cmd_os << "\nchannel_6_boom: "; cmd_os << channel_6_boom;

    cmd_os << "\nanalog_positive_supply: "; cmd_os << analog_positive_supply;

    cmd_os << "\nanalog_negative_supply: "; cmd_os << analog_negative_supply;

    cmd_os << "\ninput_power: "; cmd_os << input_power;

    cmd_os << "\nsystem_temperature: "; cmd_os << system_temperature;

    cmd_os << "\nmain_current: "; cmd_os << main_current;

    cmd_os << "\ngps_antenna_current: "; cmd_os << gps_antenna_current;

    cmd_os << "\nseismometer_1_temperature: "; cmd_os << seismometer_1_temperature;

    cmd_os << "\nseismometer_2_temperature: "; cmd_os << seismometer_2_temperature;

    cmd_os << "\nslave_processor_timeouts: "; cmd_os << slave_processor_timeouts;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace