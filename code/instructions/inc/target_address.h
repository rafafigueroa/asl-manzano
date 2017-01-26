// Manzano Software //
#ifndef _MZN_TARGET_ADDRESS_H_
#define _MZN_TARGET_ADDRESS_H_

#include <sstream>
#include <stdexcept>

#include "target.h"
#include "mzn_except.h"

namespace mzn {

//! Provides the information needed to identify a seismic network component
/*! A target address can be incomplete or partial,
    and then merge together with another to form a full target address.
    It can be passed around cheaply and concentrates all the utilities of
    figuring out what is the intended target.
    @see TargetAddress
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
struct TargetAddress {

    //! sn childs : station
    Target sn_child = Target{Scope::none_};
    //! st childs : digitizer, data_processor
    Target st_child = Target{Scope::none_};
    //! q childs : sensor
    Target q_child  = Target{Scope::none_};

    // --------------------------------------------------------------------- //
    TargetAddress() = default;
    ~TargetAddress() = default;

    // --------------------------------------------------------------------- //
    explicit
    TargetAddress(Target in_sn_child,
                  Target in_st_child = Target{Scope::none_},
                  Target in_q_child  = Target{Scope::none_}) :
            sn_child(in_sn_child),
            st_child(in_st_child),
            q_child(in_q_child) {}

    //! called from green_manzano (user_interpreter::run_user_input)
    //! when this ta is not complete, it can add parents from other ta
    //! in this case the current_ta_ from InstructionInterpreter
    // for use with an incomplete ta, where from_ta completes
    //! @throws warning if fails
    // --------------------------------------------------------------------- //
    void add_targets_from_ta(TargetAddress const & from_ta);

    //! called from green_manzano, to add one target at a time from
    //! a series of targets read from a string
    //! @throws warning if fails
    // --------------------------------------------------------------------- //
    void add_target(Target const & ta);

    //! or is full address
    // --------------------------------------------------------------------- //
    inline
    bool scope_is_complete() const {return ( Scope::none_ != scope() ); }

    //! For a ta that is complete, these functions are interested in the scope
    //! of the end of the address
    // --------------------------------------------------------------------- //
    inline
    bool scope_is_sn() const { return sn_child.scope == Scope::none_ and
                                      st_child.scope == Scope::none_ and
                                       q_child.scope == Scope::none_; }
    inline
    bool scope_is_st() const { return sn_child.scope == Scope::station and
                                      st_child.scope == Scope::none_ and
                                       q_child.scope == Scope::none_; }
    inline
    bool scope_is_q() const { return sn_child.scope == Scope::station and
                                     st_child.scope == Scope::digitizer and
                                      q_child.scope == Scope::none_; }
    inline
    bool scope_is_s() const { return sn_child.scope == Scope::station and
                                     st_child.scope == Scope::digitizer and
                                      q_child.scope == Scope::sensor; }
    inline
    bool scope_is_dp() const { return sn_child.scope == Scope::station and
                                      st_child.scope == Scope::data_processor and
                                       q_child.scope == Scope::none_; }

    // scope at the end of the address
    // --------------------------------------------------------------------- //
    Scope scope() const;

    //! scope at the end of the address
    //! should be used with full address all over the program
    // --------------------------------------------------------------------- //
    Target target() const;

    //! removes the target at the end of the address, from st0q0 to st0
    // --------------------------------------------------------------------- //
    void remove_one_target();

    //! hash using the target at the end of the address
    //! should be used with full address all over the program
    // --------------------------------------------------------------------- //
    uint32_t const hash() const;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          TargetAddress const & target_address) {

    if (target_address.sn_child.scope != Scope::none_)
        os << target_address.sn_child;

    if (target_address.st_child.scope != Scope::none_)
        os << target_address.st_child;

    if (target_address.q_child.scope != Scope::none_)
        os << target_address.q_child;

    if ( target_address.scope_is_sn() )
        os << "sn";

    return os;
}

// -------------------------------------------------------------------------- //
inline
bool operator==(TargetAddress const & lhs,
                TargetAddress const & rhs) {
    return (lhs.sn_child == rhs.sn_child) and
           (lhs.st_child == rhs.st_child) and
           (lhs.q_child == rhs.q_child);
}

} // end namespace
#endif
