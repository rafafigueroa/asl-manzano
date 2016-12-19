// Manzano Software //
#ifndef _MZN_TARGET_H_
#define _MZN_TARGET_H_

#include <sstream>

namespace mzn {


//! The building blocks of a Target
/*! @see Target
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
enum class Scope {
    sensor,
    digitizer,
    station,
    seismic_network,
    data_processor,
    none_, // none_ instead of none to avoid using c++ keyword
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, Scope const & scope) {

    switch(scope) {
        case Scope::sensor          : os << "s"; break;
        case Scope::digitizer       : os << "q"; break;
        case Scope::station         : os << "st"; break;
        case Scope::seismic_network : os << "sn"; break;
        case Scope::data_processor  : os << "dp"; break;

        default : throw std::logic_error{"@Target::operator<<"};
    }
    return os;
}

//! The building blocks of a TargetAddress
/*! The combination of a scope/index can identify
    a target in the SeismicNetwork if the parent is known
    @see TargetAddress
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
struct Target {
    Scope scope = Scope::none_;
    std::size_t index = 0;

    Target() = default;
    ~Target() = default;

    explicit
    Target(Scope in_scope,
           unsigned int in_index = 0) :
        scope(in_scope),
        index(in_index) {}
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os,
                          Target const & target) {

    os << target.scope << "[" << target.index << "]";
    return os;
}

// -------------------------------------------------------------------------- //
inline
bool operator==(Target const & lhs,
                Target const & rhs) {
    return (lhs.scope == rhs.scope) and
           (lhs.index == rhs.index);
}

} // end namespace
#endif
