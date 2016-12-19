// ** MultiCommand Class ** //
// Manzano software

#ifndef _MZN_MULTI_COMMAND_H
#define _MZN_MULTI_COMMAND_H

#include "command.h"

#include <vector>
#include <memory>

namespace mzn {
class MultiCommand : public Command {

//! Base for all commands that contains other inner commands
/*!
    @throws logic in msg_to_data, data_to_msg
    @author rfigueroa@usgs.gov
 */
public:

    //! Command::multi_cmd is set to true here only.
    MultiCommand(uint16_t const cmd_number,
                 uint16_t const cmd_data_size);

    ~MultiCommand() = default;
    MultiCommand(MultiCommand && rhs) noexcept;
    MultiCommand & operator=(MultiCommand && rhs) noexcept;
    //MultiCommand(MultiCommand const &) = default;
    //! Stores pointers to the inner commands, starts empty.
    /*! create_new_ic managed in create_new_ic.
        ic : inner_commands
        mc : multi_command
     */
    std::vector< std::unique_ptr<Command> > inner_commands;

protected:

    //! Calls msg_to_data on all inner commands
    /*! @throws logic msg is too short inside inner_commands
        auto generated in derived classes
     */
    uint16_t msg_to_data(std::vector<uint8_t> const & msg,
                         uint16_t mf_begin) override;

    //! Calls data_to_msg on all inner commands
    /*! @throws logic msg is too short inside inner_commands
        auto generated in derived classes
     */
    uint16_t data_to_msg(std::vector<uint8_t> & msg,
                         uint16_t mf_begin) const override;

    //! Pure virtual, Creates a new command, adds to ic.
    //! auto generated in derived classes
    virtual
    void create_new_ic(uint8_t const cmd_key) = 0;

    //! Pure virtual,
    /*! NOT auto generated in derived classes
     */
    virtual
    int number_of_ic(std::size_t const msg_size) const = 0;

    //! streams all commands in inner_commands
    std::ostream & os_print(std::ostream & cmd_os) const override;
};

} // << mzn
#endif // _MZN_MULTI_COMMAND_
