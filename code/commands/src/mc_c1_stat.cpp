#include "c1_stat.h"

namespace mzn {
bool C1Stat::command_active(uint8_t const cmd_key) const {
   return status_bitmap.data_.test(cmd_key);
}
}

