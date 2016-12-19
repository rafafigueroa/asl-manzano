// ** MultiCommandMap Class ** //
// Manzano software

#ifndef _MZN_MULTI_COMMAND_MAP_H
#define _MZN_MULTI_COMMAND_MAP_H

#include "multi_command.h"
#include <map>

namespace mzn {
//! MultiCommand when the inner commands are setup as a map
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */

class MultiCommandMap : public MultiCommand {

public:

    MultiCommandMap(uint16_t const cmd_number,
                    uint16_t const cmd_data_size);

    ~MultiCommandMap() = default;
    MultiCommandMap(MultiCommandMap && rhs) noexcept;
    MultiCommandMap & operator=(MultiCommandMap && rhs) noexcept;
    //MultiCommandMap & operator=(MultiCommandMap const && rhs) const noexcept {};
    //MultiCommandMap(MultiCommandMap const &) = default;

protected:

    //! pure
    /*! reads the command header to see if this command should
       be consider
     */
    virtual
    bool command_active(uint8_t const cmd_key) const = 0;

    //! returns the cmd index in the map
    int command_index(uint8_t const cmd_key) const;

    //! set inside create_new_ic
    void set_command_index(uint8_t const cmd_key,
                           int const cmd_index);

    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

private:

    //! map of command_index for cmd_key
    std::map <uint8_t, int> command_index_map_;
};

} // << mzn
#endif // _MZN_MULTI_COMMAND_MAP_
