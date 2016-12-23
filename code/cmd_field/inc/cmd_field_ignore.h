// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_IGNORE_H
#define _MZN_CMD_FIELD_IGNORE_H

#include "cmd_field.h"

namespace mzn {

//! Overloads operator<<
/*!
    Commands have fields that are expected to be ignored
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T>
class CmdFieldIgnore : public CmdField<T> {

public:

    using data_type = T;
    using CmdField<data_type>::data;
    //! constructs base
    explicit
    CmdFieldIgnore();

    ~CmdFieldIgnore() = default;
};

// ----- operator<< overload ----- //
template <typename osT>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldIgnore<osT> const & cf) {
    // stream nothing!
    return cf_os;
}

} // << mzn

#endif
