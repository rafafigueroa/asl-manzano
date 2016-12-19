// Manzano Software //
#ifndef _MZN_INSTRUCTION_INTERPRETER_H_
#define _MZN_INSTRUCTION_INTERPRETER_H_

#include <iostream>
#include <map>
#include <sstream>
#include <functional>

#include "comm.h"

namespace mzn {

//! Takes a run time ui calls the right template functions in Comm
/*
    Optionally takes a TargetAddress
    @see UserInput (ui)
    @see TargetAddress (ta)
    In addition provides checks to make sure the the ui/ta
    combination is correct
    @author rfigueroa@usgs.gov
 */
class InstructionInterpreter {

public:
    explicit
    InstructionInterpreter();

    ~InstructionInterpreter() = default;

private:

    //! current initialized to first digitizer in the sn
    TargetAddress current_ta_;

public:

    //! Comm runs the instructions and stores outputs
    Comm cm;

    //! metaprogramming to do the call of action/kind combinations
    template<Action action>
    void match(UserInstruction const & ui,
               TargetAddress const & ta);

    //! @calls functions in Comm
    //! called from green_manzano (user_interpreter)
    //! or from red_manzano (window)
    void run_instruction(UserInstruction const & ui,
                         TargetAddress const & ta);

    //! If a TargetAddress is not specified, this overload
    //! runs the default run_instruction(ui,ta) with the current_ta_
    void run_instruction(UserInstruction const & ui);

    //! called from green_manzano (user_interpreter)
    //! @throws warning if there is a bad action/kind/target_address combination
    //! does nothing otherwise
    void check_instruction_map(UserInstruction const & ui,
                               TargetAddress const & ta) const;

    //! @calls check_instruction_map(ui, current_ta_);
    void check_instruction_map(UserInstruction const & ui) const;

    void show_prompt() const;

    //! called from green_manzano (user_interpreter::run_user_input)
    //! check that the ta is complete or well formed
    void merge_and_check_target_address(TargetAddress & ta) const;

    //! and that all the elements in ta exist in the seismic network
    void check_ta_in_sn(TargetAddress const & ta) const;

    //! move one level up in the hierchical tree of the current_ta
    void current_ta_remove_one_target();
};

} // end namespace
#endif
