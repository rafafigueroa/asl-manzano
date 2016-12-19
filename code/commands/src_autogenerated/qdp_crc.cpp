#include "qdp_crc.h"

namespace mzn {
QdpCrc::QdpCrc():
    Command(0x0, 4),
    crc() { }

uint16_t QdpCrc::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "QdpCrc",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = crc.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t QdpCrc::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "QdpCrc",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = crc.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & QdpCrc::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- QDP_CRC ---  \n";

    cmd_os << "\ncrc: "; cmd_os << crc;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace