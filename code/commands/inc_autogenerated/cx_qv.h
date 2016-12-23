// ** CxQv Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CX_QV_H
#define _MZN_CMD_CX_QV_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class CxQv : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, CxQv const & cmd);

public:

    explicit CxQv();
    ~CxQv() = default;

    CxQv(CxQv && rhs) = default;
    CxQv & operator=(CxQv && rhs) = default;
    CxQv(CxQv const & rhs) = default;
    CxQv & operator=(CxQv const & rhs) = default;
    std::string const cmd_name = "cx_qv";

    CmdField<uint16_t> channel;
    CmdField<uint16_t> shift_count;
    CmdFieldDuration<uint16_t> seconds_offset;
    CmdFieldIgnore<uint16_t> spare_0;
    CmdField<int32_t> starting_value;
    CmdFieldArrayByte<40> byte_difference;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CxQv const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CX_QV_H
