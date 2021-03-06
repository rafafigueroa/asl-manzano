// ** DtOpen Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_DT_OPEN_H
#define _MZN_CMD_DT_OPEN_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class DtOpen : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, DtOpen const & cmd);

public:

    explicit DtOpen();
    ~DtOpen() = default;

    DtOpen(DtOpen && rhs) = default;
    DtOpen & operator=(DtOpen && rhs) = default;
    DtOpen(DtOpen const & rhs) = default;
    DtOpen & operator=(DtOpen const & rhs) = default;
    std::string const cmd_name = "dt_open";


    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, DtOpen const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_DT_OPEN_H
