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

            //! ta to parent
            if (user_input == "..") {
                ta_.remove_one_target();
                continue;
            }

            //! stream raw json format
            if (user_input == "raw") {
                auto const json = json_from_ta(sn, ta_);
                std::cout << json.dump(4) << std::endl;
                continue;
            }

            if (user_input == "rm") {
                // changes ta_ to parent
                remove_from_config(sn, ta_);
                continue;
            }

            if (user_input == "ed") {
                // same ta_
                change_config(sn, ta_);
                continue;
            }

            if (user_input[0] == '+') {
                // changes ta_ to newly added
                add_to_config(sn, user_input, ta_);
                continue;
            }

            if (user_input == "quit") break;

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
                           TargetAddress & ta) const {

    std::size_t scope_pos = 1;

    auto child_scope = UserInterpreter::match_scope(user_input, scope_pos);
    std::cout <<  "\nAdding a " << child_scope << " to " << ta << "\n";
    auto child_json = json_add_child_from_ta(sn, ta, child_scope);
    std::cout << std::endl << child_json.dump(4);

    auto const confirm = ask_yes("Looks good");
    if (not confirm) {std::cout << "OK, no changes"; return;}

    TargetAddress child_ta;

    // it's on
    // --------------------------------------------------------------------- //
    switch (child_scope) {

        case Scope::data_processor: {
            auto & st = sn.st_ref(ta);
            st.dp.push_back( dp_from_json(child_json) );
            auto const child_index = st.dp.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.st_child = child_target;
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            q.s.push_back( s_from_json(child_json) );
            auto const child_index = q.s.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.q_child = child_target;
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            st.q.push_back( q_from_json(child_json) );
            auto const child_index = st.q.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.st_child = child_target;
            break;
        }

        case Scope::station: {
            sn.st.push_back( st_from_json(child_json) );
            auto const child_index = sn.st.size() - 1;
            auto const child_target = Target(child_scope, child_index);
            child_ta.sn_child = child_target;
            break;
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    save_to_config_file(sn);
// child_ta is incomplete, lets add the parents
    child_ta.add_targets_from_ta(ta);
    // this should make it easier to modify this newly created object
    ta = child_ta;
}

// -------------------------------------------------------------------------- //
void MceCli::remove_from_config(SeismicNetwork & sn,
                                TargetAddress & ta) const {

    // confirm first
    auto const target = ta.target();
    std::cout <<  "\nRemoving " << target << " from " << ta << "\n";
    auto json = json_from_ta(sn, ta);
    std::cout << std::endl << json.dump(4);

    auto const confirm = ask_yes("Remove");
    if (not confirm) {std::cout << "OK, no changes"; return;}

    // since digitizers (and then stations) can't have copy/move assignment
    // due to const members by design, std::vector<T>.erase(..) can't be used.
    // However, move constructors are provided.
    // --------------------------------------------------------------------- //
    auto remove_without_assignment = [](auto & v, auto const pos) {

        // reset v
        auto temp_v = std::move(v);
        v.clear();

        // refill v without the element
        for (int i = 0; i < temp_v.size(); i++) {
            // use move constructor which is defined
            if (i != pos) v.push_back( std::move(temp_v[i]) );
        }
    };

    // it's on
    // --------------------------------------------------------------------- //
    switch (target.scope) {

        case Scope::data_processor: { auto & st = sn.st_ref(ta);
            st.dp.erase(st.dp.begin() + target.index);
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            q.s.erase(q.s.begin() + target.index);
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            remove_without_assignment(st.q, target.index);
            break;
        }

        case Scope::station: {
            remove_without_assignment(sn.st, target.index);
            break;
        }

        case Scope::seismic_network: {
            sn.st.clear();
            break;
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    save_to_config_file(sn);

    // this target no longer exist, so ta is set to its parent
    ta.remove_one_target();
}

// -------------------------------------------------------------------------- //
void MceCli::change_config(SeismicNetwork & sn,
                           TargetAddress const & ta) const {

    // confirm first
    auto const target = ta.target();
    std::cout << "\nChanging " << target << " from " << ta << "\n";
    auto original_json = json_from_ta(sn, ta);

    std::cout << "\n == JSON original ==";
    std::cout << std::endl << original_json.dump(4);

    // since digitizers (and then stations) can't have copy/move assignment
    // due to const members by design, std::vector<T>.at(..) can't be used.
    // However, move constructors are provided.
    // --------------------------------------------------------------------- //
    auto replace_without_assignment = [](auto & v,
                                         auto const pos,
                                         auto & rhs) {
        // reset v
        auto temp_v = std::move(v);
        v.clear();

        // refill v with a different element rhs at pos
        for (int i = 0; i < temp_v.size(); i++) {
            // use move constructor which is defined
            if (i == pos) v.push_back( std::move(rhs) );
            else v.push_back( std::move(temp_v[i]) );
        }
    };

    // --------------------------------------------------------------------- //
    auto cancel_change = [](auto & json) {
        std::cout << "\n == JSON modified ==";
        std::cout << std::endl << json.dump(4);
        auto const confirm = ask_yes("Change");
        if (not confirm) std::cout << "OK, no changes";
        return not confirm;
    };

    // each one confirms
    // --------------------------------------------------------------------- //
    switch (target.scope) {

        case Scope::data_processor: {
            auto & st = sn.st_ref(ta);
            auto json = json_change_dp(original_json);
            if ( cancel_change(json) ) return;
            auto dp = dp_from_json(json);
            replace_without_assignment(st.dp, target.index, dp);
            break;
        }

        case Scope::sensor: {
            auto & q = sn.q_ref(ta);
            auto json = json_change_s(original_json);
            if ( cancel_change(json) ) return;
            auto s = s_from_json(json);
            replace_without_assignment(q.s, target.index, s);
            break;
        }

        case Scope::digitizer: {
            auto & st = sn.st_ref(ta);
            auto json = json_change_q(original_json);
            if ( cancel_change(json) ) return;
            auto q = q_from_json(json);
            replace_without_assignment(st.q, target.index, q);
            break;
        }

        case Scope::station: {
            auto json = json_change_st(original_json);
            if ( cancel_change(json) ) return;
            auto st = st_from_json(json);
            replace_without_assignment(sn.st, target.index, st);
            break;
        }

        case Scope::seismic_network: {
            throw WarningException("MceCli",
                                   "change_config",
                                   "sn not a change target");
        }

        default: throw std::logic_error{"@MceCli::add_to_config"};
    }

    save_to_config_file(sn);
}

} // end namespace
