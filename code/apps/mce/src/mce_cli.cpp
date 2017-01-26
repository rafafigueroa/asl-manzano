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

        auto & sn = ii.cm.sn;

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
        std::getline(std::cin, user_input);

        try {

            //! user hit enter
            if (user_input == "") continue;

            //! stream raw json format
            if (user_input == "raw") {
                auto const json = json_from_ta(sn, ta_);
                std::cout << json.dump(4) << std::endl;
                continue;
            }

            if (user_input == "save") {
                save_to_config_file(sn);
                continue;
            }

            if (user_input == "quit") {
                save_to_config_file(sn);
                break;
            }

            if (user_input[0] == '+') {
                add_to_config(sn, user_input, ta_);
                continue;
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
void MceCli::save_to_config_file(SeismicNetwork const & sn) const {

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    std::cout << std::endl << "saving to file\n";

    auto const json = json_from_ta(sn, TargetAddress{});

    config_fs << json.dump(4) << std::endl;


}

// -------------------------------------------------------------------------- //
void MceCli::create_empty_config_file() {

    // use home as default
    auto const home_path = get_environmental_variable("HOME");
    config_home_path = home_path + std::string("/.config/manzano");

    std::cout << std::endl << "config path: " << config_home_path;

    std::ofstream config_fs;

    config_fs.open(config_home_path + "/config.json",
                   std::ofstream::out | std::ofstream::trunc);

    config_fs << "{ \"station\": [] }";
}

// -------------------------------------------------------------------------- //
void MceCli::add_to_config(SeismicNetwork & sn,
                           std::string const & user_input,
                           TargetAddress const & ta) const {

    std::size_t scope_pos = 1;

    auto child_scope = UserInterpreter::match_scope(user_input, scope_pos);

    std::cout <<  "\nAdding a " << child_scope << " to " << ta << "\n";

    auto child_json = json_child_from_ta(sn, ta, child_scope);

    std::cout << std::endl << child_json.dump(4);

    std::cout << std::endl << "Looks good? (y/n): ";
    std::string response;
    std::getline(std::cin, response);

    if (response != "y") {std::cout << "OK, no changes"; return;}

    // it's on
    // --------------------------------------------------------------------- //
    switch (child_scope) {

        case Scope::data_processor: {
            auto & st = sn.st_ref(ta);
            st.dp.push_back( dp_from_json(child_json) );
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            q.s.push_back( s_from_json(child_json) );
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            st.q.push_back( q_from_json(child_json) );
            break;
        }

        case Scope::station: {
            sn.st.push_back( st_from_json(child_json) );
            break;
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    save_to_config_file(sn);
}

} // end namespace
