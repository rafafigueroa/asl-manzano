// Green Manzano
#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>


#include "mzn_except.h"
#include "mzn_cmake_config.h"
#include "green_manzano_cmake_config.h"

#include "user_interpreter.h"

int main(int argc, char **argv) {

    std::cout << "\n +++++++ MANZANO ++++++++ \n"
              << " +        green         + \n"
              << " +         "
              << k_green_manzano_VERSION_MAJOR << "."
              << k_green_manzano_VERSION_MINOR
              << "          + \n"
              << " ++++++++++++++++++++++++ ";

    try {

        mzn::UserInterpreter user_interpreter;

        std::vector<std::string> args(argv, argv + argc);

        if (args.size() > 1) {
            // expects the file name
            user_interpreter.user_input_loop(args[1]);
        } else {
            // interactive
            user_interpreter.user_input_loop();
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


