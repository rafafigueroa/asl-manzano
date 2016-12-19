// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_DURATION_H
#define _MZN_CMD_FIELD_DURATION_H

#include "cmd_field.h"
//! incluedes/uses a external library (date.h)
#include "mzn_time.h"

namespace mzn {

//! Overloads operator(), operator<<
/*!
    The internal representation by default is an integral number of seconds
    This class should be used to measure a duration, not a point in time
    @tparam T data type. Same as std::chrono::duration parameter Rep
        (representation) when used in that context.
    @tparam Period sam as std::chrono::duration parameter Perdio
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T,
          typename Period = std::ratio<1, 1> >
class CmdFieldDuration : public CmdField<T> {

public:

    using data_type = T;
    using CmdField<data_type>::data_;
    using Duration = std::chrono::duration<T, Period>;

    //! behave like a std::chrono::duration when possible
    using rep = T;
    using period = Period;

    //! constructs base
    explicit
    CmdFieldDuration() : CmdField<T>{} {}

    template <typename input_T, typename input_Period>
    explicit
    CmdFieldDuration(std::chrono::duration<input_T, input_Period> const d) :
            CmdField<T>{} {
        (*this)(d);
    }

    ~CmdFieldDuration() = default;

    // accessor
    inline
    Duration const operator()() const;

    // mutator
    template<typename input_T, typename input_Period>
    inline
    void operator()(std::chrono::duration<input_T, input_Period> const d);


};

// operator<< overload, always have units seconds
// -------------------------------------------------------------------------- //
template <typename osT, typename osPeriod>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldDuration<osT, osPeriod> const & cf) {
    cf_os << cf();
    return cf_os;
}

// operator== overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator==(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
                CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return ( lhs() == rhs() );
}

// operator!= overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator!=(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
                CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return not (lhs == rhs);
}

// operator< overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator<(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
               CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return ( lhs() < rhs() );
}

// operator> overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator>(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
               CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return (rhs < lhs);
}

// operator<= overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator<=(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
                CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return not (lhs > rhs);
}

// operator>= overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, typename Period_lhs,
          typename T_rhs, typename Period_rhs>
inline
bool operator>=(CmdFieldDuration<T_lhs, Period_lhs> const & lhs,
                CmdFieldDuration<T_rhs, Period_rhs> const & rhs) {
    return not (lhs < rhs);
}

// operator() overload (accessor)
// -------------------------------------------------------------------------- //
template <typename T, typename Period>
inline
typename CmdFieldDuration<T, Period>::Duration const
CmdFieldDuration<T, Period>::operator() () const {

    return CmdFieldDuration<T, Period>::Duration(CmdField<T>::data_);
}

// operator() overload (mutator)
// -------------------------------------------------------------------------- //
template <typename T, typename Period>
template<typename input_T, typename input_Period>
inline
void CmdFieldDuration<T, Period>::operator()(
        std::chrono::duration<input_T, input_Period> const d) {
    // take any duration, cast to this objects internal representation
    // dont use floor<> here since that will not alert of truncation
    // if trucation is ok, use floor (or time_cast, etc) to input the value
    // i.e.: cf_ms_d( std::floor<std::chrono::milliseconds>(35us) );

    auto const d_data = Duration(d);
    // count works with floats/doubles too
    CmdField<T>::data_ = d_data.count();
};

} // << mzn

#endif
