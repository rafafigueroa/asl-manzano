#include "c1_mysn.h"

namespace mzn {
C1Mysn::C1Mysn():
    Command(0xA3, 16),
    serial_number(),
    kmi_property_tag(),
    user_tag() { }

uint16_t C1Mysn::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Mysn",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.msg_to_data(msg, mf_begin);
    mf_begin = kmi_property_tag.msg_to_data(msg, mf_begin);
    mf_begin = user_tag.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Mysn::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Mysn",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_number.data_to_msg(msg, mf_begin);
    mf_begin = kmi_property_tag.data_to_msg(msg, mf_begin);
    mf_begin = user_tag.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Mysn::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_MYSN ---  \n";

    cmd_os << "\nserial_number: "; cmd_os << serial_number;

    cmd_os << "\nkmi_property_tag: "; cmd_os << kmi_property_tag;

    cmd_os << "\nuser_tag: "; cmd_os << user_tag;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace