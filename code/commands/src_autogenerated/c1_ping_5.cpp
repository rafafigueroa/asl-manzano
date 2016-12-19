#include "c1_ping_5.h"

namespace mzn {
C1Ping5::C1Ping5():
    Command(0x38, 72),
    ping_type(),
    ignore_0(),
    version(),
    fix_flags(),
    kmi_property_tag(),
    serial_number(),
    data_port_1_packet_memory_size("bytes"),
    data_port_2_packet_memory_size("bytes"),
    data_port_3_packet_memory_size("bytes"),
    data_port_4_packet_memory_size("bytes"),
    serial_interface_1_memory_trigger_level("bytes"),
    serial_interface_2_memory_trigger_level("bytes"),
    reserved_0(),
    ethernet_interface_memory_trigger_level("bytes"),
    serial_interface_1_advanced_flags(),
    serial_interface_2_advanced_flags(),
    reserved_1(),
    ethernet_interface_advanced_flags(),
    serial_interface_1_data_port_number(),
    serial_interface_2_data_port_number(),
    reserved_2(),
    ethernet_interface_data_port_number(),
    calibration_error_bitmap(),
    firmware_major(),
    firmware_minor() { }

uint16_t C1Ping5::msg_to_data(std::vector<uint8_t> const & msg,
                              uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping5",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = version.msg_to_data(msg, mf_begin);
    mf_begin = fix_flags.msg_to_data(msg, mf_begin);
    mf_begin = kmi_property_tag.msg_to_data(msg, mf_begin);
    mf_begin = serial_number.msg_to_data(msg, mf_begin);
    mf_begin = data_port_1_packet_memory_size.msg_to_data(msg, mf_begin);
    mf_begin = data_port_2_packet_memory_size.msg_to_data(msg, mf_begin);
    mf_begin = data_port_3_packet_memory_size.msg_to_data(msg, mf_begin);
    mf_begin = data_port_4_packet_memory_size.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_1_memory_trigger_level.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_2_memory_trigger_level.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ethernet_interface_memory_trigger_level.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_1_advanced_flags.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_2_advanced_flags.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ethernet_interface_advanced_flags.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_1_data_port_number.msg_to_data(msg, mf_begin);
    mf_begin = serial_interface_2_data_port_number.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ethernet_interface_data_port_number.msg_to_data(msg, mf_begin);
    mf_begin = calibration_error_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = firmware_major.msg_to_data(msg, mf_begin);
    mf_begin = firmware_minor.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Ping5::data_to_msg(std::vector<uint8_t> & msg,
                              uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Ping5",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = ping_type.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = version.data_to_msg(msg, mf_begin);
    mf_begin = fix_flags.data_to_msg(msg, mf_begin);
    mf_begin = kmi_property_tag.data_to_msg(msg, mf_begin);
    mf_begin = serial_number.data_to_msg(msg, mf_begin);
    mf_begin = data_port_1_packet_memory_size.data_to_msg(msg, mf_begin);
    mf_begin = data_port_2_packet_memory_size.data_to_msg(msg, mf_begin);
    mf_begin = data_port_3_packet_memory_size.data_to_msg(msg, mf_begin);
    mf_begin = data_port_4_packet_memory_size.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_1_memory_trigger_level.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_2_memory_trigger_level.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ethernet_interface_memory_trigger_level.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_1_advanced_flags.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_2_advanced_flags.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ethernet_interface_advanced_flags.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_1_data_port_number.data_to_msg(msg, mf_begin);
    mf_begin = serial_interface_2_data_port_number.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ethernet_interface_data_port_number.data_to_msg(msg, mf_begin);
    mf_begin = calibration_error_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = firmware_major.data_to_msg(msg, mf_begin);
    mf_begin = firmware_minor.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Ping5::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_PING_5 ---  \n";

    cmd_os << "\nping_type: "; cmd_os << ping_type;

    cmd_os << "\nversion: "; cmd_os << version;

    cmd_os << "\nfix_flags: "; cmd_os << fix_flags;

    cmd_os << "\nkmi_property_tag: "; cmd_os << kmi_property_tag;

    cmd_os << "\nserial_number: "; cmd_os << serial_number;

    cmd_os << "\ndata_port_1_packet_memory_size: "; cmd_os << data_port_1_packet_memory_size;

    cmd_os << "\ndata_port_2_packet_memory_size: "; cmd_os << data_port_2_packet_memory_size;

    cmd_os << "\ndata_port_3_packet_memory_size: "; cmd_os << data_port_3_packet_memory_size;

    cmd_os << "\ndata_port_4_packet_memory_size: "; cmd_os << data_port_4_packet_memory_size;

    cmd_os << "\nserial_interface_1_memory_trigger_level: "; cmd_os << serial_interface_1_memory_trigger_level;

    cmd_os << "\nserial_interface_2_memory_trigger_level: "; cmd_os << serial_interface_2_memory_trigger_level;

    cmd_os << "\nethernet_interface_memory_trigger_level: "; cmd_os << ethernet_interface_memory_trigger_level;

    cmd_os << "\nserial_interface_1_advanced_flags: "; cmd_os << serial_interface_1_advanced_flags;

    cmd_os << "\nserial_interface_2_advanced_flags: "; cmd_os << serial_interface_2_advanced_flags;

    cmd_os << "\nethernet_interface_advanced_flags: "; cmd_os << ethernet_interface_advanced_flags;

    cmd_os << "\nserial_interface_1_data_port_number: "; cmd_os << serial_interface_1_data_port_number;

    cmd_os << "\nserial_interface_2_data_port_number: "; cmd_os << serial_interface_2_data_port_number;

    cmd_os << "\nethernet_interface_data_port_number: "; cmd_os << ethernet_interface_data_port_number;

    cmd_os << "\ncalibration_error_bitmap: "; cmd_os << calibration_error_bitmap;

    cmd_os << "\nfirmware_major: "; cmd_os << firmware_major;

    cmd_os << "\nfirmware_minor: "; cmd_os << firmware_minor;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace