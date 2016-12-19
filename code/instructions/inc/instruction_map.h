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

    // --------------------------------------------------------------------- //
    //! the sections below are the plan for the new version of instruction tree
    // --------------------------------------------------------------------- //

    // from SO:
    // --------------------------------------------------------------------- //
    struct EnumClassHash {
        template <typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    // --------------------------------------------------------------------- //
    template <typename Key>
    using HashType =
        typename std::conditional< std::is_enum<Key>::value,
                                   EnumClassHash,
                                   std::hash<Key> >::type;

    // --------------------------------------------------------------------- //
    template <typename Key, typename T>
    using EnumUnorderedMap = std::unordered_map< Key, T, HashType<Key> >;

    // --------------------------------------------------------------------- //
    /*!
        3 levels of maps
        ------------------------
        0 -> 1 : scope_action
        1 -> 2 : action_kind
        2 -> 3 : kind_option

        0====1========2========3

        q -> show --> target
        |    |        |
        |    |        status
        |    |
        |    get ---> poll
        |             |
        |             stat --> boom
        |                      |
        |                      power
        |
        s -> show --> target
        |    |        |
        |    |        status
        |    |
        |    start -> pulse
        |             |
        |             cal ---> sine
        |                      |
        |                      step
        ...
     */

    using Option = std::string;
    using VO = std::vector<Option>;
    using VK = std::vector<Kind>;
    using VA = std::vector<Action>;

    using KindOptionMap  = EnumUnorderedMap<Kind, VO const>;
    using ActionKindMap  = EnumUnorderedMap<Action, KindOptionMap const>;
    using ScopeActionMap = EnumUnorderedMap<Scope, ActionKindMap const>;

    // 2 -> 3 : kind_option
    static KindOptionMap const q_ko_map;
    static KindOptionMap const s_ko_map;

    // 1 -> 2 : action_kind
    static ActionKindMap const q_ak_map;
    static ActionKindMap const s_ak_map;

    // 0 -> 1 : scope_action
    static ScopeActionMap const sa_map;

    /*
    static
    const ScopeToActionKindMap q_ak_map;

    static
    const ActionKindMap q_kinds_map;

    */
    static
    const std::map< Scope , std::vector<Action > > map_scope_action;

};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::VO const v) {

    for( auto const & vector_element : v ) {
        os << vector_element << " ";
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::VK const v) {

    for( auto const & vector_element : v ) {
        os << vector_element << " ";
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::VA const v) {

    for( auto const & vector_element : v ) {
        os << vector_element << " ";
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::KindOptionMap const m) {

    for( auto const & map_pair : m ) {
        os << map_pair.first << " " << map_pair.second;
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::ActionKindMap const m) {

    for( auto const & map_pair : m ) {
        os << map_pair.first << " " << map_pair.second;
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          InstructionMap::ScopeActionMap const m) {

    for( auto const & map_pair : m ) {
        os << map_pair.first << " " << map_pair.second;
    }
    return os;
}

} // end namespace
#endif
