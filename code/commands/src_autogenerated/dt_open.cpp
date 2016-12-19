#include "dt_open.h"

namespace mzn {
DtOpen::DtOpen():
    Command(0xB, 0) { }

uint16_t DtOpen::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {


    return mf_begin;
}

uint16_t DtOpen::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {


    return mf_begin;
}

std::ostream & DtOpen::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- DT_OPEN ---  \n";
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace