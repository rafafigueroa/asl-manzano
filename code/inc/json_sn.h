#ifndef _MZN_JSON_SN_H_
#define _MZN_JSON_SN_H_

// to get environment variable
#include <string>
#include <exception>
#include <cstdlib>
#include "mzn_cmake_config.h"
#include "mzn_except.h"
#include "json.h"

namespace mzn {

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


/*!
    "sensor":
        [
          {
            "input": "A",
            "model": "STS2",
            "cals": "TIMED_SEND_TEST"
          },

          {
            "input": "B",
            "model": "STS1",
            "cals": "STS_1_E300",
            "port_e300": {"ip_remote":"x.x.x.x", "port_remote":5009,
                          "auth_code":"x", "port_host":2009,
                          "protocol_version": 0}
          }

        ]
 */
// -------------------------------------------------------------------------- //
inline
Json json_from_s(Sensor const & s) {

    std::stringstream ss_input;
    ss_input << s.config.input;

    Json json = { {"input", ss_input.str()},
                  {"model", s.config.model},
                  {"cals",  s.config.cals} };
    return json;
}

/*!
    "port_config": {"ip_remote":"x.x.x.x", "port_remote":5000,
                        "auth_code":"x", "port_host":2000,
                        "protocol_version": 2},
 */
// -------------------------------------------------------------------------- //
inline
Json json_from_ch(ConnectionHandler const & ch) {

    Json json = { {"ip_remote",   ch.ip_remote},
                  {"port_remote", ch.port_remote},
                  {"auth_code", ch.auth_code},
                  {"port_host",   ch.uc.port_host()},
                  {"protocol_version", ch.protocol_version} };
    return json;
}

/*!
"digitizer":
    [
      {
        "serial_number": "01ABC",

        "port_config": {"ip_remote":"x.x.x.x", "port_remote":5000,
                        "auth_code":"x", "port_host":2000,
                        "protocol_version": 2},

        "sensor":
        [
          {
            "input": "A",
            "model": "STS2",
            "cals": "TIMED_SEND_TEST"
          },

          {
            "input": "B",
            "model": "STS1",
            "cals": "STS_1_E300",
            "port_e300": {"ip_remote":"x.x.x.x", "port_remote":5009,
                          "auth_code":"x", "port_host":2009,
                          "protocol_version": 0}
          }

        ]
      }
    ],
 */
// -------------------------------------------------------------------------- //
inline
Json json_from_q(Digitizer const & q) {

    Json json = { {"serial_number", q.config.serial_number},
                  {"port_config", json_from_ch(q.port_config)},
                  {"sensor",  json_from_s(q.s[0])} };
    return json;
}
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

    return connection_handler;
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

    return ch_ptr;
}

// -------------------------------------------------------------------------- //
inline
Digitizer q_from_json(JsonRef const q_json) {

    std::string serial_number_string    = q_json["serial_number"];

    uint64_t const serial_number =
        std::strtoul(serial_number_string.c_str(), nullptr, 16);

    auto connection_handler_config_json  = q_json["port_config"];

    Digitizer q(
        serial_number,
        ch_from_json(connection_handler_config_json)
    );

    return q;
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
DataProcessor dp_from_json(JsonRef const dp_json) {

    std::string const user = dp_json["user"];
    std::string const pw   = dp_json["pw"];
    std::string const ip   = dp_json["ip"];

    DataProcessor dp(user, pw, ip);

    return dp;
}

//! "data_processor":{"ip":"x.x.x.x", "user":"x", "pw":"x"}
// -------------------------------------------------------------------------- //
inline
Json json_from_dp(DataProcessor const & dp) {

    Json json;
    json["data_processor"] = { {"user", dp.config.user},
                               {"pw", dp.config.pw},
                               {"ip", dp.config.ip} };
    return json;
}

// -------------------------------------------------------------------------- //
inline
Json json_from_ta(SeismicNetwork const & sn, TargetAddress const & ta) {

    auto const scope = ta.scope();

    switch (scope) {

        case Scope::data_processor: {
            auto const & dp = sn.dp_const_ref(ta);
            return json_from_dp(dp);;
        }

        case Scope::sensor: {
            auto const & s = sn.s_const_ref(ta);
            return json_from_s(s);
        }

        case Scope::digitizer: {
            auto const & q = sn.q_const_ref(ta);
            return json_from_q(q);
        }

        /*

        case Scope::seismic_network: {

        case Scope::station: {
            auto const & st = sn.st_const_ref(ta);
            auto const json = json_from_st(st);
            return json;
        }

        */

        default: throw std::logic_error{"@JsonSn::json_from_ta"};
    }
}

} // <- mzn

#endif
