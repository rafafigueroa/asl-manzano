// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "gtest/gtest.h"
#include "comm.h"
#include "seismic_network.h"
#include "user_interpreter.h"

class FixtureMce : public ::testing::Test {

public:

    mzn::Mce mce;

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureMce, mce_setup) {}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


