// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "gtest/gtest.h"
#include "comm.h"
#include "seismic_network.h"
#include "user_interpreter.h"
#include "instruction_interpreter.h"

class FixtureUserInterpreter : public ::testing::Test {

public:

    using UserInstruction = mzn::UserInstruction;
    using Action = mzn::Action;
    using Kind   = mzn::Kind;
    using Target = mzn::Target;
    using Scope  = mzn::Scope;
    using TargetAddress  = mzn::TargetAddress;

    mzn::UserInterpreter user_interpreter;

    FixtureUserInterpreter() : user_interpreter() {}

    int const action_index = 0;
    int const kind_index = 1;
    int const target_address_index = 2;

    std::vector<std::string> test_user_inputs {
        "show config q0",           //0
        "edit config q1",
        "get poll q15",
        "show status q15s2",
        "show command st3",
        "edit target dp4",          //5
        "show config sn",
        "show config q1s2",
        "edit config q0s0",
        "show status st0q1s2",
        "show status st12q2s1",     //10
        "edit target s0",
        "start cal q0",
    };

    // ui = UserInstruction
    std::vector<UserInstruction> expected_uis {
        UserInstruction( Action::show,   Kind::config ), //0
        UserInstruction( Action::edit,   Kind::config ),
        UserInstruction( Action::get,    Kind::poll   ),
        UserInstruction( Action::show,   Kind::status ),
        UserInstruction( Action::show,   Kind::command),
        UserInstruction( Action::edit,   Kind::target ), //5
        UserInstruction( Action::show,   Kind::config ),
        UserInstruction( Action::show,   Kind::config ),
        UserInstruction( Action::edit,   Kind::config ),
        UserInstruction( Action::show,   Kind::status ),
        UserInstruction( Action::show,   Kind::status ), //10
        UserInstruction( Action::edit,   Kind::target ),
        UserInstruction( Action::start,  Kind::cal    ),
    };

    // ta = TargetAddress
    std::vector<TargetAddress> expected_tas {

        TargetAddress( Target{},
                       Target(Scope::digitizer,  0) ),      //0

        TargetAddress( Target{},
                       Target(Scope::digitizer,  1) ),      //1

        TargetAddress( Target{},
                       Target(Scope::digitizer, 15) ),      //2

        TargetAddress( Target{},
                       Target(Scope::digitizer, 15),
                       Target(Scope::sensor,     2) ),      //3

        TargetAddress( Target(Scope::station,    3) ),      //4

        TargetAddress( Target{},
                       Target(Scope::data_processor, 4) ),  //5

        TargetAddress{} ,                                   //6

        TargetAddress( Target{},
                       Target(Scope::digitizer, 1),
                       Target(Scope::sensor,    2) ),       //7

        TargetAddress( Target{},
                       Target(Scope::digitizer, 0),
                       Target(Scope::sensor,    0) ),       //8

        TargetAddress( Target(Scope::station,   0),
                       Target(Scope::digitizer, 1),
                       Target(Scope::sensor,    2) ),       //9

        TargetAddress( Target(Scope::station,  12),
                       Target(Scope::digitizer, 2),
                       Target(Scope::sensor,    1) ),       //10

        TargetAddress( Target{},
                       Target{},
                       Target(Scope::sensor,    1) ),       //11

        /* This result matches the previous, because parse_user_input
            doesn't bother writing it if no target is specified.*/
        TargetAddress( Target{},
                       Target{},
                       Target(Scope::sensor,    1) ),       //12
    };

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, user_interpreter_setup) {}

// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, user_input_to_user_instruction) {

    // changes user_input
    for (auto i = 0; i < test_user_inputs.size(); i++) {

        auto user_input = test_user_inputs[i];

        auto input_tokens = user_interpreter.parse_user_input(user_input);

        auto const action =
            user_interpreter.match_action(input_tokens[action_index]);

        auto const kind =
            user_interpreter.match_kind(input_tokens[kind_index]);

        auto const ui = UserInstruction(action, kind, "");

        EXPECT_EQ(expected_uis[i], ui);
    }
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, instruction_map_to_user_instruction) {

    using namespace mzn::InstructionMap;

    std::stringstream ss;

    for (auto const & ta : expected_tas) {

        ss << "\n\n" << ta;

        for (auto const & action : filter_actions(ta) ) {
            for (auto const & kind : filter_kinds(ta, action) ) {

                // fake the input of the user for all combinations
                // available in the instruction map
                std::stringstream ss_user_input;
                ss_user_input << action << " " << kind;
                auto user_input = ss_user_input.str();

                auto input_tokens = user_interpreter.parse_user_input(user_input);

                auto const action_result =
                    user_interpreter.match_action(input_tokens[action_index]);

                auto const kind_result =
                    user_interpreter.match_kind(input_tokens[kind_index]);

                EXPECT_EQ(action, action_result);
                EXPECT_EQ(kind, kind_result);

                ss << "\n"
                   << action << "-" << action_result << " "
                   << kind << "-" << kind_result;
            }
        }
    }

    std::cout << std::endl << ss.str() << std::endl;
}

// TODO: deal with default specific
// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, ta_token_to_ta) {
    /*

    auto const action = match_action(input_tokens[action_index]);
    auto const kind = match_kind(input_tokens[kind_index]);
    auto const ui = UserInstruction(action, kind, option);

    if (input_tokens.size() == 3) {

        auto ta = match_target_address(input_tokens[target_address_index]);

        // modifies ta
        instruction_interpreter.merge_default_target_address(ta);
        // both throw if error
        instruction_interpreter.check_target_address(ta);

        instruction_interpreter.check_instruction_map(ui, ta);
        instruction_interpreter.run_instruction(ui, ta);

    } else {

        // use the current default target address
        // no merging/checking of target_address needed
        instruction_interpreter.check_instruction_map(ui);
        instruction_interpreter.run_instruction(ui);

    }
        EXPECT_EQ(expected_returns[i], response);
        EXPECT_EQ(expected_instructions[i], ui);
        EXPECT_EQ(expected_target_addresses[i], target_address);
    }
*/
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, parse_user_input_too_many_arguments) {

    std::string input = "show target st0 q0";
    EXPECT_THROW(user_interpreter.parse_user_input(input),
                 mzn::WarningException);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureUserInterpreter, parse_user_input_too_few_arguments) {

    std::string input = "show";
    EXPECT_THROW(user_interpreter.parse_user_input(input),
                 mzn::WarningException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


