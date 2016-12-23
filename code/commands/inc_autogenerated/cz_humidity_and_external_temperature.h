// ** CzHumidityAndExternalTemperature Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CZ_HUMIDITY_AND_EXTERNAL_TEMPERATURE_H
#define _MZN_CMD_CZ_HUMIDITY_AND_EXTERNAL_TEMPERATURE_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class CzHumidityAndExternalTemperature : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, CzHumidityAndExternalTemperature const & cmd);

public:

    explicit CzHumidityAndExternalTemperature();
    ~CzHumidityAndExternalTemperature() = default;

    CzHumidityAndExternalTemperature(CzHumidityAndExternalTemperature && rhs) = default;
    CzHumidityAndExternalTemperature & operator=(CzHumidityAndExternalTemperature && rhs) = default;
    CzHumidityAndExternalTemperature(CzHumidityAndExternalTemperature const & rhs) = default;
    CzHumidityAndExternalTemperature & operator=(CzHumidityAndExternalTemperature const & rhs) = default;
    std::string const cmd_name = "cz_humidity_and_external_temperature";

    CmdField<int32_t> humidity_measurement;
    CmdField<int32_t> external_temperature_measurement;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CzHumidityAndExternalTemperature const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CZ_HUMIDITY_AND_EXTERNAL_TEMPERATURE_H
