#include "cx_baler_status.h"

namespace mzn {
CxBalerStatus::CxBalerStatus():
    Command(0, 48),
    serial_1_last_time(),
    serial_1_total_number_of_cycles(),
    serial_1_baler_status_and_timeouts(),
    serial_1_baler_on_off_minutes(),
    serial_2_last_time(),
    serial_2_total_number_of_cycles(),
    serial_2_baler_status_and_timeouts(),
    serial_2_baler_on_off_minutes(),
    special_last_time(),
    special_total_number_of_cycles(),
    special_baler_status_and_timeouts(),
    special_baler_on_off_minutes(),
    ethernet_last_time(),
    ethernet_total_number_of_cycles(),
    ethernet_baler_status_and_timeouts(),
    ethernet_baler_on_off_minutes() { }

uint16_t CxBalerStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                    uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxBalerStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_1_last_time.msg_to_data(msg, mf_begin);
    mf_begin = serial_1_total_number_of_cycles.msg_to_data(msg, mf_begin);
    mf_begin = serial_1_baler_status_and_timeouts.msg_to_data(msg, mf_begin);
    mf_begin = serial_1_baler_on_off_minutes.msg_to_data(msg, mf_begin);
    mf_begin = serial_2_last_time.msg_to_data(msg, mf_begin);
    mf_begin = serial_2_total_number_of_cycles.msg_to_data(msg, mf_begin);
    mf_begin = serial_2_baler_status_and_timeouts.msg_to_data(msg, mf_begin);
    mf_begin = serial_2_baler_on_off_minutes.msg_to_data(msg, mf_begin);
    mf_begin = special_last_time.msg_to_data(msg, mf_begin);
    mf_begin = special_total_number_of_cycles.msg_to_data(msg, mf_begin);
    mf_begin = special_baler_status_and_timeouts.msg_to_data(msg, mf_begin);
    mf_begin = special_baler_on_off_minutes.msg_to_data(msg, mf_begin);
    mf_begin = ethernet_last_time.msg_to_data(msg, mf_begin);
    mf_begin = ethernet_total_number_of_cycles.msg_to_data(msg, mf_begin);
    mf_begin = ethernet_baler_status_and_timeouts.msg_to_data(msg, mf_begin);
    mf_begin = ethernet_baler_on_off_minutes.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxBalerStatus::data_to_msg(std::vector<uint8_t> & msg,
                                    uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxBalerStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = serial_1_last_time.data_to_msg(msg, mf_begin);
    mf_begin = serial_1_total_number_of_cycles.data_to_msg(msg, mf_begin);
    mf_begin = serial_1_baler_status_and_timeouts.data_to_msg(msg, mf_begin);
    mf_begin = serial_1_baler_on_off_minutes.data_to_msg(msg, mf_begin);
    mf_begin = serial_2_last_time.data_to_msg(msg, mf_begin);
    mf_begin = serial_2_total_number_of_cycles.data_to_msg(msg, mf_begin);
    mf_begin = serial_2_baler_status_and_timeouts.data_to_msg(msg, mf_begin);
    mf_begin = serial_2_baler_on_off_minutes.data_to_msg(msg, mf_begin);
    mf_begin = special_last_time.data_to_msg(msg, mf_begin);
    mf_begin = special_total_number_of_cycles.data_to_msg(msg, mf_begin);
    mf_begin = special_baler_status_and_timeouts.data_to_msg(msg, mf_begin);
    mf_begin = special_baler_on_off_minutes.data_to_msg(msg, mf_begin);
    mf_begin = ethernet_last_time.data_to_msg(msg, mf_begin);
    mf_begin = ethernet_total_number_of_cycles.data_to_msg(msg, mf_begin);
    mf_begin = ethernet_baler_status_and_timeouts.data_to_msg(msg, mf_begin);
    mf_begin = ethernet_baler_on_off_minutes.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxBalerStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_BALER_STATUS ---  \n";

    cmd_os << "\nserial_1_last_time: "; cmd_os << serial_1_last_time;

    cmd_os << "\nserial_1_total_number_of_cycles: "; cmd_os << serial_1_total_number_of_cycles;

    cmd_os << "\nserial_1_baler_status_and_timeouts: "; cmd_os << serial_1_baler_status_and_timeouts;

    cmd_os << "\nserial_1_baler_on_off_minutes: "; cmd_os << serial_1_baler_on_off_minutes;

    cmd_os << "\nserial_2_last_time: "; cmd_os << serial_2_last_time;

    cmd_os << "\nserial_2_total_number_of_cycles: "; cmd_os << serial_2_total_number_of_cycles;

    cmd_os << "\nserial_2_baler_status_and_timeouts: "; cmd_os << serial_2_baler_status_and_timeouts;

    cmd_os << "\nserial_2_baler_on_off_minutes: "; cmd_os << serial_2_baler_on_off_minutes;

    cmd_os << "\nspecial_last_time: "; cmd_os << special_last_time;

    cmd_os << "\nspecial_total_number_of_cycles: "; cmd_os << special_total_number_of_cycles;

    cmd_os << "\nspecial_baler_status_and_timeouts: "; cmd_os << special_baler_status_and_timeouts;

    cmd_os << "\nspecial_baler_on_off_minutes: "; cmd_os << special_baler_on_off_minutes;

    cmd_os << "\nethernet_last_time: "; cmd_os << ethernet_last_time;

    cmd_os << "\nethernet_total_number_of_cycles: "; cmd_os << ethernet_total_number_of_cycles;

    cmd_os << "\nethernet_baler_status_and_timeouts: "; cmd_os << ethernet_baler_status_and_timeouts;

    cmd_os << "\nethernet_baler_on_off_minutes: "; cmd_os << ethernet_baler_on_off_minutes;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace