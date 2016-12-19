#include "cy_auxiliary_board_status.h"

namespace mzn {
CyAuxiliaryBoardStatus::CyAuxiliaryBoardStatus():
    Command(0, 4),
    conversion() { }

uint16_t CyAuxiliaryBoardStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyAuxiliaryBoardStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = conversion.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CyAuxiliaryBoardStatus::data_to_msg(std::vector<uint8_t> & msg,
                                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyAuxiliaryBoardStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = conversion.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CyAuxiliaryBoardStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CY_AUXILIARY_BOARD_STATUS ---  \n";

    cmd_os << "\nconversion: "; cmd_os << conversion;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace