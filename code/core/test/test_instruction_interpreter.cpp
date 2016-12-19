// Manzano
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <thread>

#include "gtest/gtest.h"
#include "comm.h"
#include "seismic_network.h"
#include "instruction_interpreter.h"

// -------------------------------------------------------------------------- //
void dummy_q() {

    using Message = std::string;
    Message msg_send;
    auto constexpr ret_size = 30;
    // SERVER (mxe)
    auto constexpr host_port = 4500;

    // wait forever when receiving, no timeout
    mzn::UdpConnection uc(host_port);

    try {
        uc.setup_socket();

        // ----------------------------------------------------------------- //
        while (true) {

            Message msg_recv;
            msg_recv.resize(ret_size);

            std::cout << std::endl << "**** server listening ****" << std::endl;
            uc.recv(msg_recv);

            std::cout << std::endl << '@' << msg_recv  << '@' << std::endl;

            if (msg_recv.find("quit") != std::string::npos) {
                std::cout << std::endl << "**** udpServerBye!" << std::endl;
                break;
            }

            msg_send = "\nhello from SERVER\n";
            std::cout << std::endl << "**** server sending: "
                      << msg_send << std::endl;

            uc.send(msg_send);
        }

    } catch(std::exception & e) {
        std::cerr << e.what();
    }

    uc.close_socket();

}
// run_instruction runs it to the end, dont include commands like get ping
// or it will try to run it on the actual digitizer during a test
// this will be done once a dummy digitizer/hardware are provided
// -------------------------------------------------------------------------- //
class FixtureInstructionInterpreter : public ::testing::Test {

public:

    using UserInstruction = mzn::UserInstruction;
    using Action = mzn::Action;
    using Kind   = mzn::Kind;
    using Target = mzn::Target;
    using Scope  = mzn::Scope;
    using TargetAddress  = mzn::TargetAddress;

    mzn::InstructionInterpreter instruction_interpreter;

    TargetAddress ta;

    FixtureInstructionInterpreter() : instruction_interpreter() {}
};


// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, instruction_interpreter_setup) {}

// These tests assume the test stations during development
// They will eventually call the dummy dataset and dummy q330
// TODO: with the dummy, we need to make sure that the tests run exactly
// the same every time.
// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, run_instruction_show_default_target) {

    // Should be able to run without problem. Exceptions will trigger test failure.
    auto ui = UserInstruction(Action::show, Kind::target);
    instruction_interpreter.run_instruction(ui);
    std::cout << std::endl << std::endl; // Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, run_get_ping) {


    // this is a very simple test that sends a couple of back and forth
    // messages. The server side is running on its own thread.
    // use the separate client / server programs in the test directory
    // to test it on different computer/ environments
//    std::thread thread_server(dummy_q);

    // give some time for the server to setup
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );


    // Should be able to run without problem. Exceptions will trigger test failure.
    auto ui = UserInstruction(Action::get, Kind::ping);
    std::cout << std::endl << "running " << ui << "\n";
    instruction_interpreter.run_instruction(ui);
    std::cout << std::endl << std::endl; // Cleaner cout

 //   thread_server.join();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, run_get_poll) {

    // Should be able to run without problem. Exceptions will trigger test failure.
    auto ui = UserInstruction(Action::get, Kind::poll);
    std::cout << std::endl << "running " << ui << "\n";
    instruction_interpreter.run_instruction(ui);
    std::cout << std::endl << std::endl; // Cleaner cout
}
    /*
// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, dummy_digitizer) {

    // this is a very simple test that sends a couple of back and forth
    // messages. The server side is running on its own thread.
    // use the separate client / server programs in the test directory
    // to test it on different computer/ environments
    std::thread thread_dummy_q(dummy_q);

    // give some time for the server to setup
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );

    using Message = std::string;
    Message msg_send;
    auto constexpr ret_size = 30;

    // CLIENT (laptop)
    std::string const ip_remote = "";
    auto constexpr port_remote = 4500;
    auto constexpr port_host = 4400;

    mzn::UdpConnection uc(port_host);

    try {
        mzn::TimeoutDuration timeout;
        timeout.tv_usec = 100'000;
        uc.setup_socket(timeout, ip_remote, port_remote);

        for (int msg_count = 0; msg_count < 10; msg_count++) {

            msg_send = std::string("\nhello from CLIENT:")
                       + std::to_string(msg_count);

            Message msg_recv;
            msg_recv.resize(ret_size);

            std::cout << std::endl << "---- client send_recv ----" << std::endl;
            uc.send_recv(msg_send, msg_recv);
            std::cout << std::endl << "---- received: " << msg_recv << std::endl;
        }

        std::cout << std::endl << "---- client send quit ----" << std::endl;
        uc.send(std::string("quit"));

    } catch (std::exception & e) {
        std::cerr << std::endl << e.what();
        try {
            std::cout << std::endl << "---- client send quit ----" << std::endl;
            uc.send(std::string("quit"));
        } catch(...) {}
    }

    uc.close_socket();

    thread_dummy_q.join();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, run_instruction_show_full_target) {

    // Should be able to run without problem. Exceptions will trigger test failure.
    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target(Scope::station,   0),
                        Target(Scope::digitizer, 0),
                        Target(Scope::sensor,    0) );

    instruction_interpreter.run_instruction(ui, ta);
    std::cout << std::endl << std::endl; // Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, merge_target) {

    // merge with default ta
    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target{},
                        Target{},
                        Target(Scope::sensor,    0) );

    instruction_interpreter.run_instruction(ui, ta);
    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, run_edit_target) {

    //This should change the default to st1q1 without error
    auto ui = UserInstruction(Action::edit, Kind::target);

    ta = TargetAddress( Target(Scope::station,   0),
                        Target(Scope::digitizer, 0),
                        Target{} );

    instruction_interpreter.run_instruction(ui, ta);
    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, ta_fail_no_merge) {

    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target(Scope::station,   0),
                        Target{},
                        Target(Scope::sensor,    0) );

    EXPECT_THROW(instruction_interpreter.run_instruction(ui, ta),
                 mzn::WarningException);

    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, ta_fail_station_overflow) {

    auto ui = UserInstruction(Action::edit, Kind::target);

    ta = TargetAddress( Target(Scope::station,   3),
                        Target(Scope::digitizer, 1),
                        Target{} );

    EXPECT_THROW(instruction_interpreter.run_instruction(ui, ta),
                 mzn::WarningException);

    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, ta_fail_digitizer_overflow) {

    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target(Scope::station,   1),
                        Target(Scope::digitizer, 4),
                        Target{} );

    EXPECT_THROW(instruction_interpreter.run_instruction(ui, ta),
                 mzn::WarningException);

    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, ta_fail_sensor_overflow) {

    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target(Scope::station,   0),
                        Target(Scope::digitizer, 0),
                        Target(Scope::sensor, 5) );

    EXPECT_THROW(instruction_interpreter.run_instruction(ui, ta),
                 mzn::WarningException);

    std::cout << std::endl << std::endl; //Cleaner cout
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureInstructionInterpreter, ta_fail_incomplete_address) {

    auto ui = UserInstruction(Action::show, Kind::target);

    ta = TargetAddress( Target{},
                        Target{},
                        Target(Scope::sensor, 0) );

    EXPECT_THROW(instruction_interpreter.run_instruction(ui, ta),
                 mzn::WarningException);

    std::cout << std::endl << std::endl; //Cleaner cout
}

    */
// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};


