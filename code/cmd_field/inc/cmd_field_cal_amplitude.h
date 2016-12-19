// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_CAL_AMPLITUDE_H
#define _MZN_CMD_FIELD_CAL_AMPLITUDE_H

#include "cmd_field.h"

namespace mzn {

//! Overloads base operator(), operator<<
/*!
    @author rfigueroa@usgs.gov */
class CmdFieldCalAmplitude : public CmdField<int16_t> {
public:

    using data_type = int16_t;
    using CmdField<data_type>::data;

    //! constructs base
    explicit
    CmdFieldCalAmplitude();

    ~CmdFieldCalAmplitude() = default;

    //! from bit shift to value in dB (negative)
    data_type operator()() const;

    //! from value in dB (negative) to bit shift
    void operator()(data_type const & in_data);
};

// operator<< overload
// is the same as in CmdField but calls the operator() from this class
// no virtual functions are being used for the CmdField inheritance tree
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldCalAmplitude const & cf) {
    cf_os << cf();
    return cf_os;
}

} // << mzn

#endif
