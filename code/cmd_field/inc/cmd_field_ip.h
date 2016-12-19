// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_IP_H
#define _MZN_CMD_FIELD_IP_H

#include "cmd_field.h"

namespace mzn {

//! Overloads operator<<
/*!
    @author rfigueroa@usgs.gov */

class CmdFieldIp : public CmdField<uint32_t> {

public:

    using data_type = uint32_t;
    using CmdField<data_type>::data_;

    //! constructs base
    explicit
    CmdFieldIp();

    ~CmdFieldIp() = default;
};

// ----- operator<< overload ----- //
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldIp const & cf) {

    cf_os << ( (cf.data() >> 24)  & 0x000000FF ) << "."
          << ( (cf.data() >> 16)  & 0x000000FF ) << "."
          << ( (cf.data() >> 8)   & 0x000000FF ) << "."
          << ( (cf.data() >> 0)   & 0x000000FF );

    return cf_os;
}

} // << mzn

#endif
