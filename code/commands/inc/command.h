// ** Command Class ** //
// Manzano software

#ifndef _MZN_COMMAND_H
#define _MZN_COMMAND_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "cmd_field.h"
#include "cmd_field_array_byte.h"
#include "cmd_field_array_char.h"
#include "cmd_field_array_ubyte.h"
#include "cmd_field_bitmap.h"
#include "cmd_field_bitmap_types.h"
#include "cmd_field_cal_amplitude.h"
#include "cmd_field_hex.h"
#include "cmd_field_pstring.h"
#include "cmd_field_ignore.h"
#include "cmd_field_ip.h"
#include "cmd_field_time.h"
#include "cmd_field_duration.h"

#include "mzn_except.h"

namespace mzn {

//! Base for all commands to/from digitizer
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
class Command {

//! calls virtual protected function os_print
friend
std::ostream & operator<<(std::ostream & cmd_os, Command const & cmd);

public:
    //! initializes the member variables
    // --------------------------------------------------------------------- //
    Command(uint8_t const cmd_number,
            uint16_t const cmd_data_size,
            bool const multi_cmd = false);

    // --------------------------------------------------------------------- //
    ~Command() = default;

    // --------------------------------------------------------------------- //
    Command(Command && rhs) noexcept :
            cmd_number_(rhs.cmd_number_),
            cmd_data_size_(rhs.cmd_data_size_),
            multi_cmd_(rhs.multi_cmd_) {}

    // --------------------------------------------------------------------- //
    Command & operator=(Command && rhs) noexcept {
        cmd_number_ = rhs.cmd_number_;
        cmd_data_size_ = rhs.cmd_data_size_;
        multi_cmd_ = rhs.multi_cmd_;
        return *this;
    }

    // --------------------------------------------------------------------- //
    Command & operator=(Command const & rhs) noexcept {
        cmd_number_ = rhs.cmd_number_;
        cmd_data_size_ = rhs.cmd_data_size_;
        multi_cmd_ = rhs.multi_cmd_;
        return *this;
    }

    // --------------------------------------------------------------------- //
    Command(Command const & rhs) noexcept :
            cmd_number_(rhs.cmd_number_),
            cmd_data_size_(rhs.cmd_data_size_),
            multi_cmd_(rhs.multi_cmd_) {}

    // --------------------------------------------------------------------- //
    uint8_t const cmd_number() const {return cmd_number_;}
    uint8_t const cmd_data_size() const {return cmd_data_size_;}
    uint8_t const multi_cmd() const {return multi_cmd_;}

protected:
    //! Corresponds to the command number in the qdp header.
    /*! used in message_dispatch.cpp to check which msg was received
        for the different commands, e.g.: 0X23 for qcal.
     */
    uint8_t cmd_number_;

    //! The sum of all N for all the cmd_field.
    /*! used in message_dispatch.cpp to fill the msg length in qdp header.
        used in message_dispatch.cpp to know the size of expected msg.
        used in udp_connection.cpp to fit the msg size.
        used in all cmds to check msg size in msg_to_data and data_to_msg
     */
    uint16_t cmd_data_size_;

    //! is this a command that contains other commands?
    /*! used in message_dispatch.cpp to setup the size of expected mc msgs.
     */
    bool multi_cmd_;

public:
    //! pure virtual, calls data_to_msg on cmd fields in order
    //! @throws logic msg too short
    // --------------------------------------------------------------------- //
    virtual
    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const noexcept(false) = 0;

    //! pure virtual, calls msg_to_data on cmd fields in order
    //! @throws logic msg too short
    // --------------------------------------------------------------------- //
    virtual
    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) noexcept(false) = 0;

protected:

    //! Pure virtual, called by operator<<
    virtual
    std::ostream & os_print(std::ostream & os) const = 0;
};

inline
std::ostream & operator<<(std::ostream & cmd_os,
                          Command const & cmd) {

    return cmd.os_print(cmd_os);
}

} // << mzn
#endif // _MZN_COMMAND_H_
