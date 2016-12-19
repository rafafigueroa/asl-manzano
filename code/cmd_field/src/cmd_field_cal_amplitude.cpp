// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_cal_amplitude.h"
namespace mzn {

CmdFieldCalAmplitude::CmdFieldCalAmplitude() :
        CmdField<int16_t>{} {};

int16_t CmdFieldCalAmplitude::operator()() const {
    // always negative
    return - (1 + data() ) * 6;
}

void CmdFieldCalAmplitude::operator()(int16_t const & in_data) {

    if (in_data > -6 or in_data % -6 != 0 )
        throw WarningException("CmdFieldCalAmplitude",
                               "operator()",
                               "calibration value to set is not a multiple of -6");

    data_ = - (in_data/6 + 1);
}

}// << mzn

