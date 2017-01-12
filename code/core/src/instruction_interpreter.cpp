#include "instruction_interpreter.h"
#include <algorithm>

namespace mzn {

// -------------------------------------------------------------------------- //
InstructionInterpreter::InstructionInterpreter() :
            current_ta_ ( Target(Scope::station, 0),
                          Target(Scope::digitizer,  0) ),
        cm{} {}

// -------------------------------------------------------------------------- //
template<Action action>
void InstructionInterpreter::match(UserInstruction const & ui,
                                   TargetAddress const & ta) {

    switch (ui.kind) {
        case Kind::target:  cm.run<action, Kind::target>(ui, ta);  break;
        case Kind::config:  cm.run<action, Kind::config>(ui, ta);  break;
        case Kind::status:  cm.run<action, Kind::status>(ui, ta);  break;
        case Kind::command: cm.run<action, Kind::command>(ui, ta); break;
        case Kind::poll:    cm.run<action, Kind::poll>(ui, ta);    break;
        case Kind::global:  cm.run<action, Kind::global>(ui, ta);  break;
        case Kind::ping:    cm.run<action, Kind::ping>(ui, ta);    break;
        case Kind::stat:    cm.run<action, Kind::stat>(ui, ta);    break;
        case Kind::qview:   cm.run<action, Kind::qview>(ui, ta);   break;
        case Kind::dev:     cm.run<action, Kind::dev>(ui, ta);     break;
        case Kind::ctrl:    cm.run<action, Kind::ctrl>(ui, ta);    break;
        case Kind::reg:     cm.run<action, Kind::reg>(ui, ta);     break;
        case Kind::dereg:   cm.run<action, Kind::dereg>(ui, ta);   break;
        case Kind::cal:     cm.run<action, Kind::cal>(ui, ta);     break;
        case Kind::center:  cm.run<action, Kind::center>(ui, ta);  break;
        case Kind::pulse:   cm.run<action, Kind::pulse>(ui, ta);   break;
        case Kind::uptime:  cm.run<action, Kind::uptime>(ui, ta);  break;
        case Kind::mzn:     cm.run<action, Kind::mzn>(ui, ta);     break;
        case Kind::plan:    cm.run<action, Kind::plan>(ui, ta);    break;
        default:
            throw std::logic_error{"at InstructionInterpreter::run_instruction"};
    }

}

// -------------------------------------------------------------------------- //
void InstructionInterpreter::run_instruction(UserInstruction const & ui,
                                             TargetAddress const & ta) {

    switch (ui.action) {
        case Action::show:  match<Action::show>(ui, ta);  break;
        case Action::get:   match<Action::get>(ui, ta);   break;
        case Action::set:   match<Action::set>(ui, ta);   break;
        case Action::stop:  match<Action::stop>(ui, ta);  break;
        case Action::start: match<Action::start>(ui, ta); break;
        case Action::plan:  match<Action::plan>(ui, ta);  break;
        case Action::auto_: match<Action::auto_>(ui, ta);  break;
        case Action::quit:  match<Action::quit>(ui, ta);  break;

        // a special case here due that "edit target" changes this object only
        // at this point, ta has already been merged with current_ta_
        // before the merge it, as an example: current_ta_ "st0", ta "q0"
        // after merge and before the next step: current_ta_ "st0", ta "st0q0",
        // and then after next step: current_ta_ "st0q0"
        case Action::edit: {
            if (ui.kind == Kind::target) current_ta_ = ta;
            else match<Action::edit>(ui, ta);
            break;}

        default:
            throw std::logic_error{"at InstructionInterpreter::run_instruction"};
    }
}

// -------------------------------------------------------------------------- //
void InstructionInterpreter::run_instruction(UserInstruction const & ui) {

    run_instruction(ui, current_ta_);
}

// -------------------------------------------------------------------------- //
void InstructionInterpreter::show_prompt() const {
    // calls the function in the stream output
    // which knows the name of the station
    // since it has a reference to sn
    cm.stream_output.show_prompt(current_ta_);
}

// -------------------------------------------------------------------------- //
void
InstructionInterpreter::check_instruction_map(UserInstruction const & ui) const {
    check_instruction_map(ui, current_ta_);
}

// -------------------------------------------------------------------------- //
void
InstructionInterpreter::check_instruction_map(UserInstruction const & ui,
                                              TargetAddress const & ta) const {

    auto const actions = InstructionMap::filter_actions(ta);

    if ( not ( std::find(actions.begin(), actions.end(), ui.action)
               != actions.end() ) ) {

        std::stringstream ss;
        ss << "bad action / target combination"
           << "\n  actions for target \"" << ta.target() << "\" are: {";

        for (auto const action : actions) {
            ss << action << " ";
        }

        ss << "}";

        throw WarningException( "InstructionInterpreter",
                                "check_instruction_map",
                                ss.str() );
    }

    auto const kinds = InstructionMap::filter_kinds(ta, ui.action);

    if ( not ( std::find(kinds.begin(), kinds.end(), ui.kind) != kinds.end() ) ) {

        std::stringstream ss;
        ss << "bad action / kind / target combination"
           << "\n  kinds for target \"" << ta.target() << "\""
           << " and action \"" << ui.action << "\""
           << "\n  are: {";

        for (auto const kind : kinds) {
            ss << kind << " ";
        }

        ss << "}";

        throw WarningException( "InstructionInterpreter",
                                "check_instruction_map",
                                ss.str() );
    }
}

// -------------------------------------------------------------------------- //
void
InstructionInterpreter::
merge_and_check_target_address(TargetAddress & ta) const {

    ta.add_targets_from_ta(current_ta_);
    check_ta_in_sn(ta);
}

// -------------------------------------------------------------------------- //
void
InstructionInterpreter::current_ta_remove_one_target() {
    current_ta_.remove_one_target();
}

// -------------------------------------------------------------------------- //
void
InstructionInterpreter::check_ta_in_sn(TargetAddress const & ta) const {


    std::size_t sn_child_index = 0;
    std::size_t st_child_index = 0;

    // does the station exist?
    if ( ta.sn_child.scope == Scope::station and
         ta.sn_child.index >= cm.sn.st.size() ) {

        throw WarningException("InstructionInterpreter",
                               "check_ta_in_sn",
                               "station does not exist");
    } else {
        sn_child_index = ta.sn_child.index;
    }

    // does the digitizer exist?
    if ( ta.st_child.scope == Scope::digitizer and
         ta.st_child.index >= cm.sn.st[sn_child_index].q.size() ) {

        throw WarningException("InstructionInterpreter",
                               "check_ta_in_sn",
                               "digitizer does not exist");
    } else {
        st_child_index = ta.st_child.index;
    }

    // does the data_processor exist?
    if ( ta.st_child.scope == Scope::data_processor and
         ta.st_child.index >= cm.sn.st[sn_child_index].dp.size() ) {

        throw WarningException("InstructionInterpreter",
                               "check_ta_in_sn",
                               "data processor does not exist");
    } else {
        st_child_index = ta.st_child.index;
    }

    // does the sensor exist?
    if ( ta.q_child.scope == Scope::sensor and
         ta.q_child.index >=
         cm.sn.st[sn_child_index].q[st_child_index].s.size() ) {

        throw WarningException("InstructionInterpreter",
                               "check_ta_in_sn",
                               "sensor does not exist");
    }
}

} // <- mzn
