#include "c2_qv.h"

namespace mzn {
C2Qv::C2Qv():
    MultiCommand(0xCA, 12),
    starting_sequence_number(),
    seconds_count(),
    number_of_entries(),
    actual_channel_mask(),
    spare_0() { }

uint16_t C2Qv::msg_to_data(std::vector<uint8_t> const & msg,
                           uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Qv",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_sequence_number.msg_to_data(msg, mf_begin);
    mf_begin = seconds_count.msg_to_data(msg, mf_begin);
    mf_begin = number_of_entries.msg_to_data(msg, mf_begin);
    mf_begin = actual_channel_mask.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = MultiCommand::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C2Qv::data_to_msg(std::vector<uint8_t> & msg,
                           uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C2Qv",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_sequence_number.data_to_msg(msg, mf_begin);
    mf_begin = seconds_count.data_to_msg(msg, mf_begin);
    mf_begin = number_of_entries.data_to_msg(msg, mf_begin);
    mf_begin = actual_channel_mask.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = MultiCommand::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C2Qv::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C2_QV ---  \n";

    cmd_os << "\nstarting_sequence_number: "; cmd_os << starting_sequence_number;

    cmd_os << "\nseconds_count: "; cmd_os << seconds_count;

    cmd_os << "\nnumber_of_entries: "; cmd_os << number_of_entries;

    cmd_os << "\nactual_channel_mask: "; cmd_os << actual_channel_mask;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void C2Qv::create_new_ic(uint8_t const cmd_key) {

    inner_commands.push_back(
        std::unique_ptr<Command>{ std::make_unique<CxQv>() } );

}
} // end namespace