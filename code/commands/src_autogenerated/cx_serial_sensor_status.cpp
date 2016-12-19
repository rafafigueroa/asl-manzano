#include "cx_serial_sensor_status.h"

namespace mzn {
CxSerialSensorStatus::CxSerialSensorStatus():
    MultiCommand(0, 4),
    total_size_of_this_block(),
    number_of_sub_blocks() { }

uint16_t CxSerialSensorStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSerialSensorStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_size_of_this_block.msg_to_data(msg, mf_begin);
    mf_begin = number_of_sub_blocks.msg_to_data(msg, mf_begin);
    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxSerialSensorStatus::data_to_msg(std::vector<uint8_t> & msg,
                                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxSerialSensorStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_size_of_this_block.data_to_msg(msg, mf_begin);
    mf_begin = number_of_sub_blocks.data_to_msg(msg, mf_begin);
    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxSerialSensorStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_SERIAL_SENSOR_STATUS ---  \n";

    cmd_os << "\ntotal_size_of_this_block: "; cmd_os << total_size_of_this_block;

    cmd_os << "\nnumber_of_sub_blocks: "; cmd_os << number_of_sub_blocks;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void CxSerialSensorStatus::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CySerialSensorStatus>() } );

}
} // end namespace