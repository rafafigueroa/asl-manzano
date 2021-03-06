// ** F1RawInput Class ** //
// Autogenerated in main_create_cmds.cpp from cmds_format.json ** //
// Manzano Software //

#ifndef _MZN_CMD_F1_RAW_INPUT_H
#define _MZN_CMD_F1_RAW_INPUT_H

#include "command.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class F1RawInput : public Command {

friend std::ostream & operator<<(std::ostream & cmd_os, F1RawInput const & cmd);

public:

    explicit F1RawInput();
    ~F1RawInput() = default;

    F1RawInput(F1RawInput && rhs) = default;
    F1RawInput & operator=(F1RawInput && rhs) = default;
    F1RawInput(F1RawInput const & rhs) = default;
    F1RawInput & operator=(F1RawInput const & rhs) = default;
    std::string const cmd_name = "f1_raw_input";

    CmdFieldHex<uint16_t> endian_check;
    CmdField<uint16_t> number_of_samples;
    CmdFieldArrayChar<12> seed_name;
    CmdField<uint16_t> year;
    CmdField<uint16_t> doy;
    CmdField<int16_t> sample_rate_factor;
    CmdField<int16_t> sample_rate_multiplier;
    BmSeedHeaderActivityFlags activity_flags;
    BmSeedHeaderIoAndClockFlags io_and_clock_flags;
    BmSeedHeaderDataQualityFlags data_quality_flags;
    CmdFieldHex<uint8_t> timing_quality;
    CmdFieldDuration<uint32_t> sec;
    CmdFieldDuration<uint32_t, std::micro> usec;
    CmdField<uint32_t> sequence_number;

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    std::ostream & os_print(std::ostream & cmd_os) const override;
};


inline std::ostream & operator<<(std::ostream & cmd_os, F1RawInput const & cmd) {
    return cmd.os_print(cmd_os);
}

} // end namespace
#endif // _MZN_CMD_F1_RAW_INPUT_H
