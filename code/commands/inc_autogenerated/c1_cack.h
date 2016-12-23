// ** C1Cack Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_CACK_H
#define _MZN_CMD_C1_CACK_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C1Cack : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Cack const & cmd);

public:

    explicit C1Cack();
    ~C1Cack() = default;

    C1Cack(C1Cack && rhs) = default;
    C1Cack & operator=(C1Cack && rhs) = default;
    C1Cack(C1Cack const & rhs) = default;
    C1Cack & operator=(C1Cack const & rhs) = default;
    std::string const cmd_name = "c1_cack";


    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Cack const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_CACK_H
