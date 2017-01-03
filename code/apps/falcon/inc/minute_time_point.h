#ifndef _MZN_FALCON_MINUTE_TIME_POINT_H_
#define _MZN_FALCON_MINUTE_TIME_POINT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "date.h"
#include "string_utilities.h"

//! falcon specific constructor
//! not a time_point as in std::chrono::system_clock::time_point
// -------------------------------------------------------------------------- //
struct MinuteTimePoint {

    int d = 0;
    int m = 0;
    int y = 0;
    int h = 0;
    int min = 0;

    explicit
    MinuteTimePoint() = default;

    // ---------------------------------------------------------------------- //
    explicit
    MinuteTimePoint (std::vector<std::string> const & tokens) {

        auto date_tokens = get_tokens(tokens[0], '/');
        // falcon date is month/day/year (2 digit year)
        // making it 4 digit, starting with 2000
        y = std::stoi(std::string("20") + date_tokens[2]);
        m = std::stoi(date_tokens[0]);
        d = std::stoi(date_tokens[1]);

        auto time_tokens = get_tokens(tokens[1], ':');
        // falcon time is hour:minute, no seconds
        h = std::stoi(time_tokens[0]);
        min = std::stoi(time_tokens[1]);
    }

    ~MinuteTimePoint() = default;

    // ---------------------------------------------------------------------- //
    int seconds_since_midnight() const {

        /* overkill (left here, just in case is not the simple case)
        using Days = date::days;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours = std::chrono::hours;

        auto ymd_data = date::year{y}/m/d;
        if ( not ymd_data.ok() ) throw std::runtime_error("Invalid date");
        auto tp_data = date::day_point(ymd_data) + Minutes(min) + Hours(h);
        // days precision, (almost due to leap seconds) UTC midnight of tp
        auto const tp_mn = date::floor<Days>(tp_data);
        // seconds precision of difference, seconds since UTC midnight of tp
        Seconds const since_midnight = date::floor<Seconds>(tp_data - tp_mn);
        // from seconds to hour/minute/second that can be streamed
        return since_midnight.count();
        */
        return h*60*60 + min*60;
    }

    // as in january 1 is 1
    // ---------------------------------------------------------------------- //
    int julian_day() const {

        auto ymd_start = date::year{y}/1/1;
        auto ymd_data = date::year{y}/m/d;

        if ( not ymd_start.ok() ) throw std::runtime_error("Invalid date");
        if ( not ymd_data.ok() ) throw std::runtime_error("Invalid date");

        auto days_start = date::day_point(ymd_start);
        auto days_data = date::day_point(ymd_data);

        auto days_diff = (days_data - days_start );

        return days_diff.count() + 1; // if same, jan 1 is 1, not 0
    }

    // ---------------------------------------------------------------------- //
    int days_in_year() const {

        auto ymd_this = date::year{y}/1/1;
        auto ymd_next = date::year{y+1}/1/1;

        if ( not ymd_this.ok() ) throw std::runtime_error("Invalid date");
        if ( not ymd_next.ok() ) throw std::runtime_error("Invalid date");

        auto days_this = date::day_point(ymd_this);
        auto days_next = date::day_point(ymd_next);

        auto days_diff = (days_next - days_this);

        return days_diff.count(); // if same, jan 1 is 1, not 0
    }

    // ---------------------------------------------------------------------- //
    std::chrono::time_point<std::chrono::system_clock> tp() const {

        auto const ymd = date::year{y}/m/d;
        auto const days = date::day_point(ymd);
        auto const tp = days + std::chrono::seconds( seconds_since_midnight() );

        return tp;
    }

    // ---------------------------------------------------------------------- //
    bool const is_contiguous_to(MinuteTimePoint const & prev) const {

        auto const tp_prev = prev.tp();
        auto const tp_this = tp();
        auto const tp_diff = tp_this - tp_prev;

        return ( tp_diff == std::chrono::seconds(60) );
    }
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, MinuteTimePoint const & mtp) {

    os << "\ndate: " << mtp.y << "/" << mtp.m << "/" << mtp.d
       << " (day " << mtp.julian_day() << " of " << mtp.days_in_year() << ")"
       << "\ntime: " << mtp.h << ":" << mtp.min
       << " (second " << mtp.seconds_since_midnight() << " since UTC midnight)"
       << "\ndata: [";

    return os;
}

#endif // _MZN_FALCON_MINUTE_TIME_POINT_H_
