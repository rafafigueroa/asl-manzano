#include "cz_humidity_and_external_temperature.h"

namespace mzn {
CzHumidityAndExternalTemperature::CzHumidityAndExternalTemperature():
    Command(0, 8),
    humidity_measurement(),
    external_temperature_measurement() { }

uint16_t CzHumidityAndExternalTemperature::msg_to_data(std::vector<uint8_t> const & msg,
                                                       uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CzHumidityAndExternalTemperature",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = humidity_measurement.msg_to_data(msg, mf_begin);
    mf_begin = external_temperature_measurement.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CzHumidityAndExternalTemperature::data_to_msg(std::vector<uint8_t> & msg,
                                                       uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CzHumidityAndExternalTemperature",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = humidity_measurement.data_to_msg(msg, mf_begin);
    mf_begin = external_temperature_measurement.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CzHumidityAndExternalTemperature::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CZ_HUMIDITY_AND_EXTERNAL_TEMPERATURE ---  \n";

    cmd_os << "\nhumidity_measurement: "; cmd_os << humidity_measurement;

    cmd_os << "\nexternal_temperature_measurement: "; cmd_os << external_temperature_measurement;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace