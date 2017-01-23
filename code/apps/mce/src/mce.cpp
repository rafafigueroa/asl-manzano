#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>


#include "mzn_except.h"
#include "mzn_cmake_config.h"
#include "mce_cmake_config.h"

#include "mce_cli.h"

int main(int argc, char **argv) {


    std::cout << "\n ++++++++ MANZANO +++++++++ \n"
              <<   " +  configuration editor  + \n"
              << " +          "
              << k_mce_VERSION_MAJOR << "."
              << k_mce_VERSION_MINOR
              << "           + \n"
              << " ++++++++++++++++++++++++++ ";

    try {

        try {

            auto const rcp = mzn::get_runtime_config_path();

        } catch(mzn::FatalException & e) {

            std::cout << std::endl << "No runtime configuration files found";
            std::cout << e.what();
            std::cout << std::endl << "Create empty config file? (y/n): ";

            std::string response;
            std::cin >> response;

            if (response == "y") std::cout << "\ncreating empty config file";


            return EXIT_SUCCESS;
        }

        std::vector<std::string> args(argv, argv + argc);
        mzn::MceCli mce_cli;

        if (args.size() > 1) {
            // expects the file name
            std::cout << "arg" << args[1];
        } else {
            // interactive
            mce_cli.user_input_loop();
        }

    } catch(mzn::FatalException & e) {

        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;

    } catch(std::exception & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};
