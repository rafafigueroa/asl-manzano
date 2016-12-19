#include "c1_rqglob.h"

namespace mzn {
C1Rqglob::C1Rqglob():
    Command(0x1B, 0) { }

uint16_t C1Rqglob::msg_to_data(std::vector<uint8_t> const & msg,
                               uint16_t mf_begin) {


    return mf_begin;
}

uint16_t C1Rqglob::data_to_msg(std::vector<uint8_t> & msg,
                               uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & C1Rqglob::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_RQGLOB ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace