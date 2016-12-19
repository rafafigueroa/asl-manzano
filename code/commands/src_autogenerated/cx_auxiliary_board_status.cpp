#include "cx_auxiliary_board_status.h"

namespace mzn {
CxAuxiliaryBoardStatus::CxAuxiliaryBoardStatus():
    MultiCommand(0, 8),
    size_of_this_block(),
    packet_version(),
    aux_type(),
    aux_version() { }

uint16_t CxAuxiliaryBoardStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxAuxiliaryBoardStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = size_of_this_block.msg_to_data(msg, mf_begin);
    mf_begin = packet_version.msg_to_data(msg, mf_begin);
    mf_begin = aux_type.msg_to_data(msg, mf_begin);
    mf_begin = aux_version.msg_to_data(msg, mf_begin);
    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxAuxiliaryBoardStatus::data_to_msg(std::vector<uint8_t> & msg,
                                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxAuxiliaryBoardStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = size_of_this_block.data_to_msg(msg, mf_begin);
    mf_begin = packet_version.data_to_msg(msg, mf_begin);
    mf_begin = aux_type.data_to_msg(msg, mf_begin);
    mf_begin = aux_version.data_to_msg(msg, mf_begin);
    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxAuxiliaryBoardStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_AUXILIARY_BOARD_STATUS ---  \n";

    cmd_os << "\nsize_of_this_block: "; cmd_os << size_of_this_block;

    cmd_os << "\npacket_version: "; cmd_os << packet_version;

    cmd_os << "\naux_type: "; cmd_os << aux_type;

    cmd_os << "\naux_version: "; cmd_os << aux_version;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void CxAuxiliaryBoardStatus::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CyAuxiliaryBoardStatus>() } );

}
} // end namespace