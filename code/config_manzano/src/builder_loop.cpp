/*
 * builder_loop.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: akearns
 */

#include "builder_loop.h"

//using namespace mzn;

namespace mzn {

// Note: may want to eventually refactor to make this resemble component_loop
// more closely in how input is taken and gathered

char BuilderLoop::dispatch_input() {
    // returns 'x' if exit was chosen
    // returns 'a' if any valid option besides exit was chosen
    // returns 'n' if an invalid choice was chosen
    char input;
    std::cin >> input;
    // flush the buffer to prevent invalid character selections
    std::cin.clear(); std::cin.ignore(INT_MAX,'\n');

    std::cout << "Selected: " << input << std::endl;

    switch(input) {
    case 'L':
    case 'l':
        get_filename();
        return 'a';
    case 'N':
    case 'n':
        build_from_scratch();
        return 'a';
    case 'X':
    case 'x':
        return 'x';
    }

    if(json_sn_.empty()){
        // for checking cases where
        switch(input){
        case 'D':
        case 'd':
            show_current();
            return 'a';
        case 'E':
        case 'e':
            edit();
            return 'a';
        case 'V':
        case 'v':
            validate();
            return 'a';
        case 'S':
        case 's':
            save();
            return 'a';
        }
    }

    return 'n';
}

void BuilderLoop::get_filename() {

    //prompt user for a filename, call read_json on it
    std::string filename;
    std::cout << "Please enter the name of a file to read from."
              << std::endl;
    std::cin.clear();
    std::cin >> filename;
    std::cout << std::endl;


    try {
        std::cout << "Loading in file " << filename << "..." << std::endl;
        json_sn_ = read_json(filename);
        std::cout << "The active configuration has been overwritten.";
        std::cout << std::endl;
        show_current();
    } catch (WarningException &e) {
        std::cerr << std::endl << e.what();
        std::cout << "File " << filename << " could not be read." << std::endl;
        std::cout << "(Check that the specified file exists.)" << std::endl;
        std::cout << "The active configuration has not changed.";
    }

}

void BuilderLoop::build_from_scratch() {

    // create a new empty JSON and call the component loop to populate it
    std::cout << "Clearing out the current configuration." << std::endl;
    json_sn_.clear();
    edit();
}

void BuilderLoop::edit() {
    // call the component loop for the
    std::cout << "Editing the JSON file:" << std::endl;

    NetworkLoop nl(json_sn_);
    nl.initializer_prompt(json_sn_);
}

void BuilderLoop::save() const {
    // TODO: build it
    std::cout << "will prompt a user for a filename and then write to it";
}

void BuilderLoop::write_json(std::string const & filename) const {
    std::cout << "writes to the specified file";
}

void BuilderLoop::validate() const {
    if(json_sn_.empty()){
        std::stringstream ss;
        ss << "Invariant of valid JSON violated. This should never happen.";
        throw FatalException( "BuilderLoop", "validate", ss.str() );
    }
    // TODO: implement!
    // check for pathological-but-valid configurations
    std::cout << "stub to check valid but bad configurations" << std::endl;
    std::cout << "such as stations without digitizers or sensors"
              << std::endl;
    std::cout << "(a valid configuration must be loaded to even get this far)";
    // call a helper method that returns a bool if the config is valid
}

void BuilderLoop::show_current() {

    try {
        std::stringstream ss;

        // construct a seismic network
        // and then display it
        std::cout << "Here is the current configuration:" << std::endl;
        std::cout << std::endl;

        SeismicNetwork sn;
        BuilderLoop::setup(sn); // constructs sn from our given json file
        StreamOutput stream_output(sn);

        stream_output.show<Kind::target>(TargetAddress{});

        std::cout << std::endl; // stream_output doesn't end in linebreak

    } catch (mzn::WarningException & e){

        json_sn_.clear();

        std::cerr << std::endl << e.what() << std::endl;

        std::stringstream ss;
        ss << "The active JSON configuration is invalid." << std::endl;
        ss << "The current configuration has been reset." << std::endl;
        ss << "Either load a different configuration, or "
                << "create a new configuration." << std::endl;

        std::cout << ss.str();

    }

}

std::string BuilderLoop::show_options() const {

    // used to populate the terminal window with available options
    // should be called any time a task has been finished
    std::stringstream ss;

    ss << std::endl << std:: endl;

    ss << "This is the main menu.\n";
    ss << "Would you like to...\n";

    ss << "\t...load in a configuration file? (L,l)\n";
    ss << "\t...create a new configuration from scratch? (N,n)\n";
    if (not json_sn_.empty()) {
        ss << "\t...display the current configuration? (D,d)\n";
        ss << "\t...edit the current configuration? (E,e)\n";
        ss << "\t...validate the current configuration? (V,v)\n";
        ss << "\t...save the current configuration? (S,s)\n";
    }
    ss << "\t...exit the program? (X,x)\n";

    ss << "\nPlease input the letter that matches your choice.\n";

    return ss.str();
}

BuilderLoop::BasicJson BuilderLoop::json_sn() const {

    return json_sn_;

}

void BuilderLoop::set_json_sn(BasicJson const & json_sn) {

    json_sn_ = BasicJson(json_sn);
}

BuilderLoop::BasicJson
BuilderLoop::read_json(std::string const & filename) const {

    std::ifstream config_fl;
    config_fl.open(filename);

    if (not config_fl) {
        throw WarningException("BuilderLoop", "config",
                "ERROR: can't open user-specified file");
    }

    // read in the contents of the entire file
    std::stringstream buffer;
    buffer << config_fl.rdbuf();

    // parses the read-in file as a json object
    return BasicJson::parse(buffer.str());

}

void BuilderLoop::main_options() {

    while (true) {
        std::cout << show_options();
        char ex = dispatch_input();
        if(ex == 'x'){
            std::cout << "Exiting!";
            return;
        }
        if(ex == 'n'){
            std::cout << "That choice was not valid.";
            std::cin.clear();
        }
    }

}

void BuilderLoop::setup(SeismicNetwork & sn) const {

    // reset the auto-loaded seismic network
    sn.st.clear();

    // if our json file is formatted correctly then each key is a station name
    // and under that are an IP and a series of digitizers and data processors

    std::stringstream ss;

    for (auto it = json_sn_.begin();
                             it != json_sn_.end(); ++it) {

        std::string st_name = it.key(); // do we need to cast?

        BasicJson j_st = it.value(); // station data is nested json


        // TODO: replace parameter literals with static consts?
        std::string ip;
        if (j_st.find("ip") != j_st.end()) {
            ip = j_st.at("ip");
        } else {
            ss << "ERROR: station " << st_name << " has no ip set";
            throw WarningException("BuilderLoop", "display", ss.str());
        }

        Station st_(st_name);

        if (j_st.find("digitizer") != j_st.end()) {

            // digitizer module is a list of digitizer objects
            auto j_qlist = j_st.at("digitizer");

            for(auto j_q : j_qlist) {

                int base_port;
                int recv_port;
                unsigned long serial_number;
                unsigned long auth_code_config;
                unsigned long auth_code_data;
                int firmware_version;

                if ( j_q.find("base_port") != j_q.end() ) {
                    base_port = j_q.at("base_port");
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no base_port";
                    throw WarningException("BuilderLoop", "display", ss.str());
                }
                if ( j_q.find("recv_port") != j_q.end() ) {
                    recv_port = j_q.at("recv_port");
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no recv_port";
                    throw WarningException("BuilderLoop", "display", ss.str());
                }
                if ( j_q.find("serial_number") != j_q.end() ) {
                    std::string s_n_ = j_q.at("serial_number");
                    std::stringstream(s_n_) >> serial_number;
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no serial number";
                   throw WarningException("BuilderLoop", "display", ss.str());
                }
                if ( j_q.find("auth_code_config") != j_q.end() ){
                    std::string a_c_c_ = j_q.at("auth_code_config");
                    std::stringstream(a_c_c_) >> auth_code_config;
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no auth_code_config";
                    throw WarningException("BuilderLoop", "display", ss.str());
                }
                if ( j_q.find("auth_code_data") != j_q.end() ){
                    std::string a_c_d_ = j_q.at("auth_code_data");
                    std::stringstream(a_c_d_) >> auth_code_data;
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no auth_code_data";
                    throw WarningException("BuilderLoop", "display", ss.str());
                }
                if ( j_q.find("firmware_version") != j_q.end() ){
                    firmware_version = j_q.at("firmware_version");
                } else {
                    ss << "ERROR: a digitizer at "
                       << st_name << " has no firmware version";
                    throw WarningException("BuilderLoop", "display", ss.str());
                }

                Digitizer q_(ip, firmware_version, serial_number,
                                  auth_code_config, auth_code_data,
                                  base_port, recv_port);

                // locate any sensor objects
                if ( j_q.find("sensor") != j_q.end() ){
                    // sensors are also a list of JSON objects
                    if ( j_q.at("sensor").size() > MAX_SENSORS ){
                        ss << "ERROR: a digitizer at "
                           << st_name << " has too many sensors";
                        throw WarningException("BuilderLoop", "display",
                                ss.str());
                    }
                    for (auto j_sns : j_q.at("sensor")){
                        std::string port;
                        std::string model;

                        if ( j_sns.find("port") != j_sns.end() ) {
                            port = j_sns.at("port");
                        } else {
                            ss << "ERROR: a sensor at " << st_name
                               << " lacks a port name";
                            throw WarningException("BuilderLoop", "display",
                                    ss.str());
                        }

                        if( j_sns.find("model")  != j_sns.end() ) {
                            model = j_sns.at("model");
                        } else {
                            ss << "ERROR: a sensor at " << st_name
                               << " lacks a model name";
                            throw WarningException("BuilderLoop", "display",
                                    ss.str());
                        }

                        q_.s.push_back(Sensor(port, model));
                    }
                } else {
                    std::cout << "Note: a digitizer at " << st_name
                              << " has no sensors specified." << std::endl;
                    std::cout << "This configuration is technically valid, "
                              << "but very likely an error.";
                }

                st_.q.push_back(q_);

            } // loop over list of digitizers

        } else {
            std::cout << "NOTE: " << st_name << " has no digitizers specified."
                      << std::endl << "This configuration is technically "
                      << "valid, but very likely an error." << std::endl;
        }

        if ( j_st.find("data_processor") != j_st.end() ) {
            BasicJson j_dp = j_st.at("data_processor");

            if( j_dp.find("user") != j_dp.end()
                    and j_dp.find("pw") != j_dp.end()){

                std::string u = j_dp.at("user");
                std::string pw = j_dp.at("pw");

                st_.dp.push_back( DataProcessor(u, pw, ip) );
            } else{
                    ss << "ERROR: data processor at " << st_name
                       << " lacks username or password";
                    throw WarningException("BuilderLoop", "display", ss.str() );
            }
        } else {
            std::cout << "NOTE: " << st_name << " lacks a data processor.";
        }

        sn.st.push_back(st_);

    }

}

void BuilderLoop::main_options(std::string const & filename) {
    try {
        std::cout << "Loading in file from user-specified parameter...";
        std::cout << std::endl;
        json_sn_ = read_json(filename);
        show_current();
    } catch (mzn::WarningException &e) {
        std::cerr << std::endl << e.what();
        std::cout << "The program can continue, but no configuration "
                  << "has been loaded." << std::endl;
    }
    main_options();

}

} // <- mzn

