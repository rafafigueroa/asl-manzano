// ** MultiCommand Class ** //
// Manzano software

#include "multi_command.h"
namespace mzn {

// normal constructor
// -------------------------------------------------------------------------- //
MultiCommand::MultiCommand(uint16_t const cmd_number,
                           uint16_t const cmd_data_size) :
        Command(cmd_number, cmd_data_size, true),
        inner_commands{} {}

// move constructor
// -------------------------------------------------------------------------- //
MultiCommand::MultiCommand(MultiCommand && rhs) noexcept :
        Command( std::move(rhs) ),
        inner_commands( std::move(rhs.inner_commands) ) {}

/*
// move assignment operator
// -------------------------------------------------------------------------- //
MultiCommand & MultiCommand::operator=(MultiCommand && rhs) noexcept {
    // avoid self-assignment
    if (this != &rhs) {
        Command::operator=( std::move(rhs) );
        inner_commands = std::move(rhs.inner_commands);
    }
    return *this;
}
*/

// -------------------------------------------------------------------------- //
uint16_t MultiCommand::msg_to_data(std::vector<uint8_t> const & msg,
                                   uint16_t mf_begin) {

    // in case this function gets called more than once
    inner_commands.clear();

    for (int ck = 0; ck < number_of_ic( msg.size() ); ck++) {
        // new inner command, pure virtual function
        create_new_ic(ck);
        // msg_to_data on inner commands
        mf_begin = inner_commands.back() -> msg_to_data(msg, mf_begin);
    }
    return mf_begin ;
}

// -------------------------------------------------------------------------- //
uint16_t MultiCommand::data_to_msg(std::vector<uint8_t> & msg,
                                   uint16_t mf_begin) const {

    for (auto const & ic : inner_commands) {
        mf_begin = ic -> data_to_msg(msg, mf_begin);
    }
    return mf_begin ;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & MultiCommand::os_print(std::ostream & cmd_os) const {

    for (auto const & ic : inner_commands) {
        cmd_os << *(ic);
        cmd_os << "\n";
    }
    return cmd_os;
}

} // << mzn
