// ** SeismicNetwork Object ** //
// Manzano software
#include "seismic_network.h"
#include "system_calls.h"
#include "json.h"
#include "json_sn.h"

namespace mzn {

// -------------------------------------------------------------------------- //
SeismicNetwork::SeismicNetwork() : st{} {
    setup_from_config_file();
}

// -------------------------------------------------------------------------- //
void SeismicNetwork::setup_from_config_file() {

try {

    auto const runtime_config_path = get_runtime_config_path();
    std::string const file_path = runtime_config_path + "/config.json";

    auto sn_json = read_json(file_path);

    // reset the auto-loaded seismic network
    st.clear();

    // TODO: replace reference to input json file with above

    // if our json file is formatted correctly then each key is a station name
    // and under that are an IP and a series of digitizers and data processors

    std::stringstream ss;

    for (auto it = sn_json.begin(); it != sn_json.end(); it++) {

        // seismic network json has station json objects

        std::string st_name = it.key(); // do we need to cast?
        auto st_json = it.value(); // station data is nested json

        // TODO: throw info exception instead of NOTE
        st.push_back( Station(st_name) );

        // one top object with a list of digitizers
        if ( st_json.find("digitizer") != st_json.end() ) {

            // add each digitizer on the list
            for (auto const & q_json : st_json["digitizer"]) {

                st.back().q.push_back( q_from_json(q_json) );

                // locate any sensor objects
                if ( q_json.find("sensor") != q_json.end() ) {

                    // limit of 2 sensors per digitizer
                    auto constexpr max_sensors = 2;
                    if (q_json.at("sensor").size() > max_sensors) {
                        ss << "\nERROR: a digitizer at "
                           << st_name << " has too many sensors";
                        throw WarningException("BuilderLoop", "display",
                                ss.str());
                    }

                    for (auto const & s_json : q_json["sensor"]) {

                        st.back().q.back().s.push_back( s_from_json(s_json) );
                    }

                } else {
                    std::cout << "\n\nNote: a digitizer at " << st_name
                              << " has no sensors specified." << std::endl;

                    std::cout << "This configuration is technically valid, "
                              << "but very likely an error.\n";
                }

            } // loop over list of digitizers

        } else {
            std::cout << "\n\nNOTE: " << st_name << " has no digitizers specified."
                      << std::endl << "This configuration is technically "
                      << "valid, but very likely an error." << std::endl;
        }

        if ( st_json.find("data_processor") != st_json.end() ) {

            Json dp_json = st_json["data_processor"];
            st.back().dp.push_back( dp_from_json(dp_json) );

        } else {
            // std::cout << "\n\nNOTE: " << st_name << " lacks a data processor.";
        }
    }

} catch(std::out_of_range const & e) {

    // thrown by json
    throw WarningException( "SeismicNetwork", "setup_from_file", e.what() );

} catch(std::runtime_error const & e) {

    // thrown by get environmental variable
    throw WarningException( "SeismicNetwork", "setup_from_file", e.what() );
}

} // <- end of function

// ------------ Get const references to sn targets -------------- //
// -------------------------------------------------------------------------- //
Station const & SeismicNetwork::st_const_ref(TargetAddress const & ta) const {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"st_const_ref"};

    auto st_index = ta.sn_child.index;
    return st[st_index];
}

// -------------------------------------------------------------------------- //
DataProcessor const & SeismicNetwork::dp_const_ref(TargetAddress const & ta) const {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"dp_const_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"dp_const_ref"};

    auto st_index = ta.sn_child.index;
    auto dp_index = ta.st_child.index;
    return st[st_index].dp[dp_index];
}

// -------------------------------------------------------------------------- //
Digitizer const & SeismicNetwork::q_const_ref(TargetAddress const & ta) const {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"q_const_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"q_const_ref"};

    auto st_index = ta.sn_child.index;
    auto q_index = ta.st_child.index;
    return st[st_index].q[q_index];
}

// -------------------------------------------------------------------------- //
Sensor const & SeismicNetwork::s_const_ref(TargetAddress const & ta) const {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"s_const_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"s_const_ref"};
    if (ta.q_child.scope == Scope::none_) throw std::logic_error{"s_const_ref"};

    auto st_index = ta.sn_child.index;
    auto q_index = ta.st_child.index;
    auto s_index = ta.q_child.index;
    return st[st_index].q[q_index].s[s_index];
}

// ------------ Get references to sn targets -------------- //
// -------------------------------------------------------------------------- //
Station & SeismicNetwork::st_ref(TargetAddress const & ta) {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"st_ref"};

    auto st_index = ta.sn_child.index;
    return st[st_index];
}

// -------------------------------------------------------------------------- //
DataProcessor & SeismicNetwork::dp_ref(TargetAddress const & ta) {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"dp_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"dp_ref"};

    auto st_index = ta.sn_child.index;
    auto dp_index = ta.st_child.index;
    return st[st_index].dp[dp_index];
}

// -------------------------------------------------------------------------- //
Digitizer & SeismicNetwork::q_ref(TargetAddress const & ta) {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"q_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"q_ref"};

    auto st_index = ta.sn_child.index;
    auto q_index = ta.st_child.index;
    return st[st_index].q[q_index];
}

// -------------------------------------------------------------------------- //
Sensor & SeismicNetwork::s_ref(TargetAddress const & ta) {

    if (ta.sn_child.scope == Scope::none_) throw std::logic_error{"s_ref"};
    if (ta.st_child.scope == Scope::none_) throw std::logic_error{"s_ref"};
    if (ta.q_child.scope == Scope::none_) throw std::logic_error{"s_ref"};

    auto st_index = ta.sn_child.index;
    auto q_index = ta.st_child.index;
    auto s_index = ta.q_child.index;
    return st[st_index].q[q_index].s[s_index];
}

} // << mzn
