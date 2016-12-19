#include "cx_serial_sensor_status.h"
namespace mzn {
int CxSerialSensorStatus::number_of_ic(std::size_t const msg_size) const {

   return this->number_of_sub_blocks();

}
}
