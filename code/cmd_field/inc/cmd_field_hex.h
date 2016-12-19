// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_HEX_H
#define _MZN_CMD_FIELD_HEX_H

#include "cmd_field.h"

namespace mzn {

//! Overloads operator<<
/*!
    @author rfigueroa@usgs.gov
    TODO? get back to T, N in general?
 */
template <typename T>
class CmdFieldHex : public CmdField<T> {

public:

    using data_type = T;
    using CmdField<data_type>::data_;

    //! constructs base
    explicit
    CmdFieldHex();

    ~CmdFieldHex() = default;
};

//! operator<< overload to show the number as hex
template <typename osT>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldHex<osT> const & cf) {

    cf_os << std::showbase << std::uppercase << std::hex
          << cf.data()
          << std::noshowbase << std::nouppercase << std::dec;

    return cf_os;
}

//! special case for uint8_t or it would be streamed as a text char
template <>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldHex<uint8_t> const & cf) {

    cf_os << std::showbase << std::uppercase << std::hex
          << static_cast<unsigned int>( cf.data() )
          << std::noshowbase << std::nouppercase << std::dec;

    return cf_os;
}

} // << mzn

#endif
