#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "gtest/gtest.h"
#include "seismic_network.h"
#include "mzn_except.h"

class FixtureCore : public ::testing::Test {
public:

    mzn::SeismicNetwork sn;
    using Message = mzn::Qdp::Message;
    virtual void SetUp() {

        // assign test values and run a
        // simple test to confirm right name/type
        // and function call operator
    }
    //virtual void TearDown() {};

};

TEST_F(FixtureCore, seismic_network_setup) {
}

TEST_F(FixtureCore, udp_server) {

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
