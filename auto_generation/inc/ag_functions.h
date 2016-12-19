
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <bitset>
#include <algorithm>

#include "json.h"

using Json = mzn::Json;
// -------------------------------------------------------------------------- //
std::string to_camel_case(std::string cn) {

    // First letter always to upper case
    cn[0] = std::toupper(cn[0]);

    // Next letters when an underscore is found
    std::size_t underscore_pos;
    while ( (underscore_pos = cn.find("_") ) != std::string::npos ) {
        // check for underscore at the end
        if (underscore_pos == cn.size() ) {
            std::cerr << "ERROR: cmd name ending in underscore";
            std::exit(EXIT_FAILURE);
        }

        // Delete underscore and change first letter after
        cn.erase(underscore_pos, 1);
        cn[underscore_pos] = std::toupper(cn[underscore_pos]);
    }

    return cn;
}

// -------------------------------------------------------------------------- //
void mc_key_names(Json const & cmds_json,
                  std::ofstream & c_fs,
                  std::string const & cmd_name,
                  int const knn) {

    // Since the mc
    for (int kni = 0; kni < knn; kni++) {

        std::string key_name =
            cmds_json[cmd_name.c_str()]["cmd_map"][kni]["key_name"];
        c_fs << "\n";
        c_fs << "#include \"" << key_name << ".h\"";
    }

    c_fs << "\n";

    for (int kni = 0; kni < knn; kni++) {

        std::string key_name =
            cmds_json[cmd_name.c_str()]["cmd_map"][kni]["key_name"];
        c_fs << "\n";
        c_fs << "#define k_" << key_name << " " << kni;
    }

}

// -------------------------------------------------------------------------- //
void cmd_custom_new_ic_map(Json const & cmds_json,
                           std::ofstream & c_fs,
                           std::string const & cmd_name,
                           int const knn) {

    for (int kni = 0; kni < knn; kni++) {

        std::string key_name =
            cmds_json[cmd_name.c_str()]["cmd_map"][kni]["key_name"];

        std::string key_class_name = to_camel_case(key_name); // copy

        c_fs << "\n        ";
        c_fs << "case k_" << key_name << " : {";
        c_fs << "\n";
        c_fs << "\n            ";
        c_fs << "inner_commands.push_back(";
        c_fs << "\n            ";
        c_fs << "    std::unique_ptr<Command>{ std::make_unique<";
        c_fs << key_class_name << ">() } );";
        c_fs << "\n            ";
        c_fs << "break;";
        c_fs << "\n        ";
        c_fs << "}";
    }
}

// -------------------------------------------------------------------------- //
void cmd_custom_new_ic(Json const & cmds_json,
                       std::ofstream & c_fs,
                       std::string const & cmd_name,
                       int const knn) {


    std::string key_name =
        cmds_json[cmd_name.c_str()]["cmd_map"][0]["key_name"];

    std::string key_class_name = to_camel_case(key_name); // copy

    c_fs << "\n    ";
    c_fs << "inner_commands.push_back(";
    c_fs << "\n    ";
    c_fs << "    std::unique_ptr<Command>{ std::make_unique<";
    c_fs << key_class_name << ">() } );";
    c_fs << "\n";
}

// -------------------------------------------------------------------------- //
void custom_command_fields(Json const & cmds_json,
                           std::ofstream & c_fs,
                           std::string const & cmd_name,
                           int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    ";

        // now write it as cmd_field in the command
        // except for bitmaps, which have the letters Bm in position 0
        // also except when the whole type is given
        if ( cf_type.find("Bm") == 0  or
             cf_type.find("CmdField") != std::string::npos ) {

            c_fs << cf_type << " " << cf_name << ";";
        } else {
            c_fs << "CmdField" << cf_type << ", " << N << "> " << cf_name << ";";
        }
    }
}

// -------------------------------------------------------------------------- //
bool json_has_key(Json const & json, std::string const & key) {
    return ( json.find(key) != json.end() );
}

// -------------------------------------------------------------------------- //
void custom_cf_units(Json const & cmds_json,
                     std::ofstream & c_fs,
                     std::string const & cmd_name,
                     int const cfn) {

    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << ",\n    " << cf_name << "(";;

        if ( json_has_key(cf_json, "cf_units") ) {

            std::string cf_units = cf_json["cf_units"];
            c_fs << "\"" << cf_units << "\"";
        }

        c_fs << ")";
    }
}

// -------------------------------------------------------------------------- //
void custom_print(Json const & cmds_json,
                  std::ofstream & c_fs,
                  std::string const & cmd_name,
                  int const cfn,
                  std::string const & cmd_class_name) {

    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        // printing name
        std::string cf_name =cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];

        // dont print ignored cmd_field
        if ( cf_type.find("Ignore") == std::string::npos ) {

            c_fs << "\n\n"
                 << "    cmd_os << \"\\n"  << cf_name << ": \";"
                 << " cmd_os << " << cf_name << ";";
        }
    }
}

// -------------------------------------------------------------------------- //
void custom_msg_to_data(Json const & cmds_json,
                        std::ofstream & c_fs,
                        std::string const & cmd_name,
                        int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    mf_begin = ";

        // when the type is to be ignored, just move the mf_begin counter
        if ( cf_type.find("Ignore") == std::string::npos ) {
            c_fs << cf_name << ".msg_to_data(msg, mf_begin);";
        } else {
            c_fs << "mf_begin + " << N << "; // ignore type";
        }

    }
};

// -------------------------------------------------------------------------- //
void custom_data_to_msg(Json const & cmds_json,
                        std::ofstream & c_fs,
                        std::string const & cmd_name,
                        int const cfn) {

    // go over all command fields
    for (int cfi = 0; cfi < cfn; cfi++) {

        auto const cf_json = cmds_json[cmd_name.c_str()]["cmd_fields"][cfi];

        std::string cf_name = cf_json["cf_name"];
        std::string cf_type = cf_json["cf_type"];
        int N = cf_json["cf_size"];

        c_fs << "\n    mf_begin = ";

        // when the type is to be ignored, just move the mf_begin counter
        if ( cf_type.find("Ignore") == std::string::npos ) {
            c_fs << cf_name << ".data_to_msg(msg, mf_begin);";
        } else {
            c_fs << "mf_begin + " << N << "; // ignore type";
        }
    }
};

