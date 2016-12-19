#include "c2_qv.h"

namespace mzn {
int C2Qv::number_of_ic(std::size_t const msg_size) const {
    // header size = 0
    return this->number_of_entries();
}
}
