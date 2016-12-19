#include "cx_arp_status.h"

namespace mzn {
int CxArpStatus::number_of_ic(std::size_t const msg_size) const {
    // this is called after base_set_data
    // the header data is available

   return this->number_of_entries();
}

}
