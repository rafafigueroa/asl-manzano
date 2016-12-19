#include "cz_internal_temperature_measurement.h"

namespace mzn {
CzInternalTemperatureMeasurement::CzInternalTemperatureMeasurement():
    Command(0, 4),
    internal_temperature_measurement() { }

uint16_t CzInternalTemperatureMeasurement::msg_to_data(std::vector<uint8_t> const & msg,
                                                       uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CzInternalTemperatureMeasurement",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = internal_temperature_measurement.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CzInternalTemperatureMeasurement::data_to_msg(std::vector<uint8_t> & msg,
                                                       uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CzInternalTemperatureMeasurement",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = internal_temperature_measurement.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CzInternalTemperatureMeasurement::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CZ_INTERNAL_TEMPERATURE_MEASUREMENT ---  \n";

    cmd_os << "\ninternal_temperature_measurement: "; cmd_os << internal_temperature_measurement;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace