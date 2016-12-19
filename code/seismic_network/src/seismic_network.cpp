// ** SeismicNetwork Object ** //
// Manzano software
#include "seismic_network.h"
#include "json.h"

namespace mzn {

// -------------------------------------------------------------------------- //
SeismicNetwork::SeismicNetwork() : st{} {
    setup_from_config_file();
}

// -------------------------------------------------------------------------- //
Json read_json(std::string const & filename) {

    std::ifstream config_fs;

    config_fs.open(filename);

    if (not config_fs) {
        std::cerr << "ERROR: can't open configuration file" << std::endl;
        std::cerr << filename << std::endl;
        throw FatalException("SeismicNetwork",
                             "config",
                             "can't open config.json file");
    }

    // read in the contents of the entire file
    std::stringstream config_ss;
    config_ss << config_fs.rdbuf();

    // parses the read-in file as a json object
    return Json::parse( config_ss.str() );
}

// -------------------------------------------------------------------------- //
class JsonRef {

public:
    // internal object
    Json const & json;
    // implicit conversion
    JsonRef(Json const & json) : json(json) {}

    auto operator[](std::string const key) const {

        if ( json.find(key) == json.end() ) {

            std::stringstream ss;
            ss << "Error attempting to get value for key: " << key;
            throw FatalException("SeismicNetwork",
                                 "setup",
                                 ss.str() );
        } else {
            return json[key];
        }
    }
};

// -------------------------------------------------------------------------- //
inline
ConnectionHandler
ch_from_json(JsonRef const connection_handler_json) {

    std::string ip_remote      = connection_handler_json["ip_remote"];
    int const port_remote      = connection_handler_json["port_remote"];
    std::string auth_code      = connection_handler_json["auth_code"];
    int const port_host        = connection_handler_json["port_host"];
    int const protocol_version = connection_handler_json["protocol_version"];

    ConnectionHandler connection_handler(
        ip_remote,
        port_remote,
        std::strtoul(auth_code.c_str(), nullptr, 16),
        port_host,
        protocol_version);

    return std::move(connection_handler);
}

// -------------------------------------------------------------------------- //
inline
std::unique_ptr<ConnectionHandlerE300>
ch_e300_ptr_from_json(JsonRef const connection_handler_json) {

    std::string ip_remote      = connection_handler_json["ip_remote"];
    int const port_remote      = connection_handler_json["port_remote"];
    std::string auth_code      = connection_handler_json["auth_code"];
    int const port_host        = connection_handler_json["port_host"];

    auto ch_ptr = std::make_unique<ConnectionHandlerE300>(
        ip_remote,
        port_remote,
        std::strtoul(auth_code.c_str(), nullptr, 16),
        port_host);

    return std::move(ch_ptr);
}

// -------------------------------------------------------------------------- //
inline
Digitizer q_from_json(JsonRef const q_json) {

    std::string serial_number_string    = q_json["serial_number"];

    uint64_t const serial_number =
        std::strtoul(serial_number_string.c_str(), nullptr, 16);

    auto connection_handler_config_json  = q_json["port_config"];
    auto connection_handler_control_json = q_json["port_control"];
    auto connection_handler_data_json    = q_json["port_data"];

    Digitizer q(
        serial_number,
        ch_from_json(connection_handler_config_json),
        ch_from_json(connection_handler_control_json),
        ch_from_json(connection_handler_data_json)
    );

    return std::move(q);
}

// -------------------------------------------------------------------------- //
inline
Sensor s_from_json(JsonRef const s_json) {

    std::string const input = s_json["input"];
    std::string const model = s_json["model"];
    std::string const cals  = s_json["cals"];

    if ( s_json.json.find("port_e300") != s_json.json.end() ) {

        auto connection_handler_e300_json = s_json["port_e300"];

        Sensor s(
            input,
            model,
            cals,
            ch_e300_ptr_from_json(connection_handler_e300_json)
        );

        return std::move(s);
    }

    // sensor without e300
    Sensor s(
        input,
        model,
        cals
    );

    return std::move(s);
}

// -------------------------------------------------------------------------- //
inline
DataProcessor dp_from_json(JsonRef const dp_json) {

    std::string const user = dp_json["user"];
    std::string const pw   = dp_json["pw"];
    std::string const ip   = dp_json["ip"];

    DataProcessor dp(user, pw, ip);

    return std::move(dp);
}
// -------------------------------------------------------------------------- //
void SeismicNetwork::setup_from_config_file() {

try {
    std::string const runtime_config_path{k_mzn_runtime_config_DIR};
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
            std::cout << "\n\nNOTE: " << st_name << " lacks a data processor.";
        }
    }

} catch(std::out_of_range const & e) {
    throw WarningException( "SeismicNetwork", "setup_from_file", e.what() );
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
