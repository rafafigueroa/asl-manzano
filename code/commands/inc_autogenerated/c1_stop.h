// ** C1Stop Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_C1_STOP_H
#define _MZN_CMD_C1_STOP_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class C1Stop : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, C1Stop const & cmd);

public:

    explicit C1Stop();
    ~C1Stop() = default;

    C1Stop(C1Stop && rhs) = default;
    C1Stop & operator=(C1Stop && rhs) = default;
    C1Stop(C1Stop const & rhs) = default;
    C1Stop & operator=(C1Stop const & rhs) = default;
    std::string const cmd_name = "c1_stop";


    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, C1Stop const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_C1_STOP_H
