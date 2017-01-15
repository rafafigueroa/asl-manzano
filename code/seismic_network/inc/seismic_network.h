// ** SeismicNetwork Object ** //
// Manzano software

#ifndef _MZN_SEISMIC_NETWORK_H
#define _MZN_SEISMIC_NETWORK_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "station.h"
#include "target_address.h"
#include "mzn_except.h"

namespace mzn {

//! Performs the configuration of the entire seismic network
/*!
    holds the stations
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class SeismicNetwork {

public:

    // --------------------------------------------------------------------- //
    struct Config {

        friend class SeismicNetwork;
        Config() = default;
        ~Config() = default;
    };

    struct Status {
        friend class SeismicNetwork;
    };

    /*! reads the json file and configures stations, data processors, digitizers
        if successful, sets the configured flag to true
        it adds to the vector st
     */
    // --------------------------------------------------------------------- //
    SeismicNetwork();
    ~SeismicNetwork() = default;

    //! using the filename from the CMake configuration, fill this object
    void setup_from_config_file();

    //! holds a vector of stations
    std::vector<Station> st; //stations
    //! flag sets to true during constructor initialization

    // --------------------------------------------------------------------- //
    Config config;
    Status status;

    // --------------------------------------------------------------------- //
    Digitizer const & q_const_ref(TargetAddress const & ta) const;
    Sensor const & s_const_ref(TargetAddress const & ta) const;
    Station const & st_const_ref(TargetAddress const & ta) const;
    DataProcessor const & dp_const_ref(TargetAddress const & ta) const;

    // --------------------------------------------------------------------- //
    Digitizer & q_ref(TargetAddress const & ta);
    Sensor & s_ref(TargetAddress const & ta);
    Station & st_ref(TargetAddress const & ta);
    DataProcessor & dp_ref(TargetAddress const & ta) ;

    // --------------------------------------------------------------------- //
    /*
    auto & target_ref(TargetAddress const & ta) {
        // todo: one more switch than needed?
        // not quite there yet. Need to make it <scope>(index)
        // so that in stream_output: auto target sn_.target_ref(ta);
        auto scope = ta.scope();

        switch (scope) {
            case Scope::seismic_network: return & this;
            case Scope::station: return st[ta.sn_child.index];
            case Scope::digitizer:
                return st[ta.sn_child.index].q[ta.st_child.index];
            case Scope::data_processor:
                return st[ta.sn_child.index].dp[ta.st_child.index];
            case Scope::sensor:
                return
                st[ta.sn_child.index].q[ta.st_child.index].s[ta.q_child.index];

            default: throw std::logic_error{"@SeismicNetwork::target_ref"};
        }
    }
    */

};

inline
std::ostream & operator<<(std::ostream & os,
                          SeismicNetwork::Config const & config) {
    return os;
}

inline
std::ostream & operator<<(std::ostream & os,
                          SeismicNetwork::Status const & status) {
    return os;
}

} // << mzn
#endif // _MZN_SEISMIC_NETWORK_H_

