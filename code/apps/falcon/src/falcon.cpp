#include "csv_file.h"
#include "falcon_data_packet.h"
#include "tcp_connection.h"

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

        // Connect to server and send the information
        std::string const ip_remote = "136.177.121.21";
        auto constexpr port_remote = 7891;
        auto constexpr timeout_duration = std::chrono::seconds(2);

        auto constexpr port_host = 4991;
        mzn::TcpConnection cwb_connection(port_host );
        cwb_connection.setup_socket(timeout_duration,
                                    ip_remote,
                                    port_remote);


        for (auto const & msg : msgs) {

            M msg_recv(5000);

            cwb_connection.send_recv(msg, msg_recv);

            std::cout << std::endl
                << "\n**************************************"
                << "****************************";

            // print results
            std::cout << std::endl << "\nreceived:\n";

            for (auto const & b : msg_recv) {
                std::cout << b;
            }

            std::cout << std::endl;
        }

        /* FIRST time, received this:

            49 57 58 48 57 58 51 49 32 50 48 49 55 47 48 49 47 48 52 32 69 100
            103 101 77 111 109 58 32 112 114 111 99 101 115 115 32 102 105 108
            101 61 69 68 71 69 77 79 77 47 101 100 103 101 109 111 109 95 50 35
            49 46 115 101 116 117 112 32 35 84 104 114 101 97 100 115 61 49 52
            32 35 116 104 114 61 50 56 57 10 68 77 67 76 111 97 100 58 103 111 118

            //ascii with new lines added:

            19:09:31 2017/01/04
            EdgeMom: processfile= EDGEMOM/edgemom_2 #1.setup
                                                    #Threads=14
                                                    #thr=289
            DMCLoad:gov
        */

    } catch (std::exception const & e) {

        std::cerr << "\nclosing program, error:";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
