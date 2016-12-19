#include "cx_environmental_processor_status.h"

namespace mzn {
CxEnvironmentalProcessorStatus::CxEnvironmentalProcessorStatus():
    MultiCommand(0, 0) { }

uint16_t CxEnvironmentalProcessorStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                                     uint16_t mf_begin) {

    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxEnvironmentalProcessorStatus::data_to_msg(std::vector<uint8_t> & msg,
                                                     uint16_t mf_begin) const {

    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxEnvironmentalProcessorStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_ENVIRONMENTAL_PROCESSOR_STATUS ---  \n";
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void CxEnvironmentalProcessorStatus::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CyEnvironmentalProcessorStatus>() } );

}
} // end namespace