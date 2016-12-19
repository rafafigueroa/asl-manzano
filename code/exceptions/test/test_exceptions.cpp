#include <iostream>
#include "mzn_except.h"
#include "gtest/gtest.h"


// -------------------------------------------------------------------------- //
class FixtureException : public ::testing::Test {

public:

    using Exception = mzn::Exception;
    using InfoException = mzn::InfoException ;
    using WarningException = mzn::WarningException ;
    using FatalException = mzn::FatalException ;

    FixtureException() {}
};

// -------------------------------------------------------------------------- //
TEST_F(FixtureException, info_exception) {

    auto e = mzn::InfoException("class", "func", "msg");

    try {
        throw e;
    } catch (mzn::InfoException & e) {
        // std::cerr << e.what();
    }

    EXPECT_THROW(throw e, mzn::InfoException);
    EXPECT_THROW(throw e, mzn::Exception);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureException, warning_exception) {

    auto e = mzn::WarningException("class", "func", "msg");

    try {
        throw e;
    } catch (mzn::WarningException & e) {
        // std::cerr << e.what();
    }

    EXPECT_THROW(throw e, mzn::WarningException);
    EXPECT_THROW(throw e, mzn::Exception);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureException, fatal_exception) {

    auto e = mzn::FatalException("class", "func", "msg");

    try {
        throw e;
    } catch (mzn::FatalException & e) {
        // std::cerr << e.what();
    }

    EXPECT_THROW(throw e, mzn::FatalException);
    EXPECT_THROW(throw e, mzn::Exception);
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


