// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "gtest/gtest.h"
#include "comm.h"
#include "seismic_network.h"
#include "instruction_interpreter.h"
#include "network_loop.h"
#include "json.hpp"

class FixtureNetworkLoop: public ::testing::Test {

public:

    using BasicJson = nlohmann::json;

    BasicJson json_obj;
    mzn::NetworkLoop nl;
    std::stringstream ss;

    FixtureNetworkLoop() :
            json_obj{BasicJson::object()},
            nl{json_obj}
            {}

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureNetworkLoop, network_loop_setup) {}


// -------------------------------------------------------------------------- //
TEST_F(FixtureNetworkLoop, network_loop_json_adding) {

    BasicJson j = BasicJson::object();
    std::string const name = "DEV";
    std::string const ip =  "123.234.123.234";


    j[name] = {"ip", ip};

    try{
        nl.add(name, ip);

        ASSERT_EQ(j,json_obj);
    } catch (mzn::WarningException e) {
        FAIL();
    }

}

// -------------------------------------------------------------------------- //
/*
TEST_F(FixtureNetworkLoop, ip_validate) {
    std::string ip1 = "123.234.5.6";
    std::string ip2 = "256.534.12.128";
    std::string ip3 = "10.10.10.10";
    std::string ip4 = "10.10.10";
    std::string ip5 = "127127127127";

    std::string name = "TEST:";

    EXPECT_NO_THROW( nl.add(name, ip1) );
    EXPECT_THROW( nl.add(name, ip2), mzn::WarningException );
    EXPECT_NO_THROW( nl.add(name, ip3) );
    EXPECT_THROW( nl.add(name, ip4), mzn::WarningException );
    EXPECT_THROW( nl.add(name, ip5), mzn::WarningException );
}
*/


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
;

