// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_TIME_H
#define _MZN_CMD_FIELD_TIME_H

#include "cmd_field.h"
//! incluedes/uses a external library (date.h)
#include "mzn_time.h"

namespace mzn {

//! Overloads operator(), operator<<
/*!
    The internal representation in data is seconds since 1970 epoch
    for a different epoch, use the shift template parameter
    This class should be used to measure a point in time, not a duration
    see tests for additional usage information and demostration
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T = typename std::chrono::system_clock::rep,
          int shift_seconds_since_1970_epoch = 0>
class CmdFieldTime : public CmdField<T> {

public:

    using data_type = T;
    using CmdField<data_type>::data_;
    using Seconds = Time::Seconds<T>;
    using SysTimeSeconds = Time::SysTimeSeconds<T>;

    //! constructs base
    explicit
    CmdFieldTime();

    //! uses mutator in operator()
    template <typename Duration>
    explicit
    CmdFieldTime(Time::TimePoint<Duration> const tp) : CmdField<T>{} {
        (*this)(tp);
    }

    ~CmdFieldTime() = default;

    //! accessor
    Time::TimePoint<Seconds> const operator()() const;
    //! mutator
    template <typename Duration>
    void operator()(Time::TimePoint<Duration> const tp);

    //! custom accessor date (streamable)
    Time::YearMonthDay year_month_day() const {
        Time::TimePoint<Seconds> const tp = (*this)();
        Time::YearMonthDay const ymd = Time::sys_year_month_day(tp);
        return ymd;
    }

    //! custom accessor time (streamable)
    SysTimeSeconds time() const {
        Time::TimePoint<Seconds> const tp = (*this)();
        SysTimeSeconds const t = Time::sys_time_of_day(tp);
        return t;
    }
};

// construtor
// -------------------------------------------------------------------------- //
template <typename T, int shift_seconds_since_1970_epoch>
inline
CmdFieldTime<T, shift_seconds_since_1970_epoch>::CmdFieldTime() : CmdField<T>{} {};

// operator<< overload
// -------------------------------------------------------------------------- //
template <typename osT, int os_epoch_shift>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldTime<osT, os_epoch_shift> const & cf) {

    cf_os << cf.year_month_day() << " " << cf.time() << " UTC";
    return cf_os;
}

// operator== overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, int epoch_shift_lhs,
          typename T_rhs, int epoch_shift_rhs>
inline
bool operator==(CmdFieldTime<T_lhs, epoch_shift_lhs> const & lhs,
                CmdFieldTime<T_rhs, epoch_shift_rhs> const & rhs) {

    return ( lhs() == rhs() );
}

// operator!= overload
// -------------------------------------------------------------------------- //
template <typename T_lhs, int epoch_shift_lhs,
          typename T_rhs, int epoch_shift_rhs>
inline
bool operator!=(CmdFieldTime<T_lhs, epoch_shift_lhs> const & lhs,
                CmdFieldTime<T_rhs, epoch_shift_rhs> const & rhs) {

    return not (lhs == rhs);
}

// operator() overload (accessor)
// -------------------------------------------------------------------------- //
template <typename T, int shift_seconds_since_1970_epoch>
inline
Time::TimePoint< Time::Seconds<T> > const
CmdFieldTime<T, shift_seconds_since_1970_epoch>::operator()() const {

    using namespace Time;
    using Seconds = CmdFieldTime::Seconds;

    // data always in seconds! (could be specialized later)
    auto const data_seconds = Seconds(CmdField<T>::data_);
    // just in case a different epoch is used (as in the q330 digitizer)
    auto constexpr shift_seconds = Seconds(shift_seconds_since_1970_epoch);
    // a point in time
    return TimePoint<Seconds>(data_seconds + shift_seconds);
}

// operator() overload (mutator)
// -------------------------------------------------------------------------- //
template <typename T, int shift_seconds_since_1970_epoch>
template <typename Duration>
inline
void CmdFieldTime<T, shift_seconds_since_1970_epoch>::operator() (
        Time::TimePoint<Duration> const tp) {

    using Seconds = CmdFieldTime::Seconds;

    // data always in seconds! (could be specialized later)
    Time::TimePoint<Seconds> const tp_seconds = date::floor<Seconds>(tp);
    // reverse to the epoch appropriate number of seconds
    CmdField<T>::data_ = tp_seconds.time_since_epoch().count()
                         - shift_seconds_since_1970_epoch;
}

} // << mzn

#endif
