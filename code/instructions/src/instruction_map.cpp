#include "instruction_map.h"

namespace mzn {

// -------------------------------------------------------------------------- //
std::vector<Action>
InstructionMap::filter_actions(TargetAddress const & ta) {

    using VA = std::vector<Action>;
    Scope scope = ta.scope();

    switch (scope) {
        case Scope::digitizer:
            return VA{Action::show,
                      Action::edit,
                      Action::get,
                      Action::set,
                      Action::stop};

        case Scope::sensor:
            return VA{Action::show,
                      Action::edit,
                      Action::set,
                      Action::start,
                      Action::auto_};

        case Scope::data_processor:
            return VA{Action::show,
                      Action::edit,
                      Action::get};

        default : return VA{Action::show, Action::edit};
    }
}

// -------------------------------------------------------------------------- //
std::vector<Kind>
InstructionMap::filter_kinds(TargetAddress const & ta, Action const action) {

    using VK = std::vector<Kind>;

    if (action == Action::edit) return VK{Kind::target,
                                          Kind::plan};

    if (action == Action::show) return VK{Kind::target,
                                          Kind::command,
                                          Kind::config,
                                          Kind::plan,
                                          Kind::status};

    if ( ta.scope_is_q() ) {

        switch (action) {

            // once the editing of options is live,
            // there will be an edit option here too
            // it will be the same as Action::get + target

            case Action::get:

                return VK{Kind::poll,
                          Kind::qview,
                          Kind::ctrl,
                          Kind::center,
                          Kind::global,
                          Kind::ping,
                          Kind::dev,
                          Kind::stat};

            case Action::set:

                return VK{Kind::ctrl,
                          Kind::reg,
                          Kind::dereg};

            case Action::stop:

                return VK{Kind::cal};

            default :

                return VK{};
        }
    }

    if ( ta.scope_is_s() ) {

        switch (action) {
            case Action::set: return VK{Kind::center};
            case Action::start: return VK{Kind::cal, Kind::pulse};
            case Action::auto_: return VK{Kind::cal};
            default : return VK{};
        }
    }

    if ( ta.scope_is_dp() ) {
        if (action == Action::get) return VK{Kind::uptime};
    }

    return VK{};
}

// -------------------------------------------------------------------------- //
std::vector<Action>
InstructionMap::filter_actions_gui(TargetAddress const & ta) {

    using VA = std::vector<Action>;

    Scope scope = ta.scope();

    switch (scope) {
        case Scope::digitizer:
            return VA{Action::get,
                      Action::set,
                      Action::start,
                      Action::stop};

        case Scope::sensor:
            return VA{Action::get,
                      Action::set,
                      Action::start,
                      Action::plan};

        case Scope::data_processor:
            return VA{Action::get};

        default : return VA{};
    }
}

// -------------------------------------------------------------------------- //
std::vector<Kind>
InstructionMap::filter_kinds_gui(TargetAddress const & ta, Action const action) {

    using VK = std::vector<Kind>;

    if ( ta.scope_is_q() ) {

        switch (action) {

            case Action::get:

                return VK{Kind::poll,
                          Kind::ping,
                          Kind::center,
                          Kind::ctrl,
                          Kind::global,
                          Kind::dev,
                          Kind::stat};

            case Action::set:

                return VK{Kind::ctrl,
                          Kind::reg,
                          Kind::dereg};

            case Action::start:

                 return VK{Kind::qview};

            case Action::stop:

                return VK{Kind::cal};

            default :

                return VK{};
        }
    }

    if ( ta.scope_is_s() ) {

        switch (action) {
            case Action::get: return VK{Kind::center};
            case Action::set: return VK{Kind::center};
            case Action::start: return VK{Kind::cal, Kind::pulse};
            case Action::plan: return VK{Kind::cal};
            default : return VK{};
        }
    }

    if ( ta.scope_is_dp() ) {
        if (action == Action::get) return VK{Kind::uptime};
    }

    return VK{};
}

// -------------------------------------------------------------------------- //
std::vector<std::string>
InstructionMap::filter_options(Action const action,
                                        Kind const kind) {

    using VS = std::vector<std::string>;

    switch (action) {

        case Action::get: {

            switch (kind) {
                case Kind::stat: return VS{"boom", "gps", "gpssat", "power",
                                           "dataport", "pll"};
                default: return VS{};
            }
        }

        case Action::set: {

            switch (kind) {
                case Kind::ctrl: return VS{"save", "reboot", "resync", "gps_on",
                                           "gps_off", "gps_cold_start"};
                default: return VS{};
            }
        }

        case Action::start: {

            switch (kind) {
                case Kind::cal: return VS{"sine", "step",
                                          "longsine", "longstep"};
                default: return VS{};
            }
        }

        default: return VS{};
    }
}

// -------------------------------------------------------------------------- //
bool InstructionMap::has_empty_option(Action const action, Kind const kind) {

    if (action == Action::set    and kind == Kind::ctrl)   return false;
    if (action == Action::start  and kind == Kind::cal)    return false;

    return true;
}


    /*
// -------------------------------------------------------------------------- //
InstructionMap::ActionToKindMap const q_kinds_map = {

    { Action::get, InstructionMap::VK{Kind::ping,
                                      Kind::poll}
    }

};


// -------------------------------------------------------------------------- //
InstructionMap::ScopeToActionKindMap const q_ak_map = {

    { Scope::digitizer, InstructionMap::VA{Kind::ping,
                                           Kind::poll}
    }

};

    */

// -------------------------------------------------------------------------- //
using IM = InstructionMap;

// -------------------------------------------------------------------------- //
// using KindOptionMap  = EnumUnorderedMap<Kind, VO const>;
// -------------------------------------------------------------------------- //
IM::KindOptionMap const InstructionMap::q_ko_map{
    {Kind::stat,
        IM::VO{"boom", "gps", "gpssat", "power", "dataport", "pll"}},
};

// -------------------------------------------------------------------------- //
IM::KindOptionMap const InstructionMap::s_ko_map{
    {Kind::cal,
        IM::VO{"sine", "step", "longsine", "longstep"}},
};

// -------------------------------------------------------------------------- //
// ActionKindMap  = EnumUnorderedMap<Action, KindOptionMap const>;
// -------------------------------------------------------------------------- //
IM::ActionKindMap const InstructionMap::q_ak_map{
    { Action::get, q_ko_map}

        /*
        IM::VK{Kind::poll,
                          Kind::qview,
                          Kind::ctrl,
                          Kind::center,
                          Kind::ping,
                          Kind::dev,
                          Kind::stat} }
                          */
};

// -------------------------------------------------------------------------- //
IM::ActionKindMap const InstructionMap::s_ak_map{

    { Action::get, s_ko_map }
};

// -------------------------------------------------------------------------- //
// using ScopeActionMap = EnumUnorderedMap<Scope, ActionKindMap const>;
// -------------------------------------------------------------------------- //
IM::ScopeActionMap const InstructionMap::sa_map{

    { Scope::digitizer, q_ak_map },
    { Scope::sensor, s_ak_map }
};

// -------------------------------------------------------------------------- //
const std::map< Scope, std::vector<Action> >
InstructionMap::map_scope_action = {

    {Scope::digitizer,      std::vector<Action> {Action::show,
                                                 Action::edit,
                                                 Action::get,
                                                 Action::set,
                                                 Action::stop,}
    },

    {Scope::sensor,         std::vector<Action> {Action::show,
                                                 Action::edit,
                                                 Action::get,
                                                 Action::set,
                                                 Action::start,
                                                 Action::plan,}
    },
};


} // <- mzn
