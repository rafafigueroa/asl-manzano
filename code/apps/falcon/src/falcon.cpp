#include "csv_file.h"
#include "falcon_data_packet.h"

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {

    std::cout << "\n +++++ falcon: csv files to data transformer +++++\n";

    try {

        std::vector<std::string> args(argv, argv + argc);

        if (args.size() == 1) {
            std::cerr << "\nNo files to process\n";
            return EXIT_SUCCESS;
        }

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
                auto raw_input = fdp.to_raw_input_format();
            }
        }

        std::cout << "\n\n";

    } catch (std::exception const & e) {

        std::cerr << "\nclosing program, error:";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
