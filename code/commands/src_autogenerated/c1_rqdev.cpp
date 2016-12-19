#include "c1_rqdev.h"

namespace mzn {
C1Rqdev::C1Rqdev():
    Command(0x36, 0) { }

uint16_t C1Rqdev::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Rqdev::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Rqdev::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQDEV ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace