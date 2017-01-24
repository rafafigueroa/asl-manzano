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

        std::vector<std::string> args(argv, argv + argc);
        mzn::MceCli mce_cli;

        if (args.size() > 1) {
            // expects the file name
            std::cout << "arg" << args[1];
        } else {
            // interactive
            mce_cli.user_input_loop();
        }

    } catch(mzn::Exception & e) {

        std::cerr << "\nError, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;

    } catch(std::exception & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};
