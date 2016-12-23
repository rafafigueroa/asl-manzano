// ** CxGpsSatellites Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_CX_GPS_SATELLITES_H
#define _MZN_CMD_CX_GPS_SATELLITES_H

#include "command.h"


#include "cy_gps_satellites.h"

#define k_cy_gps_satellites 0

#include "multi_command.h"
namespace mzn {

// -------------------------------------------------------------------------- //
class CxGpsSatellites : public MultiCommand {

friend std::ostream & operator<<(std::ostream & cmd_os, CxGpsSatellites const & cmd);

public:

    explicit CxGpsSatellites();
    ~CxGpsSatellites() = default;

    CxGpsSatellites(CxGpsSatellites && rhs) = default;
    CxGpsSatellites & operator=(CxGpsSatellites && rhs) = default;
    std::string const cmd_name = "cx_gps_satellites";

    CmdField<uint16_t> number_of_entries;
    CmdField<uint16_t> size_of_this_block;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;
    // max keys implementation in a separate function/file
    // not part of auto generation
    int number_of_ic(std::size_t const msg_size) const override;
    void create_new_ic(uint8_t const  cmd_key) override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, CxGpsSatellites const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_CX_GPS_SATELLITES_H
