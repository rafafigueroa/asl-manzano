#include "c1_stop.h"

namespace mzn {
C1Stop::C1Stop():
    Command(0x24, 0) { }

uint16_t C1Stop::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Stop::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Stop::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_STOP ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace