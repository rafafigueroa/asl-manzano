#include "station.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void Station::stream_config(std::ostream & os) const {
    os << "\n    " << "name     : " << config.station_name;
}

// -------------------------------------------------------------------------- //
void Station::stream_status(std::ostream & os) const {

}

} // <- mzn
