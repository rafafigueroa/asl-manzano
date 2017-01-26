#ifndef _MZN_JSON_SN_H_
#define _MZN_JSON_SN_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include "mzn_cmake_config.h"
#include "mzn_except.h"
#include "json.h"
#include "string_utilities.h"

namespace mzn {

// -------------------------------------------------------------------------- //
inline
void ip_format_check(std::string const & ip) {

    try {

        // TODO: update for ipv6
        auto const ip_tokens = get_tokens(ip, '.');

        if (ip_tokens.size() != 4) throw WarningException("JsonSn",
                                                          "ip_format_check",
                                                          "not 4 byte format");

        for (int i = 0; i < ip_tokens.size(); i++) {
            int const ipb =  std::stoi(ip_tokens[i]);
            if (ipb < 0 or ipb > 255) {
                throw WarningException("JsonSn",
                                       "ip_format_check",
                                       "bad byte: " + ip_tokens[i]);
            }
        }

    } catch (std::exception const & e) {
        std::stringstream ss;
        ss << "\nip format #.#.#.# incorrect for ip input: " << ip;
        ss << "\n" << e.what();
        throw WarningException( "JsonSn", "ip_format_check", ss.str() );
    }
}

// -------------------------------------------------------------------------- //
inline
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
template <typename T>
void ask(Json & json,
         std::string const & key,
         std::string const & hint) {

    std::cout << "\n" << key;
    if (hint != "") std::cout << " (" << hint << ")";
    std::cout << ": ";

    T value;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    if ( std::cin.fail() ) {
        std::cin.clear();
        std::stringstream ss;
        ss << "error with user response for key " << key;
        throw WarningException( "JsonSn", "ask", ss.str() );
    }

    json[key] = value;
}

// -------------------------------------------------------------------------- //
template <typename T>
void ask(Json & json,
         Json const & original_json,
         std::string const & key,
         std::string const & hint) {

    std::cout << "\n" << key;
    if (hint != "") std::cout << " (" << hint << ")";
    std::cout << "[" << original_json[key] << "]";
    std::cout << ": ";

    std::string input;
    std::getline(std::cin, input);

    if ( std::cin.fail() ) {
        std::cin.clear();
        std::stringstream ss;
        ss << "error with user response for key " << key;
        throw WarningException( "JsonSn", "ask", ss.str() );
    }

    if (input == "") {
        json[key] = original_json[key];
        return;
    }

    if (std::is_same<T, std::string>::value) json[key] = input;
    else if (std::is_same<T, int>::value) json[key] = std::stoi(input);
    else throw std::logic_error("@JsonSn, ask, wrong type");
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
inline
Json json_change_ch(Json const & original_json) {

    Json ch_json;
    ask<std::string>( ch_json, original_json, "ip_remote",  "#.#.#.#");

    ip_format_check(ch_json["ip_remote"]);

    ask<int>(         ch_json, original_json, "port_remote", ">5000");
    ask<std::string>( ch_json, original_json, "auth_code", "");
    ask<int>(         ch_json, original_json, "port_host", ">2000");
    ask<int>(         ch_json, original_json, "protocol_version", "2");

    return ch_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_change_s(Json const & original_json) {

    Json s_json;
    ask<std::string>(s_json, original_json, "input", "A or B");
    ask<std::string>(s_json, original_json, "model", "STS2, STS1, etc");
    ask<std::string>(s_json, original_json, "cals", "STS2, STS2_HF_TEST, etc");

    std::cout << std::endl << "Has E300? (y/n): ";
    std::string response;
    std::getline(std::cin, response);

    if (response == "y") {
        s_json["port_e300"] = json_change_ch(original_json["port_e300"]);
    } else {
        s_json["port_e300"] = Json::object();
    }

    return s_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_change_q(Json const & original_json) {

    Json q_json;
    ask<std::string>(q_json, original_json, "serial_number", "012345ABCDEF");

    q_json["port_config"] = json_change_ch(original_json["port_config"]);
    q_json["sensor"] = original_json["sensor"];

    return q_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_change_dp(Json const & original_json) {

    Json dp_json;
    ask<std::string>(dp_json, original_json, "user", "rafa");
    ask<std::string>(dp_json, original_json, "pw", "");
    ask<std::string>(dp_json, original_json, "ip", "#.#.#.#");

    ip_format_check(dp_json["ip"]);

    return dp_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_change_st(Json const & original_json) {

    Json st_json;
    ask<std::string>(st_json, original_json, "station_name", "ABCDF");
    st_json["digitizer"] = original_json["digitizer"];
    st_json["data_processor"] = original_json["data_processor"];

    return st_json;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
inline
Json json_add_ch() {

    Json ch_json;
    ask<std::string>( ch_json, "ip_remote",  "#.#.#.#");
    ip_format_check(ch_json["ip_remote"]);

    ask<int>(         ch_json, "port_remote", ">5000");
    ask<std::string>( ch_json, "auth_code", "");
    ask<int>(         ch_json, "port_host", ">2000");
    ask<int>(         ch_json, "protocol_version", "2");


    return ch_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_add_s() {

    Json s_json;
    ask<std::string>(s_json, "input", "A or B");
    ask<std::string>(s_json, "model", "STS2, STS1, etc");
    ask<std::string>(s_json, "cals", "STS2, STS2_HF_TEST, etc");

    std::cout << std::endl << "Has E300? (y/n): "; std::string response;
    std::getline(std::cin, response);

    if (response == "y") {
        s_json["port_e300"] = json_add_ch();
    } else {
        s_json["port_e300"] = Json::object();
    }

    return s_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_add_q() {

    Json q_json;
    ask<std::string>(q_json, "serial_number", "012345ABCDEF");

    q_json["port_config"] =  json_add_ch();
    q_json["sensor"] = Json::array();

    return q_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_add_dp() {

    Json dp_json;
    ask<std::string>(dp_json, "user", "rafa");
    ask<std::string>(dp_json, "pw", "");
    ask<std::string>(dp_json, "ip", "#.#.#.#");

    ip_format_check(dp_json["ip"]);

    return dp_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_add_st() {

    Json st_json;
    ask<std::string>(st_json, "station_name", "ABCDF");
    st_json["digitizer"] = Json::array();
    st_json["data_processor"] = Json::array();

    return st_json;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
inline
Json json_from_ch(ConnectionHandler const & ch) {

    std::stringstream ss_auth_code;
    ss_auth_code << std::hex << ch.auth_code;

    Json ch_json = { {"ip_remote",        ch.ip_remote},
                     {"port_remote",      ch.port_remote},
                     {"auth_code",        ss_auth_code.str()},
                     {"port_host",        ch.uc.port_host()},
                     {"protocol_version", ch.protocol_version} };

    return ch_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_s(Sensor const & s) {

    std::stringstream ss_input;
    ss_input << s.config.input;

    Json s_json = { {"input", ss_input.str()},
                    {"model", s.config.model},
                    {"cals",  s.config.cals} };

    if (s.config.has_e300) {
        s_json["port_e300"] = json_from_ch( s.port_e300_const_ref() );
    } else {
        s_json["port_e300"] = Json::object();
    }

    return s_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_q(Digitizer const & q) {

    std::stringstream ss;
    ss << std::hex << std::uppercase << q.config.serial_number;

    Json q_json = { {"serial_number", ss.str()},
                    {"port_config",   json_from_ch(q.port_config)} };

    q_json["sensor"] = Json::array();

    for (auto i = 0; i < q.s.size(); i++) {
        q_json["sensor"][i] = json_from_s(q.s[i]);
    }

    return q_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_dp(DataProcessor const & dp) {

    Json dp_json = { {"user", dp.config.user},
                     {"pw",   dp.config.pw},
                     {"ip",   dp.config.ip} };

    return dp_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_st(Station const & st) {

    Json st_json = { {"station_name", st.config.station_name} };

    st_json["digitizer"] = Json::array();
    st_json["data_processor"] = Json::array();

    for (auto i = 0; i < st.q.size(); i++) {
        st_json["digitizer"][i] = json_from_q(st.q[i]);
    }

    for (auto i = 0; i < st.dp.size(); i++) {
        st_json["data_processor"][i] = json_from_dp(st.dp[i]);
    }

    return st_json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_sn(SeismicNetwork const & sn) {

    Json sn_json;

    sn_json["station"] = Json::array();

    for (auto i = 0; i < sn.st.size(); i++) {
        sn_json["station"][i] = json_from_st(sn.st[i]);
    }

    return sn_json;
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
            throw FatalException("SeismicNetwork", "setup",
                                 ss.str() );
        } else {
            return json[key]; }
    }
};

// -------------------------------------------------------------------------- //
inline
ConnectionHandler
ch_from_json(JsonRef const ch_json) {

    std::string ip_remote      = ch_json["ip_remote"];
    int const port_remote      = ch_json["port_remote"];
    std::string auth_code      = ch_json["auth_code"];
    int const port_host        = ch_json["port_host"];
    int const protocol_version = ch_json["protocol_version"];

    ConnectionHandler connection_handler(
        ip_remote,
        port_remote,
        std::strtoul(auth_code.c_str(), nullptr, 16),
        port_host,
        protocol_version);

    return connection_handler;
}

// -------------------------------------------------------------------------- //
inline
std::unique_ptr<ConnectionHandlerE300>
ch_e300_ptr_from_json(JsonRef const ch_json) {

    std::string ip_remote      = ch_json["ip_remote"];
    int const port_remote      = ch_json["port_remote"];
    std::string auth_code      = ch_json["auth_code"];
    int const port_host        = ch_json["port_host"];

    auto ch_ptr = std::make_unique<ConnectionHandlerE300>(
        ip_remote,
        port_remote,
        std::strtoul(auth_code.c_str(), nullptr, 16),
        port_host);

    return ch_ptr;
}

// -------------------------------------------------------------------------- //
inline
Sensor s_from_json(JsonRef const s_json) {

    std::string const input = s_json["input"];
    std::string const model = s_json["model"];
    std::string const cals  = s_json["cals"];

    if ( not s_json["port_e300"].empty() ) {

        auto ch_e300_json = s_json["port_e300"];

        Sensor s(
            input,
            model,
            cals,
            ch_e300_ptr_from_json(ch_e300_json)
        );

        return s;
    }

    // sensor without e300
    Sensor s(
        input,
        model,
        cals
    );

    return s;
}

// -------------------------------------------------------------------------- //
inline
Digitizer q_from_json(JsonRef const q_json) {

    std::string serial_number_string = q_json["serial_number"];

    uint64_t const serial_number = std::strtoul(serial_number_string.c_str(),
                                                nullptr,
                                                16);

    Json ch_config_json = q_json["port_config"];

    Digitizer q(
        serial_number,
        ch_from_json(ch_config_json)
    );

    for (auto const & s_json : q_json["sensor"]) {
        q.s.push_back( s_from_json(s_json) );
    }

    return q;
}

// -------------------------------------------------------------------------- //
inline
DataProcessor dp_from_json(JsonRef const dp_json) {

    std::string const user = dp_json["user"];
    std::string const pw   = dp_json["pw"];
    std::string const ip   = dp_json["ip"];

    DataProcessor dp(user, pw, ip);

    return dp;
}

// -------------------------------------------------------------------------- //
inline
Station st_from_json(JsonRef const st_json) {

    std::string const station_name = st_json["station_name"];

    Station st(station_name);

    for (auto const & q_json : st_json["digitizer"]) {
        st.q.push_back( q_from_json(q_json) );
    }

    for (auto const & dp_json : st_json["data_processor"]) {
        st.dp.push_back( dp_from_json(dp_json) );
    }

    return st;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_ta(SeismicNetwork const & sn, TargetAddress const & ta) {

    auto const scope = ta.scope();

    switch (scope) {

        case Scope::data_processor: {
            auto const & dp = sn.dp_const_ref(ta);
            return json_from_dp(dp);
        }

        case Scope::sensor: {
            auto const & s = sn.s_const_ref(ta);
            return json_from_s(s);
        }

        case Scope::digitizer: {
            auto const & q = sn.q_const_ref(ta);
            return json_from_q(q);
        }

        case Scope::station: {
            auto const & st = sn.st_const_ref(ta);
            return json_from_st(st);
        }

        case Scope::seismic_network: {
            return json_from_sn(sn);
        }

        default: throw std::logic_error{"@JsonSn::json_from_ta"};
    }
}

// -------------------------------------------------------------------------- //
inline
Json json_add_child_from_ta(SeismicNetwork const & sn,
                            TargetAddress const & parent_ta,
                            Scope const & child_scope) {

    auto const parent_scope = parent_ta.scope();

    switch (parent_scope) {

        case Scope::digitizer: {

            if (child_scope == Scope::sensor) return json_add_s();

             throw WarningException("JsonSn",
                                    "json_child_from_ta",
                                    "digitizer childs are sensors");
        }

        case Scope::seismic_network: {

             if (child_scope == Scope::station) return json_add_st();

             throw WarningException("JsonSn",
                                    "json_child_from_ta",
                                    "seismic network childs are stations");
        }

        case Scope::station: {

            switch (child_scope) {

                case Scope::digitizer      : return json_add_q();
                case Scope::data_processor : return json_add_dp();

                default: throw WarningException("JsonSn",
                                                "json_child_from_ta",
                                                "station childs are q and dp");
            }
        }

        default: throw std::logic_error{"@JsonSn::json_child_from_ta"};
    }
}

} // <- mzn

#endif
