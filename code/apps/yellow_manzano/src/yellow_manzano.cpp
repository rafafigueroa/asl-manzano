// Green Manzano
#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>


#include "mzn_except.h"
#include "mzn_cmake_config.h"
#include "yellow_manzano_cmake_config.h"
#include "e300_server.h"

int main(int argc, char **argv) {


    std::cout << "\n +++++++ MANZANO ++++++++ \n"
              << " +        yellow        + \n"
              << " +         "
              << k_yellow_manzano_VERSION_MAJOR << "."
              << k_yellow_manzano_VERSION_MINOR
              << "          + \n"
              << " ++++++++++++++++++++++++ ";

    try {

        mzn::E300Server e300_server;

        e300_server.run();

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


