// ** C2Regresp Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C2_REGRESP_H
#define _MZN_CMD_C2_REGRESP_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C2Regresp : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C2Regresp const & cmd);

public:

    explicit C2Regresp();
    ~C2Regresp() = default;

    C2Regresp(C2Regresp && rhs) = default;
    C2Regresp & operator=(C2Regresp && rhs) = default;
    C2Regresp(C2Regresp const & rhs) = default;
    C2Regresp & operator=(C2Regresp const & rhs) = default;
    std::string const cmd_name = "c2_regresp";

    CmdField<bool, 4> registered;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C2Regresp const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C2_REGRESP_H
