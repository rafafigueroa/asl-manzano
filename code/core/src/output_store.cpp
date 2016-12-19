// Manzano software
#include "output_store.h"
namespace mzn {

// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
    /*
void OutputStore::get_output_cmd(UserInstruction const & ui,
                                 TargetAddress const & ta,
                                 Command * const cmd) {

    switch (ui.action) {
        case Action::show:  match<Action::show>(ui, ta, cmd);  break;
        case Action::get:   match<Action::get>(ui, ta, cmd);   break;
        case Action::set:   match<Action::set>(ui, ta, cmd);   break;
        case Action::stop:  match<Action::stop>(ui, ta, cmd);  break;
        case Action::start: match<Action::start>(ui, ta, cmd); break;
        case Action::plan:  match<Action::plan>(ui, ta, cmd);  break;
        case Action::quit:  match<Action::quit>(ui, ta, cmd);  break;
        default:
            throw std::logic_error{"at OutputStore::run_instruction"};
    }

}


template<Action action>
void OutputStore::match(UserInstruction const & ui,
                        TargetAddress const & ta,
                        Command * const cmd) {

    switch (ui.kind) {
        case Kind::target:  cm.run<action, Kind::target>(ui, ta);  break;
        case Kind::config:  cm.run<action, Kind::config>(ui, ta);  break;
        case Kind::status:  cm.run<action, Kind::status>(ui, ta);  break;
        case Kind::command: cm.run<action, Kind::command>(ui, ta); break;
        case Kind::poll:    cm.run<action, Kind::poll>(ui, ta);    break;
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
        default:
            throw std::logic_error{"at OutputStore::run_instruction"};
    }

}
*/

} // <- mzn
