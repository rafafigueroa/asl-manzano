#include "user_interpreter.h"
#include <fstream>
#include <string>
#include <iterator>

namespace mzn {

// -------------------------------------------------------------------------- //
Action UserInterpreter::match_action(std::string const & token) {

    if (token == "show")    return Action::show;
    if (token == "edit")    return Action::edit;
    if (token == "get")     return Action::get;
    if (token == "set")     return Action::set;
    if (token == "plan")    return Action::plan;
    if (token == "auto")    return Action::auto_;
    if (token == "stop")    return Action::stop;
    if (token == "start")   return Action::start;

    // no match found
    std::stringstream error_msg;
    error_msg << "mismatch in action token \'" << token << "\' \n"
              << "\nactions: "
              << "\nshow edit get set plan auto stop start";

    throw WarningException("UserInterpreter",
                            "match_action",
                            error_msg.str() );
}

// -------------------------------------------------------------------------- //
Kind UserInterpreter::match_kind(std::string const & token) {

    if (token == "target")  return Kind::target;
    if (token == "config")  return Kind::config;
    if (token == "status")  return Kind::status;
    if (token == "command") return Kind::command;
    if (token == "poll")    return Kind::poll;
    if (token == "global")  return Kind::global;
    if (token == "ping")    return Kind::ping;
    if (token == "stat")    return Kind::stat;
    if (token == "qview")   return Kind::qview;
    if (token == "ctrl")    return Kind::ctrl;
    if (token == "reg")     return Kind::reg;
    if (token == "dereg")   return Kind::dereg;
    if (token == "center")  return Kind::center;
    if (token == "cal")     return Kind::cal;
    if (token == "pulse")   return Kind::pulse;
    if (token == "uptime")  return Kind::uptime;
    if (token == "plan")    return Kind::plan;
    if (token == "link")    return Kind::link;
    if (token == "wait")    return Kind::wait;
    if (token == "dev")     return Kind::dev;
    if (token == "output")  return Kind::output;

    // no kind found for that token
    //TODO: use InstructionMap
    std::stringstream error_msg;
    error_msg << "mismatch in kind token \'" << token << "\' \n"
              << "\nkinds: "
              << "\ntarget config status command poll global ping stat qview dev"
              << "\nctrl reg dereg center cal pulse plan output";

    throw WarningException("UserInterpreter",
                           "match_kind",
                           error_msg.str() );
}

// -------------------------------------------------------------------------- //
Scope UserInterpreter::match_scope(std::string const & token,
                                   std::size_t & token_index) {
    Scope scope;

    //! match by scope short name
    // there should be only ONE of each
    // something like q[0]q[1] would be an error

    if (token.find("q", token_index) == token_index) {
        scope = Scope::digitizer;
        token_index += 1;
    } else if (token.find("st", token_index) == token_index) {
        scope = Scope::station;
        token_index += 2;
    } else if (token.find("s", token_index) == token_index) {
        scope = Scope::sensor;
        token_index += 1;
    } else if (token.find("dp", token_index) == token_index) {
        scope = Scope::data_processor;
        token_index += 2;
    }

    else {

        std::stringstream error_msg;

        error_msg << "mismatch in token \'" << token << "\' \n";

        if (token.size() > 1) {
            error_msg << "at:" << Utility::underline_error(token, token_index);
        }

        error_msg << "\noptions: "
                  << "\nq st s dp";

        throw WarningException("UserInterpreter",
                               "match_scope",
                               error_msg.str() );
    }

    return scope;
}

// -------------------------------------------------------------------------- //
TargetAddress
UserInterpreter::match_target_address(std::string const & token) {

    // format SCOPE NUMBER (SCOPE NUMBER)*optionally
    // examples of the token string: "sn", "st0", "st1q0", "st0q0s1"

    // apply to all seismic network
    if (token == "sn"){
        return TargetAddress{};
    }

    TargetAddress ta{};

    // gets changed on the match_* with 2 arguments
    std::size_t token_index = 0;

    // while loop in case the address has more than one target in the ta
    while ( token_index < token.size() ) {

        Target target;

        target.scope = match_scope(token, token_index);
        target.index = Utility::match_positive_number(token, token_index);

        ta.add_target(target);
    }

    return ta;
}

// -------------------------------------------------------------------------- //
void UserInterpreter::run_user_input(std::string & user_input) {

    // order in which the tokens will be provided, for example:
    // get ping q0 = action kind target_address
    auto constexpr action_index = 0;
    auto constexpr kind_index = 1;
    auto constexpr target_address_index = 2;

    // container with all the token strings, checks number of tokens
    auto input_tokens = parse_user_input(user_input);

    std::string option;
    // an option can be specified by adding the mark ':', for example stat:boom
    // where "stat" is the Kind and "boom" is the option
    auto option_mark_location = input_tokens[kind_index].find(':');

    // the option mark ':' is found and is not the last char of the string
    if (option_mark_location != std::string::npos and
        input_tokens[kind_index].back() != ':') {

        // get and then erase the option from the kind token
        // get without the option mark: in "stat:boom", option = "boom"
        // erase including the option mark: from "stat:boom" to "stat"
        option = std::string(input_tokens[kind_index], option_mark_location + 1);
        input_tokens[kind_index].erase(option_mark_location);
    }

    auto const action = match_action(input_tokens[action_index]);
    auto const kind = match_kind(input_tokens[kind_index]);
    auto const ui = UserInstruction(action, kind, option);

    if (input_tokens.size() == 3) {

        // matches to whatever the user wrote, it can create an erroneous ta
        // like "st0s0" (instead of st0q0s0 or q0s0 or s0), gets checked next
        // it can also create an incomplete ta, like "s0" which is ok if
        // it can be merged on the next steps
        auto ta = match_target_address(input_tokens[target_address_index]);

        // also does basic checks for the target address
        // do the different targets in ta, actually exists in the sn?
        instruction_interpreter.merge_and_check_target_address(ta);

        // is this intructin valid? good: get ping, bad: set ping
        instruction_interpreter.check_instruction_map(ui, ta);
        instruction_interpreter.run_instruction(ui, ta);

    } else {

        // use the current target address
        // no merging/checking of target_address needed
        instruction_interpreter.check_instruction_map(ui);
        instruction_interpreter.run_instruction(ui);

    }
}

// -------------------------------------------------------------------------- //
void UserInterpreter::user_input_loop(std::string const & qrun_fname) {

    auto const runtime_config_path = Utility::get_runtime_config_path();
    std::string const qrun_path = runtime_config_path + "/" + qrun_fname;

    std::ifstream qrun_fs(qrun_path);
    //qrun_fs.open(qrun_path);

    if (!qrun_fs) {
        throw FatalException("UserInterpreter",
                             "user_input_loop",
                             std::string("can't open script file") + qrun_path);
    }

    std::string user_input;

    instruction_interpreter.show_prompt();

    while ( std::getline(qrun_fs, user_input) ) {

        try {

            //! quit program normally
            if (user_input == "quit") {

                UserInstruction const ui(Action::quit, Kind::mzn);
                instruction_interpreter.run_instruction(ui);
                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            run_user_input(user_input);
            instruction_interpreter.show_prompt();

        } catch (Exception const & e) {

            std::cerr << std::endl
                      << "caught error @ UserInterpreter::user_input_loop(qrun)"
                      << std::endl << e.what();
            break;
        }
    }
}

// -------------------------------------------------------------------------- //
void UserInterpreter::user_input_loop() {
    // prompts the user for their command
    // interactive

    std::string user_input;

    while (true) {

        instruction_interpreter.show_prompt();

        getline(std::cin, user_input);

        std::cout << std::flush << "### " << Time::sys_time_of_day() << " ###";

        try {

            if (user_input == "") {
                continue;
            }

            if (user_input == "..") {
                instruction_interpreter.current_ta_remove_one_target();
                continue;
            }

            //! quit program normally
            if (user_input == "quit") {

                UserInstruction const ui(Action::quit, Kind::mzn);
                instruction_interpreter.run_instruction(ui);
                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            if (user_input == "help") {
                instruction_interpreter.cm.stream_output.show_help();
                continue;
            }

            run_user_input(user_input);

        } catch (FatalException const & e) {
            std::cerr << std::endl
                      << "caught fatal error @UserInterpreter::user_input_loop()"
                      << std::endl << e.what();
            break;

        } catch (Exception const & e) {
            std::cerr << std::endl
                      << "caught error @UserInterpreter::user_input_loop()"
                      << std::endl << e.what();
        }
    }
}

// -------------------------------------------------------------------------- //
std::vector<std::string>
UserInterpreter::parse_user_input(std::string & user_input) {

    std::stringstream ss(user_input);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> input_tokens(begin, end);

    if (input_tokens.size() < 2) {
        throw WarningException("UserInterpreter",
                               "parse_user_input",
                               "minimum of two arguments");
    }

    if (input_tokens.size() > 3) {

        std::stringstream error_msg;
        error_msg << "too many arguments";

        error_msg << "\n\narguments received       : ";
        for (int i = 0; i < input_tokens.size() ; i++) {
            error_msg << input_tokens[i] << " ";
        }

        error_msg << "\narguments not understood : ";
        for (int i = 3; i < input_tokens.size() ; i++) {
            error_msg << input_tokens[i] << " ";
        }

        error_msg << "\n\n";

        throw WarningException("UserInterpreter",
                               "parse_user_input",
                               error_msg.str() );
    }

    return input_tokens;
}

} // <- mzn
