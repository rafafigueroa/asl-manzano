/*
 * network_loop.cpp
 *
 *  Created on: Nov 23, 2016
 *      Author: akearns
 */

#include "network_loop.h"

namespace mzn {

void NetworkLoop::dispatch(BasicJson const & json_sn, char const c) {

    switch (c) {
    case 'a':
        add_prompt();
        break;
    case 'm':
        modify_prompt(json_sn);
        break;
    case 'd':
        remove_prompt();
        break;
    case 'n':
        rename_prompt();
        break;
    }

}

void NetworkLoop::modify_prompt(BasicJson const & json_sn) {

    std::string name = index_prompt();
    if (name.empty()){
        return;
    } else {
        // TODO: get StationLoop and run its initializer
    }

}

void NetworkLoop::modify(BasicJson const & json_sn,
        std::string const & index) {

    // TODO: implement this after StationLoop is built

    BasicJson parent = parent_.at(index);


}

void NetworkLoop::remove(std::string const & index) {

    // index should be a string -- station name

    if( parent_.find(index) != parent_.end() ) {
        parent_.erase(index);
    } else {
        throw WarningException("network_loop","remove","station not in range");
    }

}

void NetworkLoop::remove_prompt() {

    // prompt user to find a component and then removes the one chosen

    std::string name = index_prompt();
    if (name.empty()){
        return;
    } else {
        try{
            remove(name);
        } catch (WarningException & e) {
            throw FatalException("network_loop","remove",
                                 "Existing station could not be located");
        }
    }


}

void NetworkLoop::rename_prompt() {

    std::string name = index_prompt();

    std::string replacement;

    std::cout << "Modifying the name of station " << name << '\n';
    std::cout << "Please enter a new name for this station,\n";
    std::cout << "Or leave the field blank to keep it unchanged.\n";

    std::cin >> name;

    if( name.empty() or name == replacement ) {
        std::cout << "Name will not be changed.\n";
    }

    if ( parent_.find(name) != parent_.end() ){
        std::cout << "Name is in use by another station.\n";
        std::cout << "";
    }

}

void NetworkLoop::rename(std::string const & name, std::string const & index) {
    BasicJson station = parent_.at(name);

}

void NetworkLoop::list_components() const {

    auto it = parent_.begin();
    std::cout <<  "Station List (name [ip]):" << std::endl;
    for( ; it != parent_.end(); ++it){
        BasicJson station = it.value();
        std::cout <<  it.key() << " [" << station.at("ip") << "]";
        if( station.find("digitizer") != station.end() ) {
            std::cout <<  " (" << station.at("digitizer").size() << " digitizers)";
        }
        if( station.find("data_processor") != station.end() ) {
            std::cout <<  " (has d_p)";
        }
        std::cout <<  std::endl;
    }

}

std::string NetworkLoop::index_prompt() const {

    // used to get the index of a station to modify or delete

    std::vector<std::string> station_list; // allows user to get input

    BasicJson::iterator it;
    for( it = parent_.begin(); it != parent_.end(); ++it ) {
        station_list.push_back(it.key());
    }

    std::cout <<  "Here is a map of all the stations currently in the network: ";
    std::cout <<  std::endl;

    for( int i = 0; i < station_list.size(); ++i ){
         std::cout <<  i << ".\t[\"" << station_list[i] << "]\"" << std::endl;
    }



    // now get the user input
    while(true) {


        std::cout <<  "Please input the number of the station you want," << std::endl;
        std::cout <<  "or enter a negative number to cancel." << std::endl;


        long idx;
        std::cin >> idx;
        std::cin.clear();

        if( idx < 0 ) {
            return std::string{};
        } else if ( idx < station_list.size() ) {
            return station_list[idx];
        }

        // default case for where
    }

}

void NetworkLoop::show_options() const {

    std::cout <<  std::endl;
    std::cout <<  "This is the network component configuration menu.\n";
    std::cout <<  "Would you like to...\n";
    std::cout <<  "\t...Add a new station to the network? (A,a)\n";
    if ( not parent_.empty() ) {
        std::cout << "\t...Modify an existing station? (M,m)\n";
        std::cout << "\t...Delete a station from the network? (D,d)\n";
        std::cout << "\t...Change the name of a station? (N,n)\n";
    }
    // TODO: uncomment this line once the sn-display code is separated out
    // std::cout <<  "\t...View the complete configuration? (V,v) << std::endl;
    std::cout <<  "\t...Go back to the previous menu? (P,p)\n";
    std::cout <<  "\t...Return to the main menu? (R,r)\n";

}

char NetworkLoop::collect_input() const {

    char input;
    std::cin >> input;
    // flush the buffer to prevent invalid character selections
    std::cin.clear(); std::cin.ignore(INT_MAX,'\n');

    switch (input) {
    case 'R':
    case 'r':
        return RETURN;
    case 'P':
    case 'p':
        return PREV;
    case 'A':
    case 'a':
        return 'a';
    }

    // only allow these commands to be called if we already have stations
    if ( not parent_.empty() ) {
        switch(input) {
        case 'M':
        case 'm':
            return 'm';
        case 'D':
        case 'd':
            return 'd';
        case 'N':
        case 'n':
            return 'n';
        }
    }

    return FAIL;

}

void NetworkLoop::add(std::string const & name,
        std::string const & ip) {

    ip_validate(ip);

    BasicJson station;
    station["ip"] = ip;

    parent_[name] = station;

}

void NetworkLoop::add_prompt() {

    std::cout <<  "Adding a station to the network:";
    std::cout <<  "Please enter a name for this station:" << std::endl;



    // TODO: safety check for already-existing names to prevent auto overwrite

    std::string name;

    std::cin >> name;
    std::cin.clear();

    while( parent_.find(name) != parent_.end() ) {

        std::cout <<  "This name is already being used by a station." << std::endl;
        std::cout <<  "Do you want to continue with this operation?" << std::endl;
        std::cout <<  "Doing so will overwrite the data for this station,";
        std::cout <<  std::endl;
        std::cout <<  "including removing all its subcomponents." << std::endl;
        std::cout <<  "If you do, type 'Y' or 'y' now." << std::endl;

        std::string yes;
        std::cin >> yes;

        char y = yes.front();

        if ( y == 'y' or y == 'Y') {
            break;
        }

        std::cout <<  "Please enter a different name for the station:" << std::endl;

        std::cin >> name;
        std::cin.clear();


    }

    std::string ip;

    while( ip.empty() ) {
        std::cout <<  "Please enter an IP address for the station:" << std::endl;
        std::cout <<  "(Use the format X.X.X.X)" << std::endl;

        std::cin >> ip;
        std::cin.clear();

        try {
            add(name, ip);
        } catch (WarningException & e) {
            std::cout << "Sorry, that IP address was invalid. Try again.";
            std::cout << std::endl;
            ip.clear();
        }
    }

}

// TODO: this may become split off into a separate validation module
void NetworkLoop::ip_validate(std::string const & ip) const {
    // subdivide string into 4 components, one for each byte
    std::vector<std::string> bytes;

    auto const ip_tokens = split(ip,'.');

    // Currently uses IP validation assuming IPv4 address
    // so the definition of IP_BYTES_ may have to change in the future
    if ( ip_tokens.size() != IP_BYTES_ ) {
        // too few bytes in provided file
        throw WarningException("network_loop","ip_validate",
                "wrong number of bytes in address");
    }

    for (auto const & ip_token : ip_tokens) {
        unsigned int parsed_byte; // unsigned to simplify check
        parsed_byte = std::stoul(ip_token);
        if (parsed_byte > 255) {
            throw WarningException("network_loop","ip_validate",
                    "number larger than a byte");

        }

    }

}


} // <- mzn
