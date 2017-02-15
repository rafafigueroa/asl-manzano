// ** Station Object ** //
// Manzano software

#ifndef _MZN_STATION_H
#define _MZN_STATION_H

#include <iostream>
#include <string>
#include <vector>

#include "digitizer.h"
#include "data_processor.h"

namespace mzn {

//! Holds configuration and status of station
/*!
    Holds digitizers and data_processors
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Station {
private:

    struct Config {
        friend class Station;
        Config(std::string const in_station_name) :
            station_name(in_station_name) {}
        ~Config() = default;

        std::string const station_name;
    };

    struct Status {
        friend class Station;
    };

public:

    std::vector<Digitizer> q; //digitizers
    std::vector<DataProcessor> dp;

    Config config;
    Status status;

    Station(std::string const in_station_name) :
            q{},
            dp{},
            config(in_station_name),
            status() {}

    ~Station() = default;

    // move constructor
    // ---------------------------------------------------------------------- //
    Station(Station && rhs) noexcept :
            q( std::move(rhs.q) ),
            dp( std::move(rhs.dp) ),
            config(rhs.config),
            status(rhs.status) {}

    // ---------------------------------------------------------------------- //
    void stream_config(std::ostream & os) const;
    void stream_status(std::ostream & os) const;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & st_os,
                          Station const & st) {

    st_os <<  st.config.station_name;
    return st_os;
};

} // << mzn
#endif // _MZN_STATION_H_
