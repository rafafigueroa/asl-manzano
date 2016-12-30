#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include "gtest/gtest.h"
#include "udp_connection.h"
#include "serial_connection.h"
#include "dummy_q_port.h"
#include "mzn_except.h"

// -------------------------------------------------------------------------- //
class FixtureConnection : public ::testing::Test {
public:
    virtual void SetUp() {}
    //virtual void TearDown() {};
};

// -------------------------------------------------------------------------- //
TEST_F(FixtureConnection, connection_setup) {
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureConnection, serial_connection) {

    std::string const serial_port = "/dev/ttyS1";
    auto constexpr baud_rate = 9600;
    auto constexpr timeout = std::chrono::milliseconds(500);

    mzn::SerialConnection sc(serial_port, baud_rate, timeout);

    sc.enumerate_ports();

    try {
        sc.setup_connection();
    } catch(std::exception & e) {
        std::cerr << "\n--------------------------------------------\n";
        std::cerr <<  e.what();
        std::cerr << "\n--------------------------------------------\n";
        std::cerr << "\n - will not work in computers without serial port";
        std::cerr << "\n - also, needs admin privileges on computures";
        std::cerr << " with serial port";
        std::cerr << "\n   -> ignore this error on those cases";
        std::cerr << "\n--------------------------------------------\n";
    }

    //Message msg {123, 11, 143, 1};
    std::stringstream ss;
    // ss << msg;
    std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
