#include "cy_serial_sensor_status.h"
namespace mzn {

bool CySerialSensorStatus::command_active(uint8_t const cmd_key) const {
   //{"key_name": "cz_internal_temperature_measurement"}= 0
   //{"key_name": "cz_humidity_and_external_temperature"}= 1

    if(cmd_key == 0) {
        return valid_fields.internal_temperature_measurement_is_valid();
    } else  {
        return ( valid_fields.humidity_measurement_is_valid() or
                 valid_fields.external_temperature_measurement_is_valid() );
    }
}
}

