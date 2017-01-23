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

namespace mzn {

class MceCli {

public:

    explicit
    MceCli() : sn{}, stream_output(sn) {};

    SeismicNetwork sn;
    StreamOutput stream_output;

    ~MceCli() = default;

    //! mce cli starts here
    void user_input_loop();

    void show_prompt();
};

} // end namespace
#endif
