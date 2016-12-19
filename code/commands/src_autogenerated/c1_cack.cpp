#include "c1_cack.h"

namespace mzn {
C1Cack::C1Cack():
    Command(0xA0, 0) { }

uint16_t C1Cack::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Cack::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Cack::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_CACK ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace