// ** command line interface ** //
// Manzano Software //
#ifndef _MZN_MCE_CLI_H_
#define _MZN_MCE_CLI_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "mzn_except.h"
#include "system_calls.h"
#include "user_interpreter.h"
#include "instruction_interpreter.h"

namespace mzn {

class MceCli {

public:

    explicit
    MceCli() : ta_{} {

        try {

            config_home_path = get_runtime_config_path();

        } catch(mzn::FatalException & e) {

            std::cout << std::endl << "No runtime configuration files found";
            std::cout << std::endl << "Create empty config file? (y/n): ";

            std::string response;
            std::getline(std::cin, response);

            if (response == "y") create_empty_config_file(); else throw e;
        }

    };

    std::string config_home_path;

    ~MceCli() = default;

    //! mce cli starts here
    void user_input_loop();

    void create_empty_config_file();

    void save_to_config_file(SeismicNetwork const & sn) const;

    void add_to_config(SeismicNetwork & sn,
                       std::string const & user_input,
                       TargetAddress const & ta) const;

private:

    TargetAddress ta_;

};

} // end namespace
#endif
