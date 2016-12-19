#include "cx_gps_satellites.h"

namespace mzn {
CxGpsSatellites::CxGpsSatellites():
    MultiCommand(0, 4),
    number_of_entries(),
    size_of_this_block() { }

uint16_t CxGpsSatellites::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGpsSatellites",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_entries.msg_to_data(msg, mf_begin);
    mf_begin = size_of_this_block.msg_to_data(msg, mf_begin);
    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxGpsSatellites::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGpsSatellites",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = number_of_entries.data_to_msg(msg, mf_begin);
    mf_begin = size_of_this_block.data_to_msg(msg, mf_begin);
    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxGpsSatellites::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_GPS_SATELLITES ---  \n";

    cmd_os << "\nnumber_of_entries: "; cmd_os << number_of_entries;

    cmd_os << "\nsize_of_this_block: "; cmd_os << size_of_this_block;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void CxGpsSatellites::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CyGpsSatellites>() } );

}
} // end namespace