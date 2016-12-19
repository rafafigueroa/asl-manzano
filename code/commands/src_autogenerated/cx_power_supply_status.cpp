#include "cx_power_supply_status.h"

namespace mzn {
CxPowerSupplyStatus::CxPowerSupplyStatus():
    Command(0, 20),
    charging_phase(),
    battery_temperature("Celcius"),
    battery_capacity("percent"),
    depth_of_discharge("percent"),
    battery_voltage("mV"),
    input_voltage("mV"),
    battery_current("mA"),
    absorption_set_point("mV"),
    float_set_point("mV"),
    alerts(),
    loads_off() { }

uint16_t CxPowerSupplyStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                          uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxPowerSupplyStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = charging_phase.msg_to_data(msg, mf_begin);
    mf_begin = battery_temperature.msg_to_data(msg, mf_begin);
    mf_begin = battery_capacity.msg_to_data(msg, mf_begin);
    mf_begin = depth_of_discharge.msg_to_data(msg, mf_begin);
    mf_begin = battery_voltage.msg_to_data(msg, mf_begin);
    mf_begin = input_voltage.msg_to_data(msg, mf_begin);
    mf_begin = battery_current.msg_to_data(msg, mf_begin);
    mf_begin = absorption_set_point.msg_to_data(msg, mf_begin);
    mf_begin = float_set_point.msg_to_data(msg, mf_begin);
    mf_begin = alerts.msg_to_data(msg, mf_begin);
    mf_begin = loads_off.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxPowerSupplyStatus::data_to_msg(std::vector<uint8_t> & msg,
                                          uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxPowerSupplyStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = charging_phase.data_to_msg(msg, mf_begin);
    mf_begin = battery_temperature.data_to_msg(msg, mf_begin);
    mf_begin = battery_capacity.data_to_msg(msg, mf_begin);
    mf_begin = depth_of_discharge.data_to_msg(msg, mf_begin);
    mf_begin = battery_voltage.data_to_msg(msg, mf_begin);
    mf_begin = input_voltage.data_to_msg(msg, mf_begin);
    mf_begin = battery_current.data_to_msg(msg, mf_begin);
    mf_begin = absorption_set_point.data_to_msg(msg, mf_begin);
    mf_begin = float_set_point.data_to_msg(msg, mf_begin);
    mf_begin = alerts.data_to_msg(msg, mf_begin);
    mf_begin = loads_off.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxPowerSupplyStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_POWER_SUPPLY_STATUS ---  \n";

    cmd_os << "\ncharging_phase: "; cmd_os << charging_phase;

    cmd_os << "\nbattery_temperature: "; cmd_os << battery_temperature;

    cmd_os << "\nbattery_capacity: "; cmd_os << battery_capacity;

    cmd_os << "\ndepth_of_discharge: "; cmd_os << depth_of_discharge;

    cmd_os << "\nbattery_voltage: "; cmd_os << battery_voltage;

    cmd_os << "\ninput_voltage: "; cmd_os << input_voltage;

    cmd_os << "\nbattery_current: "; cmd_os << battery_current;

    cmd_os << "\nabsorption_set_point: "; cmd_os << absorption_set_point;

    cmd_os << "\nfloat_set_point: "; cmd_os << float_set_point;

    cmd_os << "\nalerts: "; cmd_os << alerts;

    cmd_os << "\nloads_off: "; cmd_os << loads_off;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace