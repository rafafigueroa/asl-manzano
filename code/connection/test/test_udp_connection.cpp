#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include "gtest/gtest.h"
#include "udp_connection.h"
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
void udp_server() {

    using Message = std::string;
    Message msg_send;
    auto constexpr ret_size = 30;
    // SERVER (mxe)
    auto constexpr host_port = 4500;

    // wait forever when receiving, no timeout
    mzn::UdpConnection uc(host_port);

    std::stringstream ss;
    try {
        uc.setup_socket();

        // ----------------------------------------------------------------- //
        while (true) {

            Message msg_recv;
            msg_recv.resize(ret_size);

            ss << std::endl << "**** server listening ****" << std::endl;
            uc.recv(msg_recv);

            ss << std::endl << '@' << msg_recv  << '@' << std::endl;

            if (msg_recv.find("quit") != std::string::npos) {
                ss << std::endl << "**** udpServerBye!" << std::endl;
                break;
            }

            msg_send = "\nhello from SERVER\n";
            ss << std::endl << "**** server sending: "
                      << msg_send << std::endl;

            uc.send(msg_send);
        }

    } catch(std::exception & e) {
        std::cerr << e.what();
    }

    // std::cout << ss.str();
    uc.close_socket();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureConnection, udp_server_client) {

    // this is a very simple test that sends a couple of back and forth
    // messages. The server side is running on its own thread.
    // use the separate client / server programs in the test directory
    // to test it on different computer/ environments
    std::thread thread_server(udp_server);

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

    std::stringstream ss;
    try {
        auto constexpr timeout = std::chrono::milliseconds(100);
        uc.setup_socket(timeout, ip_remote, port_remote);

        for (int msg_count = 0; msg_count < 1000; msg_count++) {

            msg_send = std::string("\nhello from CLIENT:")
                       + std::to_string(msg_count);

            Message msg_recv;
            msg_recv.resize(ret_size);

            ss << std::endl << "---- client send_recv ----" << std::endl;
            uc.send_recv(msg_send, msg_recv);
            ss << std::endl << "---- received: " << msg_recv << std::endl;
        }

        ss << std::endl << "---- client send quit ----" << std::endl;
        uc.send(std::string("quit"));

    } catch (std::exception & e) {
        std::cerr << std::endl << e.what();
        try {
            ss << std::endl << "---- client send quit ----" << std::endl;
            uc.send(std::string("quit"));
        } catch(...) {}
    }

    thread_server.join();
    ss << std::endl << "------------------ udpClientGoodbye\n" << std::endl;

    // std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureConnection, qdp_stream) {

    //Message msg {123, 11, 143, 1};
    std::stringstream ss;
    // ss << msg;
    std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureConnection, dummy_q_port) {
    // starts a server on another thread, representing the remote digitizer
    auto constexpr port_remote = 4100;
    mzn::DummyQPort dummy_q_port( port_remote, std::chrono::seconds(3) );

    // give some time for the server to setup
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );

    using Message = std::string;
    Message msg_send;
    auto constexpr ret_size = 548;

    // CLIENT (this thread)
    auto constexpr port_host = 4111;
    mzn::UdpConnection uc(port_host);

    std::vector<uint8_t> const cack_msg{0XEB, 0X8A, 0X1A, 0X80,
                                        0XA0, 0X02, 0X00, 0X00,
                                        0X00, 0X00, 0X00, 0X03, };

    std::stringstream ss;

    try {
        auto constexpr timeout = std::chrono::milliseconds(100);
        std::string const ip_remote = "";
        uc.setup_socket(timeout, ip_remote, port_remote);

        for (int msg_count = 0; msg_count < 3; msg_count++) {

            msg_send = std::string("\nhello from CLIENT:")
                       + std::to_string(msg_count);

            std::vector<uint8_t> msg_recv;
            msg_recv.resize(ret_size);

            ss << std::endl << "---- client send_recv ----" << std::endl;
            uc.send_recv(msg_send, msg_recv);
            ss << std::endl << "---- received: ";
            mzn::UdpConnection::print_msg(msg_recv);

            EXPECT_EQ( msg_recv.size(), cack_msg.size() );

            for (int i = 0; i <  msg_recv.size(); i++) {
                EXPECT_EQ( static_cast<int>(msg_recv[i]),
                           static_cast<int>(cack_msg[i]) );
            }
        }

        ss << std::endl << "---- client send quit ----" << std::endl;
        uc.send(std::string("quit"));

    } catch (std::exception & e) {
        std::cerr << std::endl << e.what();
        try {
            ss << std::endl << "---- client send quit ----" << std::endl;
            uc.send(std::string("quit"));
        } catch(...) {}
    }

    // std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
