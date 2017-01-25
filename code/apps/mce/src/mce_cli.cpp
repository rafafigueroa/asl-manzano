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

        // rebuild TODO change sn contructor, total rebuild not needed
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

            if (user_input == "save") {
                save_to_config_file(ii.cm.sn);
                continue;
            }

            if (user_input == "quit") {
                save_to_config_file(ii.cm.sn);
                break;
            }

            //! quit program without saving
            if (user_input == "cancel") break;

            //! only thing left is a target address
            auto ta = UserInterpreter::match_target_address(user_input);

            ta.add_targets_from_ta(ta_);
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
void MceCli::save_to_config_file(SeismicNetwork const & sn) {

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    std::cout << std::endl << "saving to file\n";

    auto const json = json_from_ta(sn, TargetAddress{});

    config_fs << json.dump(4) << std::endl;
}

// -------------------------------------------------------------------------- //
void MceCli::create_empty_config_file() {

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    // empty json object
    config_fs << "{\n}";
}

} // end namespace
