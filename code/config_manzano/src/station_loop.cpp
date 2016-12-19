/*
 * station_loop.cpp
 *
 *  Created on: Nov 29, 2016
 *      Author: akearns
 */

#include "station_loop.h"

namespace mzn{

void NetworkLoop::show_options() const {

    std::cout <<  std::endl;
    std::cout <<  "This is the station component configuration menu.\n";
    std::cout <<  "Would you like to...\n";
    std::cout <<  "\t...Add a new component to the network? (A,a)\n";
    if ( not parent_.empty() ) {
        std::cout << "\t...Modify an existing component? (M,m)\n";
        std::cout << "\t...Delete a component from the network? (D,d)\n";
        std::cout << "\t...Change the IP address of this station? (I,i)\n";
    }
    // TODO: uncomment this line once the sn-display code is separated out
    // std::cout <<  "\t...View the complete configuration? (V,v) << std::endl;
    std::cout <<  "\t...Go back to the previous menu? (P,p)\n";
    std::cout <<  "\t...Return to the main menu? (R,r)\n";

    std::cout << "(NOTE: Network components are either digitizers or data "
              << "processors.)";
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
        case 'I':
        case 'i':
            return 'i';
        }
    }

    return FAIL;

}

}

