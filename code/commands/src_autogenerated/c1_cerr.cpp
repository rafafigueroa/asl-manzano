#include "c1_cerr.h"

namespace mzn {
C1Cerr::C1Cerr():
    Command(0xA2, 2),
    error_code() { }

uint16_t C1Cerr::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Cerr",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = error_code.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Cerr::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Cerr",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = error_code.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Cerr::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_CERR ---  \n";

    cmd_os << "\nerror_code: "; cmd_os << error_code;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace