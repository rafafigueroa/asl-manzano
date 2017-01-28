// Manzano Software //
#ifndef _MZN_INSTRUCTION_MAP_H_
#define _MZN_INSTRUCTION_MAP_H_

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>

#include "user_instruction.h"
#include "target_address.h"

namespace mzn {

//! Namespace for functions describing the acceptable combinations
/*! of Action, Kind and TargetAddress
    @see Action
    @see Kind
    @see TargetAddress
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class InstructionMap {

public:

    // --------------------------------------------------------------------- //
    static
    std::vector<Action> filter_actions(TargetAddress const & ta);

    // --------------------------------------------------------------------- //
    static
    std::vector<Kind> filter_kinds(TargetAddress const & ta,
                                   Action const action);

    // --------------------------------------------------------------------- //
    static
    std::vector<Action> filter_actions_gui(TargetAddress const & ta);

    // --------------------------------------------------------------------- //
    static
    std::vector<Kind> filter_kinds_gui(TargetAddress const & ta,
                                       Action const action);

    //! @tparam T data type of the string.
    // --------------------------------------------------------------------- //
    static
    std::vector<std::string> filter_options(Action const action,
                                            Kind const kind);

    //! some of the commands, like set ctrl, don't have a default option
    //! cannot be ran until the user specifies the specific option
    // --------------------------------------------------------------------- //
    static
    bool has_empty_option(Action const action,
                          Kind const kind);
};

} // end namespace
#endif
