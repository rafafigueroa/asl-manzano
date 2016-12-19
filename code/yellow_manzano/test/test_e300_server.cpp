// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "gtest/gtest.h"
#include "e300_server.h"

class FixtureE300Server : public ::testing::Test {

public:

    using Action = mzn::Action;
    using Kind   = mzn::Kind;
    using Target = mzn::Target;
    using Scope  = mzn::Scope;
    using TargetAddress  = mzn::TargetAddress;

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureE300Server, user_interpreter_setup) {}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


