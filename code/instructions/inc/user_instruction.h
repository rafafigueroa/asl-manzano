// Manzano Software //

#ifndef _MZN_USER_INSTRUCTION_H_
#define _MZN_USER_INSTRUCTION_H_

#include <sstream>
#include <string>

namespace mzn {

//! One of the two components of a UserInstruction
/*! the other being Kind
    @see UserInstruction
    @see Kind
    @author rfigueroa@usgs.gov
 */

//! if more than 32 action exist, change the hash function
// -------------------------------------------------------------------------- //
enum class Action {
    // ta *
    show,
    edit,
    quit,
    // q/s/dp *
    get,
    set,
    plan,
    auto_, // auto, as in autocal is the historical term used
    stop,
    // s *
    start,
};

//! One of the two components of a UserInstruction
/*! the other being Action
    @see UserInstruction
    @see Action
    @author rfigueroa@usgs.gov
 */

//! if more than 32 kinds exist, change the hash function
// -------------------------------------------------------------------------- //
enum class Kind {

    // ta show/edit *
    target,
    command,
    config,
    status,

    // quit
    mzn,

    // q get *
    qview,
    poll,
    ping,
    global,
    dev,
    stat,

    // dp get *
    uptime,

    // q set *
    ctrl,
    reg,
    dereg,

    // s get *
    center,

    // s start *
    pulse,

    // s start/plan *
    cal,

    // * edit/show *
    plan,
};

// operator<<
// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, Action const & action) {

    switch(action) {
        case Action::show  : os << "show"; break;
        case Action::edit  : os << "edit"; break;
        case Action::quit  : os << "quit"; break;
        case Action::get   : os << "get"; break;
        case Action::set   : os << "set"; break;
        case Action::plan  : os << "plan"; break;
        case Action::auto_  : os << "auto"; break;
        case Action::stop  : os << "stop"; break;
        case Action::start : os << "start"; break;

        default : throw std::logic_error{"@UserInstruction::operator<< action"};
    }
    return os;
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, Kind const & kind) {

    switch(kind) {

        case Kind::target        : os << "target"; break;
        case Kind::command       : os << "command"; break;
        case Kind::config        : os << "config"; break;
        case Kind::status        : os << "status"; break;

        case Kind::poll          : os << "poll"; break;
        case Kind::ping          : os << "ping"; break;
        case Kind::global        : os << "global"; break;
        case Kind::dev           : os << "dev"; break;
        case Kind::center        : os << "center"; break;
        case Kind::qview         : os << "qview"; break;
        case Kind::stat          : os << "stat"; break;
        case Kind::uptime        : os << "uptime"; break;

        case Kind::ctrl          : os << "ctrl"; break;
        case Kind::cal           : os << "cal"; break;
        case Kind::pulse         : os << "pulse"; break;
        case Kind::reg           : os << "reg"; break;
        case Kind::dereg         : os << "dereg"; break;
        case Kind::mzn           : os << "manzano"; break;

        case Kind::plan           : os << "plan"; break;

        default : throw std::logic_error{"@UserInstruction::operator<< kind"};
    }
    return os;
}

//! A user instruciton is Action / Kind combination
/*! the combination creates a unique instruction
    operator() provides a unique identifiers
    @see Kind
    @see Action
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
struct UserInstruction {

    Action action;
    Kind kind;
    std::string option;
    bool live;

    UserInstruction(Action const in_action,
                    Kind const in_kind,
                    std::string const in_option = "",
                    bool const in_live = false) :
            action(in_action),
            kind(in_kind),
            option(in_option),
            live(in_live) {}

    // copy assignment
    UserInstruction & operator=(UserInstruction const &) = default;
    UserInstruction & operator=(UserInstruction &) = default;

    ~UserInstruction() = default;

    uint32_t hash() const {
        return hash(action, kind);
    }

    static
    uint32_t constexpr hash(Action const action,
                            Kind const kind) {

        // the whole hash will take 9 bits at most
        // is to be combined with a target address hash
        // action 5 bits, kinds 4 bits
        auto const h_action = static_cast<uint16_t>(action);
        auto const h_kind = static_cast<uint16_t>(kind);
        return (h_kind << 5) + h_action;
    }
};

// -------------------------------------------------------------------------- //
inline
bool operator==(UserInstruction const & lhs,
                UserInstruction const & rhs) {
    return (lhs.action == rhs.action) and
           (lhs.kind == rhs.kind);
}

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          UserInstruction const & ui) {

    os << "\n" << ui.action
       << " "   << ui.kind;

    if ( not ui.option.empty() ) {
        if (ui.live) {
            os << "/" << ui.option;
        } else {
            os << ":" << ui.option;
        }
    }

    return os;
}

} // end namespace
#endif
