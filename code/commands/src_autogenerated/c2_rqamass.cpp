#include "c2_rqamass.h"

namespace mzn {
C2Rqamass::C2Rqamass():
    Command(0x58, 0) { }

uint16_t C2Rqamass::msg_to_data(std::vector<uint8_t> const & msg,
                                uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C2Rqamass::data_to_msg(std::vector<uint8_t> & msg,
                                uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C2Rqamass::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_RQAMASS ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace