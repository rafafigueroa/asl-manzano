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
#include "json_sn.h"

namespace mzn {

class MceCli {

public:

    explicit
    MceCli() : ta_{} {

        try {

            config_home_path = Utility::get_runtime_config_path();

        } catch(mzn::FatalException & e) {

            std::cout << std::endl << "No runtime configuration files found";
            auto const confirm = Utility::ask_yes("Create empty config file");
            if (confirm) create_empty_config_file(); else throw e;
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
                       TargetAddress & ta) const;

    void remove_from_config(SeismicNetwork & sn, TargetAddress & ta) const;
    void change_config(SeismicNetwork & sn, TargetAddress const & ta) const;

private:

    TargetAddress ta_;

};

} // end namespace
#endif
