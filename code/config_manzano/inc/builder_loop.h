/*
 * builder_loop.h
 *
 *  Created on: Nov 17, 2016
 *      Author: akearns
 */

#ifndef CODE_SNEDITOR_SRC_HELLOWORLD_H_
#define CODE_SNEDITOR_SRC_HELLOWORLD_H_

#include <iostream>
#include <string>
#include <sstream>
#include "stream_output.h" // not exactly a lightweight import
#include "json.hpp"

#include "network_loop.h"

namespace mzn {

class BuilderLoop {

public:

    explicit
    BuilderLoop() : json_sn_{BasicJson::object()} {}
    ~BuilderLoop() = default;

    void main_options();
    void main_options(std::string const & filename);

    BasicJson read_json(std::string const & filename) const;
    void write_json(std::string const & filename) const;

    std::string show_options() const;
    void show_current();

    BasicJson json_sn() const;
    void set_json_sn(BasicJson const & json_sn);

    // try to build a seismic network from our JSON;
    void setup(SeismicNetwork & sn) const;

    static const int MAX_SENSORS = 2;

private:

    BasicJson json_sn_;

    // used to collect user input and call interface functions
    char dispatch_input();

    // command-line interface functions (entry points for user edits)
    void get_filename();
    void edit();
    void build_from_scratch();
    void validate() const;
    void save() const;

};

} /* namespace */
#endif /* CODE_SEISMIC_NETWORK_SRC_HELLOWORLD_H_ */
