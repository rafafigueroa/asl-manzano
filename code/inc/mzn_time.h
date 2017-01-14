// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_TIME_H
#define _MZN_TIME_H

//! External library include
#include "date.h"

namespace mzn {

using namespace std::chrono_literals;

//! Utility functions using date.h Hinnant's library
/*!
    Uses seconds precision throughout.
    Types are shown explicitely since casts are the most common operation.

    pick and choose between std::chrono and date::
    here is the legend of what is taken from each
    also helps to differentiate between the type day_point and function day_point
    it seems like the new version of date.h uses other things besides day_point
    but it was failing the build, so I am sticking with this version that is working

    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
namespace Time {

//! clocks
// -------------------------------------------------------------------------- //
using SysClock = std::chrono::system_clock;

//! durations
//! one step more general than the chrono types, since it generalizes the
//! internal representation type
// -------------------------------------------------------------------------- //
template <typename Rep = std::chrono::seconds::rep>
using Seconds = std::chrono::duration<Rep, std::ratio<1, 1> >;

template <typename Rep = std::chrono::microseconds::rep>
using MicroSeconds = std::chrono::duration<Rep, std::micro>;

template <typename Rep = std::chrono::milliseconds::rep>
using MilliSeconds = std::chrono::duration<Rep, std::milli>;

template <typename Rep = std::chrono::minutes::rep>
using Minutes = std::chrono::duration<Rep, std::ratio<60> >;

template <typename Rep = std::chrono::hours::rep>
using Hours = std::chrono::duration<Rep, std::ratio<3600> >;

template <typename Rep = int>
using Days = std::chrono::duration<Rep, date::days::period>;

template <typename Rep = int>
using Years = std::chrono::duration<Rep, date::years::period>;

//! time_points
// -------------------------------------------------------------------------- //
template <typename Rep = std::chrono::seconds::rep>
using SysTimeSeconds = date::time_of_day< Seconds<Rep> >;

using DayPoint = date::day_point;

using YearMonthDay = date::year_month_day;

template <typename Duration>
using TimePoint = std::chrono::time_point<SysClock, Duration>;

// -------------------------------------------------------------------------- //
template <typename Duration = SysClock::duration,
          typename Rep = typename Duration::rep>
inline
SysTimeSeconds<typename Duration::rep> constexpr
sys_time_of_day(TimePoint<Duration> const tp = SysClock::now() ) {

    using Days = Time::Days<Rep>;

    // days precision, (almost due to leap seconds) UTC midnight of tp
    TimePoint<Days> const tp_midnight = date::floor<Days>(tp);
    // seconds precision of difference, seconds since UTC midnight of tp
    Seconds<Rep> const since_midnight =
            date::floor< Seconds<Rep> >(tp - tp_midnight);
    // from seconds to hour/minute/second that can be streamed
    return date::make_time(since_midnight);
}

// -------------------------------------------------------------------------- //
template <typename Duration = SysClock::duration>
inline
YearMonthDay constexpr
sys_year_month_day(TimePoint<Duration> const tp = SysClock::now() ) {

    using Days = Time::Days<typename Duration::rep>;

    TimePoint<Days> const tp_midnight = date::floor<Days>(tp);
    YearMonthDay const ymd = YearMonthDay(tp_midnight);
    return ymd;
}

//! returs the number of seconds between system_epoch and other_epoch
//! positive when other_epoch comes after system_epoch
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
Seconds<Rep> constexpr
shift_seconds_for_epoch(YearMonthDay const other_epoch_ymd) {

    // this is the de-facto standard system epoch, although it is not guaranteed
    YearMonthDay constexpr system_epoch_ymd = date::jan/1/1970;
    DayPoint constexpr system_epoch_tp = DayPoint(system_epoch_ymd);
    // just in case:
    static_assert( system_epoch_tp.time_since_epoch() == Seconds<Rep>(0),
                  "shift_seconds_from_epoch expects jan/1/1970 epoch");

    // transform ymd to tp in order to do the substraction
    DayPoint const other_epoch_tp = DayPoint(other_epoch_ymd);
    // return a duration between the 2 epochs
    return date::floor< Seconds<Rep> >(other_epoch_tp - system_epoch_tp);
}

// -------------------------------------------------------------------------- //
template <typename Rep>
inline
constexpr TimePoint< Seconds<Rep> >
shift_epoch(TimePoint< Seconds<Rep> > const tp,
            YearMonthDay const other_epoch_ymd) {

    return tp + shift_seconds_for_epoch<Rep>(other_epoch_ymd);
}

//! constant used to construct time for digitizer q330
// -------------------------------------------------------------------------- //
uint32_t constexpr k_shift_seconds_1970_2000 =
        shift_seconds_for_epoch<uint32_t>(date::jan/1/2000).count();

// as in january 1 is 1
// ---------------------------------------------------------------------- //
inline
int julian_day(YearMonthDay const & ymd_data) {

    auto ymd_start = date::year{ ymd_data.year() }/1/1;

    if ( not ymd_start.ok() ) throw std::runtime_error("Invalid date");
    if ( not ymd_data.ok() ) throw std::runtime_error("Invalid date");

    auto days_start = date::day_point(ymd_start);
    auto days_data = date::day_point(ymd_data);

    auto days_diff = (days_data - days_start );

    return days_diff.count() + 1; // if same, jan 1 is 1, not 0
}

// as in january 1 is 1
// ---------------------------------------------------------------------- //
inline
int julian_day(int const y, int const m, int const d) {

    return julian_day(date::year{y}/m/d);
}


} // <- Time

//              END OF TIME!                                         //
// _________________________________________________________________ //

//! operator<< overloads, this should not be needed in C++17

//! operator<< seconds of any rep (std::chrono::seconds is a special case)
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::ratio<1, 1> > const d) {

    // relatively expensive function, however the formatting should be useful

    if (d.count() < 60) {
        os << d.count() << "s";
        return os;
    }

    using Seconds = Time::Seconds<Rep>;
    using Minutes = Time::Minutes<Rep>;
    using Hours   = Time::Hours<Rep>;
    using Days    = Time::Days<Rep>;
    using Years   = Time::Years<Rep>;

    // keep track of the accounted duration
    std::chrono::duration<Rep> da{}; // 0 seconds

    // arithmetic is done internally using the ratio values
    Years   const d_y = date::floor<Years>(d - da);   da += d_y;
    Days    const d_d = date::floor<Days>(d - da);    da += d_d;
    Hours   const d_h = date::floor<Hours>(d - da);   da += d_h;
    Minutes const d_m = date::floor<Minutes>(d - da); da += d_m;
    Seconds const d_s = date::floor<Seconds>(d - da);

    std::vector<std::string> d_parts{};
    if (d_y.count() > 0) d_parts.emplace_back(std::to_string(d_y.count()) + "y");
    if (d_d.count() > 0) d_parts.emplace_back(std::to_string(d_d.count()) + "d");
    if (d_h.count() > 0) d_parts.emplace_back(std::to_string(d_h.count()) + "h");
    if (d_m.count() > 0) d_parts.emplace_back(std::to_string(d_m.count()) + "min");
    if (d_s.count() > 0) d_parts.emplace_back(std::to_string(d_s.count()) + "s");

    for (int i = 0; i < d_parts.size(); i++) {
        os << d_parts[i];
        if (i != d_parts.size() - 1) os << "_";
    }

    return os;
}

//! operator<< microseconds of any rep
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::micro> const d) {

    using MilliSeconds = Time::MilliSeconds<Rep>;
    using MicroSeconds = Time::MicroSeconds<Rep>;

    auto const d_ms = date::floor<MilliSeconds>(d);
    auto const d_us = date::floor<MicroSeconds>(d - d_ms);

    if (d_ms.count() > 0) {
        os << d_ms.count() << "ms_"
           << d_us.count() << "us";
        return os;
    }

    os << d_us.count() << "us";
    return os;
}

//! operator<< milliseconds of any rep
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::milli> const d) {

    using Seconds = Time::Seconds<Rep>;
    using MilliSeconds = Time::MilliSeconds<Rep>;

    Seconds const d_s = date::floor<Seconds>(d);
    // don't floor last conversion in case of d float representation or trunc
    MilliSeconds const d_ms(d - d_s);

    if (d_s.count() > 0) {
        os << d_s.count()  << "s_"
           << d_ms.count() << "ms";
        return os;
    }

    os << d_ms.count() << "ms";
    return os;
}

//! operator<< minutes of any rep (std::chrono::minutes is a special case)
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::ratio<60> > const d) {

    using Seconds = Time::Seconds<Rep>;
    // don't floor last conversion in case of d float representation or trunc
    os << Seconds(d);
    return os;
}

//! operator<< hours of any rep (std::chrono::hours is a special case)
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::ratio<3600> > const d) {

    using Seconds = Time::Seconds<Rep>;
    // don't floor last conversion in case of d float representation or trunc
    os << Seconds(d);
    return os;
}

//! operator<< deci seconds of any rep
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::deci> const d) {

    using MilliSeconds = Time::MilliSeconds<Rep>;
    // don't floor last conversion in case of d float representation or trunc
    os << MilliSeconds(d);
    return os;
}

//! operator<< centi seconds of any rep
// -------------------------------------------------------------------------- //
template <typename Rep>
inline
std::ostream & operator<<(std::ostream & os,
                          std::chrono::duration<Rep, std::centi> const d) {

    using MilliSeconds = Time::MilliSeconds<Rep>;
    // don't floor last conversion in case of d float representation or trunc
    os << MilliSeconds(d);
    return os;
}

} // << mzn

#endif
