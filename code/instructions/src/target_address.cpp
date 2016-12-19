#include "target_address.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void TargetAddress::add_targets_from_ta(TargetAddress const & from_ta) {

    // TODO everything is backwards
    // this functions merges from_ta with ta, it only changes
    // ta, it does not change from_ta

    /* CASE 1 : ta has ONE target
       --------------------------

       the rows represent ta, columns represent from_ta

            | sn | st | dp | q | s |
        ----------------------------
         sn | =  | =  | =  | = | = |  sn always has full scope, returns
        ----------------------------
         st | =  | X  | X  | X | X |  (st) nothing to add from from_ta
        ----------------------------
         dp | X  | +  | X  | X | X |
        ----------------------------
         q  | X  | +  | X  | X | X |
        ----------------------------
         s  | X  | X  | X  | + | X |

       '=' no change in ta
       '+' merge (add from from_ta to ta)
       'X' exception

       note that merge only happens when from_ta.scope() is st or q

       CASE 2 : ta has TWO targets
       ---------------------------

        The first part would be the same as case 1, the only difference
        would it be if it does not make a full address after merged,
        for example st0s1. Which is checked after the end.

    */

    // Check if the user's ta is a complete scoping.
    if ( scope_is_complete() ) return; // no merge needed

    switch ( from_ta.scope() ) {

        case Scope::seismic_network : {

            if (sn_child.scope == Scope::station) break;
            else {
                throw WarningException("TargetAddress",
                                       "add_targets_from_ta",
                                       "Wrong merge, current ta scope is sn");
            }
            break;
        }

        case Scope::station : {

            if (st_child.scope == Scope::digitizer or
                st_child.scope == Scope::data_processor) {
                // merge station
                sn_child = from_ta.sn_child;
            } else {
                throw WarningException("TargetAddress",
                                       "add_targets_from_ta",
                                       "Wrong merge, current ta scope is st");
            }
            break;
        }

        case Scope::digitizer : {

            if (q_child.scope == Scope::sensor) {
                // merge station and digitizer
                sn_child = from_ta.sn_child;
                st_child = from_ta.st_child;
            } else {
                throw WarningException("TargetAddress",
                                       "add_targets_from_ta",
                                       "Wrong merge, current ta scope is q");
            }
            break;
        }

        case Scope::none_ :
            throw std::logic_error{"@TargetAddress::add_targets_from_ta"};

        // sensor or data_processor
        default : {
            throw WarningException("TargetAddress",
                                   "add_targets_from_ta",
                                   "Wrong merge, current ta scope is s");
        }
    }

    // check for the case when there is more than one target in ta
    if ( not scope_is_complete() ) {

        throw WarningException(
            "TargetAddress",
            "add_targets_from_ta",
            "Incomplete target specified."\
            "\n\tPlease specify either a valid target EG st1q0s0"\
            "\n\tor a target relative to the current target_address");
    }
}

// -------------------------------------------------------------------------- //
void TargetAddress::add_target(Target const & target) {

    switch(target.scope) {
        // TODO add_sn_child
        case Scope::station   :      sn_child = target; break;
        case Scope::digitizer :      st_child = target; break;
        case Scope::data_processor : st_child = target; break;
        case Scope::sensor :          q_child = target; break;

        default :
            throw WarningException("TargetAddress",
                                   "add_target",
                                   "unhandled scope");
    }

}

// -------------------------------------------------------------------------- //
Scope TargetAddress::scope() const {

    // most common case
    if (sn_child.scope == Scope::station) {

        if (q_child.scope == Scope::none_) {

            if (st_child.scope == Scope::none_) return Scope::station;
            if (st_child.scope == Scope::digitizer) return Scope::digitizer;
            if (st_child.scope == Scope::data_processor) {
                return Scope::data_processor;
            }

        } else if (q_child.scope == Scope::sensor) return Scope::sensor;
    }

    // seismic network
    if (sn_child.scope == Scope::none_ and
        st_child.scope == Scope::none_ and
         q_child.scope == Scope::none_) return Scope::seismic_network;

    // not a fully formed TargetAddress or wrongly formed TargetAddress
    return Scope::none_;
}

// -------------------------------------------------------------------------- //
Target TargetAddress::target() const {

    auto scope = this->scope();

    switch (scope) {
        case Scope::seismic_network: return Target{Scope::seismic_network};
        case Scope::station:         return sn_child;
        case Scope::digitizer:       return st_child;
        case Scope::sensor:          return q_child;
        case Scope::data_processor:  return st_child;
        case Scope::none_: throw std::logic_error{"at TargetAddress::target()"};
    }
}

// -------------------------------------------------------------------------- //
void TargetAddress::remove_one_target() {

    if (q_child.scope == Scope::sensor) {

        q_child = Target{};
        return;
    }

    if (st_child.scope == Scope::digitizer or
        st_child.scope == Scope::data_processor) {

        st_child = Target{};
        return;
    }

    if (sn_child.scope == Scope::station) {
        sn_child = Target{};
    }

    // if sn, nothing to remove, no exceptions either
}

// -------------------------------------------------------------------------- //
uint32_t const TargetAddress::hash() const {

    // the whole hash will take 24 bits at most
    // is to be combined with a user instruction hash
    // this hash would be in the most significant bits
    // so that the combination can be done ui_hash + ta_hash

    // 3 bits
    auto h_sn_child_scope = static_cast<uint32_t>(sn_child.scope);
    // 3 bits
    auto h_st_child_scope = static_cast<uint32_t>(st_child.scope);
    // 3 bits
    auto h_q_child_scope = static_cast<uint32_t>(q_child.scope);

    // the index of all but sn_child will be at most 2
    // 3 bits will be reserved for each
    // 8 bits for sn_child or number of stations

    return (sn_child.index << 24) +
           (st_child.index << 21) +
           (q_child.index << 18) +
           (h_sn_child_scope << 15) +
           (h_st_child_scope << 12) +
           (h_q_child_scope << 9);
}

} // <- mzn
