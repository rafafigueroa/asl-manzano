// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "gtest/gtest.h"
#include "comm.h"
#include "seismic_network.h"
#include "instruction_interpreter.h"
#include "builder_loop.h"
#include "json.hpp"

class FixtureBuilderLoop: public ::testing::Test {

public:

    using BasicJson = nlohmann::json;

    mzn::BuilderLoop bl;
    std::stringstream ss;

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_setup) {}

//  Tests for the BasicJson library being used by this program

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_create) {
    BasicJson j = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    BasicJson j2 = {
            {   "pi", 3.141},
            {   "happy", true},
    };

    ASSERT_EQ(j,j2);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_combine) {

    BasicJson j = {
            {   "pi", 3.141},
            {   "happy", true},
            {   "name", "Niels"},
            {   "nothing", nullptr},
            {   "answer", {
                    {   "everything", 42}
            }},
            //{"list", {1, 0, 2}},
            {   "object", {
                    {   "currency", "USD"},
                    {   "value", 42.99}
            }}
    };

    BasicJson j2 = {
            {   "list", {1, 0, 2}},
            {   "second item", "gets passed in too"}
    };

    for(BasicJson::iterator it = j2.begin(); it != j2.end(); ++it) {
        j[it.key()] = it.value();
    }

    ss << j;
    ss << "\n";

    std::cout << ss.str() << '\n';
};

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_iter) {

    BasicJson j = {
            {   "pi", 3.141},
            {   "happy", true},
            {   "name", "Niels"},
    };

    for(BasicJson::iterator it = j.begin(); it != j.end(); ++it) {

        //Today I learned C++ doesn't allow switch-case on string literals
        if ( it.key() == "pi" ) {
            EXPECT_FLOAT_EQ(it.value(),3.141);
        }
        else if ( it.key() == "happy" ) {
            EXPECT_TRUE((bool)it.value());
        }
        else if ( it.key() == "name" ) {
            EXPECT_EQ(it.value(),"Niels");
        }
        else {
            // did something weird happen to the key set?
            FAIL() << "Unexpected key in json object";
        }

    }

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_contains) {

    BasicJson j = {
            {   "pi", 3.141},
            {   "happy", true},
            {   "name", "Niels"},
    };

    // Gtest doesn't seem to be able to handle doing this comparison directly
    EXPECT_TRUE(j.find("pi") != j.end());

    EXPECT_TRUE(j.find("invalid") == j.end());

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_duplicates) {

    BasicJson j =
            BasicJson::parse("{ \"name\": \"Niels,\", \"name\": \"Aaron\" }");

    // trying to throw it an already-formatted JSON file like in previous
    // test cases will, apparently, throw a segmentation fault

    // duplciate items get overwritten by most recent instance?

    ASSERT_TRUE(j.find("name") != j.end());

    EXPECT_NE(j.at("name"), "Niels");

    EXPECT_EQ(j.at("name"), "Aaron");
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_insert) {

    std::string s1_name = "station1";

    BasicJson j = {
            {   s1_name, nullptr}
    };

    BasicJson j2 = {
            {   "sensor1", "parameters"},
            {   "proces1", "parameters"},
    };

    j[s1_name] = j2;

    ASSERT_EQ(j.at(s1_name).size(), j2.size());

    ss << j;
    std::cout << ss.str() << '\n';

};

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_json_io) {

    std::string const runtime_config_path {k_mzn_runtime_config_DIR};
    std::string fname = runtime_config_path + "/config.json";

    std::ifstream config_fs;
    config_fs.open(fname);
    std::stringstream buffer;
    buffer << config_fs.rdbuf();

    BasicJson j = BasicJson::parse(buffer.str());

    bl.read_json(fname);
    BasicJson j2 = bl.get_json();

    ASSERT_EQ(j, j2);

    ss << std::setw(4) << j << std::endl;

    // TODO: rather than display, how can we compare this with the above file?
    std::cout << ss.str() << '\n';

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_config_options) {

    // TODO: move duplicate lines from above test & here into a setup function

    std::string const runtime_config_path {k_mzn_runtime_config_DIR};
    std::string fname = runtime_config_path + "/config.json";

    bl.read_json(fname);

    EXPECT_NE(bl.show_options().find("edit"), std::string::npos);

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_invalid_file) {

    // this should be a throw here
    EXPECT_THROW(bl.read_json("~/invalid_file.json"), mzn::WarningException);

    EXPECT_TRUE(bl.get_json().empty());

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_display_network) {
    // TODO: move duplicate lines from above test & here into a setup function

    std::string const runtime_config_path {k_mzn_runtime_config_DIR};
    std::string fname = runtime_config_path + "/config.json";

    bl.read_json(fname);

    // shouldn't be a throw here!
    EXPECT_NO_THROW(bl.show_current());

    EXPECT_NE(bl.get_json(), nullptr);

}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_failed_display) {
    //TODO: lines are not all duplicated here

    std::string const runtime_config_path {k_mzn_runtime_config_DIR};
    std::string fname = runtime_config_path + "/example_config.json";
    // (example_config does not have a valid auth code)
    bl.read_json(fname);

    // We WANT the code to handle the JSON error gracefully and not quit
    EXPECT_NO_THROW(bl.show_current());

    // however, the invalid JSON file should be cleared out
    EXPECT_TRUE(bl.get_json().empty());
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureBuilderLoop, builder_loop_bad_config) {
    //TODO: still some duplicated lines

    EXPECT_EQ(bl.get_json(), nullptr);

    std::string const runtime_config_path {k_mzn_runtime_config_DIR};
    std::string fname = runtime_config_path + "/example_config.json";

    EXPECT_NO_THROW(bl.read_json(fname));

    EXPECT_FALSE(bl.get_json().empty());

    mzn::SeismicNetwork sn;

    EXPECT_THROW(bl.setup(sn), mzn::WarningException);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
;

