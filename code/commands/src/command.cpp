// ** Command Class ** //
// Manzano software

#include "command.h"

namespace mzn {

Command::Command(uint8_t const cmd_number,
                 uint16_t const cmd_data_size,
                 bool const multi_cmd /* false */) :
        cmd_number_{cmd_number},
        cmd_data_size_{cmd_data_size},
        multi_cmd_{multi_cmd} {}

} // << mzn
