#include "c1_dev.h"

namespace mzn {
C1Dev::C1Dev():
    MultiCommand(0xB3, 0) { }

uint16_t C1Dev::msg_to_data(std::vector<uint8_t> const & msg,
                            uint16_t mf_begin) {

    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Dev::data_to_msg(std::vector<uint8_t> & msg,
                            uint16_t mf_begin) const {

    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Dev::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_DEV ---  \n";
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void C1Dev::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CxDev>() } );

}
} // end namespace