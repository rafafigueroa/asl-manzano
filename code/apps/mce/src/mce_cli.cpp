// ** command line interface ** //
// Manzano Software //
#include "mce_cli.h"

namespace mzn {

// -------------------------------------------------------------------------- //
void MceCli::user_input_loop() {
    // prompts the user for their command
    // interactive

    std::string user_input;

    while (true) {

        show_prompt();

        getline(std::cin, user_input);

        try {

            if (user_input == "") {
                continue;
            }

            //! quit program normally
            if (user_input == "quit") {

                std::cout << std::endl << "\nbye" << std::endl;
                break;
            }

            if (user_input == "help") {
                stream_output.show_help();
                continue;
            }

            std::cout << "\n" << user_input;

        } catch (FatalException const & e) {

            std::cerr << std::endl
                      << "caught fatal error @Mce::user_input_loop()"
                      << std::endl << e.what();
            break;

        } catch (Exception const & e) {

            std::cerr << std::endl
                      << "caught error @Mce::user_input_loop()"
                      << std::endl << e.what();
        }
    }
}

// -------------------------------------------------------------------------- //
void MceCli::show_prompt() {

    auto const ta = TargetAddress{};

    std::cout << std::endl << "\n---------------------------------------------";
    stream_output.show<Kind::target>(ta);
    std::cout << std::endl << "---------------------------------------------\n";
    std::cout << "\n » ";
}

} // end namespace
