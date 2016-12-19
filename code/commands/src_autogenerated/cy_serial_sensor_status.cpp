#include "cy_serial_sensor_status.h"

namespace mzn {
CySerialSensorStatus::CySerialSensorStatus():
    MultiCommandMap(0, 18),
    size_of_this_sub_block(),
    sensor_type(),
    serial_interface(),
    seconds_per_sample(),
    units(),
    integration_time(),
    fractional_digits(),
    valid_fields(),
    pressure_measurements() { }

uint16_t CySerialSensorStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CySerialSensorStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = size_of_this_sub_block.msg_to_data(msg, mf_begin);
    mf_begin = sensor_type.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface.msg_to_data(msg, mf_begin);
    mf_begin = seconds_per_sample.msg_to_data(msg, mf_begin);
    mf_begin = units.msg_to_data(msg, mf_begin);
    mf_begin = integration_time.msg_to_data(msg, mf_begin);
    mf_begin = fractional_digits.msg_to_data(msg, mf_begin);
    mf_begin = valid_fields.msg_to_data(msg, mf_begin);
    mf_begin = pressure_measurements.msg_to_data(msg, mf_begin);
    mf_begin = MultiCommandMap::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CySerialSensorStatus::data_to_msg(std::vector<uint8_t> & msg,
                                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CySerialSensorStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = size_of_this_sub_block.data_to_msg(msg, mf_begin);
    mf_begin = sensor_type.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface.data_to_msg(msg, mf_begin);
    mf_begin = seconds_per_sample.data_to_msg(msg, mf_begin);
    mf_begin = units.data_to_msg(msg, mf_begin);
    mf_begin = integration_time.data_to_msg(msg, mf_begin);
    mf_begin = fractional_digits.data_to_msg(msg, mf_begin);
    mf_begin = valid_fields.data_to_msg(msg, mf_begin);
    mf_begin = pressure_measurements.data_to_msg(msg, mf_begin);
    mf_begin = MultiCommandMap::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CySerialSensorStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CY_SERIAL_SENSOR_STATUS ---  \n";

    cmd_os << "\nsize_of_this_sub_block: "; cmd_os << size_of_this_sub_block;

    cmd_os << "\nsensor_type: "; cmd_os << sensor_type;

    cmd_os << "\nserial_interface: "; cmd_os << serial_interface;

    cmd_os << "\nseconds_per_sample: "; cmd_os << seconds_per_sample;

    cmd_os << "\nunits: "; cmd_os << units;

    cmd_os << "\nintegration_time: "; cmd_os << integration_time;

    cmd_os << "\nfractional_digits: "; cmd_os << fractional_digits;

    cmd_os << "\nvalid_fields: "; cmd_os << valid_fields;

    cmd_os << "\npressure_measurements: "; cmd_os << pressure_measurements;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void CySerialSensorStatus::create_new_ic(uint8_t const cmd_key) {

    switch(cmd_key) {
        case k_cz_internal_temperature_measurement : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CzInternalTemperatureMeasurement>() } );
            break;
        }
        case k_cz_humidity_and_external_temperature : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CzHumidityAndExternalTemperature>() } );
            break;
        }
    }
}
} // end namespace