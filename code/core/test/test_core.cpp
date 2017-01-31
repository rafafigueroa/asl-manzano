#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <limits>
#include <cmath>
#include <future>
#include <atomic>
#include <mutex>

#include "gtest/gtest.h"
#include "commands.h"
#include "comm.h"
#include "mzn_except.h"
#include "dummy_q_port.h"

#include "mzn_time.h"
#include "system_calls.h"
// -------------------------------------------------------------------------- //

class FixtureCore : public ::testing::Test {
public:

    mzn::Comm cm;
    virtual void SetUp() {}
};

// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, spcl) {

    using namespace mzn;

    std::cout << std::endl << "testing terminal" << std::endl;

    std::string input;

    for (int i = 0; i < 10; i++) {
        std::cout << "\ncols " << get_terminal_cols() << std::endl;
        std::getline(std::cin, input);
    }
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, msg_task) {

    using namespace mzn;

    SeismicNetwork sn{};
    CmdFileReader cmd_file_reader_(sn);

    auto const ui = UserInstruction(Action::plan, Kind::cal);

    auto const ta = TargetAddress( Target(Scope::station),
                                   Target(Scope::digitizer),
                                   Target(Scope::sensor) );

    auto msg_tasks =
        cmd_file_reader_.construct_msg_tasks<Action::start, Kind::cal>(ui, ta);

    for (auto const & msg_task : msg_tasks) {

        std::cout << std::endl << msg_task;
        std::cout << std::endl << "delay: " << msg_task.delay;
     }

    msg_tasks[0].stream<C1Qcal>(std::cout);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, msg_task_manager) {

    using namespace mzn;

    /*
    mzn::DummyQPort dummy_q_port( cm.sn.st[0].q[0].port_config.port_remote,
                                  std::chrono::seconds(100) );

    auto const ui = UserInstruction(Action::plan, Kind::cal);

    auto const ta = TargetAddress( Target(Scope::station),
                                   Target(Scope::digitizer),
                                   Target(Scope::sensor) );

    try {

        // TODO: run dummy automatically
//        cm.run<Action::plan, Kind::cal>(ui, ta);

    } catch(std::exception const & e) {
        std::cerr << std::endl << "!!!!!!!!!! error\n" << e.what();
        throw e;
    }
    */
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*

auto f_sleep_id = []{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::endl << " " << std::this_thread::get_id() << " ";
    std::string s{"async task"};
    return s;
};
auto f_get_fut = [](std::future<std::string> & fut) {auto s = fut.get();};

// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, basic_tasks) {

    std::vector< std::future<std::string> > futures;

    for (int i = 0; i < 10; ++i) {
        auto fut = std::async(std::launch::async, f_sleep_id);
        futures.push_back(std::move(fut));
    }

    std::for_each(futures.begin(), futures.end(), f_get_fut);
}

// -------------------------------------------------------------------------- //
std::string task() {
    std::string s{"task"};
    return s;
}
// -------------------------------------------------------------------------- //
std::string task_throws() {
    std::string s{"task that throws"};
    throw std::runtime_error("exception from task");
    return s;
}

//! not really a unit test, just testing some concepts for better understanding
// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, basic_task) {

    std::cout << std::endl << "from test" << std::endl;
    auto ftr = std::async(std::launch::async, &task);

    auto s = ftr.get();
    std::cout << s << std::endl;
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCore, basic_task_throws) {

    std::cout << std::endl << "from test that throws" << std::endl;
    auto ftr = std::async(std::launch::async, &task_throws);

    try {
        auto s = ftr.get();
        std::cout << s << std::endl;
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
}

*/
