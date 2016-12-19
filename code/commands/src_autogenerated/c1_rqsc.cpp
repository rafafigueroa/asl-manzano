#include "c1_rqsc.h"

namespace mzn {
C1Rqsc::C1Rqsc():
    Command(0x2F, 0) { }

uint16_t C1Rqsc::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Rqsc::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Rqsc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQSC ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace