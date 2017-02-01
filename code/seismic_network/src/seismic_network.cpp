// ** SeismicNetwork Object ** //
// Manzano software
#include "seismic_network.h"
#include "system_calls.h"
#include "json.h"
#include "json_sn.h"

namespace mzn {

// -------------------------------------------------------------------------- //
SeismicNetwork::SeismicNetwork() : st{} {

    try {

        auto const runtime_config_path = get_runtime_config_path();
        std::string const file_path = runtime_config_path + "/config.json";

        auto sn_json = read_json(file_path);

        for (auto const & st_json : sn_json["station"]) {
            st.push_back( st_from_json(st_json) );
        }

    } catch(std::out_of_range const & e) {

        // thrown by Json
        throw WarningException( "SeismicNetwork",
                                "setup_from_file",
                                e.what() );
    }
}

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
