// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_ARRAY_CHAR_H
#define _MZN_CMD_FIELD_ARRAY_CHAR_H

#include "cmd_field_array.h"

namespace mzn {

//! Array of chars, when intended to be printed as text (e.g. ascii) chars
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldArrayChar : public CmdFieldArray<char, N> {

public:

    using data_type = typename CmdFieldArray<char, N>::data_type;

    //! initializes base
    explicit
    CmdFieldArrayChar();

    ~CmdFieldArrayChar() = default;
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldArrayChar<osN> const & cf) {

    cf_os << "'";
    for (auto const & c : cf.data() ) cf_os << c;
    cf_os << "'";

    return cf_os;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
CmdFieldArrayChar<N>::CmdFieldArrayChar() :
        CmdFieldArray<typename data_type::value_type, N>{} {};

} // << mzn

#endif
