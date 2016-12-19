// ** MultiCommandMap Class ** //
// Manzano software

#include "multi_command_map.h"
namespace mzn {

// -------------------------------------------------------------------------- //
MultiCommandMap::MultiCommandMap(uint16_t const cmd_number,
                                 uint16_t const cmd_data_size) :
        MultiCommand(cmd_number, cmd_data_size) {}

// move constructor
// -------------------------------------------------------------------------- //
MultiCommandMap::MultiCommandMap(MultiCommandMap && rhs) noexcept :
        MultiCommand( std::move(rhs) ),
        command_index_map_( std::move(rhs.command_index_map_) ) {}

// move assignment operator
// -------------------------------------------------------------------------- //
MultiCommandMap & MultiCommandMap::operator=(MultiCommandMap && rhs) noexcept {
    // avoid self-assignment
    if (this != &rhs) {
        MultiCommand::operator=( std::move(rhs) );
        command_index_map_ = std::move(rhs.command_index_map_);
    }
    return *this;
}

// -------------------------------------------------------------------------- //
void MultiCommandMap::set_command_index(uint8_t const cmd_key,
                                        int const cmd_index) {
    command_index_map_.emplace(cmd_key, cmd_index);
}

// -------------------------------------------------------------------------- //
int MultiCommandMap::command_index(uint8_t const cmd_key) const {

    // map does not allow for duplicates
    if (command_index_map_.count(cmd_key) == 1) {
        return command_index_map_.at(cmd_key);
    } else {
        throw FatalException("MultiCommandMap",
                             "command_index",
                             "bad multi command format, \
                             \nmulti command maps dont allow for duplicates");
    }
}

// -------------------------------------------------------------------------- //
uint16_t MultiCommandMap::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    // -------------------------------------------------------- //
    // now all the other possible commands depending on status map
    // from the header
    // -------------------------------------------------------- //

    // the first step is to find out which parts of the msg belong
    // to which command. This is particularly important because
    // some of the commands have an unknown size at compile time
    // so the the message must be parsed in order

    // in case this function gets called more than once
    inner_commands.clear();

    for (auto ck = 0; ck < number_of_ic( msg.size() ); ck++) {

        if ( command_active(ck) ) {

            // creates new ic and ads to inner_commands
            // pure virtual in multi_command
            create_new_ic(ck);

            //TODO: manage exception
            set_command_index(ck, inner_commands.size() - 1);

            if (inner_commands.back() != nullptr) {
                mf_begin = inner_commands.back() -> msg_to_data(msg, mf_begin);
            } else {
                throw std::logic_error("MultiCommandMap::msg_to_data");
            }
        }
    }
    return mf_begin;
}

// -------------------------------------------------------------------------- //
uint16_t MultiCommandMap::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    for (auto const & ic : inner_commands) {
        mf_begin = ic -> data_to_msg(msg, mf_begin);
    }
    return mf_begin ;
}

} // << mzn
