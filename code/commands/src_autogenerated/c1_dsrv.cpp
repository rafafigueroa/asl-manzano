#include "c1_dsrv.h"

namespace mzn {
C1Dsrv::C1Dsrv():
    Command(0x12, 8),
    serial_number() { }

uint16_t C1Dsrv::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Dsrv",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Dsrv::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Dsrv",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Dsrv::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_DSRV ---  \n";

    cmd_os << "\nserial_number: "; cmd_os << serial_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace