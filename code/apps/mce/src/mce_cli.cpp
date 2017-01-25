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
        std::cout << "\n";
        ii.cm.stream_output.show<Kind::target>(ta_);
        std::cout << "\n---------------------------------------------";
        ii.cm.stream_output.show_prompt(ta_);
        // std::cout << std::endl << " » ";

        // ----------------------------------------------------------------- //
        getline(std::cin, user_input);


        try {

            //! stream raw json format
            if (user_input == "raw") {
                auto const json = json_from_ta(ii.cm.sn, ta_);
                std::cout << json.dump(4) << std::endl;
                continue;
            }

            //! user hit enter
            if (user_input == "") continue;

            //! quit program normally
            if (user_input == "quit") break;

            //! only thing left is a target address
            auto const ta = UserInterpreter::match_target_address(user_input);
            ii.check_ta_in_sn(ta);
            ta_ = ta;

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

    std::cout << std::endl << "\nbye" << std::endl;
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
