// Config Manzano
#include <iostream>
#include <vector>
#include <exception>
#include <cstdlib>

#include "mzn_except.h"
#include "mzn_cmake_config.h"
#include "builder_loop.h"



int main(int argc, char **argv) {

    std::cout << std::endl;
	std::cout << "┌───────────────────────────────────────────────┐\n";
	std::cout << "│ Welcome to the Manzano configuration utility! │\n";
	std::cout << "└───────────────────────────────────────────────┘\n\n";

    try {

        mzn::BuilderLoop builder_loop;

        std::vector<std::string> args(argv, argv + argc);

        if (args.size() > 1) {
            // begin with reading a file
            builder_loop.main_options(args[1]);
        } else {
            // begin with user input prompts
            builder_loop.main_options();
        }

    } catch(mzn::FatalException & e) {

        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;

    } catch(std::exception & e) {

        std::cerr << '\n' << "Unexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }


	return EXIT_SUCCESS;

};


