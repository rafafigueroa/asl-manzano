// ** C1Ping5 Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_PING_5_H
#define _MZN_CMD_C1_PING_5_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C1Ping5 : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Ping5 const & cmd);

public:

    explicit C1Ping5();
    ~C1Ping5() = default;

    C1Ping5(C1Ping5 && rhs) = default;
    C1Ping5 & operator=(C1Ping5 && rhs) = default;
    C1Ping5(C1Ping5 const & rhs) = default;
    C1Ping5 & operator=(C1Ping5 const & rhs) = default;
    std::string const cmd_name = "c1_ping_5";

    CmdField<uint16_t> ping_type;
    CmdFieldIgnore<uint16_t> ignore_0;
    CmdField<uint16_t> version;
    BmFgFix fix_flags;
    CmdField<uint32_t> kmi_property_tag;
    CmdFieldHex<uint64_t> serial_number;
    CmdField<uint32_t> data_port_1_packet_memory_size;
    CmdField<uint32_t> data_port_2_packet_memory_size;
    CmdField<uint32_t> data_port_3_packet_memory_size;
    CmdField<uint32_t> data_port_4_packet_memory_size;
    CmdField<uint32_t> serial_interface_1_memory_trigger_level;
    CmdField<uint32_t> serial_interface_2_memory_trigger_level;
    CmdFieldIgnore<uint32_t> reserved_0;
    CmdField<uint32_t> ethernet_interface_memory_trigger_level;
    BmFgSphy serial_interface_1_advanced_flags;
    BmFgSphy serial_interface_2_advanced_flags;
    CmdFieldIgnore<uint16_t> reserved_1;
    BmFgSphy ethernet_interface_advanced_flags;
    CmdField<uint16_t> serial_interface_1_data_port_number;
    CmdField<uint16_t> serial_interface_2_data_port_number;
    CmdFieldIgnore<uint16_t> reserved_2;
    CmdField<uint16_t> ethernet_interface_data_port_number;
    BmCalFail calibration_error_bitmap;
    CmdField<uint8_t> firmware_major;
    CmdField<uint8_t> firmware_minor;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Ping5 const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_PING_5_H
