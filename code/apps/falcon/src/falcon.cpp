#include "csv_file.h"
#include "falcon_data_packet.h"
#include "tcp_connection.h"
#include "udp_connection.h"

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {

    std::cout << "\n +++++ falcon: csv files to data transformer +++++\n";

    try {

        std::vector<std::string> args(argv, argv + argc);

        if (args.size() == 1) {
            std::cerr << "\nNo files to process\n";
            return EXIT_SUCCESS;
        }

        using M = std::vector<uint8_t>;
        std::vector<M> msgs;

        auto seq = 800;

        // ------------------------------------------------------------------ //
        for (int i = 1; i < args.size(); i++) {

            std::cout << std::endl
                << "\n--------------------------------------"
                << "----------------------------";

            std::cout << "\nProcessing: " << args[i];

            CsvFile csv_file(args[i]);
            auto const falcon_data_packets = csv_file.falcon_data_packets();

            for (auto const & fdp : falcon_data_packets) {
                std::cout << "\n" << fdp;
                auto const raw_input = fdp.to_raw_input_format(seq);
                msgs.push_back(raw_input);
            }
        }

        std::cout << std::endl << "\n";

        // Connect to the server
        // ------------------------------------------------------------------ //
        std::string const ip_remote = "136.177.121.21";
        auto constexpr port_remote = 7981; //0x2D1F 7981
        auto constexpr timeout_duration = std::chrono::seconds(5);

        auto constexpr port_host = 4998;

        mzn::TcpConnection cwb_connection(port_host);

        cwb_connection.setup_socket(timeout_duration,
                                    ip_remote,
                                    port_remote);

        // send_recv each one of them
        // ------------------------------------------------------------------ //
        for (auto const & msg : msgs) {

            M msg_recv(5000);

            cwb_connection.send_recv(msg, msg_recv);

            std::cout << std::endl
                << "\n**************************************"
                << "****************************";

            // print results
            std::cout << std::endl << "\nreceived (" << msg_recv.size()
                      << " bytes):\n";

            for (auto const & b : msg_recv) {
                std::cout << b;
            }

            std::cout << std::endl;
        }

    } catch (std::exception const & e) {

        std::cerr << "\nclosing program, error:";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
