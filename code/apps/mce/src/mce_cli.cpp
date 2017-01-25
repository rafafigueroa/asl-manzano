// ** command line interface ** //
// Manzano Software //
#include "mce_cli.h"
#include "json_sn.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void MceCli::user_input_loop() {
    // prompts the user for their command
    // interactive

    std::string user_input;

    while (true) {

        // rebuild
        // ----------------------------------------------------------------- //
        InstructionInterpreter ii(ta_);

        // check ta_ again, this time without catching
        // ----------------------------------------------------------------- //
        // ta_.sn_child = Target(Scope::station, 0);
        ii.check_ta_in_sn(ta_);

        // show prompt
        // ----------------------------------------------------------------- //
        std::cout << std::endl << "\n---------------------------------------------";
        ii.cm.stream_output.show<Kind::target>(ta_);
        std::cout << std::endl << "---------------------------------------------\n";
        std::cout << "\n » ";

        // ----------------------------------------------------------------- //
        getline(std::cin, user_input);

        try {

            if (user_input == "") continue;

            if (user_input == "raw") {
                auto const json = json_from_ta(ii.cm.sn, ta_);
                std::cout << json;
            }

            //! quit program normally
            if (user_input == "quit") {
                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            std::cout << "\n" << user_input;

        } catch (FatalException const & e) {

            std::cerr << std::endl
                      << "caught fatal error @Mce::user_input_loop()"
                      << std::endl << e.what();
            break;

        } catch (Exception const & e) {

            std::cerr << std::endl
                      << "caught error @Mce::user_input_loop()"
                      << std::endl << e.what();
        }
    }
}

// -------------------------------------------------------------------------- //
void MceCli::create_empty_config_file() {

    auto const home_path = get_environmental_variable("HOME");

    auto const config_home_path = home_path + std::string("/.config/manzano");

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    // empty json object
    config_fs << "{\n}";
}

} // end namespace
