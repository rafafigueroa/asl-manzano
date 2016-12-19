// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_ARRAY_BYTE_H
#define _MZN_CMD_FIELD_ARRAY_BYTE_H

#include "cmd_field_array.h"

namespace mzn {

//! Array of int8_t, when intended to be printed/used as numbers
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldArrayByte : public CmdFieldArray<int8_t, N> {

public:

    using data_type = typename CmdFieldArray<int8_t, N>::data_type;

    //! initializes base
    explicit
    CmdFieldArrayByte();

    ~CmdFieldArrayByte() = default;
};

// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldArrayByte<osN> const & cf) {

    cf_os << "[";
    for (auto const & b : cf.data() ) {
        cf_os << static_cast<int>(b) << " ";
    }
    cf_os << "]";
    return cf_os;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
inline
CmdFieldArrayByte<N>::CmdFieldArrayByte() :
        CmdFieldArray<typename data_type::value_type, N>{} {};
} // << mzn

#endif
