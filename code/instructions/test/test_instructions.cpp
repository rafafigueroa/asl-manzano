// Manzano
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "instruction_map.h"
#include <typeinfo>

class FixtureInstructions : public ::testing::Test {

public:

    using UserInstruction = mzn::UserInstruction;
    using Action = mzn::Action;
    using Kind   = mzn::Kind;
    using Target = mzn::Target;
    using Scope  = mzn::Scope;
    using TargetAddress  = mzn::TargetAddress;

    std::vector<TargetAddress> const tas;

    mzn::InstructionMap im;

    // a sample of current Targets
    FixtureInstructions() : tas {

        TargetAddress( Target{},
                       Target{},
                       Target{} ),

        TargetAddress( Target(Scope::station,   0),
                       Target{},
                       Target{} ),

        TargetAddress( Target(Scope::station,   0),
                       Target(Scope::digitizer, 0),
                       Target{} ),

        TargetAddress( Target(Scope::station,   0),
                       Target(Scope::digitizer, 0),
                       Target(Scope::sensor, 0) ),

        TargetAddress( Target(Scope::station,   0),
                       Target(Scope::digitizer, 0),
                       Target(Scope::data_processor, 0) )

    } {}


};

// Creates the TargetAddress samples
// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructions, instructions_setup) {}

TEST_F(FixtureInstructions, new_instruction_map) {

    auto & m_sa = im.sa_map.at(Scope::digitizer);
    auto & m_ak = m_sa.at(Action::get);
    auto & m_ko = m_ak.at(Kind::stat);

    std::stringstream ss;
    ss << '\n' << im.sa_map.at(Scope::digitizer).at(Action::get).at(Kind::stat)[0];
    ss << '\n' << im.sa_map;

    std::cout << ss.str();
}

// These tests assume the test stations during development
// They will eventually call the dummy dataset and dummy q330
// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructions, instructions_cout) {

    using IM = mzn::InstructionMap;

    std::stringstream ss;
    for (auto const & ta : tas) {

        ss << "\n\n" << ta;

        ss << "\n\n GREEN\n";

        for (auto const & action : IM::filter_actions(ta) ) {
            for (auto const & kind : IM::filter_kinds(ta, action) ) {

                auto ui = UserInstruction(action, kind);
                ss << ui;

                auto options = IM::filter_options(action, kind);

                if ( options.empty() and not IM::has_empty_option(action, kind) ) {
                    throw std::logic_error{"options green"};
                }

                ss << " [";
                for (auto const & option : options) ss << option << ", ";
                ss << "]";
            }
        }

        ss << "\n\n RED\n";

        for (auto const & action : IM::filter_actions_gui(ta) ) {
            for (auto const & kind : IM::filter_kinds_gui(ta, action) ) {

                auto ui = UserInstruction(action, kind);
                ss << ui;

                auto options = IM::filter_options(action, kind);

                if ( options.empty() and not IM::has_empty_option(action, kind) ) {
                    throw std::logic_error{"options red"};
                }

                ss << " [";
                for (auto const & option : options) ss << option << ", ";
                ss << "]";
            }
        }
    }

    // std::cout << std::endl << ss.str();
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


