// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_ARRAY_UBYTE_H
#define _MZN_CMD_FIELD_ARRAY_UBYTE_H

#include "cmd_field_array.h"

namespace mzn {

//! Array of unsigned bytes
//! @see CmdFieldArray
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <std::size_t N>
class CmdFieldArrayUByte : public CmdFieldArray<uint8_t, N> {

public:

    using data_type = typename CmdFieldArray<uint8_t, N>::data_type;

    //! initializes base
    explicit
    CmdFieldArrayUByte();

    ~CmdFieldArrayUByte() = default;
};

// print as number, not text character
// -------------------------------------------------------------------------- //
template <std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldArrayUByte<osN> const & cf) {

    cf_os << "[";
    //<< std::showbase << std::uppercase << std::hex;
    for ( auto const & b : cf.data() ) {
        cf_os << static_cast<unsigned int>(b) << " ";
    }
    cf_os << "]";
    // << std::noshowbase << std::nouppercase << std::dec;
    return cf_os;
}

// -------------------------------------------------------------------------- //
template <std::size_t N>
CmdFieldArrayUByte<N>::CmdFieldArrayUByte() :
        CmdFieldArray<typename data_type::value_type, N>{} {};

} // << mzn

#endif
